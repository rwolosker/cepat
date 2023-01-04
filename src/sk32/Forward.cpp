/*=========================================================

=========================================================*/
Forward::Forward(){
  initForward();
}
Forward::~Forward(){
  if(sk)
    sk32free(sk);
}
void Forward::initForward(void){
  localPort=remotePort=0;
  sk=0;
}
/*=========================================================

=========================================================*/
int Forward::getLocalPort(void){return localPort;}
String Forward::getRemoteHost(void){return remoteHost;}
int Forward::getRemotePort(void){return remotePort;}
Forward &Forward::setLocalPort(int localPort){this->localPort=localPort;return *this;}
Forward &Forward::setRemoteHost(const String &remoteHost){this->remoteHost=remoteHost;return *this;}
Forward &Forward::setRemotePort(int remotePort){this->remotePort=remotePort;return *this;}
/*=========================================================

=========================================================*/
void Forward::main(void){
  int
    addrlen=48;
  char
    *addr=cMemAllocChar(addrlen);


  if(!(sk=sk32server(localPort))){
    cPrintf("ERRO: Forward.main.\n");
    exit(0);
  }
  //sk32settimeout(sk,2);
  sk32listen(sk,100);
  while(1){
    int
      port=addrlen,c;
    SK32
      &src=*sk32create(),
      &dst=*sk32create();

    c=accept(sk->s,addr,&port);
    port=Number(*(unsigned short*)&addr[2]).endian(2).asInt();
    sk32connect(&dst,remoteHost.ptr(),remotePort,0,2);
    if(!dst.s){
      sk32free(&src);
      sk32free(&dst);
    }
    else{
      src.s=c;
    }







  }













}






