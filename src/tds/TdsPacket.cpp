/*=========================================================

=========================================================*/
TdsPacket::TdsPacket(){
  initTdsPacket();
}
TdsPacket::~TdsPacket(){
}
/*=========================================================

=========================================================*/
void TdsPacket::initTdsPacket(void){
  packetSize=0;
}
/*=========================================================

=========================================================*/
TdsPacket &TdsPacket::setPacketSize(int packetSize){
  this->packetSize=packetSize;
  return *this;
}
/*=========================================================

=========================================================*/
void TdsPacket::create(char type,ByteArray &buffer){
  if(!packetSize)
    packetSize=4096;
  int
    pLen=sizeof(TDS_PACKET);
  TDS_PACKET
    &p=*(TDS_PACKET*)cMemAlloc(pLen);
  ByteArray
    b;
  int
    bLen=buffer.len(),
    cada=packetSize-pLen,//8bytes de header
    need=bLen/cada;

  if(!need){
    p.type=type;
    p.last=1;
    p.size=0
      +pLen
      +buffer.len();
    p.size=Number(p.size).endian(2).asInt();

    b.write(&p,sizeof(TDS_PACKET));
    b.write(buffer);
  }
  else{
    buffer.seek(0);
    for(int i=0;i<need;i++){
      ByteArray
        tmp=buffer.readBuffer(cada);

      p.type=type;
      p.last=0;
      p.size=0
        +pLen
        +cada;
      p.size=Number(p.size).endian(2).asInt();
      b.write(&p,sizeof(TDS_PACKET));
      b.write(tmp);
    }
    if(bLen%cada){
      ByteArray
        tmp;
      int
        resto=buffer.len()-buffer.position();

      p.type=type;
      p.last=0;
      p.size=0
        +pLen
        +resto;
      p.size=Number(p.size).endian(2).asInt();
      b.write(&p,sizeof(TDS_PACKET));
      tmp=buffer.readBuffer(resto);
      b.write(tmp);
    }
    b[
      b.len()%packetSize
        ?b.len()-b.len()%packetSize+1
        :(b.len()/packetSize-1)*packetSize+1
    ]=1;
  }
  buffer=b;
  cMemFree(&p);
}
