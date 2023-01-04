#include "dns.h"

typedef struct PACK SK32{
  char
    debug,
    nodelay,
    nonblock,
    islib,
    isssl,
    ischuncked,
    nosleep,
    r8;
  void
    *lib,
    *libcry,
    *ctx,
    *ssl,
    *method,
    *methodtxt,
    *host,
    *ip,
    *dnstxt;
  int
    s,
    error,
    timeout,
    port,
    contentLen,
    magic;
  int  (*SHA1_Init)(void*);
  int  (*SHA1_Update)(void*,void*,long);
  int  (*SHA1_Final)(void*,void*);
  int  (*SSL_library_init)(void);
  int  (*OPENSSL_init_ssl)(long long,void*);
  void*(*TLSv1_2_client_method)(void);
  void*(*TLSv1_1_client_method)(void);
  void*(*TLSv1_client_method)(void);
  void*(*SSL_CTX_new)(void*);
  void*(*SSL_new)(void*);
  int  (*SSL_set_fd)(void*,int);
  int  (*SSL_connect)(void*);
  int  (*SSL_write)(void*,void*,int);
  int  (*SSL_read)(void*,void*,int);
  void (*SSL_free)(void*);
  void (*SSL_CTX_free)(void*);
  int  (*SSL_shutdown)(void*);
  int  (*SSL_get_rfd)(void*);
  int  (*SSL_get_wfd)(void*);
  int  (*AES_set_encrypt_key)(void*,int,void*);
  void (*AES_cbc_encrypt)(void*,void*,int,void*,void*,int);
  int  (*AES_set_decrypt_key)(void*,int,void*);
  void*(*EVP_get_cipherbyname)(void*);
  void*(*EVP_CIPHER_CTX_new)(void);
  void*(*EVP_aes_128_cbc)(void);
  int  (*EVP_EncryptInit_ex)(void*,void*,void*,void*,void*);
  int  (*EVP_EncryptUpdate)(void*,void*,int*,void*,int);
  int  (*EVP_EncryptFinal_ex)(void*,void*,int*);
  void(*EVP_CIPHER_CTX_free)(void*);
  int  (*EVP_DecryptInit_ex)(void*,void*,void*,void*,void*);
  int  (*EVP_DecryptUpdate)(void*,void*,int*,void*,int);
  int  (*EVP_DecryptFinal_ex)(void*,void*,int*);
  int  (*write)(struct SK32*,void*,int);
  int  (*read)(struct SK32*,char*,int);
  int  (*close)(struct SK32*);
}SK32;

#define AES_BLOCK_SIZE (16)
#define AES_DECRYPT    ( 0)
#define AES_ENCRYPT    ( 1)

extern char
  sk32debug;
extern int
  sk32readpart;

int   sk32close(SK32 *s);
char  sk32connect(SK32 *s,char *host,int port,int nodelay,int timeout);
int   sk32contentlength(Split &header,SK32 *s);
SK32 *sk32create(void);
int   sk32error(SK32 *s);
SK32 *sk32free(SK32 *s);
char  sk32getnonblock(SK32 *s,int value);
int   sk32gettimeout(SK32 *s);
Split sk32headers(SK32 *s);
void  sk32headersdebug(Split &list);
int   sk32iptoaddr(char *ip);
char  sk32isready(SK32 *s,int sec,int msec,char *bits);
char  sk32listen(SK32 *s,int max);
int   sk32read(SK32 *s,char *buffer,int len);
char  sk32readchunked(SK32 *s,void *out);
char  sk32readdump(SK32 *s,void *out);
int   sk32readline(SK32 *s,char *buffer);
int   sk32readnonblock(SK32 *s,char *buffer,int len);
SK32 *sk32server(int port);
void  sk32setdebug(SK32 *s,char value);
void  sk32setdns(SK32 *s,char *value);
char  sk32settimeout(SK32 *s,int second);
char  sk32setnodelay(SK32 *s,int value);
char  sk32setnonblock(SK32 *s,int value);
int   sk32write(SK32 *s,void *buffer,int len);

#include "sk32.cpp"
#include "sk32ssl.cpp"
#include "Socket32.h"
#include "Socket32.cpp"
#include "SSL32.h"
#include "SSL32.cpp"
#include "HttpGet.h"
#include "HttpGet.cpp"

//#include "Forward.h"
//#include "Forward.cpp"
