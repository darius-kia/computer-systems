# mash(x, y)

mash:
   # STORE ARGS
   lw   $t0, 4($sp)        # store x in t0
   lw   $t1, 8($sp)        # store y in t1

   # PROLOGUE
   subu $sp, $sp, 8        # expand stack by 8 bytes
   sw   $ra, 8($sp)        # push $ra (ret addr, 4 bytes)
   sw   $fp, 4($sp)        # push $fp (4 bytes)
   addu $fp, $sp, 8        # set $fp to saved $ra

   # BODY
   mul  $t0, $t0, 10       # x = x * 10
   addu $t0, $t0, $t1      # x = x + y
   move $v0, $t0

   # EPILOGUE
   move $sp, $fp           # restore $sp
   lw   $ra, ($fp)         # restore saved $ra
   lw   $fp, -4($sp)       # restore saved $fp
   j    $ra                # return to kernel