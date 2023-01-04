/*=========================================================

=========================================================*/
FieldDouble::FieldDouble(){
  size=8;
  type=C_DOUBLE;
}
FieldDouble::~FieldDouble(){
}
/*=========================================================

=========================================================*/
Field &FieldDouble::setInt(int value){
  return setDouble(value);
}
/*=========================================================

=========================================================*/
Field &FieldDouble::setDouble(double value){
  if(stream)
    *(double*)pstream()=value;
  return *this;
}
/*=========================================================

=========================================================*/
Field &FieldDouble::setText(cchar *value){
  return setDouble(Number().parse(value).asDouble());
}
/*=========================================================

=========================================================*/
Field &FieldDouble::setBool(char value){
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
Field &FieldDouble::setChar(char value){
  return setInt(value);
}
/*=========================================================

=========================================================*/
Field &FieldDouble::setNumber(const Number &value){
  return setDouble(PNUMBER(value).asDouble());
}
/*=========================================================

=========================================================*/
Field &FieldDouble::setString(const String &value){
  return setText(PSTRING(value).ptr());
}
/*=========================================================

=========================================================*/
Field &FieldDouble::setDate(const Date &value){
  return setDouble(PDATE(value).get());
}
/*=========================================================

=========================================================*/
int FieldDouble::getInt(void){
  return getDouble();
}
/*=========================================================

=========================================================*/
double FieldDouble::getDouble(void){
  return stream
    ?*(double*)pstream()
    :0;
}
/*=========================================================

=========================================================*/
char *FieldDouble::getText(void){
  String
    &string=*String().instance();

  string.sprintf("%f",getDouble());
  gco.add(&string);
  return string.ptr();
}
/*=========================================================

=========================================================*/
char FieldDouble::getBool(void){
  return getInt();
}
/*=========================================================

=========================================================*/
char FieldDouble::getChar(void){
 return getInt();
}
/*=========================================================

=========================================================*/
Number FieldDouble::getNumber(void){
  return getDouble();
}
/*=========================================================

=========================================================*/
String FieldDouble::getString(void){
  return getText();
}
/*=========================================================

=========================================================*/
Date FieldDouble::getDate(void){
  return getDouble();
}
/*=========================================================

=========================================================*/
Field &FieldDouble::instance(void){
  Field
    *f=new(cMemAlloc(sizeof(FieldDouble)))FieldDouble();

  return f->setSize(getSize());
}
/*=========================================================

=========================================================*/
String FieldDouble::toString(void){
  return getString();
}
