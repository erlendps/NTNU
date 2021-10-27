#include "o3.h"
#include "gpio.h"
#include "systick.h"

// definisjoner
// Porter
#define BTN0_PORT	GPIO_PORT_B
#define BTN1_PORT	GPIO_PORT_B

#define LED0_PORT	GPIO_PORT_E

// pinner
#define BTN0_PIN	9
#define BTN1_PIN	10

#define LED0_PIN	2

// masker
#define EXTIPSELH_Msk	0b0001

// konstanter
#define HRS_IN_SEC	3600
#define MIN_IN_SEC	60

// globale pekere
volatile gpio_map_t* GPIO = (gpio_map_t*) GPIO_BASE;
volatile systick_map_t* SYSTICK = (systick_map_t*) SYSTICK_BASE;

// enum for å holde styr på tilstand
enum states{sec_sel, min_sel, hrs_sel, on, alarm};
int currentState = sec_sel;

// tid
int totalSeconds = 0; // starter med 0
int maxSeconds = 99 * HRS_IN_SEC + 59 * MIN_IN_SEC + 59;
char timeString[8];

/**************************************************************************//**
 * @brief Konverterer nummer til string 
 * Konverterer et nummer mellom 0 og 99 til string
 *****************************************************************************/
void int_to_string(char *timestamp, unsigned int offset, int i) {
    if (i > 99) {
        timestamp[offset]   = '9';
        timestamp[offset+1] = '9';
        return;
    }

    while (i > 0) {
	    if (i >= 10) {
		    i -= 10;
		    timestamp[offset]++;
		
	    } else {
		    timestamp[offset+1] = '0' + i;
		    i=0;
	    }
    }
}

/**************************************************************************//**
 * @brief Konverterer 3 tall til en timestamp-string
 * timestamp-argumentet må være et array med plass til (minst) 7 elementer.
 * Det kan deklareres i funksjonen som kaller som "char timestamp[7];"
 * Kallet blir dermed:
 * char timestamp[7];
 * time_to_string(timestamp, h, m, s);
 *****************************************************************************/
void time_to_string(char *timestamp, int h, int m, int s) {
    timestamp[0] = '0';
    timestamp[1] = '0';
    timestamp[2] = '0';
    timestamp[3] = '0';
    timestamp[4] = '0';
    timestamp[5] = '0';
    timestamp[6] = '\0';

    int_to_string(timestamp, 0, h);
    int_to_string(timestamp, 2, m);
    int_to_string(timestamp, 4, s);
}

void updateDisplay(int totalSec) {
	int h = totalSec / HRS_IN_SEC;
	int m = (totalSec / MIN_IN_SEC) % 60;
	int s = totalSec % 60;

	time_to_string(timeString, h, m, s);
	lcd_write(timeString);
}

// skriver bitet til pinnen på addressen
void setBit(volatile word* address, int pin, int bit) {
	*address |= bit << pin;
}

// setter word på addressen
void setWord(volatile word* address, int pin, int word) {
	// tar høyde for at pinnen kan være større enn 8, og da må 8 fjernes
	// dette gjelder kun for MODE og EXTIPSEL
	if (pin >= 8) {
		pin -= 8;
	}
	// et ord er 4 bit langt.
	pin *= 4;
	// nullstiller de 4 bitene man ønsker å endre
	*address &= ~(0b1111 << pin);
	// ORer inn ordet på riktig plass
	*address |= word << pin;
}

// starter klokken. Endrer tilstand til 'on' og enabler systick interrupts.
void startClock(void) {
	currentState = on;
	SYSTICK->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

// stopper klokken ved å sette Systick enable til 0, endrer tilstand til 'alarm'
// og skrur på LED-lyset
void stopClock(void) {
	SYSTICK->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	currentState = alarm;
	setAlarmLED(true);
}

// dersom value er true, blir LED0 skrudd på, hvis ikke skrus den av.
void setAlarmLED(bool value) {
	if (value) {
		setBit(&GPIO->ports[LED0_PORT].DOUTSET, LED0_PIN, 1);
	} else {
		setBit(&GPIO->ports[LED0_PORT].DOUTCLR, LED0_PIN, 1);
	}
}

// initialiserer klokken ved å enable forskjellige I/O registre og interrupts
void initializeClock(void) {
	// setter opp interrupt for SysTick
	SYSTICK->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk;
	SYSTICK->LOAD = FREQUENCY;
	SYSTICK->VAL = 0;

	// setter opp GPIO. LED0 skal skrives til, så må settes til output
	// BTN0 og BTN1 skal leses fra og trenger derfor å bli satt til input
	// LED må på MODEL fordi den ligger på pin 2
	setWord(&GPIO->ports[LED0_PORT].MODEL, LED0_PIN, GPIO_MODE_OUTPUT);

	// BTN0 og BTN1 må på MODEH fordi de ligger på henholdsvis pin 9 og 10
	setWord(&GPIO->ports[BTN0_PORT].MODEH, BTN0_PIN, BTN0_PORT);
	setWord(&GPIO->ports[BTN1_PORT].MODEH, BTN1_PIN, BTN1_PORT);

	// interrupts på GPIO (på BTN0 og BTN1)
	// bruker EXTIPSELH fordi BTN0 og BTN1 ligger på henholdsvis pin 9 og 10
	setWord(&GPIO->EXTIPSELH, BTN0_PIN, BTN0_PORT);
	setWord(&GPIO->EXTIPSELH, BTN1_PIN, BTN1_PORT);

	setBit(&GPIO->EXTIFALL, BTN0_PIN, 1);
	setBit(&GPIO->EXTIFALL, BTN1_PIN, 1);

	setBit(&GPIO->IFC, BTN0_PIN, 1);
	setBit(&GPIO->IFC, BTN1_PIN, 1);

	setBit(&GPIO->IEN, BTN0_PIN, 1);
	setBit(&GPIO->IEN, BTN1_PIN, 1);

	updateDisplay(totalSeconds);
}

// interrupt vektorer
// systick handler, for hvert interrupt skal den
// minke totalSeconds med 1 og sjekke om den er lik 0
void SysTick_Handler(void) {
	totalSeconds -= 1;
	updateDisplay(totalSeconds);
	if (totalSeconds == 0) {
		stopClock();
	}
}

// GPIO_ODD for å håndtere BTN0.
// Avhengig av tilstanden, skal BTN0 primært brukes til å øke antall
// sekunder, minutter og timer. Ellers skal den ikke gjøre noe.
void GPIO_ODD_IRQHandler(void) {
	switch (currentState) {
	case (sec_sel):
		totalSeconds += 1;
		updateDisplay(totalSeconds);
		break;
	case (min_sel):
		totalSeconds += MIN_IN_SEC;
		updateDisplay(totalSeconds);
		break;
	case (hrs_sel):
		totalSeconds += HRS_IN_SEC;
		updateDisplay(totalSeconds);
		break;
	case (on):
		break;
	case (alarm):
		break;
	default:
		break;
	}
	if (totalSeconds >= maxSeconds) {
		totalSeconds = maxSeconds;
	}
	setBit(&GPIO->IFC, BTN0_PIN, 1);
}

// GPIO_EVEN for å håndtere BTN1
// denne skal brukes for å gå videre til neste steg.
void GPIO_EVEN_IRQHandler(void) {
	switch (currentState) {
	case (sec_sel):
		currentState = min_sel;
		break;
	case (min_sel):
		currentState = hrs_sel;
		break;
	case (hrs_sel):
		startClock();
		break;
	case (on):
		break;
	case (alarm):
		currentState = sec_sel;
		setAlarmLED(false);
		break;
	default:
		break;
	}
	setBit(&GPIO->IFC, BTN1_PIN, 1);
}

int main(void) {
    init();
    initializeClock();
    while (true);

    return 0;
}

