/*=========================================================

=========================================================*/
Socket32::Socket32(){
  initSocket32();
}
Socket32::~Socket32(){
  reset();
}
/*=========================================================

=========================================================*/
void Socket32::initSocket32(void){
  lastRead=lastWrite=erro=timeout=lineSize=nonblock=nodelay=isopen=verbose=0;
  sk=0;

}
/*=========================================================

=========================================================*/
String Socket32::getDns(void){return dns;}
Split &Socket32::getHeaders(void){return headers;}
int Socket32::getNodelay(void){return nodelay;}
int Socket32::getLineSize(void){return lineSize;}
int Socket32::getLastWrite(void){return lastWrite;}
int Socket32::getLastRead(void){return lastRead;}

int Socket32::getError(void){return erro;}
int Socket32::getNonblock(void){return nonblock;}
int Socket32::getTimeout(void){return timeout;}
Socket32 &Socket32::setDebug(int debug){this->verbose=debug;return *this;}
Socket32 &Socket32::setDns(const String &dns){this->dns=dns;return *this;}
Socket32 &Socket32::setLineSize(int lineSize){this->lineSize=lineSize;return *this;}
/*=========================================================

=========================================================*/
Socket32 &Socket32::setNodelay(int nodelay){
  if(!isopen)
    this->nodelay=nodelay;
  else{
    char
      ok=sk32setnodelay(sk,nodelay);

    if(ok)
      this->nodelay=nodelay;
  }
  return *this;
}
/*=========================================================

=========================================================*/
Socket32 &Socket32::setNonblock(int nonblock){
  if(!isopen)
    this->nonblock=nonblock;
  else{
    char
      ok=sk32setnonblock(sk,nonblock);

    if(ok)
      this->nonblock=nonblock;
  }
  return *this;
}
/*=========================================================

=========================================================*/
Socket32 &Socket32::setTimeout(int timeout){
  if(!isopen)
    this->timeout=timeout;
  else{
    char
      ok=sk32settimeout(sk,timeout);

    if(ok)
      this->timeout=timeout;
  }
  return *this;
}
/*=========================================================

=========================================================*/
SK32 &Socket32::getSk(void){
  if(sk)
    return *sk;
  else{
    SK32
      *s=(SK32*)cMemAlloc(sizeof(SK32));

    gco.add(s);
    return *s;
  }
}
/*=========================================================

=========================================================*/
Socket32 &Socket32::reset(void){
  if(sk)
    sk32free(sk);
  headers.reset();
  initSocket32();
  return *this;
}
/*=========================================================

=========================================================*/
String Socket32::resolve(const String &host){
  ulong
    ip;
  String
    buffer;

  if(!dns.len())
    setDns("127.0.0.1");
  ip=dnsResolveTry(PSTRING(host),dns.ptr(),53);
  buffer.resize(100);
  return dnsHostTxt(ip,buffer.ptr());
}
/*=========================================================

=========================================================*/
Socket32 &Socket32::close(void){
  if(sk){
    sk32free(sk);
    sk=0;
  }
  isopen=erro=lastRead=lastWrite=0;
  //initSocket32();
  return *this;
}
/*=========================================================

=========================================================*/
SK32 *Socket32::instanceSK(void){
  return (SK32*)cMemAlloc(sizeof(SK32));
}
/*=========================================================

=========================================================*/
char Socket32::open(const String &host,int port){
  String
    ip=resolve(host);
  char
    ok=0;

  close();
  erro=1;
  if(ip.equals("0.0.0.0"))
    goto FIM;
  sk=instanceSK();
  sk->debug=verbose;
  ok=sk32connect(
    sk,
    PCHAR(ip.ptr()),
    port,
    getNodelay(),
    getTimeout()
  );
  if(!ok){
    close();
    goto FIM;
  }
  if(nonblock&&!sk32setnonblock(sk,nonblock)){
    close();
    goto FIM;
  }

  erro=0;
  isopen=1;
  FIM:
    erro=ok?0:1;
  return ok;
}
/*=========================================================

=========================================================*/
Socket32 &Socket32::debug(void){
  cPrintf(
    "ip       : [%s]\n"
    "port     : [%d]\n"
    "error    : [%d]\n"
    "isopen   : [%d]\n"
    "lineSize : [%d]\n"
    "nodelay  : [%d]\n"
    "nonblock : [%d]\n"
    "timeout  : [%d]\n"
    "lastWrite: [%d]\n"
    "lastRead : [%d]\n"
    "dns      : [%s]\n"
    "\n",
    isopen?sk->ip:"",
    isopen?sk->port:0,
    erro,
    isopen,
    lineSize,
    nodelay,
    nonblock,
    timeout,
    lastWrite,
    lastRead,
    dns.ptr()
  );
  return *this;
}
/*=========================================================

=========================================================*/
char Socket32::write(const String &buffer){
  String
    &p=PSTRING(buffer);
  ByteArray
    b;

  b.set(p.ptr(),p.len());
  return write(b);
}
/*=========================================================

=========================================================*/
char Socket32::write(ByteArray &buffer){
  lastWrite=0;
  if(!isopen)
    return 0;
  else{
    sk->write(sk,buffer.ptr(),buffer.len());
    erro=sk->error;
    if(!erro)
      lastWrite=buffer.len();

    return !erro;
  }
}
/*=========================================================

=========================================================*/
char Socket32::headerResolve(void){
  headers.reset();
  if(!isopen)
    return 0;
  headers=sk32headers(sk);
  if(!(erro=sk->error)){
    sk32contentlength(headers,sk);
    return 1;
  }
  else{
    headers.reset();
    return 0;
  }
}
/*=========================================================

=========================================================*/
int Socket32::getContentLength(void){
  return isopen&&sk
    ?sk->contentLen
    :0;
}
/*=========================================================

=========================================================*/
char Socket32::isChunk(void){
  return isopen&&sk
    ?sk->ischuncked
    :0;
}
/*=========================================================

=========================================================*/
char Socket32::isOpen(void){
  return isopen&&sk;
}
/*=========================================================

=========================================================*/
char Socket32::read(ByteArray &buffer,int len){
  lastRead=0;
  if(!isopen)
    return 0;
  if(buffer.len()<len)
    buffer.resize(len);
  //buffer.fill(0);
  buffer.seek(0);
  sk->read(sk,PCHAR(buffer.ptr()),len);
  if(!(erro=sk->error)){
    lastRead=len;
    return 1;
  }
  //buffer.fill(0);
  return 0;
}
/*=========================================================

=========================================================*/
char Socket32::readChunk(ByteArray &buffer){
  lastRead=0;
  if(!isopen)
    return 0;
  else{
    void
      *tmp=0;
    char
      ok=sk32readchunked(&getSk(),&tmp);

    buffer.reset();
    if(ok){
      buffer.write(tmp,cMemSize(tmp));
      lastRead=buffer.len();
    }
    cMemFree(tmp);
    return erro=ok?1:0;
  }
}
