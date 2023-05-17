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
    subu    $sp, $sp, 12    # grow stack pointer for i, len, and str
    li      $t0, 0          # set i to 0 and store in $t0
    sw      $t0, 12($sp)    # push i to stack
    sw      $a0, 8($sp)     # push str to stack
    jal     strlen          # call strlen (a0 still has str)
    move    $t1, $v0        # store len in $t1
    sw      $t1, 4($sp)     # push len to stack
    lw      $a0, 8($sp)     # store str from stack to $a0
    lw      $t0, 12($sp)    # store i from stack to $t0

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


# the issue seems to be that on line 58, the char is not being stored in $t3, it has
# a value of 0 in spim even though line 64 is working correctly and $t4 gets the last char