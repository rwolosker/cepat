/*=========================================================

=========================================================*/
MySend::MySend(){
}
MySend::~MySend(){
}
/*=========================================================

=========================================================*/
char MySend::receive(Socket32 &socket,MyByteArray &buffer,int *rlen){
  char
    ok,
    sequence;
  int
    need;

  ok=1
    &&socket.read(buffer,4)
    &&(need=buffer.readFlInt(3))
    &&((sequence=buffer.readFlInt(1))||1)
    &&socket.read(buffer,need);

  if(rlen)
    *rlen=need;
  if(!ok)
    buffer.reset();
  return ok;
}
/*=========================================================

=========================================================*/
char MySend::send(Socket32 &socket,MyByteArray &buffer){
  return socket.write(buffer);
}
