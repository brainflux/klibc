#!/usr/bin/perl
#
# Make ARM syscall stubs
#

$minsyscall = 1;
$maxsyscall = 273;

for ( $i = $minsyscall ; $i <= $maxsyscall ; $i++ ) {
    open(STUB, '>', "sysstub/sys${i}.S")
	or die "$0: Cannot create sysstub/sys${i}.S: $!\n";
    print  STUB "\t.text\n";
    print  STUB "\t.align\t4\n";
    print  STUB "\t.globl\t__syscall${i}, #function\n";
    print  STUB "#ifdef __thumb__\n";
    print  STUB "\t.thumb_func\n";
    print  STUB "#endif\n";

    print  STUB "__syscall${i}:\n";
    print  STUB "#ifndef __thumb__\n"; # ARM version first
    printf STUB "\tswi\t#%#x\n", $i+0x90000;
    print  STUB "#else\n";	# Thumb version
    print  STUB "\tpush\t{r7}\n";
    printf STUB "\tmov\tr7, #%d\n", $i;
    print  STUB "\tswi\t0\n";
    print  STUB "\tpop\t{r7}\n";
    print  STUB "#endif\n";
    print  STUB "\tb __syscall_common\n";

    print  STUB "\t.size\t__syscall${i},.-__syscall${i}\n";
}
