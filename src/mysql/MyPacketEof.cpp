void MyPacketEof::initMyPacketEof(void){
  moreResult=0;
}
MyPacketEof::MyPacketEof(){initMyPacketEof();}
MyPacketEof::~MyPacketEof(){}
char MyPacketEof::getMoreResult(void){return moreResult;}
char MyPacketEof::action(uchar type,void **argv){
  int
    teste=*(int*)argv[1];

  moreResult=teste&MY_SERVER_MORE_RESULTS_EXISTS;
  return 1;
}
void MyPacketEof::reset(void){
  initMyPacketEof();
}
