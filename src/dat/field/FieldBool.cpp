/*=========================================================

=========================================================*/
FieldBool::FieldBool(){
  size=1;
  type=C_BOOL;
}
FieldBool::~FieldBool(){
}
/*=========================================================

=========================================================*/
Field &FieldBool::setInt(int value){
  return setChar(value);
}
/*=========================================================

=========================================================*/
Field &FieldBool::setDouble(double value){
  return setInt(value);
}
/*=========================================================

=========================================================*/
Field &FieldBool::setText(cchar *value){
  return setBool(value?*value:0);
}
/*=========================================================

=========================================================*/
Field &FieldBool::setBool(char value){
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
Field &FieldBool::setChar(char value){
  if(stream)
    *(char*)pstream()=value;
  return *this;
}
/*=========================================================

=========================================================*/
Field &FieldBool::setNumber(const Number &value){
  return setInt(PNUMBER(value).asInt());
}
/*=========================================================

=========================================================*/
Field &FieldBool::setString(const String &value){
  return setText(PSTRING(value).ptr());
}
/*=========================================================

=========================================================*/
Field &FieldBool::setDate(const Date &value){
  return setInt(PDATE(value).get());
}
/*=========================================================

=========================================================*/
int FieldBool::getInt(void){
  return getChar();
}
/*=========================================================

=========================================================*/
double FieldBool::getDouble(void){
  return getInt();
}
/*=========================================================

=========================================================*/
char *FieldBool::getText(void){
  String
    &string=*new(cMemAlloc(sizeof(String)))String();

  string=getChar()?"true":"false";
  gco.add(&string);
  return string.ptr();
}
/*=========================================================

=========================================================*/
char FieldBool::getBool(void){
  return getInt();
}
/*=========================================================

=========================================================*/
char FieldBool::getChar(void){
  return stream
    ?*(char*)pstream()
    :0;
}
/*=========================================================

=========================================================*/
Number FieldBool::getNumber(void){
  return getInt();
}
/*=========================================================

=========================================================*/
String FieldBool::getString(void){
  return getText();
}
/*=========================================================

=========================================================*/
Date FieldBool::getDate(void){
  return getInt();
}
/*=========================================================

=========================================================*/
Field &FieldBool::instance(void){
  Field
    *f=new(cMemAlloc(sizeof(FieldBool)))FieldBool();

  return f->setSize(getSize());
}
/*=========================================================

=========================================================*/
String FieldBool::toString(void){
  return getString();
}
