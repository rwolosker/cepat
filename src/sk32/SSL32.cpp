/*=========================================================

=========================================================*/
void SSL32::initSsl32(void){
  sk=0;
}
SSL32::SSL32(){
  initSsl32();
}
SSL32::~SSL32(){
  if(sk)
    sk32free(sk);
}
/*=========================================================

=========================================================*/
SK32 &SSL32::getSk(void){
  return *sk;
}
/*=========================================================

=========================================================*/
void SSL32::setKey(cchar *key){
  this->key.set(key);
}
/*=========================================================

=========================================================*/
char SSL32::init(cchar *libcrypt,cchar *libssl){
  File
    f;

  sk=sk32free(sk);
  return 1
    &&f.access(libcrypt,"r--")
    &&f.access(libssl,"r--")
    &&(sk=sk32sslinit(VOID(libcrypt),VOID(libssl)))
  ;
}
/*=========================================================

=========================================================*/
char SSL32::handshake(Socket32 &s){
  if(!s.isOpen())
    return 0;
  return sk32sslconnect(
    sk,
    &s.getSk()
  );
}
/*=========================================================
  $token="0JnY75Hn96qPliPW2nGNFe3+4QqTyaAGNB0clJlyyEo=";
  $id=openssl_decrypt(
      $token,
      "aes-128-cbc",
      "1234560000000000",
      0,
      "0000000000123456"
  );
  printf("$id\n");

=========================================================*/
char SSL32::encrypt(cchar *key,ByteArray &buffer){
  String
    pkey=String().sprintf("%-16.16s",key?key:this->key.ptr()).replace(' ','0'),
    iv=String().sprintf("%16.16s",key?key:this->key.ptr()).replace(' ','0');
  int
    ok,
    len,
    total=0,
    outLen=1024+4*buffer.len();
  void
    *ctx=sk->EVP_CIPHER_CTX_new();
  ByteArray
    tmp;

  tmp.resize(outLen);

  if(1!=(ok=sk->EVP_EncryptInit_ex(ctx,sk->EVP_aes_128_cbc(),0,pkey.ptr(),iv.ptr())))
    goto FIM;
  if(1!=(ok=sk->EVP_EncryptUpdate(ctx,tmp.ptr(),&len,buffer.ptr(),buffer.len())))
    goto FIM;
  total+=len;
  if(1!=(ok=sk->EVP_EncryptFinal_ex(ctx,PCHAR(tmp.ptr())+len,&len)))
    goto FIM;
  total+=len;

  tmp.resize(total);
  buffer
    .reset()
    .append(Base64().encode(tmp).ptr());
  FIM:
  sk->EVP_CIPHER_CTX_free(ctx);
  return ok==1?1:0;
}
/*=========================================================

=========================================================*/
char SSL32::decrypt(cchar *key,ByteArray &buffer){
  String
    pkey=String().sprintf("%-16.16s",key?key:this->key.ptr()).replace(' ','0'),
    iv=String().sprintf("%16.16s",key?key:this->key.ptr()).replace(' ','0');
  int
    ok,
    len,
    total=0,
    outLen=1024+2*buffer.len();
  void
    *ctx=sk->EVP_CIPHER_CTX_new();
  ByteArray
    tmp;

  buffer=Base64().decode(buffer.toString());
  tmp.resize(outLen);

  if(1!=(ok=sk->EVP_DecryptInit_ex(ctx,sk->EVP_aes_128_cbc(),0,pkey.ptr(),iv.ptr())))
    goto FIM;
  if(1!=(ok=sk->EVP_DecryptUpdate(ctx,tmp.ptr(),&len,buffer.ptr(),buffer.len())))
    goto FIM;
  total+=len;
  if(1!=(ok=sk->EVP_DecryptFinal_ex(ctx,PCHAR(tmp.ptr())+len,&len)))
    goto FIM;
  total+=len;

  tmp.resize(total);
  buffer=tmp;
  FIM:
  sk->EVP_CIPHER_CTX_free(ctx);
  if(ok==1)
    return ok;
  buffer.reset();
  return 0;
}
/*=========================================================
EVP_sha1
=========================================================*/
char SSL32::sha1(ByteArray &out){
  void
    *ctx,
    *sha1;
  int
    outLen=0;
  ByteArray
    tmp;

  void
    *(*EVP_MD_CTX_new)(void)=0,
    *(*EVP_get_digestbyname)(cchar*)=0,
     (*EVP_MD_CTX_free)(void*)=0;
  int
    (*EVP_DigestInit_ex)(void*,void*,void*)=0,
    (*EVP_DigestUpdate)(void*,cchar*,int)=0,
    (*EVP_DigestFinal_ex)(void*,void*,int*)=0;


  *(void**)&EVP_MD_CTX_new=cLibrarySymbol(sk->libcry,"EVP_MD_CTX_new");
  *(void**)&EVP_get_digestbyname=cLibrarySymbol(sk->libcry,"EVP_get_digestbyname");
  *(void**)&EVP_DigestInit_ex=cLibrarySymbol(sk->libcry,"EVP_DigestInit_ex");
  *(void**)&EVP_DigestUpdate=cLibrarySymbol(sk->libcry,"EVP_DigestUpdate");
  *(void**)&EVP_DigestFinal_ex=cLibrarySymbol(sk->libcry,"EVP_DigestFinal_ex");
  *(void**)&EVP_MD_CTX_free=cLibrarySymbol(sk->libcry,"EVP_MD_CTX_free");

  ctx=EVP_MD_CTX_new();
  sha1=EVP_get_digestbyname("sha1");
  EVP_DigestInit_ex(ctx,sha1,0);
  EVP_DigestUpdate(ctx,PCCHAR(out.ptr()),out.len());
  EVP_DigestFinal_ex(ctx,out.reset().resize(100).ptr(),&outLen);
  EVP_MD_CTX_free(ctx);

  out.resize(outLen);
  return 1;
}

