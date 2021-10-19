.thumb
.syntax unified

.include "gpio_constants.s"     // Register-adresser og konstanter for GPIO
.include "sys-tick_constants.s" // Register-adresser og konstanter for SysTick

.text
	.global Start
	
Start:
	// enable systick interrupt handler
	// i starten skal må CTRL ORes med alle tre maskene.
	// når knappen er implementert, trenger ikke masken som enabler å bli ORet,
	// dette styres av knappen selv.
	// CTRL register
	LDR R0, =SYSTICK_BASE
	LDR R1, =SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk
	STR R1, [R0]

	// LOAD register
	// klokkefrekvens må deles på 10 siden vi ønsker millisekunder.
	LDR R0, =(SYSTICK_BASE + SYSTICK_LOAD)
	LDR R1, =(FREQUENCY/10)
	STR R1, [R0]

	// VAL register
	// setter value til 0 siden den skal starte å telle med en gang knappen trykkes.
	LDR R0, =(SYSTICK_BASE + SYSTICK_VAL)
	MOV R1, #0
	STR R1, [R0]

	//INTERRUPT
	// skal nå sette opp interrupt register for knappen BTN0 som ligger på pin 9
	// må først sette EXTIPSELH pin-gruppe 9 til porten knappen ligger på, dvs port B.
	LDR R0, =(GPIO_BASE + GPIO_EXTIPSELH)
	MOV R1, #0b1111 << 4
	MVN R1, R1
	LDR R2, [R0]
	AND R2, R2, R1
	MOV R1, #PORT_B << 4
	ORR R2, R2, R1
	STR R2, [R0]

	// knappen er aktiv høy, så må bruke EXTIFALL.
	LDR R0, =(GPIO_BASE + GPIO_EXTIFALL) // laster minneaddressen
	MOV R1, #1 << BUTTON_PIN // button_pin = 9, så leftshifter 9 plasser.
	LDR R2, [R0]  	// henter det som ligger på R0
	ORR R2, R2, R1  // skriver inn at bit 9 skal være høy
	STR R2, [R0]	// lagrer på EXXTIFALL registeret.

	// Interrupt flag, nullstiller denne så det ikke skjer noe mystisk når man starter.
	LDR R0, =(GPIO_BASE + GPIO_IFC)
	MOV R1, #1 << BUTTON_PIN // button_pin = 9, så leftshifter 9 plasser.
	LDR R2, [R0]  	// henter det som ligger på R0
	ORR R2, R2, R1  // skriver inn at bit 9 skal være høy.
	STR R2, [R0]	// skriver til IFClear registertet, som videre clearer IF-registeret.

	// til slutt må vi enable interrupts på pinne 9
	LDR R0, =(GPIO_BASE + GPIO_IEN)
	MOV R1, #1 << BUTTON_PIN // button_pin = 9, så leftshifter 9 plasser.
	LDR R2, [R0]  	// henter det som ligger på R0
	ORR R2, R2, R1  // skriver inn at bit 9 skal være høy.
	STR R2, [R0]	// skriver til IEN-registeret at 9 skal lyttes på.

Loop:
	WFI // wait for interrupt
	B Loop

// lager systick interrupt handler
.global SysTick_Handler
.thumb_func
SysTick_Handler:
	LDR R0, =tenths
    LDR R1, [R0]
    ADD R1, R1, #1

    // sjekker om antall millisekunder er 10, hvis den er det fortsetter dne til Seconds label
    // hvor verdien nullstilles.
    CMP R1, #10
    BNE notTen

Seconds:
	// setter millisekund til 0
	MOV R1, #0
	STR R1, [R0]

	// da må også LED0 toggles.
	LDR R0, =GPIO_BASE + (PORT_SIZE * LED_PORT) + GPIO_PORT_DOUTTGL // henter toggle-addressen

	// henter pinnen vi skal toggle
	MOV R1, #1 << LED_PIN // leftshifter bitet.

	// toggler LED0
	STR R1, [R0]

	// laster inn sekunder
	LDR R0, =seconds
	LDR R1, [R0]
	ADD R1, R1, #1

	// sjekker om antall sekunder er 60. Hvis den er det forsetter den til Minutes label hvor den nullstilles.
	CMP R1, #60
	BNE notSixty

Minutes:
	// setter sekunder til 0
	MOV R1, #0
	STR R1, [R0]

	// laster inn minutter
	LDR R0, =minutes
	LDR R1, [R0]
	ADD R1, R1, #1

	// sjekker om antall minutter er 100, hvis den er det nullstilles antall minutter.
	CMP R1, #100
	BNE notHundred

	MOV R1, #0

// legg til ett millisekund på telleren
notTen:
	STR R1, [R0]
	BX LR

// legg til ett sekund på telleren
notSixty:
	STR R1, [R0]
	BX LR

// legg til ett minutt på telleren
notHundred:
	STR R1, [R0]
	BX LR

// Interrupt handler når BTN0 trykkes. Klokken skal da startes/stoppes. Alt denne da trenger å gjøre
// er å toggle SysTick_CTRL_ENABLE_Msk, samt andre opreasjoner som er nødvendige.
.global GPIO_ODD_IRQHandler
.thumb_func
GPIO_ODD_IRQHandler:
	// laster inn SYSTICK_CTRL
	LDR R0, =SYSTICK_BASE
	LDR R1, [R0]
	// her kan vi ta en bitwise XOR med SysTick_CTRL_ENABLE_Msk. Når 0te bit er 0 vil det bli 1,
	// og når det er 1 vil det bli 0.
	EOR R1, R1, SysTick_CTRL_ENABLE_Msk
	STR R1, [R0]	// legger resultatet på SYSTICK_CTRL

	// må også nullstille Interrupt-flagget.
	LDR R0, =(GPIO_BASE + GPIO_IFC)
	MOV R1, #1 << BUTTON_PIN // button_pin = 9, så leftshifter 9 plasser.
	LDR R2, [R0]  	// henter det som ligger på R0
	ORR R2, R2, R1  // skriver inn at bit 9 skal være høy.
	STR R2, [R0]	// skriver til IFClear registertet, som videre clearer IF-registeret.

	BX LR



NOP // Behold denne på bunnen av fila

