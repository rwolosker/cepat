/*=========================================================

=========================================================*/
MyPacket::MyPacket(){
  initMyPacke();
}
MyPacket::~MyPacket(){
}
void MyPacket::initMyPacke(void){
  sequence=0;
}
/*=========================================================

=========================================================*/
MyPacket &MyPacket::setSequence(int sequence){
  this->sequence=sequence;
  return *this;
}
/*=========================================================

=========================================================*/
void MyPacket::sequencePlus(void){
  sequence+=1;
}
/*=========================================================

=========================================================*/
void MyPacket::create(MyByteArray &buffer){
  ByteArray
    tmp=buffer;

  sequence+=1;
  buffer.reset();
  buffer.writeFlInt(tmp.len(),3);
  buffer.writeFlInt(sequence,1);
  buffer.append(tmp);
}
