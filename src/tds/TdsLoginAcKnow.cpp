void TdsLoginAcKnow::initTdsLoginAcKnow(void){
  version=0;
}
TdsLoginAcKnow::TdsLoginAcKnow(){
  initTdsLoginAcKnow();
}
TdsLoginAcKnow::~TdsLoginAcKnow(){
}
void TdsLoginAcKnow::reset(void){
  initTdsLoginAcKnow();
}
char TdsLoginAcKnow::action(uchar type,void **arg){
  version=*(int*)arg[2];
  return 1;
}
int TdsLoginAcKnow::getVersion(void){
  return version;
}
