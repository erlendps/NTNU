#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// MIN-DFA config
#define N_STATES 10
#define START_STATE 0
#define ACCEPT 8
#define ERROR 9

int transition_table[N_STATES][256];  // Table form of the automaton

void reset_transition_table() {
  // resets the transition table and sets that every input char leads to
  // the error state.
  for (int i = 0; i < N_STATES; i++) {
    for (int j = 0; j < 256; j++) {
      transition_table[i][j] = ERROR;
    }
  }
}

void initialize_transition_table() {
  // first reset the array so that they all point to the error-state
  reset_transition_table();
  // hardcoded, (int)'x' is a trick to convert a char to ascii code
  transition_table[0][(int)'g'] = 1;
  transition_table[0][(int)'d'] = 3;
  transition_table[1][(int)'o'] = 2;
  transition_table[2][(int)'\n'] = ACCEPT;
  transition_table[3][(int)'x'] = 4;
  transition_table[3][(int)'y'] = 4;
  transition_table[4][(int)'='] = 5;
  transition_table[5][(int)'-'] = 6;
  // numbers 0-9 in state 5 and 6
  for (int i = 0; i < 10; i++) {
    // little hack for converting a digit to ASCII code
    int index = i + '0';
    transition_table[5][index] = 7;
    transition_table[6][index] = 7;
  }
  transition_table[7][(int)'\n'] = ACCEPT;
}

// Driver program's internal state
int state = START_STATE;
int x = 421, y = 298,  // We start at the middle of the page,
    dx = 0, dy = 0;    // and with dx=dy=0

// Used to store the chars of statement we are currently reading
char lexeme_buffer[1024];
int lexeme_length = 0;

// In here we can assume that lexeme_buffer contains a valid statement, since the DFA reached ACCEPT
void handle_statement() {
  if (strncmp(lexeme_buffer, "go", 2) == 0) {
    x = x + dx;
    y = y + dy;
    printf("%d %d lineto\n", x, y);
    printf("%d %d moveto\n", x, y);
  } else if (strncmp(lexeme_buffer, "dx=", 3) == 0) {
    sscanf(lexeme_buffer + 3, "%d", &dx);
  } else if (strncmp(lexeme_buffer, "dy=", 3) == 0) {
    sscanf(lexeme_buffer + 3, "%d", &dy);
  } else {
    assert(0 && "Reached an unreachable branch!");
  }
}

int main() {
  // Setup the DFA transitions as a table
  initialize_transition_table();

  // PostScript preable to create a valid ps-file
  printf("<< /PageSize [842 595] >> setpagedevice\n");
  printf("%d %d moveto\n", x, y);

  // Main loop
  int line_num = 1;  // Used to report which line an error occured on
  int read;
  while ((read = getchar()) != EOF) {
    // Store the read char in the buffer
    lexeme_buffer[lexeme_length++] = read;
    lexeme_buffer[lexeme_length] = 0;  // Add NULL terminator

    // Use the current state and the read char to find the next state
    state = transition_table[state][read];

    // Check if we reached the ACCEPT or ERROR states
    switch (state) {
      case ACCEPT:
        handle_statement();
        state = START_STATE;
        lexeme_length = 0;
        break;
      case ERROR:
        fprintf(stderr, "error: %d: unrecognized statement: %s\n", line_num, lexeme_buffer);
        exit(EXIT_FAILURE);
      default:
        break;
    }

    // If the char was a newline, the next char will be on a new line!
    if (read == '\n')
      line_num++;
  }

  if (state != START_STATE) {
    fprintf(stderr, "error: %d: input ended in the middle of a statement: %s\n", line_num, lexeme_buffer);
    exit(EXIT_FAILURE);
  }

  printf("stroke\n");
  printf("showpage\n");
}
