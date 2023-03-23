.globl main

.section .rodata
intro: .string "On day %ld of Christmas my true love sent to me\n"
and_s: .string "and "

str01: .string "%ld partridge in a pear tree\n"
str02: .string "%ld turtle doves\n"
str03: .string "%ld french hens\n"
str04: .string "%ld calling birds\n"
str05: .string "%ld gold rings\n"
str06: .string "%ld geese a-laying\n"
str07: .string "%ld swans a-swimming\n"
str08: .string "%ld maids a-milking\n"
str09: .string "%ld ladies dancing\n"
str10: .string "%ld lords a-leaping\n"
str11: .string "%ld pipers piping\n"
str12: .string "%ld drummers drumming\n"

lines:
    .quad str01, str02, str03, str04, str05, str06
    .quad str07, str08, str09, str10, str11, str12

.section .text
/* The main function. */
main:
    pushq %rbp
    movq %rsp, %rbp

    # set up main loop counter
    movq $0, %r12

    # check if we have reached the end of the loop
    main_loop_test:
    cmp $12, %r12   # set flags
    jge main_loop_end # if r12 - 12 >= 0 we are finished

    main_loop:
    # prepare argument for print_verse function
    movq %r12, %rdi
    movq $0, %rsi # so we get even number of arguments when calling function
    call print_verse
    # increment counter and jump to test condition
    inc %r12
    jmp main_loop_test

    main_loop_end:
    # we are finished, do cleanup
    leave
    ret


/* Function for printing a verse given by the parameter N.
 */
print_verse:
    # Push base pointer to stack, place the stack pointer in base pointer
    pushq %rbp
    movq %rsp, %rbp

    # place argument in r13, we can use this to decrement, i.e reversing the loop
    # r13 will also works as our index for iterating over the lines arrau
    movq %rdi, %r13
    
    # test and set register if rdi != 0. This means that if rdi != 0, then we should
    # print "and " before the last line in a verse
    # defualt value of 0 (don't print)
    movq $0, %r14
    contain_and_test:
    cmp $0, %rdi
    je continue
    add $1, %r14 # r14 holds a flag if it shoudl print "and" at the last iteration

    continue:

    # set up base pointer for the array, we place it in r15
    movq $lines, %r15

    # print the intro
    movq %r13, %rsi
    add $1, %rsi
    movq $intro, %rdi
    call printf

    # check if we have reached the end of the loop (r13 < 0)
    loop_test:
    cmp $0, %r13
    je last_iteration # if r13 == 0, then we are at the last iteration
    jl end_loop

    loop_body:
    # store number of x we give, which should be r13 + 1
    # we can place the result in rsi, since this is our 2nd parameter in the print function
    movq %r13, %rsi
    add $1, %rsi

    # now fetch the string
    movq (%r15, %r13, 8), %rdi

    # call print
    call printf
    # decrement "counter" and jump back to test
    dec %r13
    jmp loop_test

    last_iteration:
    # last iteration
    # check if flag is set
    cmp $0, %r14
    # it not set, just jump to loop_body
    je loop_body
    # Else we print "and "
    movq $and_s, %rdi
    movq $0, %rsi
    call printf
    jmp loop_body

    end_loop:
    # add extra space
    movq $'\n', %rdi
    call putchar
    # cleanup
    leave
    ret
