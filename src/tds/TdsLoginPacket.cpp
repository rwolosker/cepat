/*=========================================================

=========================================================*/
TdsLoginPacket::TdsLoginPacket(){
}
TdsLoginPacket::~TdsLoginPacket(){
}
/*=========================================================

=========================================================*/
String TdsLoginPacket::getDatabase(void){return database;}
TdsLoginPacket &TdsLoginPacket::setDatabase(const String&database){this->database=database;return *this;}
String TdsLoginPacket::getPass(void){return pass;}
TdsLoginPacket &TdsLoginPacket::setPass(const String&pass){this->pass=pass;return *this;}
String TdsLoginPacket::getServer(void){return server;}
TdsLoginPacket &TdsLoginPacket::setServer(const String&server){this->server=server;return *this;}
String TdsLoginPacket::getUser(void){return user;}
TdsLoginPacket &TdsLoginPacket::setUser(const String&user){this->user=user;return *this;}
/*=========================================================

=========================================================*/
char TdsLoginPacket::bitReverse(char c){
  char
    a=c>>4,
    b=0xF&c;

  return b*(/*FODA-SE*/16)+a;
}
/*=========================================================

=========================================================*/
void TdsLoginPacket::password(ByteArray &pass){
  for(int i=0;i<pass.len();i++)
    pass[i]=bitReverse(pass[i]^0x5A);
}
/*=========================================================

=========================================================*/
ByteArray TdsLoginPacket::create(void){
  TDS_LOGIN_7
    &login=*(TDS_LOGIN_7*)cMemAlloc(sizeof(TDS_LOGIN_7));
  ByteArray
    host,user,pass,app,server,lib,lang,database,out;

  host.setUtf16("localhost");
  user.setUtf16(this->user);
  pass.setUtf16(this->pass);
    password(pass);
  app.setUtf16("app");
  server.setUtf16(this->server);
  lib.setUtf16("core");
  lang.setUtf16("us_english");
  database.setUtf16(this->database);

  login.tdsVersion=Number(TDS_VERSION_71).endian(4).asInt();
  login.packetSize=4096;
  login.flag1=0xE0;
  login.flag2=0x3;
  login.sqlType=0x08;
  login.hostPos=-1;
  login.hostLen=host.len()/2;
  login.userPos=-1;
  login.userLen=user.len()/2;
  login.passPos=-1;
  login.passLen=pass.len()/2;
  login.appPos=-1;
  login.appLen=app.len()/2;
  login.serverPos=-1;
  login.serverLen=server.len()/2;
  login.libPos=-1;
  login.libLen=lib.len()/2;
  login.langPos=-1;
  login.langLen=lang.len()/2;
  login.databasePos=-1;
  login.databaseLen=database.len()/2;

  out.write((void*)&login,sizeof(TDS_LOGIN_7));
  login.hostPos=out.position();
  out.write(host);
  login.userPos=out.position();
  out.write(user);
  login.passPos=out.position();
  out.write(pass);
  login.appPos=out.position();
  out.write(app);
  login.serverPos=out.position();
  out.write(server);
  login.libPos=out.position();
  out.write(lib);
  login.langPos=out.position();
  out.write(lang);
  login.databasePos=out.position();
  out.write(database);
  login.msgSize
    =login.nextPos
    =login.ntAuthPos
    =out.position();

  out.seek(0);
    out.write(&login,sizeof(TDS_LOGIN_7));
  out.seek(0);

  cMemFree(&login);

  return out;
}
