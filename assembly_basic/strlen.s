# strlen(str)

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
