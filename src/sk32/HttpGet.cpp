HttpGet::HttpGet(){
  initHttpGet();
}
HttpGet::~HttpGet(){
}
void HttpGet::initHttpGet(void){
  port=timeOut=noDelay=nonBlock=0;
}
/*=========================================================

=========================================================*/
HttpGet  &HttpGet::setTimeOut(int timeOut){this->timeOut=timeOut;return *this;}
HttpGet  &HttpGet::setPort(int port){this->port=port;return *this;}
HttpGet  &HttpGet::setNoDelay(char noDelay){this->noDelay=noDelay;return *this;}
HttpGet  &HttpGet::setNonBlock(char nonBlock){this->nonBlock=nonBlock;return *this;}
HttpGet  &HttpGet::setHost(const String &host){this->host=host;return *this;}
HttpGet  &HttpGet::setDns(const String &dns){this->dns=dns;return *this;}
String HttpGet::getErro(void){return erro;}
/*=========================================================

=========================================================*/
HttpGet &HttpGet::open(void){
  header.reset();
  erro.reset();
  socket
    .close()
    .reset();
  if(!dns.len())
    dns="127.0.0.1";
  if(!port){
    erro="ERRO: port.";
    goto FIM;
  }
  if(!host.len()){
    erro="ERRO: len.";
    goto FIM;
  }

  if(!timeOut)
    timeOut=30;
  socket
    .setDns(dns)
    .setNodelay(noDelay)
    .setNonblock(nonBlock)
    .setTimeout(timeOut);

  if(!socket.open(host,port)){
    erro="ERRO: connect.";
    goto FIM;
  }
  FIM:
  return *this;
}
/*=========================================================

=========================================================*/
char HttpGet::sendUrl(const String &url){
  String
    tmp;

  tmp=String().sprintf(
    "GET %s HTTP/1.1\r\n"
    "Host: %s\r\n"
    "Connection: Close\r\n"
    "\r\n",
    PSTRING(url).ptr(),
    host.ptr()
  );
  if(!socket.isOpen()){
    erro="ERRO: not open.";
    return 0;
  }
  if(!socket.write(tmp)){
    erro="ERRO: write.";
    return 0;
  }
  return 1;
}
/*=========================================================

=========================================================*/
char HttpGet::headers(void){
  if(!socket.isOpen()){
    erro="ERRO: not open.";
    return 0;
  }
  if(!socket.headerResolve()){
    erro="ERRO: headerResolve.";
    return 0;
  }
  header=socket.getHeaders();
  return 1;
}
/*=========================================================

=========================================================*/
char HttpGet::download(ByteArray &buffer){
  if(!socket.isOpen()){
    erro="ERRO: not open.";
    return 0;
  }
  if(socket.isChunk()){
    void
      *tmp=0;
    char
      ok=sk32readchunked(&socket.getSk(),&tmp);

    if(ok)
      buffer.write(tmp,cMemSize(tmp));
    else
      erro="ERRO: chuncked.";
    cMemFree(tmp);

    return ok?1:0;
  }
  else{
    char
      ok=socket.read(buffer,socket.getContentLength());

    if(!ok)
      erro="ERRO: read all.";
    return ok;
  }
}
/*=========================================================

=========================================================*/
HttpGet &HttpGet::download(const String &url,ByteArray &buffer){

  1
    &&sendUrl(url)
    &&headers()
    &&download(buffer.reset());

  socket.close();
  return *this;
}
