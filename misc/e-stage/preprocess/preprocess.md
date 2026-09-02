# 预处理

## 观察预处理结果

```c
// a.c
#include <stdio.h>
#define MSG                                                                    \
  "Hello \
World!\n"
#define _str(x) #x
#define _concat(a, b) a##b
int main() {
  printf(MSG /* "hi!\n" */);
#ifdef __riscv
  printf("Hello RISC-V!\n");
#endif
  _concat(pr, intf)(_str(RISC - V));
  return 0;
}
```

在 `gcc -E a.c` 之后

```c
# 0 "a.c"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "a.c"

# 1 "/usr/include/stdio.h" 1 3 4
# 28 "/usr/include/stdio.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 1 3 4
# 33 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
# 1 "/usr/include/features.h" 1 3 4
# 415 "/usr/include/features.h" 3 4
# 1 "/usr/include/features-time64.h" 1 3 4
# 20 "/usr/include/features-time64.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 1 3 4
# 21 "/usr/include/features-time64.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/timesize.h" 1 3 4
# 19 "/usr/include/x86_64-linux-gnu/bits/timesize.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 1 3 4
# 20 "/usr/include/x86_64-linux-gnu/bits/timesize.h" 2 3 4
# 22 "/usr/include/features-time64.h" 2 3 4
# 416 "/usr/include/features.h" 2 3 4
# 523 "/usr/include/features.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 1 3 4
# 730 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 1 3 4
# 731 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/long-double.h" 1 3 4
# 732 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 2 3 4
# 524 "/usr/include/features.h" 2 3 4
# 547 "/usr/include/features.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 1 3 4
# 10 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/gnu/stubs-64.h" 1 3 4
# 11 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 2 3 4
# 548 "/usr/include/features.h" 2 3 4
# 34 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 2 3 4
# 29 "/usr/include/stdio.h" 2 3 4





# 1 "/usr/lib/gcc/x86_64-linux-gnu/14/include/stddef.h" 1 3 4
# 214 "/usr/lib/gcc/x86_64-linux-gnu/14/include/stddef.h" 3 4

# 214 "/usr/lib/gcc/x86_64-linux-gnu/14/include/stddef.h" 3 4
typedef long unsigned int size_t;
# 35 "/usr/include/stdio.h" 2 3 4


# 1 "/usr/lib/gcc/x86_64-linux-gnu/14/include/stdarg.h" 1 3 4
# 40 "/usr/lib/gcc/x86_64-linux-gnu/14/include/stdarg.h" 3 4
typedef __builtin_va_list __gnuc_va_list;
# 38 "/usr/include/stdio.h" 2 3 4

# 1 "/usr/include/x86_64-linux-gnu/bits/types.h" 1 3 4
# 27 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 1 3 4
# 28 "/usr/include/x86_64-linux-gnu/bits/types.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/timesize.h" 1 3 4
# 19 "/usr/include/x86_64-linux-gnu/bits/timesize.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 1 3 4
# 20 "/usr/include/x86_64-linux-gnu/bits/timesize.h" 2 3 4
# 29 "/usr/include/x86_64-linux-gnu/bits/types.h" 2 3 4


typedef unsigned char __u_char;
typedef unsigned short int __u_short;
typedef unsigned int __u_int;
typedef unsigned long int __u_long;


typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef signed short int __int16_t;
typedef unsigned short int __uint16_t;
typedef signed int __int32_t;
typedef unsigned int __uint32_t;

typedef signed long int __int64_t;
typedef unsigned long int __uint64_t;






typedef __int8_t __int_least8_t;
typedef __uint8_t __uint_least8_t;
typedef __int16_t __int_least16_t;
typedef __uint16_t __uint_least16_t;
typedef __int32_t __int_least32_t;
typedef __uint32_t __uint_least32_t;
typedef __int64_t __int_least64_t;
typedef __uint64_t __uint_least64_t;



typedef long int __quad_t;
typedef unsigned long int __u_quad_t;







typedef long int __intmax_t;
typedef unsigned long int __uintmax_t;
# 141 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/typesizes.h" 1 3 4
# 142 "/usr/include/x86_64-linux-gnu/bits/types.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/time64.h" 1 3 4
# 143 "/usr/include/x86_64-linux-gnu/bits/types.h" 2 3 4


typedef unsigned long int __dev_t;
typedef unsigned int __uid_t;
typedef unsigned int __gid_t;
typedef unsigned long int __ino_t;
typedef unsigned long int __ino64_t;
typedef unsigned int __mode_t;
typedef unsigned long int __nlink_t;
typedef long int __off_t;
typedef long int __off64_t;
typedef int __pid_t;
typedef struct { int __val[2]; } __fsid_t;
typedef long int __clock_t;
typedef unsigned long int __rlim_t;
typedef unsigned long int __rlim64_t;
typedef unsigned int __id_t;
typedef long int __time_t;
typedef unsigned int __useconds_t;
typedef long int __suseconds_t;
typedef long int __suseconds64_t;

typedef int __daddr_t;
typedef int __key_t;


typedef int __clockid_t;


typedef void * __timer_t;


typedef long int __blksize_t;




typedef long int __blkcnt_t;
typedef long int __blkcnt64_t;


typedef unsigned long int __fsblkcnt_t;
typedef unsigned long int __fsblkcnt64_t;


typedef unsigned long int __fsfilcnt_t;
typedef unsigned long int __fsfilcnt64_t;


typedef long int __fsword_t;

typedef long int __ssize_t;


typedef long int __syscall_slong_t;

typedef unsigned long int __syscall_ulong_t;



typedef __off64_t __loff_t;
typedef char *__caddr_t;


typedef long int __intptr_t;


typedef unsigned int __socklen_t;




typedef int __sig_atomic_t;
# 40 "/usr/include/stdio.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h" 1 3 4




# 1 "/usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h" 1 3 4
# 13 "/usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h" 3 4
typedef struct
{
  int __count;
  union
  {
    unsigned int __wch;
    char __wchb[4];
  } __value;
} __mbstate_t;
# 6 "/usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h" 2 3 4




typedef struct _G_fpos_t
{
  __off_t __pos;
  __mbstate_t __state;
} __fpos_t;
# 41 "/usr/include/stdio.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h" 1 3 4
# 10 "/usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h" 3 4
typedef struct _G_fpos64_t
{
  __off64_t __pos;
  __mbstate_t __state;
} __fpos64_t;
# 42 "/usr/include/stdio.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/types/__FILE.h" 1 3 4



struct _IO_FILE;
typedef struct _IO_FILE __FILE;
# 43 "/usr/include/stdio.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/types/FILE.h" 1 3 4



struct _IO_FILE;


typedef struct _IO_FILE FILE;
# 44 "/usr/include/stdio.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h" 1 3 4
# 36 "/usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h" 3 4
struct _IO_FILE;
struct _IO_marker;
struct _IO_codecvt;
struct _IO_wide_data;




typedef void _IO_lock_t;





struct _IO_FILE
{
  int _flags;


  char *_IO_read_ptr;
  char *_IO_read_end;
  char *_IO_read_base;
  char *_IO_write_base;
  char *_IO_write_ptr;
  char *_IO_write_end;
  char *_IO_buf_base;
  char *_IO_buf_end;


  char *_IO_save_base;
  char *_IO_backup_base;
  char *_IO_save_end;

  struct _IO_marker *_markers;

  struct _IO_FILE *_chain;

  int _fileno;
  int _flags2:24;

  char _short_backupbuf[1];
  __off_t _old_offset;


  unsigned short _cur_column;
  signed char _vtable_offset;
  char _shortbuf[1];

  _IO_lock_t *_lock;







  __off64_t _offset;

  struct _IO_codecvt *_codecvt;
  struct _IO_wide_data *_wide_data;
  struct _IO_FILE *_freeres_list;
  void *_freeres_buf;
  struct _IO_FILE **_prevchain;
  int _mode;

  char _unused2[15 * sizeof (int) - 5 * sizeof (void *)];
};
# 45 "/usr/include/stdio.h" 2 3 4


# 1 "/usr/include/x86_64-linux-gnu/bits/types/cookie_io_functions_t.h" 1 3 4
# 27 "/usr/include/x86_64-linux-gnu/bits/types/cookie_io_functions_t.h" 3 4
typedef __ssize_t cookie_read_function_t (void *__cookie, char *__buf,
                                          size_t __nbytes);







typedef __ssize_t cookie_write_function_t (void *__cookie, const char *__buf,
                                           size_t __nbytes);







typedef int cookie_seek_function_t (void *__cookie, __off64_t *__pos, int __w);


typedef int cookie_close_function_t (void *__cookie);






typedef struct _IO_cookie_io_functions_t
{
  cookie_read_function_t *read;
  cookie_write_function_t *write;
  cookie_seek_function_t *seek;
  cookie_close_function_t *close;
} cookie_io_functions_t;
# 48 "/usr/include/stdio.h" 2 3 4





typedef __gnuc_va_list va_list;
# 64 "/usr/include/stdio.h" 3 4
typedef __off_t off_t;
# 78 "/usr/include/stdio.h" 3 4
typedef __ssize_t ssize_t;






typedef __fpos_t fpos_t;
# 129 "/usr/include/stdio.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/stdio_lim.h" 1 3 4
# 130 "/usr/include/stdio.h" 2 3 4
# 149 "/usr/include/stdio.h" 3 4
extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;






extern int remove (const char *__filename) __attribute__ ((__nothrow__ , __leaf__));

extern int rename (const char *__old, const char *__new) __attribute__ ((__nothrow__ , __leaf__));



extern int renameat (int __oldfd, const char *__old, int __newfd,
       const char *__new) __attribute__ ((__nothrow__ , __leaf__));
# 184 "/usr/include/stdio.h" 3 4
extern int fclose (FILE *__stream) __attribute__ ((__nonnull__ (1)));
# 194 "/usr/include/stdio.h" 3 4
extern FILE *tmpfile (void)
  __attribute__ ((__malloc__)) __attribute__ ((__malloc__ (fclose, 1))) ;
# 211 "/usr/include/stdio.h" 3 4
extern char *tmpnam (char[20]) __attribute__ ((__nothrow__ , __leaf__)) ;




extern char *tmpnam_r (char __s[20]) __attribute__ ((__nothrow__ , __leaf__)) ;
# 228 "/usr/include/stdio.h" 3 4
extern char *tempnam (const char *__dir, const char *__pfx)
   __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__malloc__)) __attribute__ ((__malloc__ (__builtin_free, 1)));






extern int fflush (FILE *__stream);
# 245 "/usr/include/stdio.h" 3 4
extern int fflush_unlocked (FILE *__stream);
# 264 "/usr/include/stdio.h" 3 4
extern FILE *fopen (const char *__restrict __filename,
      const char *__restrict __modes)
  __attribute__ ((__malloc__)) __attribute__ ((__malloc__ (fclose, 1))) ;




extern FILE *freopen (const char *__restrict __filename,
        const char *__restrict __modes,
        FILE *__restrict __stream) __attribute__ ((__nonnull__ (3)));
# 299 "/usr/include/stdio.h" 3 4
extern FILE *fdopen (int __fd, const char *__modes) __attribute__ ((__nothrow__ , __leaf__))
  __attribute__ ((__malloc__)) __attribute__ ((__malloc__ (fclose, 1))) ;





extern FILE *fopencookie (void *__restrict __magic_cookie,
     const char *__restrict __modes,
     cookie_io_functions_t __io_funcs) __attribute__ ((__nothrow__ , __leaf__))
  __attribute__ ((__malloc__)) __attribute__ ((__malloc__ (fclose, 1))) ;




extern FILE *fmemopen (void *__s, size_t __len, const char *__modes)
  __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__malloc__)) __attribute__ ((__malloc__ (fclose, 1))) ;




extern FILE *open_memstream (char **__bufloc, size_t *__sizeloc) __attribute__ ((__nothrow__ , __leaf__))
  __attribute__ ((__malloc__)) __attribute__ ((__malloc__ (fclose, 1))) ;
# 334 "/usr/include/stdio.h" 3 4
extern void setbuf (FILE *__restrict __stream, char *__restrict __buf) __attribute__ ((__nothrow__ , __leaf__))
  __attribute__ ((__nonnull__ (1)));



extern int setvbuf (FILE *__restrict __stream, char *__restrict __buf,
      int __modes, size_t __n) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));




extern void setbuffer (FILE *__restrict __stream, char *__restrict __buf,
         size_t __size) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));


extern void setlinebuf (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));







extern int fprintf (FILE *__restrict __stream,
      const char *__restrict __format, ...) __attribute__ ((__nonnull__ (1)));




extern int printf (const char *__restrict __format, ...);

extern int sprintf (char *__restrict __s,
      const char *__restrict __format, ...) __attribute__ ((__nothrow__));





extern int vfprintf (FILE *__restrict __s, const char *__restrict __format,
       __gnuc_va_list __arg) __attribute__ ((__nonnull__ (1)));




extern int vprintf (const char *__restrict __format, __gnuc_va_list __arg);

extern int vsprintf (char *__restrict __s, const char *__restrict __format,
       __gnuc_va_list __arg) __attribute__ ((__nothrow__));



extern int snprintf (char *__restrict __s, size_t __maxlen,
       const char *__restrict __format, ...)
     __attribute__ ((__nothrow__)) __attribute__ ((__format__ (__printf__, 3, 4)));

extern int vsnprintf (char *__restrict __s, size_t __maxlen,
        const char *__restrict __format, __gnuc_va_list __arg)
     __attribute__ ((__nothrow__)) __attribute__ ((__format__ (__printf__, 3, 0)));





extern int vasprintf (char **__restrict __ptr, const char *__restrict __f,
        __gnuc_va_list __arg)
     __attribute__ ((__nothrow__)) __attribute__ ((__format__ (__printf__, 2, 0))) ;
extern int __asprintf (char **__restrict __ptr,
         const char *__restrict __fmt, ...)
     __attribute__ ((__nothrow__)) __attribute__ ((__format__ (__printf__, 2, 3))) ;
extern int asprintf (char **__restrict __ptr,
       const char *__restrict __fmt, ...)
     __attribute__ ((__nothrow__)) __attribute__ ((__format__ (__printf__, 2, 3))) ;




extern int vdprintf (int __fd, const char *__restrict __fmt,
       __gnuc_va_list __arg)
     __attribute__ ((__format__ (__printf__, 2, 0)));
extern int dprintf (int __fd, const char *__restrict __fmt, ...)
     __attribute__ ((__format__ (__printf__, 2, 3)));







extern int fscanf (FILE *__restrict __stream,
     const char *__restrict __format, ...) __attribute__ ((__nonnull__ (1)));




extern int scanf (const char *__restrict __format, ...) ;

extern int sscanf (const char *__restrict __s,
     const char *__restrict __format, ...) __attribute__ ((__nothrow__ , __leaf__));





# 1 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 1 3 4
# 131 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 1 3 4
# 24 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/long-double.h" 1 3 4
# 25 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 2 3 4
# 132 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 2 3 4
# 438 "/usr/include/stdio.h" 2 3 4
# 463 "/usr/include/stdio.h" 3 4
extern int fscanf (FILE *__restrict __stream, const char *__restrict __format, ...) __asm__ ("" "__isoc99_fscanf")

                                __attribute__ ((__nonnull__ (1)));
extern int scanf (const char *__restrict __format, ...) __asm__ ("" "__isoc99_scanf")
                              ;
extern int sscanf (const char *__restrict __s, const char *__restrict __format, ...) __asm__ ("" "__isoc99_sscanf") __attribute__ ((__nothrow__ , __leaf__))

                      ;
# 490 "/usr/include/stdio.h" 3 4
extern int vfscanf (FILE *__restrict __s, const char *__restrict __format,
      __gnuc_va_list __arg)
     __attribute__ ((__format__ (__scanf__, 2, 0))) __attribute__ ((__nonnull__ (1)));





extern int vscanf (const char *__restrict __format, __gnuc_va_list __arg)
     __attribute__ ((__format__ (__scanf__, 1, 0))) ;


extern int vsscanf (const char *__restrict __s,
      const char *__restrict __format, __gnuc_va_list __arg)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__format__ (__scanf__, 2, 0)));
# 540 "/usr/include/stdio.h" 3 4
extern int vfscanf (FILE *__restrict __s, const char *__restrict __format, __gnuc_va_list __arg) __asm__ ("" "__isoc99_vfscanf")



     __attribute__ ((__format__ (__scanf__, 2, 0))) __attribute__ ((__nonnull__ (1)));
extern int vscanf (const char *__restrict __format, __gnuc_va_list __arg) __asm__ ("" "__isoc99_vscanf")

     __attribute__ ((__format__ (__scanf__, 1, 0))) ;
extern int vsscanf (const char *__restrict __s, const char *__restrict __format, __gnuc_va_list __arg) __asm__ ("" "__isoc99_vsscanf") __attribute__ ((__nothrow__ , __leaf__))



     __attribute__ ((__format__ (__scanf__, 2, 0)));
# 575 "/usr/include/stdio.h" 3 4
extern int fgetc (FILE *__stream) __attribute__ ((__nonnull__ (1)));
extern int getc (FILE *__stream) __attribute__ ((__nonnull__ (1)));





extern int getchar (void);






extern int getc_unlocked (FILE *__stream) __attribute__ ((__nonnull__ (1)));
extern int getchar_unlocked (void);
# 600 "/usr/include/stdio.h" 3 4
extern int fgetc_unlocked (FILE *__stream) __attribute__ ((__nonnull__ (1)));
# 611 "/usr/include/stdio.h" 3 4
extern int fputc (int __c, FILE *__stream) __attribute__ ((__nonnull__ (2)));
extern int putc (int __c, FILE *__stream) __attribute__ ((__nonnull__ (2)));





extern int putchar (int __c);
# 627 "/usr/include/stdio.h" 3 4
extern int fputc_unlocked (int __c, FILE *__stream) __attribute__ ((__nonnull__ (2)));







extern int putc_unlocked (int __c, FILE *__stream) __attribute__ ((__nonnull__ (2)));
extern int putchar_unlocked (int __c);






extern int getw (FILE *__stream) __attribute__ ((__nonnull__ (1)));


extern int putw (int __w, FILE *__stream) __attribute__ ((__nonnull__ (2)));







extern char *fgets (char *__restrict __s, int __n, FILE *__restrict __stream)
     __attribute__ ((__access__ (__write_only__, 1, 2))) __attribute__ ((__nonnull__ (3)));
# 689 "/usr/include/stdio.h" 3 4
extern __ssize_t __getdelim (char **__restrict __lineptr,
                             size_t *__restrict __n, int __delimiter,
                             FILE *__restrict __stream) __attribute__ ((__nonnull__ (4)));
extern __ssize_t getdelim (char **__restrict __lineptr,
                           size_t *__restrict __n, int __delimiter,
                           FILE *__restrict __stream) __attribute__ ((__nonnull__ (4)));


extern __ssize_t getline (char **__restrict __lineptr,
                          size_t *__restrict __n,
                          FILE *__restrict __stream) __attribute__ ((__nonnull__ (3)));







extern int fputs (const char *__restrict __s, FILE *__restrict __stream)
  __attribute__ ((__nonnull__ (2)));





extern int puts (const char *__s);






extern int ungetc (int __c, FILE *__stream) __attribute__ ((__nonnull__ (2)));






extern size_t fread (void *__restrict __ptr, size_t __size,
       size_t __n, FILE *__restrict __stream)
  __attribute__ ((__nonnull__ (4)));




extern size_t fwrite (const void *__restrict __ptr, size_t __size,
        size_t __n, FILE *__restrict __s) __attribute__ ((__nonnull__ (4)));
# 756 "/usr/include/stdio.h" 3 4
extern size_t fread_unlocked (void *__restrict __ptr, size_t __size,
         size_t __n, FILE *__restrict __stream)
  __attribute__ ((__nonnull__ (4)));
extern size_t fwrite_unlocked (const void *__restrict __ptr, size_t __size,
          size_t __n, FILE *__restrict __stream)
  __attribute__ ((__nonnull__ (4)));







extern int fseek (FILE *__stream, long int __off, int __whence)
  __attribute__ ((__nonnull__ (1)));




extern long int ftell (FILE *__stream) __attribute__ ((__nonnull__ (1)));




extern void rewind (FILE *__stream) __attribute__ ((__nonnull__ (1)));
# 793 "/usr/include/stdio.h" 3 4
extern int fseeko (FILE *__stream, __off_t __off, int __whence)
  __attribute__ ((__nonnull__ (1)));




extern __off_t ftello (FILE *__stream) __attribute__ ((__nonnull__ (1)));
# 819 "/usr/include/stdio.h" 3 4
extern int fgetpos (FILE *__restrict __stream, fpos_t *__restrict __pos)
  __attribute__ ((__nonnull__ (1)));




extern int fsetpos (FILE *__stream, const fpos_t *__pos) __attribute__ ((__nonnull__ (1)));
# 850 "/usr/include/stdio.h" 3 4
extern void clearerr (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));

extern int feof (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));

extern int ferror (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));



extern void clearerr_unlocked (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));
extern int feof_unlocked (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));
extern int ferror_unlocked (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));







extern void perror (const char *__s) __attribute__ ((__cold__));




extern int fileno (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));




extern int fileno_unlocked (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));
# 887 "/usr/include/stdio.h" 3 4
extern int pclose (FILE *__stream) __attribute__ ((__nonnull__ (1)));





extern FILE *popen (const char *__command, const char *__modes)
  __attribute__ ((__malloc__)) __attribute__ ((__malloc__ (pclose, 1))) ;






extern char *ctermid (char *__s) __attribute__ ((__nothrow__ , __leaf__))
  __attribute__ ((__access__ (__write_only__, 1)));
# 931 "/usr/include/stdio.h" 3 4
extern void flockfile (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));



extern int ftrylockfile (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));


extern void funlockfile (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));
# 949 "/usr/include/stdio.h" 3 4
extern int __uflow (FILE *);
extern int __overflow (FILE *, int);
# 973 "/usr/include/stdio.h" 3 4

# 3 "a.c" 2






# 8 "a.c"
int main() {
  printf("Hello World!\n" );



  printf("RISC - V");
  return 0;
}

```

## 寻找头文件

`gcc -E a.c --verbose > /dev/null`

```sh
$ gcc -E a.c --verbose > /dev/null
Using built-in specs.
COLLECT_GCC=gcc
OFFLOAD_TARGET_NAMES=nvptx-none:amdgcn-amdhsa
OFFLOAD_TARGET_DEFAULT=1
Target: x86_64-linux-gnu
Configured with: ../src/configure -v --with-pkgversion='Debian 14.2.0-19' --with-bugurl=file:///usr/share/doc/gcc-14/README.Bugs --enable-languages=c,ada,c++,go,d,fortran,objc,obj-c++,m2,rust --prefix=/usr --with-gcc-major-version-only --program-suffix=-14 --program-prefix=x86_64-linux-gnu- --enable-shared --enable-linker-build-id --libexecdir=/usr/libexec --without-included-gettext --enable-threads=posix --libdir=/usr/lib --enable-nls --enable-bootstrap --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --with-default-libstdcxx-abi=new --enable-libstdcxx-backtrace --enable-gnu-unique-object --disable-vtable-verify --enable-plugin --enable-default-pie --with-system-zlib --enable-libphobos-checking=release --with-target-system-zlib=auto --enable-objc-gc=auto --enable-multiarch --disable-werror --enable-cet --with-arch-32=i686 --with-abi=m64 --with-multilib-list=m32,m64,mx32 --enable-multilib --with-tune=generic --enable-offload-targets=nvptx-none=/build/reproducible-path/gcc-14-14.2.0/debian/tmp-nvptx/usr,amdgcn-amdhsa=/build/reproducible-path/gcc-14-14.2.0/debian/tmp-gcn/usr --enable-offload-defaulted --without-cuda-driver --enable-checking=release --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=x86_64-linux-gnu --with-build-config=bootstrap-lto-lean --enable-link-serialization=3
Thread model: posix
Supported LTO compression algorithms: zlib zstd
gcc version 14.2.0 (Debian 14.2.0-19)
COLLECT_GCC_OPTIONS='-E' '-v' '-mtune=generic' '-march=x86-64'
 /usr/libexec/gcc/x86_64-linux-gnu/14/cc1 -E -quiet -v -imultiarch x86_64-linux-gnu a.c -mtune=generic -march=x86-64 -fasynchronous-unwind-tables -dumpbase a.c -dumpbase-ext .c
ignoring nonexistent directory "/usr/local/include/x86_64-linux-gnu"
ignoring nonexistent directory "/usr/lib/gcc/x86_64-linux-gnu/14/include-fixed/x86_64-linux-gnu"
ignoring nonexistent directory "/usr/lib/gcc/x86_64-linux-gnu/14/include-fixed"
ignoring nonexistent directory "/usr/lib/gcc/x86_64-linux-gnu/14/../../../../x86_64-linux-gnu/include"
#include "..." search starts here:
#include <...> search starts here:
 /usr/lib/gcc/x86_64-linux-gnu/14/include
 /usr/local/include
 /usr/include/x86_64-linux-gnu
 /usr/include
End of search list.
COMPILER_PATH=/usr/libexec/gcc/x86_64-linux-gnu/14/:/usr/libexec/gcc/x86_64-linux-gnu/14/:/usr/libexec/gcc/x86_64-linux-gnu/:/usr/lib/gcc/x86_64-linux-gnu/14/:/usr/lib/gcc/x86_64-linux-gnu/
LIBRARY_PATH=/usr/lib/gcc/x86_64-linux-gnu/14/:/usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu/:/usr/lib/gcc/x86_64-linux-gnu/14/../../../../lib/:/lib/x86_64-linux-gnu/:/lib/../lib/:/usr/lib/x86_64-linux-gnu/:/usr/lib/../lib/:/usr/lib/gcc/x86_64-linux-gnu/14/../../../:/lib/:/usr/lib/
COLLECT_GCC_OPTIONS='-E' '-v' '-mtune=generic' '-march=x86-64'
```

-I dir 可以把某个文件夹加入头文件搜索列表，prefix 一个 = 的时候会从 sysroot 开始搜索。
又详细分为 -iquote dir、-isystem dir、-idirafter dir 三种。
搜索顺序是从左到右，可以用这个来覆盖系统的头文件目录，因为会先搜索新标记的文件夹。

创建 `stdio.h`：

```c
#define printf Well
```

之后 `gcc -I. -E a.c`：

```c
# 0 "a.c"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "a.c"

# 1 "./stdio.h" 1
# 3 "a.c" 2





int main() {
  Well("Hello World!\n" );



  Well("RISC - V");
  return 0;
}
```

## 使用 target 到 RISC-V 的 GCC 预处理

`riscv64-linux-gun-gcc -E a.c` 得到：

```c
# 0 "a.c"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/riscv64-linux-gnu/include/stdc-predef.h" 1 3
# 0 "<command-line>" 2
# 1 "a.c"

# 1 "./stdio.h" 1
# 3 "a.c" 2





int main() {
  Well("Hello World!\n" );

  Well("Hello RISC-V!\n");

  Well("RISC - V");
  return 0;
}
zhugy@laptop:~/Desktop/YSYX/misc/e-stage$ riscv64-linux-gnu-gcc -E a.c
# 0 "a.c"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/riscv64-linux-gnu/include/stdc-predef.h" 1 3
# 0 "<command-line>" 2
# 1 "a.c"

# 1 "/usr/riscv64-linux-gnu/include/stdio.h" 1 3
# 28 "/usr/riscv64-linux-gnu/include/stdio.h" 3
# 1 "/usr/riscv64-linux-gnu/include/bits/libc-header-start.h" 1 3
# 33 "/usr/riscv64-linux-gnu/include/bits/libc-header-start.h" 3
# 1 "/usr/riscv64-linux-gnu/include/features.h" 1 3
# 415 "/usr/riscv64-linux-gnu/include/features.h" 3
# 1 "/usr/riscv64-linux-gnu/include/features-time64.h" 1 3
# 20 "/usr/riscv64-linux-gnu/include/features-time64.h" 3
# 1 "/usr/riscv64-linux-gnu/include/bits/wordsize.h" 1 3
# 21 "/usr/riscv64-linux-gnu/include/features-time64.h" 2 3
# 1 "/usr/riscv64-linux-gnu/include/bits/timesize.h" 1 3
# 22 "/usr/riscv64-linux-gnu/include/features-time64.h" 2 3
# 416 "/usr/riscv64-linux-gnu/include/features.h" 2 3
# 523 "/usr/riscv64-linux-gnu/include/features.h" 3
# 1 "/usr/riscv64-linux-gnu/include/sys/cdefs.h" 1 3
# 730 "/usr/riscv64-linux-gnu/include/sys/cdefs.h" 3
# 1 "/usr/riscv64-linux-gnu/include/bits/wordsize.h" 1 3
# 731 "/usr/riscv64-linux-gnu/include/sys/cdefs.h" 2 3
# 1 "/usr/riscv64-linux-gnu/include/bits/long-double.h" 1 3
# 732 "/usr/riscv64-linux-gnu/include/sys/cdefs.h" 2 3
# 524 "/usr/riscv64-linux-gnu/include/features.h" 2 3
# 547 "/usr/riscv64-linux-gnu/include/features.h" 3
# 1 "/usr/riscv64-linux-gnu/include/gnu/stubs.h" 1 3




# 1 "/usr/riscv64-linux-gnu/include/bits/wordsize.h" 1 3
# 6 "/usr/riscv64-linux-gnu/include/gnu/stubs.h" 2 3
# 17 "/usr/riscv64-linux-gnu/include/gnu/stubs.h" 3
# 1 "/usr/riscv64-linux-gnu/include/gnu/stubs-lp64d.h" 1 3
# 18 "/usr/riscv64-linux-gnu/include/gnu/stubs.h" 2 3
# 548 "/usr/riscv64-linux-gnu/include/features.h" 2 3
# 34 "/usr/riscv64-linux-gnu/include/bits/libc-header-start.h" 2 3
# 29 "/usr/riscv64-linux-gnu/include/stdio.h" 2 3





# 1 "/usr/lib/gcc-cross/riscv64-linux-gnu/14/include/stddef.h" 1 3 4
# 214 "/usr/lib/gcc-cross/riscv64-linux-gnu/14/include/stddef.h" 3 4

# 214 "/usr/lib/gcc-cross/riscv64-linux-gnu/14/include/stddef.h" 3 4
typedef long unsigned int size_t;
# 35 "/usr/riscv64-linux-gnu/include/stdio.h" 2 3


# 1 "/usr/lib/gcc-cross/riscv64-linux-gnu/14/include/stdarg.h" 1 3 4
# 40 "/usr/lib/gcc-cross/riscv64-linux-gnu/14/include/stdarg.h" 3 4
typedef __builtin_va_list __gnuc_va_list;
# 38 "/usr/riscv64-linux-gnu/include/stdio.h" 2 3

# 1 "/usr/riscv64-linux-gnu/include/bits/types.h" 1 3
# 27 "/usr/riscv64-linux-gnu/include/bits/types.h" 3
# 1 "/usr/riscv64-linux-gnu/include/bits/wordsize.h" 1 3
# 28 "/usr/riscv64-linux-gnu/include/bits/types.h" 2 3
# 1 "/usr/riscv64-linux-gnu/include/bits/timesize.h" 1 3
# 29 "/usr/riscv64-linux-gnu/include/bits/types.h" 2 3


typedef unsigned char __u_char;
typedef unsigned short int __u_short;
typedef unsigned int __u_int;
typedef unsigned long int __u_long;


typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef signed short int __int16_t;
typedef unsigned short int __uint16_t;
typedef signed int __int32_t;
typedef unsigned int __uint32_t;

typedef signed long int __int64_t;
typedef unsigned long int __uint64_t;






typedef __int8_t __int_least8_t;
typedef __uint8_t __uint_least8_t;
typedef __int16_t __int_least16_t;
typedef __uint16_t __uint_least16_t;
typedef __int32_t __int_least32_t;
typedef __uint32_t __uint_least32_t;
typedef __int64_t __int_least64_t;
typedef __uint64_t __uint_least64_t;



typedef long int __quad_t;
typedef unsigned long int __u_quad_t;







typedef long int __intmax_t;
typedef unsigned long int __uintmax_t;
# 141 "/usr/riscv64-linux-gnu/include/bits/types.h" 3
# 1 "/usr/riscv64-linux-gnu/include/bits/typesizes.h" 1 3
# 142 "/usr/riscv64-linux-gnu/include/bits/types.h" 2 3
# 1 "/usr/riscv64-linux-gnu/include/bits/time64.h" 1 3
# 143 "/usr/riscv64-linux-gnu/include/bits/types.h" 2 3


typedef unsigned long int __dev_t;
typedef unsigned int __uid_t;
typedef unsigned int __gid_t;
typedef unsigned long int __ino_t;
typedef unsigned long int __ino64_t;
typedef unsigned int __mode_t;
typedef unsigned int __nlink_t;
typedef long int __off_t;
typedef long int __off64_t;
typedef int __pid_t;
typedef struct { int __val[2]; } __fsid_t;
typedef long int __clock_t;
typedef unsigned long int __rlim_t;
typedef unsigned long int __rlim64_t;
typedef unsigned int __id_t;
typedef long int __time_t;
typedef unsigned int __useconds_t;
typedef long int __suseconds_t;
typedef long int __suseconds64_t;

typedef int __daddr_t;
typedef int __key_t;


typedef int __clockid_t;


typedef void * __timer_t;


typedef int __blksize_t;




typedef long int __blkcnt_t;
typedef long int __blkcnt64_t;


typedef unsigned long int __fsblkcnt_t;
typedef unsigned long int __fsblkcnt64_t;


typedef unsigned long int __fsfilcnt_t;
typedef unsigned long int __fsfilcnt64_t;


typedef long int __fsword_t;

typedef long int __ssize_t;


typedef long int __syscall_slong_t;

typedef unsigned long int __syscall_ulong_t;



typedef __off64_t __loff_t;
typedef char *__caddr_t;


typedef long int __intptr_t;


typedef unsigned int __socklen_t;




typedef int __sig_atomic_t;
# 40 "/usr/riscv64-linux-gnu/include/stdio.h" 2 3
# 1 "/usr/riscv64-linux-gnu/include/bits/types/__fpos_t.h" 1 3




# 1 "/usr/riscv64-linux-gnu/include/bits/types/__mbstate_t.h" 1 3
# 13 "/usr/riscv64-linux-gnu/include/bits/types/__mbstate_t.h" 3
typedef struct
{
  int __count;
  union
  {
    unsigned int __wch;
    char __wchb[4];
  } __value;
} __mbstate_t;
# 6 "/usr/riscv64-linux-gnu/include/bits/types/__fpos_t.h" 2 3




typedef struct _G_fpos_t
{
  __off_t __pos;
  __mbstate_t __state;
} __fpos_t;
# 41 "/usr/riscv64-linux-gnu/include/stdio.h" 2 3
# 1 "/usr/riscv64-linux-gnu/include/bits/types/__fpos64_t.h" 1 3
# 10 "/usr/riscv64-linux-gnu/include/bits/types/__fpos64_t.h" 3
typedef struct _G_fpos64_t
{
  __off64_t __pos;
  __mbstate_t __state;
} __fpos64_t;
# 42 "/usr/riscv64-linux-gnu/include/stdio.h" 2 3
# 1 "/usr/riscv64-linux-gnu/include/bits/types/__FILE.h" 1 3



struct _IO_FILE;
typedef struct _IO_FILE __FILE;
# 43 "/usr/riscv64-linux-gnu/include/stdio.h" 2 3
# 1 "/usr/riscv64-linux-gnu/include/bits/types/FILE.h" 1 3



struct _IO_FILE;


typedef struct _IO_FILE FILE;
# 44 "/usr/riscv64-linux-gnu/include/stdio.h" 2 3
# 1 "/usr/riscv64-linux-gnu/include/bits/types/struct_FILE.h" 1 3
# 36 "/usr/riscv64-linux-gnu/include/bits/types/struct_FILE.h" 3
struct _IO_FILE;
struct _IO_marker;
struct _IO_codecvt;
struct _IO_wide_data;




typedef void _IO_lock_t;





struct _IO_FILE
{
  int _flags;


  char *_IO_read_ptr;
  char *_IO_read_end;
  char *_IO_read_base;
  char *_IO_write_base;
  char *_IO_write_ptr;
  char *_IO_write_end;
  char *_IO_buf_base;
  char *_IO_buf_end;


  char *_IO_save_base;
  char *_IO_backup_base;
  char *_IO_save_end;

  struct _IO_marker *_markers;

  struct _IO_FILE *_chain;

  int _fileno;
  int _flags2:24;

  char _short_backupbuf[1];
  __off_t _old_offset;


  unsigned short _cur_column;
  signed char _vtable_offset;
  char _shortbuf[1];

  _IO_lock_t *_lock;







  __off64_t _offset;

  struct _IO_codecvt *_codecvt;
  struct _IO_wide_data *_wide_data;
  struct _IO_FILE *_freeres_list;
  void *_freeres_buf;
  struct _IO_FILE **_prevchain;
  int _mode;

  char _unused2[15 * sizeof (int) - 5 * sizeof (void *)];
};
# 45 "/usr/riscv64-linux-gnu/include/stdio.h" 2 3


# 1 "/usr/riscv64-linux-gnu/include/bits/types/cookie_io_functions_t.h" 1 3
# 27 "/usr/riscv64-linux-gnu/include/bits/types/cookie_io_functions_t.h" 3
typedef __ssize_t cookie_read_function_t (void *__cookie, char *__buf,
                                          size_t __nbytes);







typedef __ssize_t cookie_write_function_t (void *__cookie, const char *__buf,
                                           size_t __nbytes);







typedef int cookie_seek_function_t (void *__cookie, __off64_t *__pos, int __w);


typedef int cookie_close_function_t (void *__cookie);






typedef struct _IO_cookie_io_functions_t
{
  cookie_read_function_t *read;
  cookie_write_function_t *write;
  cookie_seek_function_t *seek;
  cookie_close_function_t *close;
} cookie_io_functions_t;
# 48 "/usr/riscv64-linux-gnu/include/stdio.h" 2 3





typedef __gnuc_va_list va_list;
# 64 "/usr/riscv64-linux-gnu/include/stdio.h" 3
typedef __off_t off_t;
# 78 "/usr/riscv64-linux-gnu/include/stdio.h" 3
typedef __ssize_t ssize_t;






typedef __fpos_t fpos_t;
# 129 "/usr/riscv64-linux-gnu/include/stdio.h" 3
# 1 "/usr/riscv64-linux-gnu/include/bits/stdio_lim.h" 1 3
# 130 "/usr/riscv64-linux-gnu/include/stdio.h" 2 3
# 149 "/usr/riscv64-linux-gnu/include/stdio.h" 3
extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;






extern int remove (const char *__filename) __attribute__ ((__nothrow__ , __leaf__));

extern int rename (const char *__old, const char *__new) __attribute__ ((__nothrow__ , __leaf__));



extern int renameat (int __oldfd, const char *__old, int __newfd,
       const char *__new) __attribute__ ((__nothrow__ , __leaf__));
# 184 "/usr/riscv64-linux-gnu/include/stdio.h" 3
extern int fclose (FILE *__stream) __attribute__ ((__nonnull__ (1)));
# 194 "/usr/riscv64-linux-gnu/include/stdio.h" 3
extern FILE *tmpfile (void)
  __attribute__ ((__malloc__)) __attribute__ ((__malloc__ (fclose, 1))) ;
# 211 "/usr/riscv64-linux-gnu/include/stdio.h" 3
extern char *tmpnam (char[20]) __attribute__ ((__nothrow__ , __leaf__)) ;




extern char *tmpnam_r (char __s[20]) __attribute__ ((__nothrow__ , __leaf__)) ;
# 228 "/usr/riscv64-linux-gnu/include/stdio.h" 3
extern char *tempnam (const char *__dir, const char *__pfx)
   __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__malloc__)) __attribute__ ((__malloc__ (__builtin_free, 1)));






extern int fflush (FILE *__stream);
# 245 "/usr/riscv64-linux-gnu/include/stdio.h" 3
extern int fflush_unlocked (FILE *__stream);
# 264 "/usr/riscv64-linux-gnu/include/stdio.h" 3
extern FILE *fopen (const char *__restrict __filename,
      const char *__restrict __modes)
  __attribute__ ((__malloc__)) __attribute__ ((__malloc__ (fclose, 1))) ;




extern FILE *freopen (const char *__restrict __filename,
        const char *__restrict __modes,
        FILE *__restrict __stream) __attribute__ ((__nonnull__ (3)));
# 299 "/usr/riscv64-linux-gnu/include/stdio.h" 3
extern FILE *fdopen (int __fd, const char *__modes) __attribute__ ((__nothrow__ , __leaf__))
  __attribute__ ((__malloc__)) __attribute__ ((__malloc__ (fclose, 1))) ;





extern FILE *fopencookie (void *__restrict __magic_cookie,
     const char *__restrict __modes,
     cookie_io_functions_t __io_funcs) __attribute__ ((__nothrow__ , __leaf__))
  __attribute__ ((__malloc__)) __attribute__ ((__malloc__ (fclose, 1))) ;




extern FILE *fmemopen (void *__s, size_t __len, const char *__modes)
  __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__malloc__)) __attribute__ ((__malloc__ (fclose, 1))) ;




extern FILE *open_memstream (char **__bufloc, size_t *__sizeloc) __attribute__ ((__nothrow__ , __leaf__))
  __attribute__ ((__malloc__)) __attribute__ ((__malloc__ (fclose, 1))) ;
# 334 "/usr/riscv64-linux-gnu/include/stdio.h" 3
extern void setbuf (FILE *__restrict __stream, char *__restrict __buf) __attribute__ ((__nothrow__ , __leaf__))
  __attribute__ ((__nonnull__ (1)));



extern int setvbuf (FILE *__restrict __stream, char *__restrict __buf,
      int __modes, size_t __n) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));




extern void setbuffer (FILE *__restrict __stream, char *__restrict __buf,
         size_t __size) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));


extern void setlinebuf (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));







extern int fprintf (FILE *__restrict __stream,
      const char *__restrict __format, ...) __attribute__ ((__nonnull__ (1)));




extern int printf (const char *__restrict __format, ...);

extern int sprintf (char *__restrict __s,
      const char *__restrict __format, ...) __attribute__ ((__nothrow__));





extern int vfprintf (FILE *__restrict __s, const char *__restrict __format,
       __gnuc_va_list __arg) __attribute__ ((__nonnull__ (1)));




extern int vprintf (const char *__restrict __format, __gnuc_va_list __arg);

extern int vsprintf (char *__restrict __s, const char *__restrict __format,
       __gnuc_va_list __arg) __attribute__ ((__nothrow__));



extern int snprintf (char *__restrict __s, size_t __maxlen,
       const char *__restrict __format, ...)
     __attribute__ ((__nothrow__)) __attribute__ ((__format__ (__printf__, 3, 4)));

extern int vsnprintf (char *__restrict __s, size_t __maxlen,
        const char *__restrict __format, __gnuc_va_list __arg)
     __attribute__ ((__nothrow__)) __attribute__ ((__format__ (__printf__, 3, 0)));





extern int vasprintf (char **__restrict __ptr, const char *__restrict __f,
        __gnuc_va_list __arg)
     __attribute__ ((__nothrow__)) __attribute__ ((__format__ (__printf__, 2, 0))) ;
extern int __asprintf (char **__restrict __ptr,
         const char *__restrict __fmt, ...)
     __attribute__ ((__nothrow__)) __attribute__ ((__format__ (__printf__, 2, 3))) ;
extern int asprintf (char **__restrict __ptr,
       const char *__restrict __fmt, ...)
     __attribute__ ((__nothrow__)) __attribute__ ((__format__ (__printf__, 2, 3))) ;




extern int vdprintf (int __fd, const char *__restrict __fmt,
       __gnuc_va_list __arg)
     __attribute__ ((__format__ (__printf__, 2, 0)));
extern int dprintf (int __fd, const char *__restrict __fmt, ...)
     __attribute__ ((__format__ (__printf__, 2, 3)));







extern int fscanf (FILE *__restrict __stream,
     const char *__restrict __format, ...) __attribute__ ((__nonnull__ (1)));




extern int scanf (const char *__restrict __format, ...) ;

extern int sscanf (const char *__restrict __s,
     const char *__restrict __format, ...) __attribute__ ((__nothrow__ , __leaf__));





# 1 "/usr/riscv64-linux-gnu/include/bits/floatn.h" 1 3
# 23 "/usr/riscv64-linux-gnu/include/bits/floatn.h" 3
# 1 "/usr/riscv64-linux-gnu/include/bits/long-double.h" 1 3
# 24 "/usr/riscv64-linux-gnu/include/bits/floatn.h" 2 3
# 95 "/usr/riscv64-linux-gnu/include/bits/floatn.h" 3
# 1 "/usr/riscv64-linux-gnu/include/bits/floatn-common.h" 1 3
# 24 "/usr/riscv64-linux-gnu/include/bits/floatn-common.h" 3
# 1 "/usr/riscv64-linux-gnu/include/bits/long-double.h" 1 3
# 25 "/usr/riscv64-linux-gnu/include/bits/floatn-common.h" 2 3
# 96 "/usr/riscv64-linux-gnu/include/bits/floatn.h" 2 3
# 438 "/usr/riscv64-linux-gnu/include/stdio.h" 2 3
# 463 "/usr/riscv64-linux-gnu/include/stdio.h" 3
extern int fscanf (FILE *__restrict __stream, const char *__restrict __format, ...) __asm__ ("" "__isoc99_fscanf")

                                __attribute__ ((__nonnull__ (1)));
extern int scanf (const char *__restrict __format, ...) __asm__ ("" "__isoc99_scanf")
                              ;
extern int sscanf (const char *__restrict __s, const char *__restrict __format, ...) __asm__ ("" "__isoc99_sscanf") __attribute__ ((__nothrow__ , __leaf__))

                      ;
# 490 "/usr/riscv64-linux-gnu/include/stdio.h" 3
extern int vfscanf (FILE *__restrict __s, const char *__restrict __format,
      __gnuc_va_list __arg)
     __attribute__ ((__format__ (__scanf__, 2, 0))) __attribute__ ((__nonnull__ (1)));





extern int vscanf (const char *__restrict __format, __gnuc_va_list __arg)
     __attribute__ ((__format__ (__scanf__, 1, 0))) ;


extern int vsscanf (const char *__restrict __s,
      const char *__restrict __format, __gnuc_va_list __arg)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__format__ (__scanf__, 2, 0)));
# 540 "/usr/riscv64-linux-gnu/include/stdio.h" 3
extern int vfscanf (FILE *__restrict __s, const char *__restrict __format, __gnuc_va_list __arg) __asm__ ("" "__isoc99_vfscanf")



     __attribute__ ((__format__ (__scanf__, 2, 0))) __attribute__ ((__nonnull__ (1)));
extern int vscanf (const char *__restrict __format, __gnuc_va_list __arg) __asm__ ("" "__isoc99_vscanf")

     __attribute__ ((__format__ (__scanf__, 1, 0))) ;
extern int vsscanf (const char *__restrict __s, const char *__restrict __format, __gnuc_va_list __arg) __asm__ ("" "__isoc99_vsscanf") __attribute__ ((__nothrow__ , __leaf__))



     __attribute__ ((__format__ (__scanf__, 2, 0)));
# 575 "/usr/riscv64-linux-gnu/include/stdio.h" 3
extern int fgetc (FILE *__stream) __attribute__ ((__nonnull__ (1)));
extern int getc (FILE *__stream) __attribute__ ((__nonnull__ (1)));





extern int getchar (void);






extern int getc_unlocked (FILE *__stream) __attribute__ ((__nonnull__ (1)));
extern int getchar_unlocked (void);
# 600 "/usr/riscv64-linux-gnu/include/stdio.h" 3
extern int fgetc_unlocked (FILE *__stream) __attribute__ ((__nonnull__ (1)));
# 611 "/usr/riscv64-linux-gnu/include/stdio.h" 3
extern int fputc (int __c, FILE *__stream) __attribute__ ((__nonnull__ (2)));
extern int putc (int __c, FILE *__stream) __attribute__ ((__nonnull__ (2)));





extern int putchar (int __c);
# 627 "/usr/riscv64-linux-gnu/include/stdio.h" 3
extern int fputc_unlocked (int __c, FILE *__stream) __attribute__ ((__nonnull__ (2)));







extern int putc_unlocked (int __c, FILE *__stream) __attribute__ ((__nonnull__ (2)));
extern int putchar_unlocked (int __c);






extern int getw (FILE *__stream) __attribute__ ((__nonnull__ (1)));


extern int putw (int __w, FILE *__stream) __attribute__ ((__nonnull__ (2)));







extern char *fgets (char *__restrict __s, int __n, FILE *__restrict __stream)
     __attribute__ ((__access__ (__write_only__, 1, 2))) __attribute__ ((__nonnull__ (3)));
# 689 "/usr/riscv64-linux-gnu/include/stdio.h" 3
extern __ssize_t __getdelim (char **__restrict __lineptr,
                             size_t *__restrict __n, int __delimiter,
                             FILE *__restrict __stream) __attribute__ ((__nonnull__ (4)));
extern __ssize_t getdelim (char **__restrict __lineptr,
                           size_t *__restrict __n, int __delimiter,
                           FILE *__restrict __stream) __attribute__ ((__nonnull__ (4)));


extern __ssize_t getline (char **__restrict __lineptr,
                          size_t *__restrict __n,
                          FILE *__restrict __stream) __attribute__ ((__nonnull__ (3)));







extern int fputs (const char *__restrict __s, FILE *__restrict __stream)
  __attribute__ ((__nonnull__ (2)));





extern int puts (const char *__s);






extern int ungetc (int __c, FILE *__stream) __attribute__ ((__nonnull__ (2)));






extern size_t fread (void *__restrict __ptr, size_t __size,
       size_t __n, FILE *__restrict __stream)
  __attribute__ ((__nonnull__ (4)));




extern size_t fwrite (const void *__restrict __ptr, size_t __size,
        size_t __n, FILE *__restrict __s) __attribute__ ((__nonnull__ (4)));
# 756 "/usr/riscv64-linux-gnu/include/stdio.h" 3
extern size_t fread_unlocked (void *__restrict __ptr, size_t __size,
         size_t __n, FILE *__restrict __stream)
  __attribute__ ((__nonnull__ (4)));
extern size_t fwrite_unlocked (const void *__restrict __ptr, size_t __size,
          size_t __n, FILE *__restrict __stream)
  __attribute__ ((__nonnull__ (4)));







extern int fseek (FILE *__stream, long int __off, int __whence)
  __attribute__ ((__nonnull__ (1)));




extern long int ftell (FILE *__stream) __attribute__ ((__nonnull__ (1)));




extern void rewind (FILE *__stream) __attribute__ ((__nonnull__ (1)));
# 793 "/usr/riscv64-linux-gnu/include/stdio.h" 3
extern int fseeko (FILE *__stream, __off_t __off, int __whence)
  __attribute__ ((__nonnull__ (1)));




extern __off_t ftello (FILE *__stream) __attribute__ ((__nonnull__ (1)));
# 819 "/usr/riscv64-linux-gnu/include/stdio.h" 3
extern int fgetpos (FILE *__restrict __stream, fpos_t *__restrict __pos)
  __attribute__ ((__nonnull__ (1)));




extern int fsetpos (FILE *__stream, const fpos_t *__pos) __attribute__ ((__nonnull__ (1)));
# 850 "/usr/riscv64-linux-gnu/include/stdio.h" 3
extern void clearerr (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));

extern int feof (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));

extern int ferror (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));



extern void clearerr_unlocked (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));
extern int feof_unlocked (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));
extern int ferror_unlocked (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));







extern void perror (const char *__s) __attribute__ ((__cold__));




extern int fileno (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));




extern int fileno_unlocked (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));
# 887 "/usr/riscv64-linux-gnu/include/stdio.h" 3
extern int pclose (FILE *__stream) __attribute__ ((__nonnull__ (1)));





extern FILE *popen (const char *__command, const char *__modes)
  __attribute__ ((__malloc__)) __attribute__ ((__malloc__ (pclose, 1))) ;






extern char *ctermid (char *__s) __attribute__ ((__nothrow__ , __leaf__))
  __attribute__ ((__access__ (__write_only__, 1)));
# 931 "/usr/riscv64-linux-gnu/include/stdio.h" 3
extern void flockfile (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));



extern int ftrylockfile (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));


extern void funlockfile (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));
# 949 "/usr/riscv64-linux-gnu/include/stdio.h" 3
extern int __uflow (FILE *);
extern int __overflow (FILE *, int);
# 973 "/usr/riscv64-linux-gnu/include/stdio.h" 3

# 3 "a.c" 2






# 8 "a.c"
int main() {
  printf("Hello World!\n" );

  printf("Hello RISC-V!\n");

  printf("RISC - V");
  return 0;
}
```

` diff -u --color <(riscv64-linux-gnu-gcc -E a.c  ) <(gcc -E a.c)` 可以看一下 diff，
发现除了 attribute 的路径之外唯一差距就是前者多了一个 `printf("Hello RISC-V!\n");`。

## 对比预定义宏

`echo | gcc -dM -E - | sort` 可以看到预定义宏，其中的 `-dM` 是生成所有 define 的 macro。
-d 一般是 DEBUG INFO 的意思。

还可以看到 gcc 和 riscv64-linux-gnu-gcc 的对比：

```diff
$ diff -u --color <(echo | gcc -dM -E - | sort) <(echo | riscv64-linux-gnu-gcc -dM -E - | sort)
--- /dev/fd/63  2026-09-02 08:04:01.173883948 +0800
+++ /dev/fd/62  2026-09-02 08:04:01.173883948 +0800
@@ -1,36 +1,15 @@
-#define __amd64 1
-#define __amd64__ 1
 #define __ATOMIC_ACQ_REL 4
 #define __ATOMIC_ACQUIRE 2
 #define __ATOMIC_CONSUME 1
-#define __ATOMIC_HLE_ACQUIRE 65536
-#define __ATOMIC_HLE_RELEASE 131072
 #define __ATOMIC_RELAXED 0
 #define __ATOMIC_RELEASE 3
 #define __ATOMIC_SEQ_CST 5
-#define __BFLT16_DECIMAL_DIG__ 4
-#define __BFLT16_DENORM_MIN__ 9.18354961579912115600575419704879436e-41BF16
-#define __BFLT16_DIG__ 2
-#define __BFLT16_EPSILON__ 7.81250000000000000000000000000000000e-3BF16
-#define __BFLT16_HAS_DENORM__ 1
-#define __BFLT16_HAS_INFINITY__ 1
-#define __BFLT16_HAS_QUIET_NAN__ 1
-#define __BFLT16_IS_IEC_60559__ 0
-#define __BFLT16_MANT_DIG__ 8
-#define __BFLT16_MAX_10_EXP__ 38
-#define __BFLT16_MAX__ 3.38953138925153547590470800371487867e+38BF16
-#define __BFLT16_MAX_EXP__ 128
-#define __BFLT16_MIN_10_EXP__ (-37)
-#define __BFLT16_MIN__ 1.17549435082228750796873653722224568e-38BF16
-#define __BFLT16_MIN_EXP__ (-125)
-#define __BFLT16_NORM_MAX__ 3.38953138925153547590470800371487867e+38BF16
 #define __BIGGEST_ALIGNMENT__ 16
-#define __BITINT_MAXWIDTH__ 65535
 #define __BYTE_ORDER__ __ORDER_LITTLE_ENDIAN__
 #define __CHAR16_TYPE__ short unsigned int
 #define __CHAR32_TYPE__ unsigned int
 #define __CHAR_BIT__ 8
-#define __code_model_small__ 1
+#define __CHAR_UNSIGNED__ 1
 #define __DBL_DECIMAL_DIG__ 17
 #define __DBL_DENORM_MIN__ ((double)4.94065645841246544176568792868221372e-324L)
 #define __DBL_DIG__ 15
@@ -47,30 +26,8 @@
 #define __DBL_MIN__ ((double)2.22507385850720138309023271733240406e-308L)
 #define __DBL_MIN_EXP__ (-1021)
 #define __DBL_NORM_MAX__ ((double)1.79769313486231570814527423731704357e+308L)
-#define __DEC128_EPSILON__ 1E-33DL
-#define __DEC128_MANT_DIG__ 34
-#define __DEC128_MAX__ 9.999999999999999999999999999999999E6144DL
-#define __DEC128_MAX_EXP__ 6145
-#define __DEC128_MIN__ 1E-6143DL
-#define __DEC128_MIN_EXP__ (-6142)
-#define __DEC128_SUBNORMAL_MIN__ 0.000000000000000000000000000000001E-6143DL
-#define __DEC32_EPSILON__ 1E-6DF
-#define __DEC32_MANT_DIG__ 7
-#define __DEC32_MAX__ 9.999999E96DF
-#define __DEC32_MAX_EXP__ 97
-#define __DEC32_MIN__ 1E-95DF
-#define __DEC32_MIN_EXP__ (-94)
-#define __DEC32_SUBNORMAL_MIN__ 0.000001E-95DF
-#define __DEC64_EPSILON__ 1E-15DD
-#define __DEC64_MANT_DIG__ 16
-#define __DEC64_MAX__ 9.999999999999999E384DD
-#define __DEC64_MAX_EXP__ 385
-#define __DEC64_MIN__ 1E-383DD
-#define __DEC64_MIN_EXP__ (-382)
-#define __DEC64_SUBNORMAL_MIN__ 0.000000000000001E-383DD
 #define __DEC_EVAL_METHOD__ 2
-#define __DECIMAL_BID_FORMAT__ 1
-#define __DECIMAL_DIG__ 21
+#define __DECIMAL_DIG__ 36
 #define __ELF__ 1
 #define __FINITE_MATH_ONLY__ 0
 #define __FLOAT_WORD_ORDER__ __ORDER_LITTLE_ENDIAN__
@@ -154,22 +111,22 @@
 #define __FLT64_MIN__ 2.22507385850720138309023271733240406e-308F64
 #define __FLT64_MIN_EXP__ (-1021)
 #define __FLT64_NORM_MAX__ 1.79769313486231570814527423731704357e+308F64
-#define __FLT64X_DECIMAL_DIG__ 21
-#define __FLT64X_DENORM_MIN__ 3.64519953188247460252840593361941982e-4951F64x
-#define __FLT64X_DIG__ 18
-#define __FLT64X_EPSILON__ 1.08420217248550443400745280086994171e-19F64x
+#define __FLT64X_DECIMAL_DIG__ 36
+#define __FLT64X_DENORM_MIN__ 6.47517511943802511092443895822764655e-4966F64x
+#define __FLT64X_DIG__ 33
+#define __FLT64X_EPSILON__ 1.92592994438723585305597794258492732e-34F64x
 #define __FLT64X_HAS_DENORM__ 1
 #define __FLT64X_HAS_INFINITY__ 1
 #define __FLT64X_HAS_QUIET_NAN__ 1
 #define __FLT64X_IS_IEC_60559__ 1
-#define __FLT64X_MANT_DIG__ 64
+#define __FLT64X_MANT_DIG__ 113
 #define __FLT64X_MAX_10_EXP__ 4932
-#define __FLT64X_MAX__ 1.18973149535723176502126385303097021e+4932F64x
+#define __FLT64X_MAX__ 1.18973149535723176508575932662800702e+4932F64x
 #define __FLT64X_MAX_EXP__ 16384
 #define __FLT64X_MIN_10_EXP__ (-4931)
 #define __FLT64X_MIN__ 3.36210314311209350626267781732175260e-4932F64x
 #define __FLT64X_MIN_EXP__ (-16381)
-#define __FLT64X_NORM_MAX__ 1.18973149535723176502126385303097021e+4932F64x
+#define __FLT64X_NORM_MAX__ 1.18973149535723176508575932662800702e+4932F64x
 #define __FLT_DECIMAL_DIG__ 9
 #define __FLT_DENORM_MIN__ 1.40129846432481707092372958328991613e-45F
 #define __FLT_DIG__ 6
@@ -189,8 +146,11 @@
 #define __FLT_MIN_EXP__ (-125)
 #define __FLT_NORM_MAX__ 3.40282346638528859811704183484516925e+38F
 #define __FLT_RADIX__ 2
-#define __FXSR__ 1
-#define __GCC_ASM_FLAG_OUTPUTS__ 1
+#define __FP_FAST_FMA 1
+#define __FP_FAST_FMAF 1
+#define __FP_FAST_FMAF32 1
+#define __FP_FAST_FMAF32x 1
+#define __FP_FAST_FMAF64 1
 #define __GCC_ATOMIC_BOOL_LOCK_FREE 2
 #define __GCC_ATOMIC_CHAR16_T_LOCK_FREE 2
 #define __GCC_ATOMIC_CHAR32_T_LOCK_FREE 2
@@ -202,8 +162,6 @@
 #define __GCC_ATOMIC_SHORT_LOCK_FREE 2
 #define __GCC_ATOMIC_TEST_AND_SET_TRUEVAL 1
 #define __GCC_ATOMIC_WCHAR_T_LOCK_FREE 2
-#define __GCC_CONSTRUCTIVE_SIZE 64
-#define __GCC_DESTRUCTIVE_SIZE 64
 #define __GCC_HAVE_DWARF2_CFI_ASM 1
 #define __GCC_HAVE_SYNC_COMPARE_AND_SWAP_1 1
 #define __GCC_HAVE_SYNC_COMPARE_AND_SWAP_2 1
@@ -219,7 +177,6 @@
 #define __GNUC_WIDE_EXECUTION_CHARSET_NAME "UTF-32LE"
 #define __gnu_linux__ 1
 #define __GXX_ABI_VERSION 1019
-#define __HAVE_SPECULATION_SAFE_VALUE 1
 #define __INT16_C(c) c
 #define __INT16_MAX__ 0x7fff
 #define __INT16_TYPE__ short int
@@ -265,24 +222,22 @@
 #define __INTPTR_TYPE__ long int
 #define __INTPTR_WIDTH__ 64
 #define __INT_WIDTH__ 32
-#define __k8 1
-#define __k8__ 1
-#define __LDBL_DECIMAL_DIG__ 21
-#define __LDBL_DENORM_MIN__ 3.64519953188247460252840593361941982e-4951L
-#define __LDBL_DIG__ 18
-#define __LDBL_EPSILON__ 1.08420217248550443400745280086994171e-19L
+#define __LDBL_DECIMAL_DIG__ 36
+#define __LDBL_DENORM_MIN__ 6.47517511943802511092443895822764655e-4966L
+#define __LDBL_DIG__ 33
+#define __LDBL_EPSILON__ 1.92592994438723585305597794258492732e-34L
 #define __LDBL_HAS_DENORM__ 1
 #define __LDBL_HAS_INFINITY__ 1
 #define __LDBL_HAS_QUIET_NAN__ 1
 #define __LDBL_IS_IEC_60559__ 1
-#define __LDBL_MANT_DIG__ 64
+#define __LDBL_MANT_DIG__ 113
 #define __LDBL_MAX_10_EXP__ 4932
-#define __LDBL_MAX__ 1.18973149535723176502126385303097021e+4932L
+#define __LDBL_MAX__ 1.18973149535723176508575932662800702e+4932L
 #define __LDBL_MAX_EXP__ 16384
 #define __LDBL_MIN_10_EXP__ (-4931)
 #define __LDBL_MIN__ 3.36210314311209350626267781732175260e-4932L
 #define __LDBL_MIN_EXP__ (-16381)
-#define __LDBL_NORM_MAX__ 1.18973149535723176502126385303097021e+4932L
+#define __LDBL_NORM_MAX__ 1.18973149535723176508575932662800702e+4932L
 #define __linux 1
 #define __linux__ 1
 #define linux 1
@@ -292,8 +247,6 @@
 #define __LONG_WIDTH__ 64
 #define __LP64__ 1
 #define _LP64 1
-#define __MMX__ 1
-#define __MMX_WITH_SSE__ 1
 #define __NO_INLINE__ 1
 #define __ORDER_BIG_ENDIAN__ 4321
 #define __ORDER_LITTLE_ENDIAN__ 1234
@@ -307,10 +260,30 @@
 #define __PTRDIFF_TYPE__ long int
 #define __PTRDIFF_WIDTH__ 64
 #define __REGISTER_PREFIX__
+#define __riscv 1
+#define __riscv_a 2001000
+#define __riscv_arch_test 1
+#define __riscv_atomic 1
+#define __riscv_c 2000000
+#define __riscv_cmodel_medany 1
+#define __riscv_compressed 1
+#define __riscv_d 2002000
+#define __riscv_div 1
+#define __riscv_f 2002000
+#define __riscv_fdiv 1
+#define __riscv_flen 64
+#define __riscv_float_abi_double 1
+#define __riscv_fsqrt 1
+#define __riscv_i 2001000
+#define __riscv_m 2000000
+#define __riscv_misaligned_slow 1
+#define __riscv_mul 1
+#define __riscv_muldiv 1
+#define __riscv_xlen 64
+#define __riscv_zicsr 2000000
+#define __riscv_zifencei 2000000
 #define __SCHAR_MAX__ 0x7f
 #define __SCHAR_WIDTH__ 8
-#define __SEG_FS 1
-#define __SEG_GS 1
 #define __SHRT_MAX__ 0x7fff
 #define __SHRT_WIDTH__ 16
 #define __SIG_ATOMIC_MAX__ 0x7fffffff
@@ -319,9 +292,7 @@
 #define __SIG_ATOMIC_WIDTH__ 32
 #define __SIZE_MAX__ 0xffffffffffffffffUL
 #define __SIZEOF_DOUBLE__ 8
-#define __SIZEOF_FLOAT128__ 16
 #define __SIZEOF_FLOAT__ 4
-#define __SIZEOF_FLOAT80__ 16
 #define __SIZEOF_INT128__ 16
 #define __SIZEOF_INT__ 4
 #define __SIZEOF_LONG__ 8
@@ -335,10 +306,6 @@
 #define __SIZEOF_WINT_T__ 4
 #define __SIZE_TYPE__ long unsigned int
 #define __SIZE_WIDTH__ 64
-#define __SSE__ 1
-#define __SSE2__ 1
-#define __SSE2_MATH__ 1
-#define __SSE_MATH__ 1
 #define __STDC__ 1
 #define __STDC_HOSTED__ 1
 #define __STDC_IEC_559__ 1
@@ -396,5 +363,3 @@
 #define __WINT_MIN__ 0U
 #define __WINT_TYPE__ unsigned int
 #define __WINT_WIDTH__ 32
-#define __x86_64 1
-#define __x86_64__ 1
```

## 预定义宏的文档

在[手册](https://gcc.gnu.org/onlinedocs/cpp/Predefined-Macros.html)中：
预定义宏分为标准宏、通用宏和系统特定宏。

标准宏比如 `__FILE__` 和 `__LINE__` 还有 `__DATE__` 和 `__TIME__` 等。

预定义宏是 GNU C 扩展。有比如 `__COUNTER__`，`__GNUC__`，`__FILE_NAME__`，`__ELF__` 等。

系统特定预定义宏是用来指示所用的系统和机器类型，可以用 `cpp -dM` 查看所有宏，用 `#ifdef` 来测试。
在历史上有些宏没有前缀，比如 `unix`，此时 GCC 都会定义一个双下划线版本 `__unix__`。
