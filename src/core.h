
/**********************************************************
g++ -dM -E -x c++ - < /dev/null
**********************************************************/
#if !defined(__GNUC__)
  #define __GNUC__ 0
#endif

typedef const char cchar;
typedef const void cvoid;
typedef unsigned char uchar;
typedef unsigned long ulong;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef const unsigned char cuchar;
typedef long double dbl64;
typedef long long int64;
typedef unsigned long long uint64;

#define PACK __attribute__((packed, aligned(1)))

typedef struct PACK C_CALL{
  char p[512];
}C_CALL;
typedef struct PACK FLOCK{
  short
    type,
    seek;
  int
    start,
    end,
    pid;
}FLOCK;
typedef struct PACK TIMESPEC{
  ulong
    sec,
    nsec;
}TIMESPEC;
typedef struct PACK TM{
  int
    sec,
    min,
    hour,
    mday,
    mon,
    year,
    wday,
    yday,
    isdst;
  long
    gmtoff;
  char
    *zone;
}TM;

inline void* operator new(uint,void *p){
  return p;
}

#define PCHAR(x)    ((char*)(x))
#define PCCHAR(x)   (cchar*)(x)
#define PUCHAR(x)   (uchar*)(x)
#define VOID(x)     (void*)(x)
#define SIZEOF_VOID (4)
#define FOUND(x)    (((x)!=-1)?1:0)
#define USED __attribute__((used))
#define NEW(gc,size,clazz) new(gc.add(cMemAlloc(size)))clazz();
#define CSTRING(s) (*(String*)&s)
#define PSTRING(value) (*((String*)&value))
#define STRING2REF(value) (*(String*)value.me())
#define PBYTEARRAY(value) (*((ByteArray*)&value))
#define PNUMBER(value) (*((Number*)&value))
#define PDATE(value) (*((Date*)&value))
#ifndef __clang__
  #define METHOD(n) (reinterpret_cast<void*>(n))
#else
  #define METHOD(n) cMethodAddress(n)
#endif
#define OBJECT(n) ((Object*)n)
#define TABLESORT(v,sort) TableSort &v=*(TableSort*)&sort;
#define TZ_RECIFE "America/Fortaleza"
#define REFLECTION_SIZEOF_MAX (1024)

#ifndef NO_LIBC

extern char *__tzname[2];
extern int   __daylight;
extern long  __timezone;
extern char *__progname;
extern char *__progname_full;

extern"C"{
  int     accept(int,void*,void*);
  int     abs(int);
  int     bind(int,void*,int);
  void   *calloc(int,int);
  double  ceil(double);
  int     chdir(cchar*);
  int     close(int);
  int     closedir(void*);
  int     clock_gettime(int,void*);
  int     connect(int,void*,int);
  void    dlclose(void*);
  char   *dlerror(void);
  void   *dlopen(char*,int);
  void   *dlsym(cvoid*,cvoid*);
  int     dup(int);
  int     dup2(int,int);
  int    *__errno_location(void);
  int     execv(cchar*,char**);
  void    exit(int);
  int     fchmod(int,int);
  int     fclose(void*);
  int     fcntl(int,int,...);
  int     flock(int,int);
  double  floor(double);
  dbl64   floorl(dbl64);
  double  fmod(double,double);
  int     fork(void);
  int     fread(void*,int,int,void*);
  double  frexp(double,int*);
  void   *free(void*);
  int     ftruncate(int,int);
  int     fstat(int,void*);
  int     fsync(int);
  char   *getcwd(cchar*,int);
  char   *getenv(cchar*);
  int     getpid(void);
  int     getsockopt(int,int,int,void*,void*);
  int     isinf(double);
  int     isnan(double);
  int     isprint(int);
  double  ldexp (double,int);
  int     listen(int,int);
  int64   llabs(int64);
  void   *localtime_r(void*,void*);
  int     lockf(int,int,int);
  double  log10(double);
  int     lseek(int,int,int);
  void   *malloc(int);
  int     malloc_usable_size(void*);
  C_CALL  mallinfo(void);
  void   *memcpy(void*,cvoid*,int);
  void   *memmem(void*,int,void*,int);
  void   *memset(void*,int,int);
  int     mkstemp(cchar*);
  int     open(cchar*,int,int);
  void   *opendir(cchar*);
  int     pclose (void*);
  void   *popen(cchar*,cchar*);
  double  pow(double,double);
  int     printf(cvoid*,...);
  int     putenv(cchar*);
  void    qsort(void*,int,int,void*);
  void    qsort_r(void*,int,int,void*,void*);
  void    _quicksort(void*,int,int,void*,void*);
  int     read(int,void*,int);
  int     readdir_r(void*,void*,void**);
  void   *realloc(cvoid*,int);
  int     recv(int,void*,int,int);
  int     recvfrom(int,void*,int,int,void*,void*);
  int     remove(cchar*);
  void    rewind(void*);
  int     select(int,void*,void*,void*,void*);
  int     send(int,void*,int,int);
  int     sendto(int,void*,int,int,void*,int);
  int     setenv(cchar*,cchar*,int);
  int     setsockopt(int,int,int,void*,int);
  int     sleep(int);
  int     snprintf(char*,int,cchar*,...);
  int     socket(int,int,int);
  int     sprintf(char*,cchar*,...);
  int     sscanf(cchar*,cchar*,...);
  char   *strpbrk(cchar*,cchar*);
  char   *strchr(cchar*,int);
  char   *strrchr(cchar*,int);
  int     strcasecmp(cchar*,cchar*);
  int     strcmp(cchar*,cchar*);
  int     strncmp(cchar*,cchar*,int);
  int     strftime(void*,int,void*,void*);
  char   *strptime(cchar*,cchar*,void*);
  char   *strcpy(char*,cchar*);
  char   *strncpy(char*,cchar*,int);
  int     strlen(cchar*);
  char   *strstr(cchar*,cchar*);
  char   *strtok_r(char*,cchar*,char**);
  int     system(cchar*);
  int     time(void*);
  int     timegm(void*);
  uint    timelocal(void*);
  int     times(void*);
  void    tzset(void);
  int     unsetenv(cchar*);
  int     usleep(int);
  int     vasprintf(char**,cchar*,void*);
  int     waitpid(int,int*,int);
  int     write(int,cvoid*,int);
  int   __xstat64(int,void*,void*);
}
#endif

#include "type.h"

#define AF_INET       2
#define DT_DIR        4
#define DT_REG        8
#define F_GETFL       3
#define F_SETFL       4
#define F_SETLK       6
#define F_UNLCK       2
#define F_WRLCK       1
#define IPPROTO_IP    0
#define IPPROTO_UDP   17
#define IP_RECVERR    11
#define IP_RECVTOS    13
#define LOCK_EX       4
#define LOCK_UN       8
#define MSG_NOSIGNAL  0x4000
#define O_CREAT       0100
#define O_EXCL        0200
#define O_LARGEFILE   0100000
#define O_NONBLOCK    SOCK_NONBLOCK
#define O_RDONLY      00
#define O_RDWR        02
#define O_WRONLY      01
#define PF_INET       2
#define SEEK_CUR      1
#define SEEK_END      2
#define SEEK_SET      0
#define S_IFDIR       0040000
#define S_IFMT        0170000
#define S_IRGRP       00040
#define S_IRUSR       00400
#define S_IRWXU       00700
#define S_IWGRP       00020
#define S_IWUSR       00200

/* File types.  */

#define SOCK_CLOEXEC  02000000
#define SOCK_DGRAM    2
#define SOCK_NONBLOCK 00004000
#define SOCK_STREAM   1
#define SO_ERROR      4
#define SOL_IP        0
#define SOL_SOCKET    1
#define SOL_TCP       6
#define SO_RCVTIMEO   20
#define SO_SNDTIMEO   21
#define TCP_NODELAY   1
#define SO_REUSEADDR  2

typedef struct PACK NODE{
  int size;
  void *ptr;
  NODE *next;
  NODE *prev;
  NODE *last;
}NODE;

#include "config.cpp"

extern NODE
  *cMemNode;
extern int
  cStdout,
  cDateMax,
  cDateUtcMax;
extern char
  cDateTz;
extern void
  *cHexdumpPrint;

class String;
class ByteArray;
class Map;

void   cCpp(String &path,String &name,ByteArray &buffer);
void   cCppInclude(String &fileName,String &linha,ByteArray &buffer);
void   cCppMain(cchar *file);
double cDateParseFast(cchar *value);
void   cDateTzConfig(void);
void   cDateTzChange(cchar *value);
void   cDebug(char debug,cchar *format,...);
void   cGnuStart(int *start,char isarray=1);
void   cHexdump(void *ptr,int size,int width=8);
char   cIconvToLower(uchar c);
char   cIconvToUpper(uchar c);
char   cIconvNormalize(uchar c);
void  *cLibraryFree(void *lib);
void  *cLibraryLoad(char *file);
void  *cLibrarySymbol(void *lib,char *name);
char  *cMemAddChar(char *str);
void  *cMemAlloc(int size);
char  *cMemAllocChar(int size);
void  *cMemCpy(void *src,long size,void *dst);
void   cMemDebug(void);
void  *cMemFree(void *ptr);
void   cMemFreeAll(...);
void   cMemFreeAllImpl(void *ptr);
NODE  *cMemNodePtr(void *ptr);
void   cMemPrintUsed(char real=0);
void  *cMemRealloc(void *ptr,int size);
void  *cMemReset(void *ptr,int size);
void  *cMemSet(int c,int size,void *ptr);
int    cMemSize(void *ptr);
int    cMemUsed(char real=0);
void  *cMethodAddress(...);
NODE  *cNodeAdd(NODE *src,NODE *node);
void   cNodeDebug(NODE *node);
NODE  *cNodeDel(NODE *item,NODE *node,void*(*pfree)(void*)=0);
double cNumberParse(cchar *value);
void   cPrintf(cchar *format,...);
double cProcessEnd(void *t1,char print=0);
void  *cProcessStart(void);
char   cStrCompare(cchar *a,cchar *b);
char   cStrCompareNormalize(cchar *a,cchar *b);
char  *cStrCopy(cchar *src,char *dst);
int    cStrIndexOf(cchar *value,int query);
int    cStrIndexOf(cchar *value,cchar *query);
int    cStrLen(cchar *str);
char  *cStrMid(char *src,int pos,int size,char *dst);
char  *cStrTr(char *str,int len,char *from,char *to,int trlen);
char  *cStrReplace(cchar *src,cchar *query,cchar *replace,cchar *dst);
char  *cStrTrim(cchar *src,cchar *dst);
char  *cVsprintf(void *pformat,int *size=0);

class StringList;
class Date;
class Sort;

#include "source/qsort.cpp"
#include "iconv.h"
#include "core.cpp"

#include "header/Object.h"
#include "header/Gc.h"
#include "header/GcObject.h"
#include "header/CharRef.h"
#include "header/String.h"
#include "header/Split.h"
#include "header/Type.h"
#include "header/ByteArray.h"
#include "header/Base64.h"
#include "header/Number.h"
#include "header/Sort.h"
#include "julian/julian.h"
#include "header/DateFormat.h"
#include "header/Date.h"
#include "header/File.h"
#include "header/StringList.h"
#include "header/MapItem.h"
#include "header/Map.h"
#include "header/Reflection.h"
#include "header/Factory.h"
#include "header/ext/ext.h"
#include "header/app.h"
#include "header/DirItem.h"
#include "header/Dir.h"
#include "header/Shell.h"
#include "header/RegExp.h"

#include "source/Object.cpp"
#include "source/Gc.cpp"
#include "source/GcObject.cpp"
#include "source/CharRef.cpp"
#include "source/String.cpp"
#include "source/ByteArray.cpp"
#include "source/File.cpp"
#include "source/Number.cpp"
#include "source/StringList.cpp"
#include "source/Sort.cpp"
#include "source/Date.cpp"
#include "source/DateFormat.cpp"
#include "source/MapItem.cpp"
#include "source/Map.cpp"
#include "source/Type.cpp"
#include "source/Reflection.cpp"
#include "source/Factory.cpp"
#include "source/app.cpp"
#include "source/Dir.cpp"
#include "source/DirItem.cpp"
#include "source/Split.cpp"
#include "source/Shell.cpp"
#include "source/RegExp.cpp"
#include "source/Base64.cpp"
