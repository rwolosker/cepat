/*=========================================================

=========================================================*/
FieldText::FieldText(){
  size=0;
  type=C_TEXT;
}
FieldText::~FieldText(){
}
/*=========================================================

=========================================================*/
Field &FieldText::setInt(int value){
  return setText(String().sprintf("%d",value));
}
/*=========================================================

=========================================================*/
Field &FieldText::setDouble(double value){
  return setText(String().sprintf("%f",value));
}
/*=========================================================

=========================================================*/
Field &FieldText::setText(cchar *value){
  if(stream){
    String
      tmp=value;

    tmp[getSize()-1]=0;
    cStrCopy(tmp,PCHAR(pstream()));
  }
  return *this;
}
/*=========================================================

=========================================================*/
Field &FieldText::setBool(char value){
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
  return setText(value?"true":"false");
}
/*=========================================================

=========================================================*/
Field &FieldText::setChar(char value){
  return setInt(value);
}
/*=========================================================

=========================================================*/
Field &FieldText::setNumber(const Number &value){
  return setDouble(PNUMBER(value).asDouble());
}
/*=========================================================

=========================================================*/
Field &FieldText::setString(const String &value){
  return setText(PSTRING(value).ptr());
}
/*=========================================================

=========================================================*/
Field &FieldText::setDate(const Date &value){
  return setString(PDATE(value).format());
}
/*=========================================================

=========================================================*/
int FieldText::getInt(void){
  return getDouble();
}
/*=========================================================

=========================================================*/
double FieldText::getDouble(void){
  return Number().parse(getText());
}
/*=========================================================

=========================================================*/
char *FieldText::getText(void){
  String
    &string=*String().instance();

  if(stream)
    string=PCCHAR(pstream());
  gco.add(&string);
  return string.ptr();
}
/*=========================================================

=========================================================*/
char FieldText::getBool(void){
  return getInt();
}
/*=========================================================

=========================================================*/
char FieldText::getChar(void){
 return getInt();
}
/*=========================================================

=========================================================*/
Number FieldText::getNumber(void){
  return getDouble();
}
/*=========================================================

=========================================================*/
String FieldText::getString(void){
  return getText();
}
/*=========================================================

=========================================================*/
Date FieldText::getDate(void){
  return getDouble();
}
/*=========================================================

=========================================================*/
Field &FieldText::instance(void){
  Field
    *f=new(cMemAlloc(sizeof(FieldText)))FieldText();

  return f->setSize(getSize());
}
/*=========================================================

=========================================================*/
String FieldText::toString(void){
  return getString();
}
