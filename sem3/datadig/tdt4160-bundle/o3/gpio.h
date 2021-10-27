#ifndef GPIO_H
#define GPIO_H

// Pin-datatype
typedef struct{
	unsigned int   port;
	unsigned int   pin;
} port_pin_t;

// GPIO pin modes
#define GPIO_MODE_INPUT  0b0001
#define GPIO_MODE_OUTPUT 0b0100

// GPIO-adresse
#define GPIO_BASE 0x40006000

// GPIO port-nummere
#define GPIO_PORT_A 0
#define GPIO_PORT_B 1
#define GPIO_PORT_C 2
#define GPIO_PORT_D 3
#define GPIO_PORT_E 4
#define GPIO_PORT_F 5

// GPIO struct
typedef struct{
	volatile word CTRL;
	volatile word MODEL;
	volatile word MODEH;
	volatile word DOUT;
	volatile word DOUTSET;
	volatile word DOUTCLR;
	volatile word DOUTTGL;
	volatile word DIN;
	volatile word PINLOCKN;
} gpio_port_map_t;

// GPIO MAP
typedef struct{
	volatile gpio_port_map_t ports[6];
	volatile word empty_spaces[10];
	volatile word EXTIPSELL;
	volatile word EXTIPSELH;
	volatile word EXTIRISE;
	volatile word EXTIFALL;
	volatile word IEN;
	volatile word IF;
	volatile word IFS;
	volatile word IFC;
	volatile word ROUTE;
	volatile word INSENSE;
	volatile word LOCK;
	volatile word CTRL;
	volatile word CMD;
	volatile word EM4WUEN;
	volatile word EM4WUPOL;
	volatile word EM4WUCAUSE;
} gpio_map_t;


#endif
