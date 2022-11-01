#define _GNU_SOURCE

#include <dirent.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <linux/input.h>
#include <poll.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

// The game state can be used to detect what happens on the playfield
#define GAMEOVER 0
#define ACTIVE (1 << 0)
#define ROW_CLEAR (1 << 1)
#define TILE_ADDED (1 << 2)

// colors used
#define NUM_OF_COLORS 10
#define RED 0x8800
#define BLUE 0x0093
#define GREEN 0x24c6
#define PURPLE 0x7094
#define YELLOW 0xde40
#define CRASH_PINK 0xdabe
#define SKY_BLUE 0x75be
#define BROWN 0x7ae3
#define ORANGE 0xec83
#define LIGHT_PINK 0xee9a
#define DEFAULT_COLOR 0xdabe

// struct for the framebuffer
typedef struct {
  u_int16_t pixel[8][8];
} fb_t;

// If you extend this structure, either avoid pointers or adjust
// the game logic allocate/deallocate and reset the memory
// ADDED: a color field
typedef struct {
  bool occupied;
  u_int16_t color;
} tile;

typedef struct {
  unsigned int x;
  unsigned int y;
} coord;

typedef struct {
  coord const grid;                      // playfield bounds
  unsigned long const uSecTickTime;      // tick rate
  unsigned long const rowsPerLevel;      // speed up after clearing rows
  unsigned long const initNextGameTick;  // initial value of nextGameTick

  unsigned int tiles;  // number of tiles played
  unsigned int rows;   // number of rows cleared
  unsigned int score;  // game score
  unsigned int level;  // game level

  tile *rawPlayfield;  // pointer to raw memory of the playfield
  tile **playfield;    // This is the play field array
  unsigned int state;
  coord activeTile;  // current tile

  unsigned long tick;          // incremeted at tickrate, wraps at nextGameTick
                               // when reached 0, next game state calculated
  unsigned long nextGameTick;  // sets when tick is wrapping back to zero
                               // lowers with increasing level, never reaches 0
} gameConfig;

gameConfig game = {
    .grid = {8, 8},
    .uSecTickTime = 10000,
    .rowsPerLevel = 2,
    .initNextGameTick = 50,
};

// the sense hat frame buffer
fb_t *fb;

// pollfd struct
struct pollfd pfd = {
    .events = POLLIN,
};

// buffer for storing the input_event
struct input_event ie_buffer;

// color array
u_int16_t colors[] = {
    RED,
    GREEN,
    BLUE,
    PURPLE,
    YELLOW,
    CRASH_PINK,
    SKY_BLUE,
    ORANGE,
    LIGHT_PINK,
    BROWN,
};

// Predicate that tests whether a file is a framebuffer device.
int is_fb_device(char *filename) {
  // compare the first 2 bytes of d_name
  return strncmp("fb", filename, strlen("fb") - 1) == 0;
}

// Predicate that tests whether a file is a event device.
// Assumes name is on form eventX, as per https://www.kernel.org/doc/html/latest/input/input.html#evdev
int is_event_device(char *filename) {
  // compare the first 5 bytes of d_name
  return strncmp("event", filename, strlen("event") - 1) == 0;
}

// returns the fd for the Sense hat joystick, if it exists. If not it returns a
// negative value, indicating that an error occured.
int open_event_device() {
  // set up the directory, directory entry and file descriptor
  DIR *directory;
  struct dirent *dir_entry;
  int fd = -1;

  // open the directory
  directory = opendir("/dev/input");
  if (directory == NULL) {
    fprintf(stderr, "Could not open /dev/input");
    return -1;
  }

  // scan the directory using the is_event_device predicate.
  while ((dir_entry = readdir(directory)) != NULL) {
    if (is_event_device(dir_entry->d_name)) {
      char filename[64];  // a filename is max 64 bytes
      char name[256];     // the max length of a device name is 256 bytes

      // fetch the filename of the current entry
      // snprintf is safe, since we write max 64 bytes
      snprintf(filename, sizeof(filename), "/dev/input/%s", dir_entry->d_name);
      fd = open(filename, O_RDWR);
      if (fd < 0) {
        // something went wrong, continue at next iteration
        continue;
      }
      // get event device name and place it in name
      ioctl(fd, EVIOCGNAME(sizeof(name)), name);
      // then check if its the correct device
      if (strcmp("Raspberry Pi Sense HAT Joystick", name) == 0) {
        fprintf(stdout, "Found Raspberry Pi Sense Hat Joystick on /dev/input/%s\n", dir_entry->d_name);
        break;
      }
      close(fd);
      fd = -1;
    }
  }
  // close the directory
  closedir(directory);

  return fd;
}

// Returns the fd for the Sense Hat framebuffer. If it does not
// find the fb, it returns -1, indicating an error.
int open_fbdevice() {
  DIR *directory;
  struct dirent *dir_entry;
  int fd = -1;
  // also set up the buffer for the frame buffer info
  struct fb_fix_screeninfo fix_info;

  // open directory
  directory = opendir("/dev");
  if (directory == NULL) {
    fprintf(stdout, "Could not open /dev");
  }

  // scan the directory using the is_fb_device predicate
  while ((dir_entry = readdir(directory)) != NULL) {
    if (is_fb_device(dir_entry->d_name)) {
      char filename[64];
      // set filename to the dir_name
      snprintf(filename, sizeof(filename), "/dev/%s", dir_entry->d_name);
      fd = open(filename, O_RDWR);
      if (fd < 0) {
        // bad file, continue at next iteration
        continue;
      }
      // get screen info, and place it in the buffer for the frame buffer
      ioctl(fd, FBIOGET_FSCREENINFO, &fix_info);
      // check if its the correct framebuffer
      if (strcmp("RPi-Sense FB", fix_info.id) == 0) {
        fprintf(stdout, "Found RPi-Sense FB on /dev/%s\n", dir_entry->d_name);
        break;
      }
      // close fd since it was not the correct one, and reset fd
      close(fd);
      fd = -1;
    }
  }
  // close directory
  closedir(directory);

  return fd;
}

// This function is called on the start of your application
// Here you can initialize what ever you need for your task
// return false if something fails, else true
bool initializeSenseHat() {
  // fetch fb fd and event_dev fd and check if they are valid
  int frame_buffer_fd = open_fbdevice();
  int event_dev_fd = open_event_device();
  if (frame_buffer_fd < 0 || event_dev_fd < 0) {
    fprintf(stderr, "Could not find the sense hat framebuffer or joystick.\n");
    return false;
  }

  /* FRAMEBUFFER */
  // create a memory mapping
  fb = mmap(NULL, sizeof(fb_t), PROT_READ | PROT_WRITE, MAP_SHARED, frame_buffer_fd, 0);
  if (fb == MAP_FAILED) {
    fprintf(stderr, "Memory mapping failed\n");
    return false;
  }
  // close the frame buffer file. When we have a mapping, we don't need the file to be open.
  close(frame_buffer_fd);

  // reset all pixels to 0, so no color is shown
  memset(fb, 0, sizeof(fb_t));

  /* EVENT DEVICE*/
  // set the fd of the pollfd struct
  pfd.fd = event_dev_fd;

  // the sense hat is now set up correctly, and can be accessed with with fb[i][j]
  return true;
}

// This function is called when the application exits
// Here you can free up everything that you might have opened/allocated
void freeSenseHat() {
  // reset framebuffer
  memset(fb, 0, sizeof(fb_t));
  // deallocate memorymapping
  munmap(fb, sizeof(fb_t));
  // close joystick fd
  close(pfd.fd);
}

// This function should return the key that corresponds to the joystick press
// KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, with the respective direction
// and KEY_ENTER, when the the joystick is pressed
// !!! when nothing was pressed you MUST return 0 !!!
int readSenseHatJoystick() {
  // check if the joystick wishes to be read. Returns immediatly after checking, so we dont block
  // the rest of the program. Poll returns a negative value if an error occured, and positive otherwise
  if (poll(&pfd, 1, 0)) {
    if (read(pfd.fd, &ie_buffer, sizeof(struct input_event)) < 0) {
      // error on reading, just return 0
      return 0;
    }
    // check if we are pressing or holding continously
    if (ie_buffer.value == 1 || ie_buffer.value == 2) {
      // we then can return the code
      return ie_buffer.code;
    }
    // else, if value == 0, we have released the joystick, but it still registers
    // that we have something to read. We then return 0
    return 0;
  }
  // the joystick has nothing that we want to read.
  return 0;
}

// function for fetching a random color from the colors array
u_int16_t getRandomColor() {
  return colors[rand() % NUM_OF_COLORS];
}

// The game logic uses only the following functions to interact with the playfield.
// if you choose to change the playfield or the tile structure, you might need to
// adjust this game logic <> playfield interface

static inline void newTile(coord const target) {
  game.playfield[target.y][target.x].occupied = true;
  // ADDED: set the tile to have a random color
  game.playfield[target.y][target.x].color = getRandomColor();
}

static inline void copyTile(coord const to, coord const from) {
  memcpy((void *)&game.playfield[to.y][to.x], (void *)&game.playfield[from.y][from.x], sizeof(tile));
}

static inline void copyRow(unsigned int const to, unsigned int const from) {
  memcpy((void *)&game.playfield[to][0], (void *)&game.playfield[from][0], sizeof(tile) * game.grid.x);
}

static inline void resetTile(coord const target) {
  memset((void *)&game.playfield[target.y][target.x], 0, sizeof(tile));
}

static inline void resetRow(unsigned int const target) {
  memset((void *)&game.playfield[target][0], 0, sizeof(tile) * game.grid.x);
}

static inline bool tileOccupied(coord const target) {
  return game.playfield[target.y][target.x].occupied;
}

static inline bool rowOccupied(unsigned int const target) {
  for (unsigned int x = 0; x < game.grid.x; x++) {
    coord const checkTile = {x, target};
    if (!tileOccupied(checkTile)) {
      return false;
    }
  }
  return true;
}

static inline void resetPlayfield() {
  for (unsigned int y = 0; y < game.grid.y; y++) {
    resetRow(y);
  }
}

// This function should render the gamefield on the LED matrix. It is called
// every game tick. The parameter playfieldChanged signals whether the game logic
// has changed the playfield
void renderSenseHatMatrix(bool const playfieldChanged) {
  // if the playfield has not changed, dont waste I/O time.
  if (!playfieldChanged) {
    return;
  }

  // iterate through the framebuffer memory-mapping, and set the value
  // to be the current tile's color
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      coord const current_tile = {x, y};
      // get the color if the tile is occupied, otherwise set it to 0 (no color)
      fb->pixel[y][x] = tileOccupied(current_tile) ? game.playfield[y][x].color : 0x0;
    }
  }
}

// Below here comes the game logic. Keep in mind: You are not allowed to change how the game works!
// that means no changes are necessary below this line! And if you choose to change something
// keep it compatible with what was provided to you!

bool addNewTile() {
  game.activeTile.y = 0;
  game.activeTile.x = (game.grid.x - 1) / 2;
  if (tileOccupied(game.activeTile))
    return false;
  newTile(game.activeTile);
  return true;
}

bool moveRight() {
  coord const newTile = {game.activeTile.x + 1, game.activeTile.y};
  if (game.activeTile.x < (game.grid.x - 1) && !tileOccupied(newTile)) {
    copyTile(newTile, game.activeTile);
    resetTile(game.activeTile);
    game.activeTile = newTile;
    return true;
  }
  return false;
}

bool moveLeft() {
  coord const newTile = {game.activeTile.x - 1, game.activeTile.y};
  if (game.activeTile.x > 0 && !tileOccupied(newTile)) {
    copyTile(newTile, game.activeTile);
    resetTile(game.activeTile);
    game.activeTile = newTile;
    return true;
  }
  return false;
}

bool moveDown() {
  coord const newTile = {game.activeTile.x, game.activeTile.y + 1};
  if (game.activeTile.y < (game.grid.y - 1) && !tileOccupied(newTile)) {
    copyTile(newTile, game.activeTile);
    resetTile(game.activeTile);
    game.activeTile = newTile;
    return true;
  }
  return false;
}

bool clearRow() {
  if (rowOccupied(game.grid.y - 1)) {
    for (unsigned int y = game.grid.y - 1; y > 0; y--) {
      copyRow(y, y - 1);
    }
    resetRow(0);
    return true;
  }
  return false;
}

void advanceLevel() {
  game.level++;
  switch (game.nextGameTick) {
    case 1:
      break;
    case 2 ... 10:
      game.nextGameTick--;
      break;
    case 11 ... 20:
      game.nextGameTick -= 2;
      break;
    default:
      game.nextGameTick -= 10;
  }
}

void newGame() {
  game.state = ACTIVE;
  game.tiles = 0;
  game.rows = 0;
  game.score = 0;
  game.tick = 0;
  game.level = 0;
  resetPlayfield();
}

void gameOver() {
  game.state = GAMEOVER;
  game.nextGameTick = game.initNextGameTick;
}

bool sTetris(int const key) {
  bool playfieldChanged = false;

  if (game.state & ACTIVE) {
    // Move the current tile
    if (key) {
      playfieldChanged = true;
      switch (key) {
        case KEY_LEFT:
          moveLeft();
          break;
        case KEY_RIGHT:
          moveRight();
          break;
        case KEY_DOWN:
          while (moveDown()) {
          };
          game.tick = 0;
          break;
        default:
          playfieldChanged = false;
      }
    }

    // If we have reached a tick to update the game
    if (game.tick == 0) {
      // We communicate the row clear and tile add over the game state
      // clear these bits if they were set before
      game.state &= ~(ROW_CLEAR | TILE_ADDED);

      playfieldChanged = true;
      // Clear row if possible
      if (clearRow()) {
        game.state |= ROW_CLEAR;
        game.rows++;
        game.score += game.level + 1;
        if ((game.rows % game.rowsPerLevel) == 0) {
          advanceLevel();
        }
      }

      // if there is no current tile or we cannot move it down,
      // add a new one. If not possible, game over.
      if (!tileOccupied(game.activeTile) || !moveDown()) {
        if (addNewTile()) {
          game.state |= TILE_ADDED;
          game.tiles++;
        } else {
          gameOver();
        }
      }
    }
  }

  // Press any key to start a new game
  if ((game.state == GAMEOVER) && key) {
    playfieldChanged = true;
    newGame();
    addNewTile();
    game.state |= TILE_ADDED;
    game.tiles++;
  }

  return playfieldChanged;
}

int readKeyboard() {
  struct pollfd pollStdin = {
      .fd = STDIN_FILENO,
      .events = POLLIN};
  int lkey = 0;

  if (poll(&pollStdin, 1, 0)) {
    lkey = fgetc(stdin);
    if (lkey != 27)
      goto exit;
    lkey = fgetc(stdin);
    if (lkey != 91)
      goto exit;
    lkey = fgetc(stdin);
  }
exit:
  switch (lkey) {
    case 10:
      return KEY_ENTER;
    case 65:
      return KEY_UP;
    case 66:
      return KEY_DOWN;
    case 67:
      return KEY_RIGHT;
    case 68:
      return KEY_LEFT;
  }
  return 0;
}

void renderConsole(bool const playfieldChanged) {
  if (!playfieldChanged)
    return;

  // Goto beginning of console
  fprintf(stdout, "\033[%d;%dH", 0, 0);
  for (unsigned int x = 0; x < game.grid.x + 2; x++) {
    fprintf(stdout, "-");
  }
  fprintf(stdout, "\n");
  for (unsigned int y = 0; y < game.grid.y; y++) {
    fprintf(stdout, "|");
    for (unsigned int x = 0; x < game.grid.x; x++) {
      coord const checkTile = {x, y};
      fprintf(stdout, "%c", (tileOccupied(checkTile)) ? '#' : ' ');
    }
    switch (y) {
      case 0:
        fprintf(stdout, "| Tiles: %10u\n", game.tiles);
        break;
      case 1:
        fprintf(stdout, "| Rows:  %10u\n", game.rows);
        break;
      case 2:
        fprintf(stdout, "| Score: %10u\n", game.score);
        break;
      case 4:
        fprintf(stdout, "| Level: %10u\n", game.level);
        break;
      case 7:
        fprintf(stdout, "| %17s\n", (game.state == GAMEOVER) ? "Game Over" : "");
        break;
      default:
        fprintf(stdout, "|\n");
    }
  }
  for (unsigned int x = 0; x < game.grid.x + 2; x++) {
    fprintf(stdout, "-");
  }
  fflush(stdout);
}

inline unsigned long uSecFromTimespec(struct timespec const ts) {
  return ((ts.tv_sec * 1000000) + (ts.tv_nsec / 1000));
}

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;
  // This sets the stdin in a special state where each
  // keyboard press is directly flushed to the stdin and additionally
  // not outputted to the stdout
  {
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);
    printf("\n%d\n", ttystate.c_cc[VMIN]);
    ttystate.c_lflag &= ~(ICANON | ECHO);
    ttystate.c_cc[VMIN] = 1;
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
  }
  // random
  srand(time(NULL));

  // Allocate the playing field structure
  game.rawPlayfield = (tile *)malloc(game.grid.x * game.grid.y * sizeof(tile));
  game.playfield = (tile **)malloc(game.grid.y * sizeof(tile *));
  if (!game.playfield || !game.rawPlayfield) {
    fprintf(stderr, "ERROR: could not allocate playfield\n");
    return 1;
  }
  for (unsigned int y = 0; y < game.grid.y; y++) {
    game.playfield[y] = &(game.rawPlayfield[y * game.grid.x]);
  }

  // Reset playfield to make it empty
  resetPlayfield();
  // Start with gameOver
  gameOver();

  if (!initializeSenseHat()) {
    fprintf(stderr, "ERROR: could not initilize sense hat\n");
    return 1;
  };

  // Clear console, render first time
  fprintf(stdout, "\033[H\033[J");
  renderConsole(true);
  renderSenseHatMatrix(true);

  while (true) {
    struct timeval sTv, eTv;
    gettimeofday(&sTv, NULL);

    int key = readSenseHatJoystick();
    if (!key)
      key = readKeyboard();
    if (key == KEY_ENTER)
      break;

    bool playfieldChanged = sTetris(key);
    renderConsole(playfieldChanged);
    renderSenseHatMatrix(playfieldChanged);

    // Wait for next tick
    gettimeofday(&eTv, NULL);
    unsigned long const uSecProcessTime = ((eTv.tv_sec * 1000000) + eTv.tv_usec) - ((sTv.tv_sec * 1000000 + sTv.tv_usec));
    if (uSecProcessTime < game.uSecTickTime) {
      usleep(game.uSecTickTime - uSecProcessTime);
    }
    game.tick = (game.tick + 1) % game.nextGameTick;
  }

  // bring terminal back to the initial state
  {
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);
    ttystate.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
  }

  freeSenseHat();
  free(game.playfield);
  free(game.rawPlayfield);
  printf("\n");

  return 0;
}
