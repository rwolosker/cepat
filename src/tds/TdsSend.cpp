/*=========================================================

=========================================================*/
TdsSend::TdsSend(){
}
TdsSend::~TdsSend(){
}
/*=========================================================

=========================================================*/
char TdsSend::send(Socket32 &socket,ByteArray &buffer){
  return socket.write(buffer);
}
/*=========================================================

=========================================================*/
char TdsSend::receive(Socket32 &socket,ByteArray &buffer){
  int
    packetLen=sizeof(TDS_PACKET),
    need;
  TDS_PACKET
    &packet=*(TDS_PACKET*)cMemAlloc(packetLen);
  char
    ok=0,first=1;
  ByteArray
    tmp;

  buffer.reset();
  while(1){
    ok=socket.read(tmp,packetLen);
    if(!ok)
      break;
    cMemCpy(tmp.ptr(),packetLen,&packet);

    need=0
      +Number(packet.size).endian(2).asInt()
      -packetLen;
    ok=socket.read(tmp,need);
    if(!ok)
      break;

    buffer
      .write(&packet,first?packetLen:0)
      .append(tmp,need);
    first=0;

    if(packet.last)
      break;
  }

  if(!ok)
    buffer.reset();
  cMemFree(&packet);
  return ok;
}
