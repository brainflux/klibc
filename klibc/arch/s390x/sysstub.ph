# -*- perl -*-
#
# arch/s390x/sysstub.ph
#
# Script to generate system call stubs
#

sub make_sysstub($$$$@) {
    my($fname, $type, $sname, $stype, @args) = @_;

    open(OUT, '>', "syscalls/${fname}.S");
    print OUT "#include <asm/unistd.h>\n";
    print OUT "\n";
    print OUT "\t.type ${fname},\@function\n";
    print OUT "\t.globl ${fname}\n";
    print OUT "${fname}:\n";
    print OUT ".if __NR_${sname} < 256\n";
    print OUT "\tsvc __NR_${sname}\n";
    print OUT ".else\n";
    print OUT "\tlghi %r0,__NR_${sname}\n";
    print OUT "\tsvc 0\n";
    print OUT ".endif\n";
    print OUT "\tbr %r14\n";
    print OUT "\t.size ${fname},.-${fname}\n";
    close(OUT);
}

1;
