/*=========================================================

=========================================================*/
WeTag::WeTag(){
}
WeTag::~WeTag(){
}
String WeTag::echo(cchar *value){
  return value;
}
/*=========================================================

=========================================================*/
void WeTag::setStr(cchar *name,cchar *value){
  getMap().setValue(name,value);
}
void WeTag::setInt(cchar *name,int value){
  getMap().setValue(name,value);
}
