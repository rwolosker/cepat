/*=========================================================

=========================================================*/
FieldDate::FieldDate(){
  size=4;
  type=C_DATE;
}
FieldDate::~FieldDate(){
}
/*=========================================================

=========================================================*/
Field &FieldDate::setInt(int value){
  if(stream)
    *(int*)pstream()=value;
  return *this;
}
/*=========================================================

=========================================================*/
Field &FieldDate::setDouble(double value){
  return setInt(value);
}
/*=========================================================

=========================================================*/
Field &FieldDate::setText(cchar *value){
  return setDouble(Date().parse(value));
}
/*=========================================================

=========================================================*/
Field &FieldDate::setBool(char value){
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
Field &FieldDate::setChar(char value){
  return setInt(value);
}
/*=========================================================

=========================================================*/
Field &FieldDate::setNumber(const Number &value){
  return setDouble(PNUMBER(value).asDouble());
}
/*=========================================================

=========================================================*/
Field &FieldDate::setString(const String &value){
  return setText(PSTRING(value).ptr());
}
/*=========================================================

=========================================================*/
Field &FieldDate::setDate(const Date &value){
  return setDouble(PDATE(value).get());
}
/*=========================================================

=========================================================*/
int FieldDate::getInt(void){
  return stream
    ?*(int*)pstream()
    :0;
}
/*=========================================================

=========================================================*/
double FieldDate::getDouble(void){
  return getInt();
}
/*=========================================================

=========================================================*/
char *FieldDate::getText(void){
  String
    &string=*new(cMemAlloc(sizeof(String)))String();

  string=Date(getDouble()).format("dd/MM/yyyy");
  gco.add(&string);
  return string.ptr();
}
/*=========================================================

=========================================================*/
char FieldDate::getBool(void){
  return getInt();
}
/*=========================================================

=========================================================*/
char FieldDate::getChar(void){
 return getInt();
}
/*=========================================================

=========================================================*/
Number FieldDate::getNumber(void){
  return getDouble();
}
/*=========================================================

=========================================================*/
String FieldDate::getString(void){
  return getText();
}
/*=========================================================

=========================================================*/
Date FieldDate::getDate(void){
  return getDouble();
}
/*=========================================================

=========================================================*/
Field &FieldDate::instance(void){
  Field
    *f=new(cMemAlloc(sizeof(FieldDate)))FieldDate();

  return f->setSize(getSize());
}
/*=========================================================

=========================================================*/
String FieldDate::toString(void){
  return getString();
}
