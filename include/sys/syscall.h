/*
 * sys/syscall.h
 *
 * Generic system call interface macros
 */
#ifndef _SYS_SYSCALL_H

#include <errno.h>
#include <asm/unistd.h>

/*
 * If we're compiling i386 in PIC mode, we need to treat %ebx
 * specially.  Most of these are copied from the equivalent file in
 * newlib and were written by Werner Almesberger.
 */
#if defined(__PIC__) && defined(__i386__)

#undef _syscall1
#define _syscall1(type,name,type1,arg1) \
type name(type1 arg1) \
{ \
long __res; \
__asm__ volatile ("push %%ebx; movl %2,%%ebx; int $0x80; pop %%ebx" \
	: "=a" (__res) \
	: "0" (__NR_##name),"r" ((long)(arg1))); \
__syscall_return(type,__res); \
}

#undef _syscall2
#define _syscall2(type,name,type1,arg1,type2,arg2) \
type name(type1 arg1,type2 arg2) \
{ \
long __res; \
__asm__ volatile ("push %%ebx; movl %2,%%ebx; int $0x80; pop %%ebx" \
	: "=a" (__res) \
	: "0" (__NR_##name),"r" ((long)(arg1)),"c" ((long)(arg2))); \
__syscall_return(type,__res); \
}

#undef _syscall3
#define _syscall3(type,name,type1,arg1,type2,arg2,type3,arg3) \
type name(type1 arg1,type2 arg2,type3 arg3) \
{ \
long __res; \
__asm__ volatile ("push %%ebx; movl %2,%%ebx; int $0x80; pop %%ebx" \
	: "=a" (__res) \
	: "0" (__NR_##name),"r" ((long)(arg1)),"c" ((long)(arg2)), \
		"d" ((long)(arg3))); \
__syscall_return(type,__res); \
}

#undef _syscall4
#define _syscall4(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4) \
type name (type1 arg1, type2 arg2, type3 arg3, type4 arg4) \
{ \
long __res; \
__asm__ volatile ("push %%ebx; movl %2,%%ebx; int $0x80; pop %%ebx" \
	: "=a" (__res) \
	: "0" (__NR_##name),"r" ((long)(arg1)),"c" ((long)(arg2)), \
	  "d" ((long)(arg3)),"S" ((long)(arg4))); \
__syscall_return(type,__res); \
}

#undef _syscall5
#define _syscall5(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4, \
          type5,arg5) \
type name (type1 arg1,type2 arg2,type3 arg3,type4 arg4,type5 arg5) \
{ \
long __res; \
__asm__ volatile ("push %%ebx; movl %2,%%ebx; int $0x80; pop %%ebx" \
	: "=a" (__res) \
	: "0" (__NR_##name),"m" ((long)(arg1)),"c" ((long)(arg2)), \
	  "d" ((long)(arg3)),"S" ((long)(arg4)),"D" ((long)(arg5))); \
__syscall_return(type,__res); \
}

#undef _syscall6
#define _syscall6(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4, \
          type5,arg5,type6,arg6) \
type name (type1 arg1,type2 arg2,type3 arg3,type4 arg4,type5 arg5,type6 arg6) \
{ \
long __res; \
__asm__ volatile ("push %%ebx; pushl %%ebp; movl %2,%%ebx; " \
                  "movl %%eax,%%ebp; movl %1,%%eax; int $0x80; " \
                  "pop %%ebp ; pop %%ebx" \
	: "=a" (__res) \
	: "i" (__NR_##name),"m" ((long)(arg1)),"c" ((long)(arg2)), \
	  "d" ((long)(arg3)),"S" ((long)(arg4)),"D" ((long)(arg5)), \
	  "a" ((long)(arg6))); \
__syscall_return(type,__res); \
}

#elif defined(__powerpc__)

/* PowerPC seems to lack _syscall6() in its headers */
/* This seems to work on both 32- and 64-bit ppc */

#ifndef _syscall6

#define _syscall6(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4, \
          type5,arg5,type6,arg6) \
type name (type1 arg1,type2 arg2,type3 arg3,type4 arg4,type5 arg5,type6 arg6) \
{ \
        unsigned long __sc_ret, __sc_err;                               \
        {                                                               \
                register unsigned long __sc_0 __asm__ ("r0");           \
                register unsigned long __sc_3 __asm__ ("r3");           \
                register unsigned long __sc_4 __asm__ ("r4");           \
                register unsigned long __sc_5 __asm__ ("r5");           \
                register unsigned long __sc_6 __asm__ ("r6");           \
                register unsigned long __sc_7 __asm__ ("r7");           \
                register unsigned long __sc_8 __asm__ ("r8");           \
                                                                        \
                __sc_3 = (unsigned long) (arg1);                        \
                __sc_4 = (unsigned long) (arg2);                        \
                __sc_5 = (unsigned long) (arg3);                        \
                __sc_6 = (unsigned long) (arg4);                        \
                __sc_7 = (unsigned long) (arg5);                        \
                __sc_8 = (unsigned long) (arg6);                        \
                __sc_0 = __NR_##name;                                   \
                __asm__ __volatile__                                    \
                        ("sc           \n\t"                            \
                         "mfcr %1      "                                \
                        : "=&r" (__sc_3), "=&r" (__sc_0)                \
                        : "0"   (__sc_3), "1"   (__sc_0),               \
                          "r"   (__sc_4),                               \
                          "r"   (__sc_5),                               \
                          "r"   (__sc_6),                               \
                          "r"   (__sc_7),                               \
                          "r"   (__sc_8)                                \
                        : __syscall_clobbers);                          \
                __sc_ret = __sc_3;                                      \
                __sc_err = __sc_0;                                      \
        }                                                               \
        __syscall_return (type);                                        \
}

#endif

#endif

#endif /* SYSCALL_H */
