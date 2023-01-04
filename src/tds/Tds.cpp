/*=========================================================

=========================================================*/
Tds::Tds(){
  initTds();
}
Tds::~Tds(){
}
/*=========================================================

=========================================================*/
void Tds::initTds(void){
  version=packetSize=0;
}
/*=========================================================

=========================================================*/
char Tds::connect(void){
  TdsLoginPacket
    login;
  TdsPacket
    packet;
  TdsPacketExecute
    exec;
  TdsSend
    send;
  ByteArray
    buffer;
  TdsError
    erro;
  TdsLoginAcKnow
    ack;
  TdsEnvChange
    env;

  if(!openSocket())
    return 0;
  login
    .setDatabase(database)
    .setUser(username)
    .setPass(password)
    .setServer(host);
  buffer=login.create();
  packet.create(TDS_LOGIN_PACKET,buffer);
  if(!send.send(socket,buffer)){
    isopen=0;
    lasterror=1;
    error="packet not send.";
    return 0;
  }
  if(!send.receive(socket,buffer)){
    isopen=0;
    lasterror=1;
    error="packet not received.";
    return 0;
  }
  exec
    .actionAdd(TDS_LOGIN_ACKNOW,&ack)
    .actionAdd(TDS_ERROR_PACKET,&erro)
    .actionAdd(TDS_ENV_CHANGE,&env)
    .exec(buffer);

  if(erro.getMsgNo()){
    isopen=0;
    lasterror=1;
    error=erro.getMsg();
    return 0;
  }

  version=ack.getVersion();
  packetSize=env.getPacketSize();

  isopen=1;
  return 1;
}
/*=========================================================

=========================================================*/
char Tds::execute(const String &sql,TdsQueryDef *def){
  TdsPacket
    packet;
  TdsPacketExecute
    exec;
  TdsQueryPacket
    query;
  TdsSend
    send;
  ByteArray
    buffer;
  TdsError
    erro;
  TdsResultDone
    done;

  if(!isopen){
    lasterror=1;
    error="not connected.";
    return 0;
  }
  buffer=query.setSql(sql).create();
  packet.create(TDS_QUERY,buffer);
  if(!send.send(socket,buffer)){
    isopen=0;
    lasterror=1;
    error="packet not send.";
    return 0;
  }
  if(!send.receive(socket,buffer)){
    isopen=0;
    lasterror=1;
    error="packet not received.";
    return 0;
  }
  exec
    .actionAdd(TDS_ERROR_PACKET,&erro)
    .actionAdd(TDS_RESULT_DONE,&done);
  if(def)
    exec
      .actionAdd(TDS_RESULT,def)
      .actionAdd(TDS_ROW_RESULT,def);
  exec.exec(buffer);

  affectedInsert=done.getAffectedInsert();
  affectedDelete=done.getAffectedDelete();
  affectedUpdate=done.getAffectedUpdate();

  lasterror=0;
  return 1;
}
/*=========================================================

=========================================================*/
char Tds::executeSql(const String &sql){
  return execute(sql,0);
}
/*=========================================================

=========================================================*/
Table Tds::query(const String &sql){
  TdsQueryDef
    def;

  execute(sql,&def);
  return def.getTable();
}




