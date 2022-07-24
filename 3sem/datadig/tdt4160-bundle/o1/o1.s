.thumb
.syntax unified

.include "gpio_constants.s"     // Register-adresser og konstanter for GPIO

.text
	.global Start

Start:
	// Jeg trenger addressen til DOUTSET og DOUTCLR på LED_PORT, og addressen til DIN på BUTTON_PORT
	LDR R0, =GPIO_BASE + (PORT_SIZE * LED_PORT) + GPIO_PORT_DOUTSET // henter minneaddressen til DOUTSET for LED_PORT og lagrer i R0
	LDR R1, =GPIO_BASE + (PORT_SIZE * LED_PORT) + GPIO_PORT_DOUTCLR	// henter minneaddressen til DOUTCLR for LED_PORT og lagrer i R1
	LDR R2, =GPIO_BASE + (PORT_SIZE * BUTTON_PORT) + GPIO_PORT_DIN 	// henter minneaddresen til DIN for BUTTON_PORT og lagrer i R2

	// Trenger nå hva jeg vil sammenligne med
	MOV R3, #1 << LED_PIN    // LED_PIN = 2, så i R3 lagres 1 left bitshiftet 2 plasser
	MOV R4, #1 << BUTTON_PIN // BUTTON_PIN = 9

// i loopen må jeg sjekke om knappen er trykket ned eller ikke
Loop:
	LDR R5, [R2]	 // henter ut verdien som ligger på button_pinnen (input)
	AND R5, R5, R4   // fjerner de verdiene jeg ikke trenger
	CMP R5, R4       // sjekker om de er like
	BEQ On			 // branch til 'On' hvis de er like, ellers, gå til Off label

Off:
	// knappen holdes ikke nede, da skal vi skru av led
	STR R3, [R0]  // knappen er aktiv lav
	B Loop

On:
	// knappen holdes nede, da skal vi sette på LED
	STR R3, [R1]
	B Loop

NOP // Behold denne på bunnen av fila

