/*=========================================================

=========================================================*/
String::String(void){
  init();
}
String::~String(){
  reset();
}
/*=========================================================

=========================================================*/
String::String(const String &value){
  init();
  set(value.str);
}
String::String(String &value){
  init();
  set(value.str);
}
String::String(cchar *value){
  init();
  set(value);
}
String::String(cchar value){
  init();
  set(value);
}
String::String(int value){
  init();
  set(value);
}
String::String(double value){
  init();
  set(value);
}
/*=========================================================

=========================================================*/
void String::init(void){
  needLen=0;
  str=0;
  length=0;
}
/*=========================================================

=========================================================*/
String &String::set(cchar *value){
  if(str!=value){
    reset();
    length=cStrLen(value);
    str=cStrCopy(value,cMemAllocChar(length+1));
  }
  return *this;
}
/*=========================================================

=========================================================*/
String &String::set(cchar value){
  char
    *tmp=cMemAllocChar(2);

  tmp[0]=value;
  reset();
  str=tmp;
  length=1;
  needLen=1;
  return *this;
}
/*=========================================================

=========================================================*/
String &String::set(int value){
  return sprintf("%d",value);
}
/*=========================================================

=========================================================*/
String &String::set(double value){
  return sprintf("%f",value);
}
/*=========================================================

=========================================================*/
String &String::reset(void){
  if(str)
    cMemFree(str);
  init();
  return *this;
}
/*=========================================================

=========================================================*/
String &String::debug(void){
  cPrintf("this=[%X] str=[%X][%s] len=[%d] mem=[%d]\n",this,str,str?str:"",len(),cMemSize(str));
  return *this;
}
/*=========================================================

=========================================================*/
String &String::operator=(const String &value){
  set(value.str);
  return *this;
}
/*=========================================================

=========================================================*/
String &String::operator=(cchar *value){
  set(value);
  return *this;
}
/*=========================================================

=========================================================*/
String String::copy(void){
  return str;
}
/*=========================================================

=========================================================*/
String::operator char*(void){
  return str;
}
/*=========================================================

=========================================================*/
char *String::ptr(void){
  return str;
}
/*=========================================================

=========================================================*/
String &String::ref(void){
  return *this;
}
/*=========================================================

=========================================================*/
String &String::reLen(void){
  needLen=1;
  return *this;
}
/*=========================================================

=========================================================*/
int String::len(void){
  if(needLen){
    length=cStrLen(str);
    needLen=0;
  }
  return length;
}
/*=========================================================

=========================================================*/
int String::size(void){
  return str?cMemSize(str):0;
}
/*=========================================================

=========================================================*/
String String::lCase(void){
  String
    string=copy();
  char
    *str=string.ptr();

  for(int i=0;i<len();i++)
    str[i]=cIconvToLower(str[i]);
  return string;
}
/*=========================================================

=========================================================*/
String String::uCase(void){
  String
    string=copy();
  char
    *str=string.ptr();

  for(int i=0;i<len();i++)
    str[i]=cIconvToUpper(str[i]);
  return string;
}
/*=========================================================

=========================================================*/
String String::normalize(void){
  String
    string=copy();
  char
    *str;
  
  //if(string.isUtf())
  //  string.fromUtf();
  str=string.ptr();
  for(int i=0;i<string.len();i++)
    str[i]=cIconvNormalize(str[i]);
  return string;
}
/*=========================================================

=========================================================*/
String &String::resize(int size){
  if(size<=1)
    return reset();
  if(size>cMemSize(str)){
    str=PCHAR(cMemRealloc(str,size));
    length=cStrLen(str);
  }
  else{
    char
      *tmp=PCHAR(cMemCpy(str,size-1,cMemAlloc(size)));

    reset();
    length=cStrLen(str=tmp);
  }
  return *this;
}
/*=========================================================

=========================================================*/
String &String::squeeze(void){
  if(cMemSize(str)>len()+1){
    char
      *tmp=cStrCopy(str,0);

    set(tmp);
    cMemFree(tmp);
  }
  return *this;
}
/*=========================================================

=========================================================*/
String &String::squeeze(void *value,char free){
  if(value!=str){
    set(PCHAR(value));
    if(free)
      cMemFree(value);
  }
  return *this;
}
/*=========================================================

=========================================================*/
String &String::append(const String &value){
  if(!len()&&!size())
    return *this=value;
  else{
    String
      &v=PSTRING(value);
    int
      lenA=len(),
      lenB=v.len(),
      lenT=lenA+lenB+1;

    if(lenT<=1)
      lenT=2;
    if(lenT>=size())
      resize(lenT+1);
    cMemCpy(
      v.ptr(),
      lenB,
      ptr()+lenA
    );
    needLen=1;
    return *this;
  }
}
/*=========================================================

=========================================================*/
String &String::appendf(cchar *format,...){
  return append(String().squeeze(cVsprintf(&format)));
}
/*=========================================================

=========================================================*/
String &String::truncate(int len){
  return resize(len+1);
}
/*=========================================================

=========================================================*/
String String::toUtf(void){
  String
    string;

  string.resize(len()*2+2);
  {
    uchar
      *ps=PUCHAR(ptr()),
      *pb=PUCHAR(string.ptr());


    while(*ps){
      unsigned char
        c=*ps;

      if(c>=0xA1/*&&c<=0xFF*/){
        *pb++=0XC3;
        *pb++=(c-0x40);
      }
      else
        *pb++=c;

      ps++;
    }
    string.needLen=1;
    return string.squeeze();
  }
}
/*=========================================================
https://www.rapidtables.com/code/text/ascii-table.html
=========================================================*/
String &String::fromUtf(void){
  return fromUtf(copy());
}
String &String::fromUtf(const String &value){
  reset().
  resize(value.length+1);
  {
    uchar
      *ps=PUCHAR(value.str),
      *pd=PUCHAR(ptr());

    while(ps&&*ps){
      if(*ps!=0xC3)
        *pd++=*ps++;
      else{
        if(*(ps+1)>=0x80){
          ps++;
          *pd++=*ps+0x40;
          ps++;
        }
        else
          *pd++=*ps++;


      }
    }
    *pd=0;
    //cHexdump(ptr(),cMemSize(ptr()));
    needLen=1;
    //len();
    squeeze();
    //cHexdump(ptr(),cMemSize(ptr()));
    return *this;
  }
}
/*=========================================================
  String
    tmp=copy(),
    query=value.str;

  if(start<0||start>=tmp.len())
    return -1;

  if(ignoreCase){
    query=query.lCase();
    tmp=tmp.lCase();
  }

  {
    char
      *pt=tmp.ptr(),
      *pq=query.ptr(),
      *p=pt+start,
      *f=strstr(p,pq);

    return f?f-pt:-1;
  }

=========================================================*/
int String::inStr(int start,const String &value,char ignoreCase){
  if(start<0||start>=len())
    return -1;
  if(ignoreCase)
    return lCase().inStr(start,PSTRING(value).lCase(),0);

  {
    char
      *p=ptr(),
      *q=PSTRING(value).ptr(),
      *f=strstr(p+start,q);

    return f?f-p:-1;
  }
}
/*=========================================================

=========================================================*/
int String::inStrRev(const String &value,char ignoreCase,int start){
  String
    tmp=copy(),
    query=value.str;

  if(ignoreCase){
    query=query.lCase();
    tmp=tmp.lCase();
  }
  if(start==-1)
    start=tmp.len();
  if(start<0||start>tmp.len())
    return -1;
  {
    char
      *pt=tmp.ptr(),
      *pq=query.ptr();

    pt[start]=0;
    while(1){
      char
        *f=strrchr(pt,*pq);

      if(!f)
        return -1;
      if(!strncmp(f,pq,query.len()))
        return f-pt;
      *f=0;
    }
    return -1;
  }
}
/*=========================================================

=========================================================*/
int String::inStr(const String &value,char ignoreCase){
  return inStr(0,value,ignoreCase);  
}
/*=========================================================

=========================================================*/
String &String::sprintf(cchar *format,...){
  reset();
  return squeeze(cVsprintf(&format));
}
/*=========================================================

=========================================================*/
String String::lTrim(void){
  String
    string=copy();
  char
    *p=string.ptr();

  while(p&&*p&&(*p==32||*p==9))
    p++;
  cStrCopy(p,string.ptr());
  string.needLen=1;
  return string;
}
/*=========================================================

=========================================================*/
String String::rTrim(void){
  String
    string=copy();
  char
    *p=string.ptr()+string.len();

  if(!string.len())
    return string;
  while(p&&p>=string.ptr()&&(*p==32||*p==9||!*p))
    p--;
  *(p+1)=0;
  string.needLen=1;
  return string;
}
/*=========================================================

=========================================================*/
String String::trim(void){
  char
    *p0=ptr(),
    *p=p0,
    *pn=p+len()-1;
  String
    res;

  if(!len())
    return res;

  while(p&&*p&&(*p==32||*p==9))
    p++;
  while(p&&*p&&pn&&*pn&&pn>p0&&(*pn==32||*pn==9))
    pn--;

  cMemCpy(
    p,
    pn+1-p,
    res.resize((pn-p)+2).ptr()
  );
  res.needLen=1;
  return res;
  //return lTrim().rTrim().squeeze();
}
/*=========================================================

=========================================================*/
String String::mid(int start,int size){
  if(start<0)
    start=0;
  if(start>=len())
    return String();
  {
    String
      res=ptr()+start;
    
    if(size!=-1&&size<res.len()){
      res.ptr()[size]=0;
      res.needLen=1;
      res.squeeze();
    }
    return res;
  }
}
/*=========================================================

=========================================================*/
String String::space(int size){
  if(size<0)
    return String();
  return String().sprintf("%-*.*s",size,size,"");
}
/*=========================================================

=========================================================*/
String String::left(int size){
  if(size<=0)
    return String();
  return mid(0,size);
}
/*=========================================================

=========================================================*/
String String::right(int size){
  if(size<=0)
    return String();
  return mid(len()-size);
}
/*=========================================================

=========================================================*/
String String::rSet(int size){
  return String().sprintf("%*.*s",size,size,ptr());
}
/*=========================================================

=========================================================*/
String String::lSet(int size){
  return String().sprintf("%-*.*s",size,size,ptr());
}
/*=========================================================

=========================================================*/
String String::replace(const String &query,const String &value){
  if(!len())
    return *this;
  else{
    String
      copia=str,
      res;
    char
      *p=copia.ptr(),
      *pn=0,
      *t;
    int
      qlen=query.length,
      pos;

      
    while(1){
      if(!p||!*p||!(t=strstr(p,query.str)))
        break;
      pos=t-p;
      p[pos]=0;
      pn=p+pos+qlen;
      //se nao fosse recursivo, a logica eh a mesma do split.
      
      p=(res=String().append(p).append(value).append(pn)).ptr();
    }

    return res.len()?res:*this;
  }
}
/*=========================================================

=========================================================*/
char String::at(int x){
  if(x<0)
    x=0;
  if(!x&&!str){
    str=cMemAllocChar(1);
    return at(x);
  }
  if(x>=size())
    resize(x+1);
  needLen=1;
  return str[x];
}
/*=========================================================

=========================================================*/
char String::compare(const String &value,char ignoreCase){
  String
    copia=copy(),
    cvalue=value;
  int
    ok;
  
  if(ignoreCase==1){
    copia=copia.lCase();
    cvalue=cvalue.lCase();
  }
  if(ignoreCase==2){
    copia=copia.normalize();
    cvalue=cvalue.normalize();
  }
  if(!copia.str)
    return -1;
  if(!cvalue.str)
    return 1;

  ok=strcmp(copia,cvalue);
  return !ok?0:ok>0?1:-1;
}
/*=========================================================

=========================================================*/
char String::equals(const String &value,char ignoreCase){
  return !compare(value,ignoreCase);
}
/*=========================================================

=========================================================*/
String String::operator+(const String &value){
  return String().append(*this).append(value);
}
String String::operator+=(const String &value){
  return append(value);
}
char String::operator==(const String &value){
  return equals(value);
}
char String::operator!=(const String &value){
  return !equals(value);
}
char String::operator>=(const String &value){
  return compare(value)>=0;
}
char String::operator>(const String &value){
  return compare(value)>0;
}
char String::operator<=(const String &value){
  return compare(value)<=0;
}
char String::operator<(const String &value){
  return compare(value)<0;
}
char String::operator==(cchar *value){
  return operator==(String(value));
}
char String::operator!=(cchar *value){
  return operator!=(String(value));
}
char String::operator>=(cchar *value){
  return operator>=(String(value));
}
char String::operator>(cchar *value){
  return operator>(String(value));
}
char String::operator<=(cchar *value){
  return operator<=(String(value));
}
char String::operator<(cchar *value){
  return operator<(String(value));
}
/*=========================================================

=========================================================*/
String &String::insert(int pos,const String &value){
  if(!pos){
    return set(String().append(value).append(ptr()));
  }
  
  return 0||(pos<0&&(pos=0))||pos<len()
    ?set(String()
      .append(mid(0,pos))
      .append(value)
      .appendf(mid(pos)))
    :append(value);
}
/*=========================================================

=========================================================*/
String &String::insert(int pos,char c,int size){
  String
    tmp;

  if(size<=0)
    size=0;

  memset(
    tmp.resize(size+1).ptr(),
    c,size
  );
  return insert(pos,tmp.reLen());
}
/*=========================================================

=========================================================*/
CharRef String::operator[](int x){
  CharRef
    ref;
  
  if(x<0)
    x=0;
  at(x);
  ref.set(&ptr()[x]);
  return ref;
}
/*=========================================================

=========================================================*/
char String::isUtf(void){
  return FOUND(inStr((char)0xC3));
}
/*=========================================================

=========================================================*/
String *String::instance(void){
  return new(cMemAlloc(sizeof(String)))String();
}
/*=========================================================

=========================================================*/
StringList String::split(const String &query){
  StringList
    list;
  String
    q=query;

  if(len()&&q.len()){
    String
      copia=str;
    char
      *p=copia.ptr(),
      *pn=0,*t;
    int
      pos;

    while(1){
      if(!p||!*p||!(t=strstr(p,q)))
        break;
      pos=t-p;
      p[pos]=0;
      pn=p+pos+q.len();
      list.add(p);
      p+=pos+q.len();
    }
    if(pn)
      list.add(pn);
  }

  if(!list.len())
    list.add(*this);

  return list;
}
/*=========================================================

=========================================================*/
void String::fastTrim(String &out){
  if(out.ptr()==ptr()){
    return;
  }
  if(!len()){
    out.reset();
    return;
  }
  if(out.size()<size())
    out.resize(size());
  *out.ptr()=0;
  for(int i=0;i<len();i++)
    if(ptr()[i]==32||ptr()[i]==9)
      continue;
    else{
      cStrCopy(ptr()+i,out.ptr());
      for(int j=len()-i-1;j>=0;j--){
        if(out.ptr()[j]==32||out.ptr()[j]==9)
          continue;
        else{
          out.ptr()[j+1]=0;
          out.needLen=1;
          break;
        }
      }
      break;
    }
}
