MyByteArray::MyByteArray(){}
MyByteArray::~MyByteArray(){}
/*=========================================================

=========================================================*/
void MyByteArray::writeFlInt(int64 value,int size){
  write(&value,size);
}
/*=========================================================

=========================================================*/
int64 MyByteArray::readFlInt(int size){
  int64
    res=0;

  cMemCpy(readBuffer(size).ptr(),size,&res);
  return res;
}
/*=========================================================

=========================================================*/
void MyByteArray::writeLeInt(int64 value){
  if(value<251){
    writeFlInt(value,1);
    return;
  }
  if(value>=251&&value<(int64)pow(2,16)){
    writeChar(0xFC);
    writeFlInt(value,2);
    return;
  }
  if(value>=(int64)pow(2,16)&&value<(int64)pow(2,24)){
    writeChar(0xFD);
    writeFlInt(value,3);
    return;
  }
  if(value>=(int64)pow(2,24)&&value<(int64)pow(2,64)){
    writeChar(0xFE);
    writeFlInt(value,8);
    return;
  }
}
/*=========================================================

=========================================================*/
int64 MyByteArray::readLeInt(void){
  uchar
    c=readChar();

  if(c<0xFB)
    return c;
  else{
    int
      size=0;

    switch(c){
      case 0xFC:size=2;break;
      case 0xFD:size=3;break;
      case 0xFE:size=8;break;
    }
    return readFlInt(size);
  }
}
/*=========================================================

=========================================================*/
void MyByteArray::writeFlString(const String &value,int size){
  ByteArray
    buffer=PSTRING(value).ptr();

  buffer.resize(size);
  append(buffer);
}
/*=========================================================

=========================================================*/
String MyByteArray::readFlString(int size){
  return readBuffer(size).toString();
}
/*=========================================================

=========================================================*/
void MyByteArray::writeNuString(const String &value){
  ByteArray
    buffer=PSTRING(value).ptr();

  buffer.resize(PSTRING(value).len()+1);
  append(buffer);
}
/*=========================================================

=========================================================*/
String MyByteArray::readNuString(void){
  int
    q=0,
    x=indexOf(position(),&q,1);

  return readBuffer(x-position()+1).toString();
}
/*=========================================================

=========================================================*/
void MyByteArray::writeLeString(const String &value){
  ByteArray
    buffer=PSTRING(value).ptr();

  writeLeInt(buffer.len());
  append(buffer);
}
/*=========================================================

=========================================================*/
String MyByteArray::readLeString(void){
  int64
    size=readLeInt();

  return readBuffer(size).toString();
}
/*=========================================================
MYSQL2
len=total lidos do pacote. streamRead;
=========================================================*/
String MyByteArray::readEofString(int len){
  return readFlString((!len?length:len)-pos);
}
