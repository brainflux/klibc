/*
 * arch/i386/include/klibc/archsys.h
 *
 * Architecture-specific syscall definitions
 */

#ifndef _KLIBC_ARCHSYS_H
#define _KLIBC_ARCHSYS_H

/*
 * All syscalls go via stub function
 */
extern long __syscall(int, ...);


#undef _syscall0
#define _syscall0(type,name) \
type name(void) \
{ \
	return (type)__syscall(__NR_##name); \
}


#undef _syscall1
#define _syscall1(type,name,type1,arg1) \
type name(type1 arg1) \
{ \
	return (type)__syscall(__NR_##name, arg1); \
}

#undef _syscall2
#define _syscall2(type,name,type1,arg1,type2,arg2) \
type name(type1 arg1,type2 arg2) \
{ \
	return (type)__syscall(__NR_##name, arg1, arg2); \
}

#undef _syscall3
#define _syscall3(type,name,type1,arg1,type2,arg2,type3,arg3) \
type name(type1 arg1,type2 arg2,type3 arg3) \
{ \
	return (type)__syscall(__NR_##name, arg1, arg2, arg3); \
}

#undef _syscall4
#define _syscall4(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4) \
type name (type1 arg1, type2 arg2, type3 arg3, type4 arg4) \
{ \
	return (type)__syscall(__NR_##name, arg1, arg2, arg3, arg4); \
}

#undef _syscall5
#define _syscall5(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4, \
          type5,arg5) \
type name (type1 arg1,type2 arg2,type3 arg3,type4 arg4,type5 arg5) \
{ \
	return (type)__syscall(__NR_##name, arg1, arg2, arg3, arg4, arg5); \
}

#undef _syscall6
#define _syscall6(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4, \
          type5,arg5,type6,arg6) \
type name (type1 arg1,type2 arg2,type3 arg3,type4 arg4,type5 arg5,type6 arg6) \
{ \
	return (type)__syscall(__NR_##name, arg1, arg2, arg3, arg4, arg5, arg6); \
}

#endif /* _KLIBC_ARCHSYS_H */
