/*=========================================================

=========================================================*/
FieldDatetime::FieldDatetime(){
  size=8;
  type=C_DATETIME;
}
FieldDatetime::~FieldDatetime(){
}
/*=========================================================

=========================================================*/
Field &FieldDatetime::setInt(int value){
  return setDouble(value);
}
/*=========================================================

=========================================================*/
Field &FieldDatetime::setDouble(double value){
  if(stream)
    *(double*)pstream()=value;
  return *this;
}
/*=========================================================

=========================================================*/
Field &FieldDatetime::setText(cchar *value){
  return setDouble(Date().parse(value));
}
/*=========================================================

=========================================================*/
Field &FieldDatetime::setBool(char value){
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
Field &FieldDatetime::setChar(char value){
  return setInt(value);
}
/*=========================================================

=========================================================*/
Field &FieldDatetime::setNumber(const Number &value){
  return setDouble(PNUMBER(value).asDouble());
}
/*=========================================================

=========================================================*/
Field &FieldDatetime::setString(const String &value){
  return setText(PSTRING(value).ptr());
}
/*=========================================================

=========================================================*/
Field &FieldDatetime::setDate(const Date &value){
  return setDouble(PDATE(value).get());
}
/*=========================================================

=========================================================*/
int FieldDatetime::getInt(void){
  return getDouble();
}
/*=========================================================

=========================================================*/
double FieldDatetime::getDouble(void){
  return stream
    ?*(double*)pstream()
    :0;
}
/*=========================================================

=========================================================*/
char *FieldDatetime::getText(void){
  String
    &string=*new(cMemAlloc(sizeof(String)))String();

  string=Date(getDouble()).format();
  gco.add(&string);
  return string.ptr();
}
/*=========================================================

=========================================================*/
char FieldDatetime::getBool(void){
  return getInt();
}
/*=========================================================

=========================================================*/
char FieldDatetime::getChar(void){
 return getInt();
}
/*=========================================================

=========================================================*/
Number FieldDatetime::getNumber(void){
  return getDouble();
}
/*=========================================================

=========================================================*/
String FieldDatetime::getString(void){
  return getText();
}
/*=========================================================

=========================================================*/
Date FieldDatetime::getDate(void){
  return getDouble();
}
/*=========================================================

=========================================================*/
Field &FieldDatetime::instance(void){
  Field
    *f=new(cMemAlloc(sizeof(FieldDatetime)))FieldDatetime();

  return f->setSize(getSize());
}
/*=========================================================

=========================================================*/
String FieldDatetime::toString(void){
  return getString();
}
