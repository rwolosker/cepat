void TdsResultDone::initTdsResultDone(void){
  affectedInsert=affectedDelete=affectedUpdate=0;
}
TdsResultDone::TdsResultDone(){
  initTdsResultDone();
}
TdsResultDone::~TdsResultDone(){
}
void TdsResultDone::reset(void){
  initTdsResultDone();
}
char TdsResultDone::action(uchar type,void **arg){
  int
    unKnown=*(short*)arg[1],
    rowCount=*(short*)arg[2];

  switch(unKnown){
    case TDS_DONE_DELETE:
      affectedDelete+=rowCount;
      break;
    case TDS_DONE_INSERT:
      affectedInsert+=rowCount;
      break;
    case TDS_DONE_UPDATE:
      affectedUpdate+=rowCount;
      break;
  }

  return 1;
}
int TdsResultDone::getAffectedInsert(void){
  return affectedInsert;
}
int TdsResultDone::getAffectedUpdate(void){
  return affectedUpdate;
}
int TdsResultDone::getAffectedDelete(void){
  return affectedDelete;
}
