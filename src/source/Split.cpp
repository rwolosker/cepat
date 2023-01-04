/*=========================================================

=========================================================*/
Split::Split(){
  initSplit();
}
/*=========================================================

=========================================================*/
Split::Split(const Split &value){
  initSplit();
  *this=value;
}
Split::Split(cchar *value,char de){
  initSplit();
  split(value,de);
}
Split::Split(cchar *value,cchar *de){
  initSplit();
  split(value,de);
}
Split::Split(int minSize){
  initSplit();
  this->minSize=minSize;
}
Split::~Split(){
  reset();
}
/*=========================================================

=========================================================*/
void Split::initSplit(void){
  var=0;
  length=0;
  minSize=0;
}
/*=========================================================

=========================================================*/
Split &Split::reset(void){
  if(var)
    cMemFree(var);
  initSplit();
  return *this;
}
/*=========================================================

=========================================================*/
Split &Split::split(cchar *value,char de){
  char
    c[2]={de,0};

  return split(value,c);
}
/*=========================================================

=========================================================*/
Split &Split::split(cchar *value,cchar *de){
  reset();
  if(!value||!*value)
    goto FIM;
  {
    char
      *tmp=cStrCopy(value,0),
      *pt=tmp,
      *r;
    int
      dLen=cStrLen(de);

    while((r=strstr(pt,de))){
      int
        pos=r-pt;

      pt[pos]=0;
      add(pt);
      pt+=pos+dLen;
    }
    if(pt&&*pt)
      add(pt);

    cMemFree(tmp);
  }
  FIM:
  return *this;
}
/*=========================================================

=========================================================*/
int Split::len(void){
  return length;
}
/*=========================================================

=========================================================*/
String Split::last(void){
  return get(len()-1);
}
/*=========================================================

=========================================================*/
Split &Split::debug(char dump){
  cPrintf("len    : [%d]\n",len());
  cPrintf("minSize: [%d]\n",minSize);
  for(int i=0;i<len();i++)
    cPrintf(" %s\n",get(i).ptr());
  cPrintf("\n");
  if(dump)
    cHexdump(var,cMemSize(var));
  return *this;
}
/*=========================================================

=========================================================*/
String Split::get(int x){
  return x>=0&&x<len()
    ?var[x]
    :"";
}
/*=========================================================

=========================================================*/
String Split::operator[](int x){
  return get(x);
}
/*=========================================================

=========================================================*/
Split &Split::operator=(const Split &value){
  if(this==((Split*)&value)->me())
    return *this;
  reset();
  minSize=value.minSize;
  if(!value.length)
    return *this;
  {
    char
      *p=PCHAR(value.var),
      **nvar=(char**)cMemAlloc(minSize),
      *pnvar=PCHAR(nvar);
    int
      lx=value.var[value.length-1]-p;

    cMemCpy(
      VOID(&p[lx]),
      minSize-lx,
      VOID(pnvar+minSize-(minSize-lx))
    );
    for(int i=0;i<value.length;i++)
      nvar[i]=(pnvar+minSize)-((p+minSize)-value.var[i]);

    var=nvar;
    length=value.length;
  }
  return *this;
}
/*=========================================================

=========================================================*/
Split &Split::setMinSize(int minSize){
  this->minSize=minSize;
  return *this;
}
/*=========================================================

=========================================================*/
Split &Split::add(cchar *value){
  if(!value)
    value="";
  if(!var){
    int
      len=cStrLen(value);

    var=(char**)cMemAlloc(minSize=10+len+2);

    {
      char
        *p=PCHAR(var);

      cMemCpy(VOID(value),len,VOID(&p[minSize-len-1]));
      length+=1;
      var[0]=&p[minSize-len-1];
    }
  }
  else{
    char
      *p=PCHAR(var);
    int
      len=cStrLen(value),
      lx=var[length-1]-p,
      pos=lx-len-1;

    if(&p[pos]<PCHAR(var)+SIZEOF_VOID*length+SIZEOF_VOID*2){
      int
        nMinSize=lx+8+2*minSize+len*2+2+4;
      char
        **nvar=(char**)cMemAlloc(nMinSize),
        *np=PCHAR(nvar);

      cMemCpy(VOID(&p[lx]),minSize-lx,VOID(np+nMinSize-(minSize-lx)));
      for(int i=0;i<length;i++)
        nvar[i]=(np+nMinSize)-((p+minSize)-var[i]);

      cMemFree(var);

      minSize=nMinSize;
      p=PCHAR(var=nvar);
      lx=var[length-1]-p,
      pos=lx-len-1;
    }
    cMemCpy(VOID(value),len,VOID(&p[pos]));
    var[length]=&p[pos];
    length+=1;
  }
  return *this;
}
/*=========================================================

=========================================================*/
char **Split::ptr(){
  return var;
}
/*=========================================================

=========================================================*/
Split &Split::sortCustom(Object *obj,void *method){
  Sort
    sort;

  sort
    .setObj(obj)
    .setCompare(method)
    .sort(
       var,
       SIZEOF_VOID,
       0,
       len()-1);

  reorg();
  return *this;
}
/*=========================================================

=========================================================*/
Split &Split::sort(void){
  class DoIt:public Sort{
    public:
      char compare(Sort &sort,void *a,void *b){
        char
          **as=(char**)a,
          **bs=(char**)b;

        return cStrCompare(*as,*bs);
      }
  };
  DoIt
    sort;

  sort
    .setObj(&sort)
    .setCompare(METHOD(&DoIt::compare))
    .sort(
       var,
       SIZEOF_VOID,
       0,
       len()-1);

  reorg();
  return *this;
}
/*=========================================================

=========================================================*/
int Split::searchImpl(void *p){
  class DoIt:public Sort{
    public:
      char compare(Sort &sort,void *a,void *b){
        char
          *as=*(char**)a,
          *bs=getParamString(0).ptr();

        return cStrCompare(bs,as);
      }
  };
  DoIt
    sort;

  return sort
    .setSearch(METHOD(&DoIt::compare))
    .setParam(p)
    .search(var,SIZEOF_VOID,0,len()-1);
}
/*=========================================================

=========================================================*/
int Split::search(...){
  char
    *p=0;

  return searchImpl(&p);
}
/*=========================================================

=========================================================*/
char Split::contains(const String &value){
 return FOUND(search(&value));
}
/*=========================================================

=========================================================*/
void Split::reorg(void){
  int
    clen,
    size=cMemSize(var);
  char
    *tmp=cMemAllocChar(size),
    *p=tmp+size;

  for(int i=0;i<len();i++){
    clen=cStrLen(var[i]);
    cMemCpy(
      var[i],
      clen,
      p-=clen+1
    );
    var[i]=PCHAR(var)+(p-tmp);
  }

  cMemCpy(
    p,
    size-(p-tmp),
    PCHAR(var)+(p-tmp)
  );

  cMemFree(tmp);
}
/*=========================================================

=========================================================*/
Split &Split::unique(void){
  Split
    unico;
  char
    *esse=0;

  for(int i=0;i<len();i++)
    if(cStrCompare(esse,var[i]))
      unico.add(esse=var[i]);

  reset();
  var=unico.var;
  length=unico.length;
  unico.var=0;
  return *this;
}
/*=========================================================

=========================================================*/
String Split::join(const String &de){
  if(!len())
    return String();
  else{
    ByteArray
      buffer;
    char
      **p=ptr();

    buffer.resize(cMemSize(p));
    while(p&&*p)
      buffer
        .writeStr(de)
        .writeStr(*p++);

    return buffer.toString().mid(PSTRING(de).len());
  }
}
/*=========================================================

=========================================================*/
Split *Split::instance(void){
  return new(cMemAlloc(sizeof(Split)))Split();
}
/*=========================================================

=========================================================*/
Split Split::mid(int start,int size){
  if(start>=len())
    return Split();
  else{
    Split
      tmp;

    for(int i=start;i<len();i++)
      tmp.add(get(i));
    if(size){
      Split
        esse;

      for(int i=0;i<size&&i<tmp.len();i++)
        esse.add(tmp[i]);
      tmp=esse;
    }
    return tmp;
  }
}
