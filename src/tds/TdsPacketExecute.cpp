/*=========================================================

=========================================================*/
TdsPacketExecute::TdsPacketExecute(){
}
TdsPacketExecute::~TdsPacketExecute(){
}
/*=========================================================

=========================================================*/
TdsPacketExecute &TdsPacketExecute::actionAdd(uchar type,Object *bean){
  action.seek(action.len());
  action.writeInt(type);
  action.writePtr(bean);
  return *this;
}
/*=========================================================

=========================================================*/
void TdsPacketExecute::actionReset(void){
  void
    **item=(void**)action.ptr();
  int
    len=action.len()/4;

  for(int i=0;i<len;i+=2){
    TdsAction
      *action=(TdsAction*)item[i+1];

    action->reset();
  }
}
/*=========================================================

=========================================================*/
TdsPacketExecute &TdsPacketExecute::exec(ByteArray &buffer){
  actionReset();
  buffer.seek(0);
  while(1){
    uchar
      type=buffer.readUchar();

    switch(type){
      case 0x4:
        buffer.seek(buffer.position()+7);
        break;
      case 0xAA:
        pkAA(buffer);
        break;
      case 0xAB:
        pkAB(buffer);
        break;
      case 0xAD:
        pkAD(buffer);
        break;
      case 0xFD:
        pkFD(buffer);
        break;
      case 0xE3:
        pkE3(buffer);
        break;
      case 0xD1:
        pkD1(buffer);
        break;
      case 0x81:
        pk81(buffer);
        break;
      case 0xA9:
        pkA9(buffer);
        break;
      default:
        cPrintf("ERRO: TdsPacketExecute.exec [%X]\n<pre>",type);
        buffer.debug(1);

        exit(0);
    }
    if(buffer.position()==buffer.len())
      break;
  }
  return *this;
}
/*=========================================================

=========================================================*/
void TdsPacketExecute::pkAction(int type,void **arg){
  int
    x=action.indexOf(&type,4);

  if(FOUND(x)){
    void
      **item=(void**)action.ptr();
    TdsAction
      *action=(TdsAction*)item[(x/4)+1];

    action->action(type,arg);
  }
}
/*=========================================================

=========================================================*/
void TdsPacketExecute::pkAD(ByteArray &buffer){
  short
    length=buffer.readShort();
  char
    ack=buffer.readChar();
  int
    version=buffer.readInt();
  uchar
    t1Len=buffer.readUchar();
  String
    t1=buffer.readUtf16(t1Len);
  int
    ver=buffer.readInt();
  void
    **arg=(void**)cMemAlloc(6*SIZEOF_VOID);

  arg[ 0]=&length;
  arg[ 1]=&ack;
  arg[ 2]=&version;
  arg[ 3]=&t1Len;
  arg[ 4]=&t1;
  arg[ 5]=&ver;

  gc.add(arg);
  pkAction(0xAD,arg);
}
/*=========================================================

=========================================================*/
void TdsPacketExecute::pk81(ByteArray &buffer){
  void
    **arg=(void**)cMemAlloc(1*SIZEOF_VOID);

  arg[ 0]=&buffer;
  gc.add(arg);
  pkAction(0x81,arg);
}
/*=========================================================

=========================================================*/
void TdsPacketExecute::pkD1(ByteArray &buffer){
  void
    **arg=(void**)cMemAlloc(1*SIZEOF_VOID);

  arg[ 0]=&buffer;
  gc.add(arg);
  pkAction(0xD1,arg);
}
/*=========================================================

=========================================================*/
void TdsPacketExecute::pkA9(ByteArray &buffer){
 buffer.readShort();
 buffer.readInt();
}
/*=========================================================

=========================================================*/
void TdsPacketExecute::pkE3(ByteArray &buffer){
  short
    length=buffer.readShort();
  uchar
    code=buffer.readUchar(),
    t1Len=0;
  String
    t1;
  uchar
    t2Len=0;
  String
    t2;
  short
    codePage=0,
    flag=0;
  uchar
    charSet=0;
  void
    **arg=(void**)cMemAlloc(9*SIZEOF_VOID);

  if(code==0x7){
    //http://www.freetds.org/tds.html#collate
    codePage=buffer.readShort();
    flag=buffer.readShort();
    charSet=buffer.readUchar();
    buffer.readBuffer(length-(1/*code*/+2+2+1));
  }
  else{
    t1Len=buffer.readUchar();
    t1=buffer.readUtf16(t1Len);
    t2Len=buffer.readUchar();
    t2=buffer.readUtf16(t2Len);
  }

  arg[ 0]=&length;
  arg[ 1]=&code;
  arg[ 2]=&t1Len;
  arg[ 3]=&t1;
  arg[ 4]=&t2Len;
  arg[ 5]=&t2;
  arg[ 6]=&codePage;
  arg[ 7]=&flag;
  arg[ 8]=&charSet;

  gc.add(arg);
  pkAction(0xE3,arg);
}
/*=========================================================

=========================================================*/
void TdsPacketExecute::pkHexDump(ByteArray &buffer,int size,int skip){
  uchar
    *ptr=buffer.ptr();

  ptr+=(buffer.position()+skip);
  cHexdump(ptr,size);
}
/*=========================================================

=========================================================*/
void TdsPacketExecute::pkFD(ByteArray &buffer){
  short
    bit=buffer.readShort(),
    unknown=buffer.readShort();
  int
    rowCount=buffer.readInt();
  void
    **arg=(void**)cMemAlloc(3*SIZEOF_VOID);

  arg[ 0]=&bit;
  arg[ 1]=&unknown;
  arg[ 2]=&rowCount;

  gc.add(arg);
  pkAction(0xFD,arg);
}
/*=========================================================

=========================================================*/
void TdsPacketExecute::pkAA(ByteArray &buffer){
  short
    pkLen=buffer.readShort();
  int
    msgNumber=buffer.readInt();
  char
    state=buffer.readChar(),
    level=buffer.readChar();
  short
    msgLen=buffer.readShort();
  String
    msg=buffer.readUtf16(msgLen);
  uchar
    srvLen=buffer.readUchar();
  String
    server=buffer.readUtf16(srvLen);
  char
    procLen=buffer.readChar();
  String
    procName=buffer.readUtf16(procLen);
  short
    line=buffer.readShort();
  void
    **arg=(void**)cMemAlloc(11*SIZEOF_VOID);

  arg[ 0]=&pkLen;
  arg[ 1]=&msgNumber;
  arg[ 2]=&state;
  arg[ 3]=&level;
  arg[ 4]=&msgLen;
  arg[ 5]=&msg;
  arg[ 6]=&srvLen;
  arg[ 7]=&server;
  arg[ 8]=&procLen;
  arg[ 9]=&procName;
  arg[10]=&line;

  gc.add(arg);
  pkAction(0xAA,arg);
}
/*=========================================================

=========================================================*/
void TdsPacketExecute::pkAB(ByteArray &buffer){
  short
    pkLen=buffer.readShort();
  int
    msgNumber=buffer.readInt();
  char
    state=buffer.readChar(),
    level=buffer.readChar();
  short
    msgLen=buffer.readShort();
  String
    msg=buffer.readUtf16(msgLen);
  uchar
    srvLen=buffer.readUchar();
  String
    server=buffer.readUtf16(srvLen);
  char
    procLen=buffer.readChar();
  String
    procName=buffer.readUtf16(procLen);
  short
    line=buffer.readShort();
  void
    **arg=(void**)cMemAlloc(11*SIZEOF_VOID);

  arg[ 0]=&pkLen;
  arg[ 1]=&msgNumber;
  arg[ 2]=&state;
  arg[ 3]=&level;
  arg[ 4]=&msgLen;
  arg[ 5]=&msg;
  arg[ 6]=&srvLen;
  arg[ 7]=&server;
  arg[ 8]=&procLen;
  arg[ 9]=&procName;
  arg[10]=&line;

  gc.add(arg);

  pkAction(0xAB,arg);
}

