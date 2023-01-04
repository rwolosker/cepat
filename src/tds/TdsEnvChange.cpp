void TdsEnvChange::initTdsEnvChange(void){
  packetSize=0;
}
TdsEnvChange::TdsEnvChange(){
  initTdsEnvChange();
}
TdsEnvChange::~TdsEnvChange(){
}
void TdsEnvChange::reset(void){
  initTdsEnvChange();
}
char TdsEnvChange::action(uchar type,void **arg){
  int
    code=*(char*)arg[1];

  switch(code){
    case TDS_ENV_PACKET_SIZE:
      {
        String
          &value=*(String*)arg[5];

        packetSize=Number(value).asInt();
      }
      break;
  }
  return 1;
}
int TdsEnvChange::getPacketSize(void){
  return packetSize;
}
