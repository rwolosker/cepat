/*=========================================================

=========================================================*/
void MyLoginPacket::initMyLoginPacket(void){

}
MyLoginPacket::MyLoginPacket(){
  initMyLoginPacket();
}
MyLoginPacket::~MyLoginPacket(){
}
/*=========================================================

=========================================================*/
MyLoginPacket &MyLoginPacket::setUser(const String &username){this->username=username;return *this;}
MyLoginPacket &MyLoginPacket::setPass(const String &password){this->password=password;return *this;}
MyLoginPacket &MyLoginPacket::setDatabase(const String &database){this->database=database;return *this;}
MyLoginPacket &MyLoginPacket::setV10(MyHandshakeV10 &v10){this->v10=&v10;return *this;}
MyLoginPacket &MyLoginPacket::setSSL(SSL32 &ssl){this->ssl=&ssl;return *this;}
/*=========================================================

=========================================================*/
ByteArray MyLoginPacket::passwordGen(void){
  ByteArray
    stage1,stage2,update;

  stage1.set(password);
  ssl->sha1(stage1);

  stage2.append(stage1);
  ssl->sha1(stage2);

  update
    .append(v10->getPart1())
    .append(v10->getPart2())
    .append(stage2);
  ssl->sha1(update);

  for(int i=0;i<20;i++)
    update[i]=(update[i]^stage1[i]);

  return update;
}
/*=========================================================

=========================================================*/
MyByteArray MyLoginPacket::create(void){
  MyByteArray
    buffer;

  buffer.writeFlInt(0x0003A68D,4);
  buffer.writeFlInt(0x00FFFFFF,4);
  buffer.writeFlInt(47,1);
  buffer.write(ByteArray().resize(23));
  buffer.writeNuString(username);
  buffer.writeFlInt(20,1);
  buffer.write(passwordGen());
  buffer.writeNuString(database);
  buffer.writeNuString("mysql_native_password");

  return buffer;
}
/*=========================================================

=========================================================*/
void MyLoginPacket::test(cchar *senha,cchar *cpart1,cchar *cpart2,cchar *cres){
  ByteArray
    part1,part2,res,meu;
  String
    tmp;
  Split
    split;
  MyHandshakeV10
    v10;

  split.split(
    tmp.set(cpart1).replace("  "," "),
    " "
  );
  for(int i=0;i<split.len();i++)
    part1[i]=Number().fromHex(split[i]).asInt();
  split.split(
    tmp.set(cpart2).replace("  "," "),
    " "
  );
  for(int i=0;i<split.len();i++)
    part2[i]=Number().fromHex(split[i]).asInt();
  split.split(
    tmp.set(cres).replace("  "," "),
    " "
  );
  for(int i=0;i<split.len();i++)
    res[i]=Number().fromHex(split[i]).asInt();

  v10.setPart1(part1);
  v10.setPart2(part2);
  this->v10=&v10;
  password=senha;
  meu=passwordGen();







  //part1.debug(1);
  //part2.debug(1);
  //res.debug(1);
  meu.debug(1);













}



