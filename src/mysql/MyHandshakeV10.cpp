/*=========================================================

=========================================================*/
void MyHandshakeV10::initMyHandshakeV10(void){

}
/*=========================================================

=========================================================*/
MyHandshakeV10::MyHandshakeV10(){
  initMyHandshakeV10();
}
MyHandshakeV10::~MyHandshakeV10(){
}
/*=========================================================

=========================================================*/
ByteArray MyHandshakeV10::getPart1(void){return part1;}
ByteArray MyHandshakeV10::getPart2(void){return part2;}
void MyHandshakeV10::setPart1(ByteArray &part1){this->part1=part1;}
void MyHandshakeV10::setPart2(ByteArray &part2){this->part2=part2;}
/*=========================================================

=========================================================*/
char MyHandshakeV10::action(uchar type,void **argv){
  part1.reset().append(*(ByteArray*)argv[0]);
  part2.reset().append(*(ByteArray*)argv[1]);
  return 1;
}
/*=========================================================

=========================================================*/
void MyHandshakeV10::reset(void){
  initMyHandshakeV10();
}
