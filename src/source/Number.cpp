/*=========================================================

=========================================================*/
Number::Number(){
  init();
}
Number::Number(double value){
  this->value=value;
}
Number::Number(cchar *value){
  parse((value));
}
Number::~Number(){
}
void Number::init(){
  value=0;
}
/*=========================================================

=========================================================*/
Number *Number::instance(void){
  return new(cMemAlloc(sizeof(Number)))Number();
}
/*=========================================================

=========================================================*/
void Number::debug(){
  cPrintf("%f\n",value);
}
/*=========================================================

=========================================================*/
String Number::toString(void){
  return String().sprintf("%f",value);
}
/*=========================================================

=========================================================*/
Number::operator dbl64(void){return get();}
Number::operator char(void){return (char)get();}
Number::operator uchar(void){return (uchar)get();}
Number::operator int(void){return (int)get();}
Number::operator int64(void){return (int64)get();}
Number::operator short(void){return (short)get();}
Number::operator long(void){return (long)get();}
Number::operator double(void){return (double)get();}
Number::operator float(void){return (float)get();}
Number::operator bool(void){return (bool)get();}

char Number::operator==(double value){return this->value==value;}
char Number::operator==(int value){return this->value==value;}
char Number::operator!=(double value){return this->value!=value;}
char Number::operator>(double value){return this->value>value;}
char Number::operator>=(double value){return this->value>=value;}
char Number::operator<(double value){return this->value<value;}
char Number::operator<=(double value){return this->value<=value;}

Number &Number::operator+=(double value){this->value+=value;return *this;}
Number &Number::operator-=(double value){this->value-=value;return *this;}
Number &Number::operator*=(double value){this->value*=value;return *this;}
Number &Number::operator/=(double value){this->value/=value;return *this;}
Number &Number::operator%=(double value){long m=(long)this->value,v=(long)value;this->value=m%v;return *this;}
Number &Number::operator=(double value){return set(value);}
Number &Number::operator++(void){value++;return *this;}
Number &Number::operator--(void){value--;return *this;}

Number Number::operator+(double value){return this->value+value;}
Number Number::operator-(double value){return this->value-value;}
Number Number::operator*(double value){return this->value*value;}
Number Number::operator/(double value){return this->value/value;}
Number Number::operator%(double value){long m=(long)this->value,v=(long)value;return m%v;}
/*=========================================================

=========================================================*/
Number &Number::set(double value){
  this->value=value;
  return *this;
}
double Number::get(void){
  return value;
}
/*=========================================================

=========================================================*/
Number &Number::parse(cchar *value){
  return set(cNumberParse(value));
}
/*=========================================================

=========================================================*/
Number &Number::fromBinary(cchar *value){
  String
    s=value;
  double
    total=0;

  for(int i=s.len(),j=0;i>=0;i--)
    if(s[i]!='0'&&s[i]!='1')
      continue;
    else
      total+=((s.at(i)-'0')*pow(2,j++));
  return set(total);
}
/*=========================================================

=========================================================*/
Number &Number::fromHex(cchar *value){
  String
    s=String(value).uCase();
  ulong
    total=0;

  if(!s.len())
    return set(total);

  for(int i=s.len(),j=0;i>=0;i--)
    switch(s.at(i)){
       case '0':case '1':case '2':case '3':
       case '4':case '5':case '6':case '7':
       case '8':case '9':
        total+=(ulong)((s.at(i)-'0')*pow(16,j++));
        break;
       case 'A':case 'B':
       case 'C':case 'D':case 'E':case 'F':
         total+=(ulong)((s.at(i)-'7')*pow(16,j++));
        break;
       case 'X':
        goto NEXT;
    }
  NEXT:
  return set(total);
}
/*=========================================================

=========================================================*/
String Number::toBinary(int minSize){
  String
    res=String().space(256);
  int
    pos=255;
  ulong
    value=(ulong)this->value;

  res[pos]='0';
  while(value){
    res[pos--]=(value%2)+'0';
    value/=2;
  }
  if(FOUND(minSize))
    while(255-pos<minSize)
      res[pos--]='0';
  return res.trim();
}
/*=========================================================

=========================================================*/
String Number::toHex(void){
  String
    res=String().space(256);
  int
    pos=255;
  ulong
    value=(ulong)this->value;

  res[pos]='0';
  while(value){
    ulong
      d=value%16;

    res[pos--]=d>=0&&d<=9?d+'0':d+'7';
    value/=16;
  }
  return res.trim();
}
/*=========================================================

=========================================================*/
Number &Number::fromPtr(void *ptr){
  return set(!ptr?0:(ulong)ptr);
}
/*=========================================================

=========================================================*/
void *Number::asPtr(void){
  return (void*)(ulong)value;
}
/*=========================================================

=========================================================*/
Number Number::copy(void){
  return value;
}
/*=========================================================

=========================================================*/
Number &Number::endian(int size){
  ulong
    val=(ulong)value,
    src=0,
    dst=0;
  uchar
    *ps=(uchar*)&src,
    *pd=(uchar*)&dst;

  if(size>4)size=4;
  if(size<0)size=0;
  cMemCpy(&val,size,&src);
  for(int i=0;i<size;i++)
    pd[size-i-1]=ps[i];
  value=dst;
  return *this;
}
/*=========================================================
NULO valido retorna ZERO
=========================================================*/
char Number::isNumber(cchar *value){
  char
    *p=PCHAR(value);

  while(p&&*p){
    char
      c=*p++,
      ok=c=='-'||c==46||(c>=48&&c<=57);

    if(!ok)
      return 0;
  }
  return 1;
}
/*=========================================================

=========================================================*/
int Number::asInt(void){
  return this->operator int();
}
double Number::asDouble(void){
  return this->operator double();
}
