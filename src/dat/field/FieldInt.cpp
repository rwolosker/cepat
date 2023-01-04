/*=========================================================

=========================================================*/
FieldInt::FieldInt(){
  size=4;
  type=C_INT;
}
FieldInt::~FieldInt(){
}
/*=========================================================

=========================================================*/
Field &FieldInt::setInt(int value){
  if(stream)
    *(int*)pstream()=value;
  return *this;
}
/*=========================================================

=========================================================*/
Field &FieldInt::setDouble(double value){
  return setInt(value);
}
/*=========================================================

=========================================================*/
Field &FieldInt::setText(cchar *value){
  return setInt(Number().parse(value).asInt());
}
/*=========================================================

=========================================================*/
Field &FieldInt::setBool(char value){
  switch(value){
  case 0:
  case 'f':
  case 'F':
  case 'N':
  case 'n':
    value=0;
    break;
  default:
    value=1;
    break;
  }
  return setInt(value);
}
/*=========================================================

=========================================================*/
Field &FieldInt::setChar(char value){
  return setInt(value);
}
/*=========================================================

=========================================================*/
Field &FieldInt::setNumber(const Number &value){
  return setInt(PNUMBER(value).asInt());
}
/*=========================================================

=========================================================*/
Field &FieldInt::setString(const String &value){
  return setText(PSTRING(value).ptr());
}
/*=========================================================

=========================================================*/
Field &FieldInt::setDate(const Date &value){
  return setInt(PDATE(value).get());
}
/*=========================================================

=========================================================*/
int FieldInt::getInt(void){
  return stream
    ?*(int*)pstream()
    :0;
}
/*=========================================================

=========================================================*/
double FieldInt::getDouble(void){
  return getInt();
}
/*=========================================================

=========================================================*/
char *FieldInt::getText(void){
  String
    &string=*new(cMemAlloc(sizeof(String)))String();

  string.sprintf("%d",getInt());
  gco.add(&string);
  return string.ptr();
}
/*=========================================================

=========================================================*/
char FieldInt::getBool(void){
  return getInt();
}
/*=========================================================

=========================================================*/
char FieldInt::getChar(void){
 return getInt();
}
/*=========================================================

=========================================================*/
Number FieldInt::getNumber(void){
  return getInt();
}
/*=========================================================

=========================================================*/
String FieldInt::getString(void){
  return getText();
}
/*=========================================================

=========================================================*/
Date FieldInt::getDate(void){
  return getInt();
}
/*=========================================================

=========================================================*/
Field &FieldInt::instance(void){
  Field
    *f=new(cMemAlloc(sizeof(FieldInt)))FieldInt();

  return f->setSize(getSize());
}
/*=========================================================

=========================================================*/
String FieldInt::toString(void){
  return getString();
}
