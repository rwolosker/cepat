void MyPacketOk::initMyPacketOk(void){
  rowsAffected=lastInsertId=0;
  moreResult=0;
}
MyPacketOk::MyPacketOk(){initMyPacketOk();}
MyPacketOk::~MyPacketOk(){}
int  MyPacketOk::getRowsAffected(void){return rowsAffected;}
int  MyPacketOk::getLastInsertId(void){return lastInsertId;}
char MyPacketOk::getMoreResult(void){return moreResult;}
char MyPacketOk::action(uchar type,void **argv){
  rowsAffected=*(int*)argv[0];
  lastInsertId=*(int*)argv[1];

  int
    teste=*(int*)argv[2];

  moreResult=teste&MY_SERVER_MORE_RESULTS_EXISTS;
  return 1;
}
void MyPacketOk::reset(void){
  initMyPacketOk();
}
