void MyPacketError::initMyPacketError(void){
  code=0;
}
MyPacketError::MyPacketError(){initMyPacketError();}
MyPacketError::~MyPacketError(){}
int MyPacketError::getCode(void){return code;}
String MyPacketError::getState(void){return state;}
String MyPacketError::getMsg(void){return msg;}
char MyPacketError::action(uchar type,void **argv){
  code=*(int*)argv[0];
  state=*(String*)argv[1];
  msg=*(String*)argv[2];
  return 1;
}
void MyPacketError::reset(void){
  state.reset();
  msg.reset();
  initMyPacketError();
}
