/**********************************************************
https://www.cpan.org/src/5.0/perl-5.14.4.tar.bz2
https://ftp.openssl.org/source/old/1.1.1/openssl-1.1.1d.tar.gz
Configure linux-x86
**********************************************************/
char  sk32sslconnect(SK32 *ssl,SK32 *s);
char  sk32sslconnectmethod(SK32 *ssl,void *method,SK32 *s);
SK32 *sk32sslinit(void *libcry,void *libssl);
/*=========================================================

=========================================================*/
char sk32sslconnectmethod(SK32 *ssl,void *method,SK32 *s){
  if(!ssl||!s)
    return 0;
  s->error=1;
  if(s->s<=0)
    return 0;
  if(!ssl->islib)
    return 0;
  {
    int
      len=sizeof(SK32)/sizeof(char*),
      i;
    void
      **pl=(void**)ssl,
      **ps=(void**)s,
      *(*pf)();

    for(i=len-1;i&&(ulong)pl[i]!=0x4D;i--)
      ps[i]=pl[i];
    s->isssl=1;
    cDebug(s->debug,"sk32sslconnectm: method=[%s]",method);
    if(!(s->method=cLibrarySymbol(ssl->lib,PCHAR(method))))
      if(!(s->method=cLibrarySymbol(ssl->libcry,PCHAR(method))))
        return sk32close(s);
    cDebug(s->debug,"sk32sslconnectm: method=[%X]",s->method);
    *(void**)&pf=s->method;
    s->method=pf();
    cDebug(s->debug,"sk32sslconnectm: method=[%X]",s->method);
    if(!(s->ctx=s->SSL_CTX_new(s->method)))
      return sk32close(s);
    cDebug(s->debug,"sk32sslconnectm: ctx=[%X]",s->ctx);
    if(!(s->ssl=s->SSL_new(s->ctx)))
      return sk32close(s);
    cDebug(s->debug,"sk32sslconnectm: ssl=[%X]",s->ssl);
    s->methodtxt=cMemAddChar(PCHAR(method));
    s->SSL_set_fd(s->ssl,s->s);
    cDebug(s->debug,"sk32sslconnectm: [%d]->[%X]",s->s,s->ssl);
    i=s->SSL_connect(s->ssl);
    cDebug(s->debug,"sk32sslconnectm: connect=[%d]",i);
    if(1!=i)
      return sk32close(s);
    s->error=0;
    s->close=sk32close;
    s->read=sk32read;
    s->write=sk32write;
    return 1;
  }
}
/*=========================================================

=========================================================*/
SK32 *sk32sslinit(void *libcry,void *libssl){
  if(!(libcry=cLibraryLoad(PCHAR(libcry))))
    return 0;
  if(!(libssl=cLibraryLoad(PCHAR(libssl)))){
    cLibraryFree(libcry);
    return 0;
  }
  {
    SK32
      *ssl=sk32create();

    ssl->lib=libssl;
    ssl->libcry=libcry;
    ssl->islib=1;

    #define symbol(name)\
      *(void**)&ssl->name=cLibrarySymbol(ssl->lib,#name);

    symbol(SHA1_Init)
    symbol(SHA1_Update)
    symbol(SHA1_Final)
    symbol(SSL_library_init)
    symbol(OPENSSL_init_ssl)
    symbol(TLSv1_2_client_method)
    symbol(TLSv1_1_client_method)
    symbol(TLSv1_client_method)
    symbol(SSL_CTX_new)
    symbol(SSL_new)
    symbol(SSL_set_fd)
    symbol(SSL_connect)
    symbol(SSL_write)
    symbol(SSL_read)
    symbol(SSL_free)
    symbol(SSL_CTX_free)
    symbol(SSL_shutdown)
    symbol(SSL_get_rfd)
    symbol(SSL_get_wfd)
    symbol(AES_set_encrypt_key)
    symbol(AES_cbc_encrypt)
    symbol(AES_set_decrypt_key)
    symbol(EVP_get_cipherbyname)
    symbol(EVP_CIPHER_CTX_new)
    symbol(EVP_aes_128_cbc)
    symbol(EVP_EncryptInit_ex)
    symbol(EVP_EncryptUpdate)
    symbol(EVP_EncryptFinal_ex)
    symbol(EVP_DecryptInit_ex)
    symbol(EVP_DecryptUpdate)
    symbol(EVP_DecryptFinal_ex)
    symbol(EVP_CIPHER_CTX_free)

    #undef symbol

    if(ssl->SSL_library_init)
      ssl->SSL_library_init();
    else
      ssl->OPENSSL_init_ssl(0,0);

    ssl->magic=0x4D;
    return ssl;
  }
}
/*=========================================================

=========================================================*/
char sk32sslconnect(SK32 *ssl,SK32 *s){
  char
    *method="TLSv1_2_client_method";

  return sk32sslconnectmethod(ssl,method,s);
}
