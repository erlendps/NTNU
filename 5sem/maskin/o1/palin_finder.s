.global _start

.section .text

_start:
	// initial values
	mov sp, #0x3ffffffc 	// set up stack pointer to highest memory location
	ldr r4, =input 			// r0 contains address of the first char
	ldr r5, =0xff201000 	// r5 contains the address of the JTAG UART
	ldr r6, =0xff200000 	// r6 contains the address of the LEDs
	
	// first check the input, determine the length and convert to lowercase
	bl check_input
	
	// input is ready, we can now check whether its a palindrome
	bl check_palindrome
	
	// we are done!
	b exit


// function for counting the length of input and 
check_input:
	// add lr to stack
	sub sp, sp, #4 			// make room for stack pointer
	str lr, [sp]
	mov r1, #0 				// counter for string length, reset so we get correct result

check_input_loop:
	// the idea is to convert to lowercase, we dont need to worry about whitespace, since we can just adjust the offset
	// when we are inside check_palindrome.
	// for asci characters the 6th bit decides whether is a capital letter or not
	// we can therefore bitwise OR the character with 0x20 which would yield the lowercase letter
	
	ldrb r0, [r4, r1] 		// load the bytecode for the char at offset r0 (current length)
	cmp r0, #0 				// check ifs a null-byte, meaning we reached end of string
	beq jump_to_lr
	
	// its a "valid" char, lets convert it to lowercase and store it in memory (if needed)
	bl convert_to_lowercase
	
	add r1, r1, #1 			// increment length
	b check_input_loop 		// continue loop at next char
	
convert_to_lowercase:
	// convert to lowercase
	orr r2, r0, #0x20
	cmp r0, r2 				// check if it was lowercase to start with, so we dont do an uneccesary i/o
	beq exit_conversion		// if its uppercase we store it in memory
	strb r2, [r4, r1]		// store byte r3 in memory address r4 at offset r1
	
exit_conversion:
	bx lr					// no need for stack, since we are not going deeper
	

// function that checks for a palindrome
check_palindrome:
	mov r0, #0 				// ensure that the counter start at 0
	sub r1, r1, #1 			// remove one from length since we are not dealing in indexes (offsets)
	
check_palindrome_loop:
	// We check for a palindrome by checking the "edges" of the string.
	// As we do more iterations the edges are shifted towards the center of the string.
	// If we encounter a case where on of the edges is whitespace (0x20), we simply
	// shift this to the right/left, depending from which side its from.
	// If two chars are unequal (and not whitespace) we know the string is not a palindrome.
	// r0 is our left-to-center counter and r1 is our right-to-center counter.
	// r2 holds current left char and r3 holds current right char
	// In essence, we have 5 cases:
	// 1: left == " " => continue with offsets left+1 and right
	// 2: right == " " => continue with offsets left and right-1
	// 3: left == right => continue with offsets left+1 and right-1
	// 4: left != right => branch to palindrome_not_found
	// 5: r8 >= r9 => branch to palindrome_found
	cmp r0, r1				// check if r0 >= r1
	bge palindrome_found	// branches if r0 >= r1, meaning we have a palindrome
	
	ldrb r2, [r4, r0]		// load byte at offset r0
	ldrb r3, [r4, r1]		// load byte at offset r1
	
	cmp r2, r3				// check if equal
	beq chars_equal
	
	cmp r2, #0x20			// check whether left char is whitespace
	beq left_char_whitespace
	
	cmp r3, #0x20			// check wheter right char is whitespace
	beq right_char_whitespace
	
	// else, they are not equal and not a palindrome
	b palindrome_not_found
	
chars_equal:
	add r0, r0, #1 			// increase left offset
	sub r1, r1, #1 			// decrease right offset
	b check_palindrome_loop // continue at next iteration

left_char_whitespace:
	add r0, r0, #1 			// increase left offset
	b check_palindrome_loop // continue at next iteration

right_char_whitespace:
	sub r1, r1, #1 			// decrease right offset
	b check_palindrome_loop // continue at next iteration


// function for when a palindrome is found. Lights up 5 rightmost LEDs and writes to JTAG UART
palindrome_found:
	// we want to light up the 5 rightmost leds, meaning the 5 least significant bits
	mov r1, #0x1f 			// 0x1f = 0b00011111
	
	// add lr to stack
	sub sp, sp, #4 			// make room for stack pointer
	str lr, [sp]
	
	bl write_to_led 		// branch to write_to_led_function
	
	ldr r0, =palindrome_detected // put palindrome_detected string in r0
	bl write_to_jtag
	
	b jump_to_lr
	

// function for when a palindrome is not found. Lights up 5 lefttmost LEDs and writes to JTAG UART.
palindrome_not_found:
	// Switch on only the 5 leftmost LEDs
	// Write 'Not a palindrome' to UART
	// bits 10-5 should be lit up, we can orr with 0x3d0
	mov r1, #0x3e0
	
	// add lr to stack
	sub sp, sp, #4			// make room for stack pointer
	str lr, [sp]
	
	bl write_to_led 		// branch to write_to_led function
	
	ldr r0, =no_palindrome 	// put no_palindrome string in r0
	bl write_to_jtag
	
	b jump_to_lr
	
// function for writing to the LEDs
write_to_led:
	// Only the 10 least significant bit are used. AND its content with 0xfffffc00 to reset
	ldr r2, [r6] 			// load LED into register
	ldr r3, =0xfffffc00 	// load the right side for the AND operation
	and r2, r2, r3 			// reset its content, i.e 10 lsbs are set to 0
	
	// r1 will decide whether bits 10-5 are lit up or 4-0
	orr r2, r2, r1
	str r2, [r6]
	
	bx lr // jump back to caller


// function for writing to JTAG UART, it is lightly inspired by the docs of the JTAG UART
// in the armv7 de1soc docs.
write_to_jtag:
	sub sp, sp, #4 			// make room for stack pointer
	str lr, [sp] 			// store lr in stack
	mov r2, #0				// pointer for current char

write_to_jtag_loop:
	ldrb r1, [r0, r2] 		// the string is decided by the caller
	cmp r1, #0				// check if we reached the end of the loop
	beq jump_to_lr 			// jump back to caller
	bl put_jtag				// else we put the input in the jtag
	add r2, r2, #1 			// increment our pointer
	b write_to_jtag_loop 	// continue at next iteration

put_jtag:
	str r1, [r5] 			// write to JTAG UART
	bx lr
	
	
jump_to_lr:
	ldr lr, [sp] 			// load the value from the stack pointer into the link register
	add sp, sp, #4 			// "pop" from the stack
	bx lr 					// branch to the link register address

	
exit:
	b exit
	

.section .data
.align
	palindrome_detected: .asciz "Palindrome detected\n"
	no_palindrome: .asciz "Not a palindrome\n"
	input: .asciz "level"
	// input: .asciz "8448"
    // input: .asciz "KayAk"
    // input: .asciz "step on no pets"
    // input: .asciz "Never odd or even"

.end
