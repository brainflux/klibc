# -*- perl -*-
#
# arch/cris/sysstub.ph
#
# Script to generate system call stubs
#

sub make_sysstub($$$$@) {
    my($fname, $type, $sname, $stype, @args) = @_;

    open(OUT, '>', "syscalls/${fname}.S");
    print OUT "#include <asm/unistd.h>\n";
    print OUT "\n";
    print OUT "\t.text\n";
    print OUT "\t.type\t_${fname},\@function\n";
    print OUT "\t.globl\t_${fname}\n";
    print OUT "\t.balign\t4\n";
    print OUT "_${fname}:\n";
    print OUT "\tjump\t___syscall_common\n";
    print OUT "#if __NR_${sname} <= 31\n";
    print OUT "\t  moveq\t__NR_${sname}, \$r9\n";
    print OUT "#else\n";
    print OUT "\t  move.d\t__NR_${sname}, \$r9\n";
    print OUT "#endif\n";
    print OUT "\t.size _${fname},.-_${fname}\n";
    close(OUT);
}

1;
