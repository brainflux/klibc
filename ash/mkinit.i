# 1 "mkinit.c"
 




































char copyright[] =
"@(#) Copyright (c) 1991 The Regents of the University of California.\n All rights reserved.\n";




 
static char rcsid[] = "mkinit.c,v 1.4 1993/08/01 18:58:09 mycroft Exp";


 











# 1 "/usr/include/sys/cdefs.h" 1 3
 




















 

# 1 "/usr/include/features.h" 1 3
 




















 



























































 



















 





 



 







 
# 138 "/usr/include/features.h" 3


 









 





 



























# 196 "/usr/include/features.h" 3


































 



 








 




 

# 1 "/usr/include/sys/cdefs.h" 1 3
 

















# 166 "/usr/include/sys/cdefs.h" 3

# 250 "/usr/include/features.h" 2 3


 








 





 

 








# 1 "/usr/include/gnu/stubs.h" 1 3
 






































# 278 "/usr/include/features.h" 2 3




# 24 "/usr/include/sys/cdefs.h" 2 3



 





 








 




# 71 "/usr/include/sys/cdefs.h" 3


 







 



# 103 "/usr/include/sys/cdefs.h" 3



 








 















 








 








 









 







# 60 "mkinit.c" 2

# 1 "../klibc/include/stdio.h" 1
 






# 1 "../klibc/include/klibc/extern.h" 1
 













# 8 "../klibc/include/stdio.h" 2

# 1 "../klibc/include/stdarg.h" 1
 






 
# 1 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h" 1 3
 





























































 






typedef void *__gnuc_va_list;



 



 














void va_end (__gnuc_va_list);		 


 



 












 






















 
 













# 168 "/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h" 3


 




 

 

 

typedef __gnuc_va_list va_list;
























# 9 "../klibc/include/stdarg.h" 2


 



# 9 "../klibc/include/stdio.h" 2

# 1 "../klibc/include/stddef.h" 1
 








# 1 "../klibc/include/bits32/bitsize/stddef.h" 1
 










typedef unsigned int size_t;



typedef signed int   ptrdiff_t;


# 10 "../klibc/include/stddef.h" 2

















# 10 "../klibc/include/stdio.h" 2

# 1 "../klibc/include/unistd.h" 1
 








# 1 "../klibc/include/sys/types.h" 1
 










# 1 "../klibc/include/stdint.h" 1
 






# 1 "../klibc/include/bits32/bitsize/stdint.h" 1
 






typedef signed char 		int8_t;
typedef short int		int16_t;
typedef int			int32_t;
typedef long long int		int64_t;

typedef unsigned char 		uint8_t;
typedef unsigned short int	uint16_t;
typedef unsigned int		uint32_t;
typedef unsigned long long int	uint64_t;

typedef int			int_fast16_t;
typedef int			int_fast32_t;

typedef unsigned int		uint_fast16_t;
typedef unsigned int		uint_fast32_t;

typedef int			intptr_t;
typedef unsigned int		uintptr_t;









# 8 "../klibc/include/stdint.h" 2


typedef int8_t   int_least8_t;
typedef int16_t  int_least16_t;
typedef int32_t  int_least32_t;
typedef int64_t  int_least64_t;

typedef uint8_t  uint_least8_t;
typedef uint16_t uint_least16_t;
typedef uint32_t uint_least32_t;
typedef uint64_t uint_least64_t;

typedef int8_t   int_fast8_t;
typedef int64_t  int_fast64_t;

typedef uint8_t  uint_fast8_t;
typedef uint64_t uint_fast64_t;

typedef int64_t  intmax_t;
typedef uint64_t uintmax_t;














































# 1 "../klibc/include/bits32/bitsize/stdintlimits.h" 1
 





















# 74 "../klibc/include/stdint.h" 2



































# 1 "../klibc/include/bits32/bitsize/stdintconst.h" 1
 

















# 109 "../klibc/include/stdint.h" 2





# 12 "../klibc/include/sys/types.h" 2



typedef ptrdiff_t ssize_t;

# 1 "../klibc/linux/include/linux/posix_types.h" 1



# 1 "../klibc/linux/include/linux/stddef.h" 1














# 4 "../klibc/linux/include/linux/posix_types.h" 2


 










 


















typedef struct {
	unsigned long fds_bits [(1024 / (8 * sizeof(unsigned long)) ) ];
} __kernel_fd_set;

 
typedef void (*__kernel_sighandler_t)(int);

 
typedef int __kernel_key_t;

# 1 "../klibc/linux/include/asm/posix_types.h" 1



 





typedef unsigned short	__kernel_dev_t;
typedef unsigned long	__kernel_ino_t;
typedef unsigned short	__kernel_mode_t;
typedef unsigned short	__kernel_nlink_t;
typedef long		__kernel_off_t;
typedef int		__kernel_pid_t;
typedef unsigned short	__kernel_ipc_pid_t;
typedef unsigned short	__kernel_uid_t;
typedef unsigned short	__kernel_gid_t;
typedef unsigned int	__kernel_size_t;
typedef int		__kernel_ssize_t;
typedef int		__kernel_ptrdiff_t;
typedef long		__kernel_time_t;
typedef long		__kernel_suseconds_t;
typedef long		__kernel_clock_t;
typedef int		__kernel_daddr_t;
typedef char *		__kernel_caddr_t;
typedef unsigned short	__kernel_uid16_t;
typedef unsigned short	__kernel_gid16_t;
typedef unsigned int	__kernel_uid32_t;
typedef unsigned int	__kernel_gid32_t;

typedef unsigned short	__kernel_old_uid_t;
typedef unsigned short	__kernel_old_gid_t;


typedef long long	__kernel_loff_t;


typedef struct {



	int	__val[2];

} __kernel_fsid_t;

# 78 "../klibc/linux/include/asm/posix_types.h"



# 46 "../klibc/linux/include/linux/posix_types.h" 2



# 17 "../klibc/include/sys/types.h" 2

# 1 "../klibc/linux/include/asm/types.h" 1



typedef unsigned short umode_t;

 




typedef __signed__ char __s8;
typedef unsigned char __u8;

typedef __signed__ short __s16;
typedef unsigned short __u16;

typedef __signed__ int __s32;
typedef unsigned int __u32;


typedef __signed__ long long __s64;
typedef unsigned long long __u64;


 


# 55 "../klibc/linux/include/asm/types.h"



# 18 "../klibc/include/sys/types.h" 2


 


# 152 "../klibc/include/sys/types.h"


 




struct ustat {
	__kernel_daddr_t	f_tfree;
	__kernel_ino_t		f_tinode;
	char			f_fname[6];
	char			f_fpack[6];
};


# 10 "../klibc/include/unistd.h" 2

# 1 "../klibc/include/sys/select.h" 1
 






# 1 "../klibc/include/sys/time.h" 1
 







# 1 "../klibc/linux/include/linux/time.h" 1



# 1 "../klibc/linux/include/asm/param.h" 1


























# 4 "../klibc/linux/include/linux/time.h" 2

# 1 "../klibc/linux/include/linux/types.h" 1
# 152 "../klibc/linux/include/linux/types.h"

# 5 "../klibc/linux/include/linux/time.h" 2




struct timespec {
	time_t	tv_sec;		 
	long	tv_nsec;	 
};


struct timeval {
	time_t		tv_sec;		 
	suseconds_t	tv_usec;	 
};

struct timezone {
	int	tz_minuteswest;	 
	int	tz_dsttime;	 
};

# 120 "../klibc/linux/include/linux/time.h"















 







struct  itimerspec {
        struct  timespec it_interval;     
        struct  timespec it_value;        
};

struct	itimerval {
	struct	timeval it_interval;	 
	struct	timeval it_value;	 
};


# 9 "../klibc/include/sys/time.h" 2


extern  int gettimeofday(struct timeval *, struct timezone *);
extern  int settimeofday(const struct timeval *, const struct timezone *);
extern  int getitimer(int, struct itimerval *);
extern  int setitimer(int, const struct itimerval *, struct itimerval *);


# 8 "../klibc/include/sys/select.h" 2



extern  int select(int, fd_set *, fd_set *, fd_set *, struct timeval *);


# 11 "../klibc/include/unistd.h" 2


extern  char **environ;
extern  void __attribute__((noreturn))  _exit(int);

extern  pid_t fork(void);
extern  pid_t vfork(void);
extern  pid_t getpid(void);
extern  int setpgid(pid_t, pid_t);
extern  pid_t getppid(void);
extern  pid_t getpgrp(void);
extern  int setpgrp(void);
extern  pid_t setsid(void);
extern  pid_t getsid(pid_t);
extern  int execv(const char *, char * const *);
extern  int execvp(const char *, char * const *);
extern  int execve(const char *, char * const *, char * const *);
extern  int execvpe(const char *, char * const *, char * const *);
extern  int execl(const char *, const char *, ...);
extern  int execlp(const char *, const char *, ...);
extern  int execle(const char *, const char *, ...);
extern  int execlpe(const char *, const char *, ...);

extern  int setuid(uid_t);
extern  uid_t getuid(void);
extern  int seteuid(uid_t);
extern  uid_t geteuid(void);
extern  int setgid(gid_t);
extern  gid_t getgid(void); 
extern  int setegid(gid_t);
extern  gid_t getegid(void);
extern  int getgroups(int, gid_t *);
extern  int setgroups(size_t, const gid_t *);
extern  int setreuid(uid_t, uid_t);
extern  int setregid(gid_t, gid_t);
extern  int setresuid(uid_t, uid_t, uid_t);
extern  int setresgid(gid_t, gid_t, gid_t);
extern  int getfsuid(uid_t);
extern  int setfsuid(uid_t);

extern  int access(const char *, int);
extern  int link(const char *, const char *);
extern  int unlink(const char *);
extern  int chdir(const char *);
extern  int mknod(const char *, mode_t, dev_t);
extern  int chmod(const char *, mode_t);
extern  int mkdir(const char *, mode_t);
extern  int rmdir(const char *);
extern  int pipe(int *);
extern  int chroot(const char *);
extern  int symlink(const char *, const char *);
extern  int readlink(const char *, char *, size_t);
extern  int chown(const char *, uid_t, gid_t);
extern  int fchown(int, uid_t, gid_t);
extern  int lchown(const char *, uid_t, gid_t);
extern  char *getcwd(char *, size_t);

extern  int sync(void);

extern  ssize_t read(int, void *, size_t);
extern  ssize_t write(int, const void *, size_t);

extern  int open(const char *, int, ...);

extern  int close(int);
extern  off_t lseek(int, off_t, int);
extern  loff_t llseek(int, loff_t, int);
extern  int dup(int);
extern  int dup2(int, int);
extern  int fcntl(int, int, long);
extern  int ioctl(int, int, void *);
extern  int flock(int, int);
extern  int fsync(int);
extern  int fdatasync(int);

extern  int pause(void);
extern  unsigned int alarm(unsigned int);
extern  unsigned int sleep(unsigned int);
extern  void usleep(unsigned long);

extern  int gethostname(char *, size_t);
extern  int sethostname(const char *, size_t);
extern  int getdomainname(char *, size_t);
extern  int setdomainname(const char *, size_t);

extern  void *__brk(void *);
extern  int brk(void *);
extern  void *sbrk(ptrdiff_t);

extern  int getopt(int, char * const *, const char *);
extern  char *optarg;
extern  int optind, opterr, optopt;


# 11 "../klibc/include/stdio.h" 2


 

struct _IO_file;
typedef struct _IO_file FILE;

















static __inline__ int fileno(FILE *__f)
{
   
  return (int)(size_t)__f;
}

static __inline__ FILE * __create_file(int __fd)
{
  return (FILE *)(size_t)__fd;
}

extern  FILE *fopen(const char *, const char *);

static __inline__ FILE *fdopen(int __fd, const char *__m)
{
  (void)__m; return __create_file(__fd);
}
static __inline__ int fclose(FILE *__f)
{
  extern int close(int);
  return close(fileno(__f));
}
static __inline__ int fseek(FILE *__f, off_t __o, int __w)
{
  extern off_t lseek(int, off_t, int);
  return (lseek(fileno(__f), __o, __w) == (off_t)-1) ? -1 : 0;
}
static __inline__ off_t ftell(FILE *__f)
{
  extern off_t lseek(int, off_t, int);
  return lseek(fileno(__f), 0, 1 );
}

extern  int fputs(const char *, FILE *);
extern  int puts(const char *);

extern  size_t __fread(void *, size_t, FILE *);
extern  size_t __fwrite(const void *, size_t, FILE *);


extern  __inline__ size_t
fread(void *__p, size_t __s, size_t __n, FILE *__f)
{
  return __fread(__p, __s*__n, __f)/__s;
}
extern  __inline__ size_t
fwrite(void *__p, size_t __s, size_t __n, FILE *__f)
{
  return __fwrite(__p, __s*__n, __f)/__s;
}


extern  int printf(const char *, ...);
extern  int vprintf(const char *, va_list);
extern  int fprintf(FILE *, const char *, ...);
extern  int vfprintf(FILE *, const char *, va_list);
extern  int sprintf(char *, const char *, ...);
extern  int vsprintf(char *, const char *, va_list);
extern  int snprintf(char *, size_t n, const char *, ...);
extern  int vsnprintf(char *, size_t n, const char *, va_list);

extern  int sscanf(const char *, const char *, ...);
extern  int vsscanf(const char *, const char *, va_list);


# 61 "mkinit.c" 2

# 1 "../klibc/include/fcntl.h" 1
 






# 1 "../klibc/linux/include/linux/fcntl.h" 1



# 1 "../klibc/linux/include/asm/fcntl.h" 1



 





































 


 




 



 


 











struct flock {
	short l_type;
	short l_whence;
	off_t l_start;
	off_t l_len;
	pid_t l_pid;
};

struct flock64 {
	short  l_type;
	short  l_whence;
	loff_t l_start;
	loff_t l_len;
	pid_t  l_pid;
};




# 4 "../klibc/linux/include/linux/fcntl.h" 2





 





 










# 48 "../klibc/linux/include/linux/fcntl.h"



# 8 "../klibc/include/fcntl.h" 2



# 62 "mkinit.c" 2



 










 









struct text {
	char *nextc;
	int nleft;
	struct block *start;
	struct block *last;
};      

struct block {
	struct block *next;
	char text[512 ];
};


 



struct event {
	char *name;		 
	char *routine;		 
	char *comment;		 
	struct text code;		 
};


char writer[] = "/*\n * This file was generated by the mkinit program.\n */\n\n";





char init[] = "/*\n * Initialization code.\n */\n";




char reset[] = "/*\n * This routine is called when an error or an interrupt occurs in an\n * interactive shell and control is returned to the main command loop.\n */\n";





char shellproc[] = "/*\n * This routine is called to initialize the shell to run a shell procedure.\n */\n";





struct event event[] = {
	{"INIT", "init", init},
	{"RESET", "reset", reset},
	{"SHELLPROC", "initshellproc", shellproc},
	{((void *)0) , ((void *)0) }
};


char *curfile;				 
int linno;				 
char *header_files[200];		 
struct text defines;			 
struct text decls;			 
int amiddecls;				 


void readfile(), doevent(), doinclude(), dodecl(), output();
void addstr(), addchar(), writetext();



FILE *ckfopen();
char *savestr();
void *ckmalloc  (int)    ;
void error();

main(argc, argv)
	char **argv;
	{
	char **ap;
	int fd;
	char c;

	if (argc < 2)
		error("Usage:  mkinit command file...");
	header_files[0] = "\"shell.h\"";
	header_files[1] = "\"mystring.h\"";
	for (ap = argv + 2 ; *ap ; ap++)
		readfile(*ap);
	output();
	if (file_changed()) {
		unlink("init.c" );
		link("init.c.new" , "init.c" );
		unlink("init.c.new" );
	} else {
		unlink("init.c.new" );
		if (touch("init.o" ))
			exit(0);		 
	}
	printf("%s\n", argv[1]);
	execl("/bin/sh", "sh", "-c", argv[1], (char *)0);
	error("Can't exec shell");
}


 



void
readfile(fname)
	char *fname;
	{
	FILE *fp;
	char line[1024];
	struct event *ep;

	fp = ckfopen(fname, "r");
	curfile = fname;
	linno = 0;
	amiddecls = 0;
	while (fgets(line, sizeof line, fp) != ((void *)0) ) {
		linno++;
		for (ep = event ; ep->name ; ep++) {
			if (line[0] == ep->name[0] && match(ep->name, line)) {
				doevent(ep, fp, fname);
				break;
			}
		}
		if (line[0] == 'I' && match("INCLUDE", line))
			doinclude(line);
		if (line[0] == 'M' && match("MKINIT", line))
			dodecl(line, fp);
		if (line[0] == '#' && gooddefine(line))
			addstr(line, &defines);
	}
	fclose(fp);
}


int
match(name, line)
	char *name;
	char *line;
	{
	register char *p, *q;

	p = name, q = line;
	while (*p) {
		if (*p++ != *q++)
			return 0;
	}
	if (*q != '{' && *q != ' ' && *q != '\t' && *q != '\n')
		return 0;
	return 1;
}


int
gooddefine(line)
	char *line;
	{
	register char *p;

	if (! match("#define", line))
		return 0;			 
	p = line + 7;
	while (*p == ' ' || *p == '\t')
		p++;
	while (*p != ' ' && *p != '\t') {
		if (*p == '(')
			return 0;		 
		p++;
	}
	while (*p != '\n' && *p != '\0')
		p++;
	if (p[-1] == '\\')
		return 0;			 
	return 1;
}


void
doevent(ep, fp, fname)
	register struct event *ep;
	FILE *fp;
	char *fname;
	{
	char line[1024];
	int indent;
	char *p;

	sprintf(line, "\n      /* from %s: */\n", fname);
	addstr(line, &ep->code);
	addstr("      {\n", &ep->code);
	for (;;) {
		linno++;
		if (fgets(line, sizeof line, fp) == ((void *)0) )
			error("Unexpected EOF");
		if ((strcmp( line ,   "}\n" ) == 0) )
			break;
		indent = 6;
		for (p = line ; *p == '\t' ; p++)
			indent += 8;
		for ( ; *p == ' ' ; p++)
			indent++;
		if (*p == '\n' || *p == '#')
			indent = 0;
		while (indent >= 8) {
			addchar('\t', &ep->code);
			indent -= 8;
		}
		while (indent > 0) {
			addchar(' ', &ep->code);
			indent--;
		}
		addstr(p, &ep->code);
	}
	addstr("      }\n", &ep->code);
}


void
doinclude(line)
	char *line;
	{
	register char *p;
	char *name;
	register char **pp;

	for (p = line ; *p != '"' && *p != '<' && *p != '\0' ; p++);
	if (*p == '\0')
		error("Expecting '\"' or '<'");
	name = p;
	while (*p != ' ' && *p != '\t' && *p != '\n')
		p++;
	if (p[-1] != '"' && p[-1] != '>')
		error("Missing terminator");
	*p = '\0';

	 
	for (pp = header_files ; *pp && ! (strcmp( *pp ,   name ) == 0)  ; pp++);
	if (*pp == ((void *)0) )
		*pp = savestr(name);
}


void
dodecl(line1, fp)
	char *line1;
	FILE *fp;
	{
	char line[1024];
	register char *p, *q;

	if (strcmp(line1, "MKINIT\n") == 0) {  
		addchar('\n', &decls);
		do {
			linno++;
			if (fgets(line, sizeof line, fp) == ((void *)0) )
				error("Unterminated structure declaration");
			addstr(line, &decls);
		} while (line[0] != '}');
		amiddecls = 0;
	} else {
		if (! amiddecls)
			addchar('\n', &decls);
		q = ((void *)0) ;
		for (p = line1 + 6 ; *p != '=' && *p != '/' ; p++);
		if (*p == '=') {		 
			for (q = p ; *q && *q != ';' ; q++);
			if (*q == '\0')
				q = ((void *)0) ;
			else {
				while (p[-1] == ' ')
					p--;
				*p = '\0';
			}
		}
		addstr("extern", &decls);
		addstr(line1 + 6, &decls);
		if (q != ((void *)0) )
			addstr(q, &decls);
		amiddecls = 1;
	}
}



 



void
output() {
	FILE *fp;
	char **pp;
	struct event *ep;

	fp = ckfopen("init.c.new" , "w");
	fputs(writer, fp);
	for (pp = header_files ; *pp ; pp++)
		fprintf(fp, "#include %s\n", *pp);
	fputs("\n\n\n", fp);
	writetext(&defines, fp);
	fputs("\n\n", fp);
	writetext(&decls, fp);
	for (ep = event ; ep->name ; ep++) {
		fputs("\n\n\n", fp);
		fputs(ep->comment, fp);
		fprintf(fp, "\nvoid\n%s() {\n", ep->routine);
		writetext(&ep->code, fp);
		fprintf(fp, "}\n");
	}
	fclose(fp);
}


 



int
file_changed() {
	register FILE *f1, *f2;
	register int c;

	if ((f1 = fopen("init.c" , "r")) == ((void *)0) 
	 || (f2 = fopen("init.c.new" , "r")) == ((void *)0) )
		return 1;
	while ((c = getc(f1)) == getc(f2)) {
		if (c == (-1) )
			return 0;
	}
	return 1;
}


 



int
touch(file)
	char *file;
	{
	int fd;
	char c;

	if ((fd = open(file, 02 )) < 0)
		return 0;
	if (read(fd, &c, 1) != 1) {
		close(fd);
		return 0;
	}
	lseek(fd, 0L, 0);
	write(fd, &c, 1);
	close(fd);
	return 1;
}



 





void
addstr(s, text)
	register char *s;
	register struct text *text;
	{
	while (*s) {
		if (--text->nleft < 0)
			addchar(*s++, text);
		else
			*text->nextc++ = *s++;
	}
}


void
addchar(c, text)
	register struct text *text;
	{
	struct block *bp;

	if (--text->nleft < 0) {
		bp = ckmalloc(sizeof *bp);
		if (text->start == ((void *)0) )
			text->start = bp;
		else
			text->last->next = bp;
		text->last = bp;
		text->nextc = bp->text;
		text->nleft = 512  - 1;
	}
	*text->nextc++ = c;
}

 


void
writetext(text, fp)
	struct text *text;
	FILE *fp;
	{
	struct block *bp;

	if (text->start != ((void *)0) ) {
		for (bp = text->start ; bp != text->last ; bp = bp->next)
			fwrite(bp->text, sizeof (char), 512 , fp);
		fwrite(bp->text, sizeof (char), 512  - text->nleft, fp);
	}
}

FILE *
ckfopen(file, mode)
	char *file;
	char *mode;
	{
	FILE *fp;

	if ((fp = fopen(file, mode)) == ((void *)0) ) {
		fprintf(((FILE *)2) , "Can't open %s\n", file);
		exit(2);
	}
	return fp;
}

void *
ckmalloc(nbytes) {
	register char *p;
	char *malloc();

	if ((p = malloc(nbytes)) == ((void *)0) )
		error("Out of space");
	return p;
}

char *
savestr(s)
	char *s;
	{
	register char *p;

	p = ckmalloc(strlen(s) + 1);
	strcpy(p, s);
	return p;
}

void
error(msg)
	char *msg;
	{
	if (curfile != ((void *)0) )
		fprintf(((FILE *)2) , "%s:%d: ", curfile, linno);
	fprintf(((FILE *)2) , "%s\n", msg);
	exit(2);
}
