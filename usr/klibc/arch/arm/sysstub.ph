# -*- perl -*-
#
# arch/arm/sysstub.ph
#
# Script to generate system call stubs
#


sub make_sysstub($$$$$@) {
    my($outputdir, $fname, $type, $sname, $stype, @args) = @_;

    open(OUT, '>', "${outputdir}/${fname}.S");
    print  OUT "#include <asm/unistd.h>\n";
    print  OUT "#include <klibc/asmmacros.h>\n";

    print  OUT "	.text\n";
    print  OUT "	.type	${fname}, #function\n";
    print  OUT "	.globl	${fname}\n";

    print  OUT "#ifndef __thumb__\n";
    print  OUT "	.align	4\n";

    print  OUT "#ifndef __ARM_EABI__\n";

    # ARM version first
    print  OUT "${fname}:\n";
    print  OUT "	stmfd	sp!,{r4,r5,lr}\n";
    print  OUT "	ldr	r4,[sp,#12]\n";
    print  OUT "	ldr	r5,[sp,#16]\n";
    print  OUT "	swi	# __NR_${sname}\n";
    print  OUT "	b	__syscall_common\n";

    print  OUT "#else /* __ARM_EABI__ */\n";

    # ARM EABI version
    print  OUT "${fname}:\n";
    print  OUT "	stmfd	sp!,{r4,r5,r7,lr}\n";
    print  OUT "#if ARM_VALID_IMM(__NR_${sname})\n";
    print  OUT "	mov	r7, # __NR_${sname}\n";
    print  OUT "	b	__syscall_common\n";
    print  OUT "#else\n";
    print  OUT "	mov	r7, # (__NR_${sname} & 0xff00)\n";
    print  OUT "	add	r7, r7, # (__NR_${sname} & 0xff)\n";
    print  OUT "	b	__syscall_common\n";
    print  OUT "#endif\n";

    print  OUT "#endif /* __ARM_EABI__ */\n";
    print  OUT "#else /* __thumb__ */\n";

    # Thumb version
    print  OUT "	.align	4\n";
    print  OUT "	.thumb_func\n";
    print  OUT "${fname}:\n";
    print  OUT "	push	{r4,r5,r7,lr}\n";
    print  OUT "#if __NR_${sname} <= 0xff\n";
    print  OUT "	mov	r7, # __NR_${sname}\n";
    print  OUT "	bl	__syscall_common\n";
    print  OUT "#else\n";
    print  OUT "	ldr	r7, 1f\n";
    print  OUT "	bl	__syscall_common\n";
    print  OUT "	.align	4\n";
    print  OUT "1:\n";
    print  OUT "	.word	__NR_${sname}\n";
    print  OUT "#endif\n";

    print  OUT "#endif /* __thumb__*/\n";

    print  OUT "	.size	__syscall${i},.-__syscall${i}\n";
}

1;
