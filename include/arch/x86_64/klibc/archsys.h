/*
 * arch/x86_64/include/klibc/archsys.h
 *
 * Architecture-specific syscall definitions
 */

#ifndef _KLIBC_ARCHSYS_H
#define _KLIBC_ARCHSYS_H

/* x86-64 seems to miss _syscall6() from its headers */

#ifndef _syscall6

#define _syscall6(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4, \
          type5,arg5,type6,arg6) \
type name (type1 arg1,type2 arg2,type3 arg3,type4 arg4,type5 arg5,type6 arg6) \
{ \
long __res; \
register long __r10 asm("%r10") = (long)arg4;
register long __r8  asm("%r8")  = (long)arg5;
register long __r9  asm("%r9")  = (long)arg6;
__asm__ volatile (__syscall \
        : "=a" (__res) \
        : "0" (__NR_##name),"D" ((long)(arg1)),"S" ((long)(arg2)), \
          "d" ((long)(arg3)),"r" ((long)(arg4)),"r" ((long)(arg5)),
          "r" ((long)(arg6)) : \
        __syscall_clobber); \
__syscall_return(type,__res); \
}

#endif /* _syscall6 missing */

#endif /* _KLIBC_ARCHSYS_H */
