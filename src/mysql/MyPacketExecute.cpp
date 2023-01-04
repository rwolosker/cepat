/*=========================================================

=========================================================*/
MyPacketExecute::MyPacketExecute(){
  initMyPacketExecute();
}
MyPacketExecute::~MyPacketExecute(){
}
void MyPacketExecute::initMyPacketExecute(void){
  onError=0;
}
/*=========================================================

=========================================================*/
MyPacketExecute &MyPacketExecute::setOnError(MyAction *onError){
  this->onError=onError;
  return *this;
}
/*=========================================================

=========================================================*/
MyPacketExecute &MyPacketExecute::actionAdd(uchar type,Object *bean){
  action.seek(action.len());
  action.writeInt(type);
  action.writePtr(bean);
  return *this;
}
/*=========================================================

=========================================================*/
void MyPacketExecute::actionReset(void){
  /*
  void
    **item=(void**)action.ptr();
  int
    len=action.len()/4;

  for(int i=0;i<len;i+=2){
    TdsAction
      *action=(TdsAction*)item[i+1];

    action->reset();
  }
  */
}
/*=========================================================

=========================================================*/
MyPacketExecute &MyPacketExecute::exec(MyByteArray &buffer){
  actionReset();
  buffer.seek(0);

  while(1){
    uchar
      type=buffer.readUchar();

    switch(type){
      case 0x0A:
        pk0A(buffer);
        break;
      case 0x00:
        pk00(buffer);
        break;
      case 0xFE:
        pkFE(buffer);
        break;
      case 0xFF:
        pkFF(buffer);
        break;
      default:
        if(onError&&onError->onError(buffer))
          onError=0;
        else{
          buffer.debug(1);
          cPrintf("ERRO: MyPacketExecute.exec [%X]\n",type);
          exit(0);
        }
    }
    if(buffer.position()==buffer.len())
      break;
  }
  return *this;
}
/*=========================================================

=========================================================*/
void MyPacketExecute::pkAction(int type,void **arg){
  int
    len=action.len()/4;

  for(int i=0;i<len;i+=2)
    if( *(int*)(action.ptr()+((i+0)*4))==type){
      MyAction
        *a=(MyAction*)*(void**)(action.ptr()+((i+1)*4));

      a->action(type,arg);
      break;
    }
}
/*=========================================================

=========================================================*/
void MyPacketExecute::pk00(MyByteArray &buffer){
  int
    affectedRows=buffer.readLeInt(),
    lastInsertId=buffer.readLeInt(),
    flag=buffer.readShort();
  void
    **arg=(void**)cMemAlloc(3*SIZEOF_VOID);

  buffer.readBuffer(buffer.len()-buffer.position());

  arg[ 0]=&affectedRows;
  arg[ 1]=&lastInsertId;
  arg[ 2]=&flag;

  gc.add(arg);
  pkAction(0x00,arg);
}
/*=========================================================

=========================================================*/
void MyPacketExecute::pkFE(MyByteArray &buffer){
  int
    warn=buffer.readShort(),
    flag=buffer.readShort();
  void
    **arg=(void**)cMemAlloc(2*SIZEOF_VOID);

  buffer.readBuffer(buffer.len()-buffer.position());

  arg[ 0]=&warn;
  arg[ 1]=&flag;

  gc.add(arg);
  pkAction(0xFE,arg);
}
/*=========================================================

=========================================================*/
void MyPacketExecute::pkFF(MyByteArray &buffer){
  int
    errCode;
  String
    state,msg;
  void
    **arg=(void**)cMemAlloc(3*SIZEOF_VOID);

  errCode=buffer.readShort();
  buffer.readBuffer(1);
  state=buffer.readFlString(5);
  msg=buffer.readEofString();

  arg[ 0]=&errCode;
  arg[ 1]=&state;
  arg[ 2]=&msg;

  gc.add(arg);
  pkAction(0xFF,arg);
}
/*=========================================================

=========================================================*/
void MyPacketExecute::pk0A(MyByteArray &buffer){
  String
    serverVersion,
    authPluginName;
  ByteArray
    part1,
    part2;
  Number
    flag1,
    statusFlag,
    flag2;
  void
    **arg=(void**)cMemAlloc(2*SIZEOF_VOID);

  serverVersion=buffer.readNuString();
  /*connectionId=*/buffer.readInt(4);
  part1=buffer.readBuffer(8);
  buffer.readBuffer(1);
  flag1=buffer.readShort();
  /*characterSet=*/buffer.readUchar();
  statusFlag=buffer.readShort();
  flag2=buffer.readShort();
  buffer.readBuffer(1);
  buffer.readBuffer(10);
  part2=buffer.readBuffer(12);
  buffer.readBuffer(1);
  authPluginName=buffer.readNuString();

  arg[ 0]=&part1;
  arg[ 1]=&part2;

  gc.add(arg);
  pkAction(0x0A,arg);
}
/*=========================================================

=========================================================*/
void MyPacketExecute::pkHexDump(ByteArray &buffer,int size,int skip){
  uchar
    *ptr=buffer.ptr();

  ptr+=(buffer.position()+skip);
  cHexdump(ptr,size);
}
