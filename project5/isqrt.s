# Name: Darius Kianersi
# UID: 118799810
# Username: radius

    .text

isqrt:
    # PROLOGUE
    subu    $sp, $sp, 8     # grow stack by 8
    sw      $ra, 8($sp)     # store the return address
    sw      $fp, 4($sp)     # store the frame pointer
    addu    $fp, $sp, 8     # set the new frame pointer

    # BODY
    bge     $a0, 2, rec     # continue function if n >= 2
    move    $v0, $a0        # move n to v0
    j		ret				# jump to return

rec:
    subu    $sp, $sp, 4     # grow stack by 4
    sw      $a0, 4($sp)     # push n to stack
    srl		$a0, $a0, 2	    # $a0 = n >> 2
    jal     isqrt           # recur
    sll		$t0, $v0, 1	    # left shift the returned val
    addu    $t1, $t0, 1     # large = small + 1
    mult    $t1, $t1        # hi, lo = large*large
    mfhi    $t2             # store hi in t2
    mflo    $t3             # store lo in t3
    bgtz    $t2, rsmall     # jump if hi is > 0
    lw      $t4, 4($sp)     # pop n from stack -> t4
    addu    $sp, $sp, 4     # shrink stack by 4
    bgt     $t3, $t4, rsmall# jump if large*large > n
    move    $v0, $t1        # store large in v0
    j       ret

rsmall:
    move    $v0, $t0        # store small in v0
    j       ret

ret:
    # EPILOGUE
    move    $sp, $fp        # set stack pointer to frame pointer
    lw      $ra, ($fp)      # load return address
    lw      $fp, -4($fp)    # restore old frame pointer
    j		$ra				# return
    