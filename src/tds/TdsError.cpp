/*=========================================================

=========================================================*/
TdsError::TdsError(){
  initTdsError();
}
TdsError::~TdsError(){

}
/*=========================================================

=========================================================*/
void TdsError::initTdsError(void){
  msgNo=line=0;
}
/*=========================================================

=========================================================*/
void TdsError::reset(void){
  msg.reset();
  initTdsError();
}
/*=========================================================

=========================================================*/
char TdsError::action(uchar type,void **arg){
  msgNo=*(int*)arg[1];
  msg=*(String*)arg[5];
  line=*(short*)arg[10];
  return 1;
}
/*=========================================================

=========================================================*/
int TdsError::getMsgNo(void){return msgNo;}
int TdsError::getLine(void){return line;}
String TdsError::getMsg(void){return msg;}
