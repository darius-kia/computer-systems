# Name: Darius Kianersi
# UID: 118799810
# Username: radius

    .text

fibonacci:
    # PROLOGUE
    subu    $sp, $sp, 8     # grow stack by 8
    sw      $ra, 8($sp)     # store return address
    sw      $fp, 4($sp)     # store frame pointer
    addu    $fp, $sp, 8     # set frame pointer

    # BODY
    bgt		$a0, 1, rec
    move    $v0, $a0        # return n if n <= 1
    j       ret

rec:
    subu    $sp, $sp, 4     # grow stack for n
    sw      $a0, 4($sp)     # push n to stack

    subu    $a0, $a0, 1     # subtract 1 from n
    jal     fibonacci       # recur with fib(n-1)
    subu    $sp, $sp, 4     # grow stack for return value
    sw      $v0, 4($sp)     # push return value to stack

    lw      $a0, 8($sp)     # load n to a0
    subu    $a0, $a0, 2     # subtract 2 from n
    jal     fibonacci       # recur with fib(n-2)
    lw      $t0, 4($sp)     # load fib(n-1) to $t0
    addu    $v0, $v0, $t0   # add fib(n-1) + fib(n-2)

ret:
    # EPILOGUE
    move    $sp, $fp        # restore $sp
    lw      $ra, ($fp)      # restore saved $ra
    lw      $fp, -4($fp)    # restore saved $fp
    j       $ra             # return to kernel
