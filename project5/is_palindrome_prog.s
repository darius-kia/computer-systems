   .data
str1:
   .asciiz "abba"
str2:
   .asciiz "racecar"
str3:
   .asciiz "swap paws",
str4:
   .asciiz "not a palindrome"
str5:
   .asciiz "another non palindrome"
str6:
   .asciiz "almost but tsomla"

# array of char pointers = {&str1, &str2, ..., &str6}
ptr_arr:
   .word str1, str2, str3, str4, str5, str6, 0

yes_str:
   .asciiz " --> Y\n"
no_str:
   .asciiz " --> N\n"

   .text

# main(): ##################################################
#   char ** j = ptr_arr
#   while (*j != 0):
#     rval = is_palindrome(*j)
#     printf("%s --> %c\n", *j, rval ? yes_str: no_str)
#     j++
#
main:
   li   $sp, 0x7ffffffc    # initialize $sp

   # PROLOGUE
   subu $sp, $sp, 8        # expand stack by 8 bytes
   sw   $ra, 8($sp)        # push $ra (ret addr, 4 bytes)
   sw   $fp, 4($sp)        # push $fp (4 bytes)
   addu $fp, $sp, 8        # set $fp to saved $ra

   subu $sp, $sp, 8        # save s0, s1 on stack before using them
   sw   $s0, 8($sp)        # push $s0
   sw   $s1, 4($sp)        # push $s1

   la   $s0, ptr_arr        # use s0 for j. init ptr_arr
main_while:
   lw   $s1, ($s0)         # use s1 for *j
   beqz $s1, main_end      # while (*j != 0):
   move $a0, $s1           #    print_str(*j)
   li   $v0, 4
   syscall
   move $a0, $s1           #    v0 = is_palindrome(*j)
   jal  is_palindrome
   beqz $v0, main_print_no #    if v0 != 0:
   la   $a0, yes_str       #       print_str(yes_str)
   b    main_print_resp
main_print_no:             #    else:
   la   $a0, no_str        #       print_str(no_str)
main_print_resp:
   li   $v0, 4
   syscall

   addu $s0, $s0, 4       #     j++
   b    main_while        # end while
main_end:

   # EPILOGUE
   move $sp, $fp           # restore $sp
   lw   $ra, ($fp)         # restore saved $ra
   lw   $fp, -4($sp)       # restore saved $fp
   j    $ra                # return to kernel
# end main ################################################
# Name: Darius Kianersi
# UID: 118799810
# Username: radius

    .text

strlen:
   # PROLOGUE
   subu  $sp, $sp, 8        # expand stack by 8 bytes
   sw    $ra, 8($sp)        # push $ra (ret addr, 4 bytes)
   sw    $fp, 4($sp)        # push $fp (4 bytes)
   addu  $fp, $sp, 8        # set $fp to saved $ra

   # BODY
   li    $t0, 0             # initialize counter
loop:
   lb    $t1, ($a0)         # initialize pointer
   beqz  $t1, endloop       # break loop at end of string
   addiu $a0, $a0, 1        # increment pointer
   addi  $t0, $t0, 1        # increment counter
   j     loop
endloop:
   move  $v0, $t0

   # EPILOGUE
   move  $sp, $fp           # restore $sp
   lw    $ra, ($fp)         # restore saved $ra
   lw    $fp, -4($sp)       # restore saved $fp
   j     $ra                # return to kernel

is_palindrome:
    # PROLOGUE
    subu    $sp, $sp, 8     # grow stack pointer
    sw      $ra, 8($sp)     # push return address to stack
    sw      $fp, 4($sp)     # push frame pointer to stack
    addu    $fp, $sp, 8     # set frame pointer

    # BODY
    subu    $sp, $sp, 8     # grow stack pointer for i and len
    li      $t0, 0          # set i to 0 and store in $t0
    sw      $t0, 8($sp)     # push i to stack
    jal     strlen          # call strlen (a0 still has str)
    move    $t1, $v0        # store len in $t1
    sw      $t1, 4($sp)     # push len to stack
    lw      $t0, 8($sp)     # store i from stack to $t0

    li      $t5, 2
    div		$t1, $t5		# $t1 / 2
    mflo	$t2				# store quotient in $t2

start:
    blt	    $t0, $t1, ploop # if i < len/2, continue loop
    li      $v0, 1          # store 1 in return
    j       end             # jump to end

ploop:
    addu    $a0, $a0, $t0   # increment $a0 by i
    lb      $t3, ($a0)      # store left char in $t3
    subu    $a0, $a0, $t0   # decrement $a0 by i

    addu    $a0, $a0, $t1   # increment $a0 by len
    subu    $a0, $a0, $t0   # decrement $a0 by i
    subu    $a0, $a0, 1     # decrement $a0 by 1
    lb      $t4, ($a0)      # store right char in $t4

    addu    $a0, $a0, 1     # increment $a0 by 1
    addu    $a0, $a0, $t0   # increment $a0 by i
    subu    $a0, $a0, $t1   # decrement $a0 by len

    bne     $t3, $t4 nequal # check if chars are equal
    addi    $t0, $t0, 1     # increment i
    j       start           # go to start of loop

nequal:
    li      $v0, 0          # store 0 in return

end:
    # PROLOGUE
    move    $sp, $fp        # restore stack pointer
    lw      $ra, ($fp)      # restore $ra
    lw      $fp, -4($sp)    # restore frame pointer
    j		$ra				# return to kernel