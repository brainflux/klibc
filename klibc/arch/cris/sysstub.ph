# -*- perl -*-
#
# arch/cris/sysstub.ph
#
# Script to generate system call stubs
#

# We use a subsection for the syscall stubs, so we can be
# sure they're within reach of a 16-bit BA

sub make_sysstub($$$$@) {
    my($fname, $type, $sname, $stype, @args) = @_;

    open(OUT, '>', "syscalls/${fname}.S");
    print OUT "#include <asm/unistd.h>\n";
    print OUT "\n";
    print OUT "\t.set noreorder\n";
    print OUT "\n";
    print OUT "\t.text\n";
    print OUT "\t.subsection \".syscall\"\n";
    print OUT "\t.type ${fname},\@function\n";
    print OUT "\t.globl ${fname}\n";
    print OUT "${fname}:\n";
    print OUT "\tba\t__syscall_common\n";
    print OUT "\t  move.d\t__NR_${sname}, $r9\n";
    print OUT "\t.size ${fname},.-${fname}\n";
    close(OUT);
}

1;
