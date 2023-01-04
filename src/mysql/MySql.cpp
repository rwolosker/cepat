/*=========================================================

=========================================================*/
void MySql::initMySql(void){
  ssl=0;
}
MySql::MySql(){
  initMySql();
}
MySql::~MySql(){

}
MySql &MySql::setSSL(SSL32 &ssl){
  this->ssl=&ssl;
  return *this;
}
/*=========================================================

=========================================================*/
char MySql::send(MySend &send,MyByteArray &buffer){
  if(!send.send(socket,buffer)){
    isopen=0;
    lasterror=1;
    error="packet not send.";
    return 0;
  }
  return 1;
}
/*=========================================================

=========================================================*/
char MySql::receive(MySend &send,MyByteArray &buffer){
  char
    ok=0;

  if(!send.receive(socket,buffer)){
    isopen=0;
    lasterror=1;
    error="packet not received.";
    goto FIM;
  }
  ok=1;
  FIM:
  return ok;
}
/*=========================================================

=========================================================*/
char MySql::connect(void){
  if(!ssl){
    cPrintf("ERRO: SSL configure.\n");
    exit(0);
  }
  if(!openSocket())
    return 0;

  MyPacket
    packet;
  MyPacketOk
    pok;
  MyPacketError
    perr;
  MyLoginPacket
    login;
  MySend
    sr;
  MyByteArray
    buffer;
  MyHandshakeV10
    v10;
  MyPacketExecute
    exe;

  if(!receive(sr,buffer))
    return 0;
  exe.actionAdd(MY_HAND_SHAKE_V10,&v10);
  exe.actionAdd(MY_PACKET_ERR,&perr);
  exe.actionAdd(MY_PACKET_OK,&pok);
  exe.exec(buffer);
  if(perr.getCode()){
    isopen=0;
    lasterror=1;
    error=perr.getMsg();
    return 0;
  }
  buffer=login
    .setDatabase(database)
    .setPass(password)
    .setUser(username)
    .setSSL(*ssl)
    .setV10(v10)
    .create();
  packet.create(buffer);
  if(!send(sr,buffer))
    return 0;
  if(!receive(sr,buffer))
    return 0;
  exe.exec(buffer);
  if(perr.getCode()){
    isopen=0;
    lasterror=1;
    error=perr.getMsg();
    return 0;
  }
  isopen=1;
  return 1;
}
/*=========================================================

=========================================================*/
Table MySql::getTable(int x){
  return *(Table*)table.get(x);
}
/*=========================================================

=========================================================*/
void MySql::addTable(Table &table){
  Table
    &nova=*Table().instance();

  nova=table;
  this->table.add(&nova);
}
/*=========================================================

=========================================================*/
char MySql::executeSql(const String &sql){
  if(!isopen)
    return 0;

  MyPacket
    packet;
  MyPacketOk
    pok;
  MyPacketEof
    peof;
  MyPacketError
    perr;
  MyLoginPacket
    login;
  MySend
    sr;
  MyByteArray
    buffer;
  MyPacketExecute
    exe;
  MyPacketQuery
    query;
  MyPacketCols
    pcols;
  char
    ok=0;

  table.reset();
  pcols.setMySql(*this);

  exe.actionAdd(MY_PACKET_ERR,&perr);
  exe.actionAdd(MY_PACKET_OK,&pok);
  exe.actionAdd(MY_PACKET_EOF,&peof);
  buffer=query.query(sql);
  packet
    .setSequence(-1)
    .create(buffer);

  if(!send(sr,buffer))
    return 0;

  pcols.configure(sr,exe,socket);
  socket.getSk().nosleep=1;

  do{
    if(!receive(sr,buffer))
      goto FIM;
    exe.setOnError(&pcols);
    exe.exec(buffer);
    if(perr.getCode()){
      lasterror=1;
      error=perr.getMsg();
      goto FIM;
    }
    affectedInsert+=pok.getRowsAffected();
  }while(pok.getMoreResult()||peof.getMoreResult());
  ok=1;
  lasterror=0;
  FIM:
  socket.getSk().nosleep=0;
  return ok;
}
/*=========================================================

=========================================================*/
Table MySql::query(const String &sql){
  MyPacket
    packet;
  MyByteArray
    buffer,tmp;
  MyPacketQuery
    query;
  int
    lidos,max=4096;
  class Cols:public MyPacketCols{
    public:
      Table getTable(void){return table;}
      void doit(MyByteArray &buffer){
        TableDef
          def;
        int
          pos,size,fes=0;

        buffer.seek(4);
        cols=buffer.readInt(1);
        if(cols==0xFF)
          return;
        for(int i=0;i<cols;i++){
          size=buffer.readFlInt(3);
          buffer.readFlInt(1);
          pos=buffer.position();
          readCol(buffer,def);
          buffer.seek(buffer.position()+(pos+size-buffer.position()));
        }
        table=def.create();
        while(1){
          size=buffer.readFlInt(3);
          buffer.readFlInt(1);
          pos=buffer.position();
          if(buffer.at(buffer.position())==0xFE){
            fes+=1;
            if(fes==2)
              break;
            buffer.seek(buffer.position()+(pos+size-buffer.position()));
            continue;
          }
          table.addNew();
          for(int i=0;i<cols;i++)
            switch(table.getDef().get(i).getType()){
              case C_DATE:
              case C_DATETIME:
                table.putDbl(i,cDateParseFast(buffer.readLeString()));
                break;
              default:
                table.putTxt(i,buffer.readLeString());
            }
        }
      }
  };
  Cols
    cols;

  buffer=query.query(sql);
  packet
    .setSequence(-1)
    .create(buffer);
  if(!isopen||!socket.write(buffer))
    return Table();
  buffer.resize(max+1);
  while(1){
    usleep(10);
    lidos=recv(socket.getSk().s,buffer.ptr(),max,0);
    if(FOUND(lidos))
      tmp.write(buffer.ptr(),lidos);
    if(lidos!=max&&!sk32isready(&socket.getSk(),0,100,"r--"))
      break;
  }
  cols.doit(tmp);
  return cols.getTable();
}
