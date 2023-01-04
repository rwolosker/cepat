/*=========================================================

=========================================================*/
void Sort::initSort(){
  obj=0;
  arg=0;
  param=methodCompare=methodSearch=methodUnique=0;
}
Sort::Sort(){
  initSort();
}
Sort::~Sort(){
}
/*=========================================================

=========================================================*/
char Sort::compare(Sort &sort,void *a,void *b){
  return 0;
}
/*=========================================================

=========================================================*/
Sort &Sort::setObj(Object *obj){
  *(void**)&this->obj=obj;
  return *this;
}
Sort &Sort::setCompare(void *method){
  this->methodCompare=method;
  return *this;
}
Sort &Sort::setSearch(void *method){
  this->methodSearch=method;
  return *this;
}
Sort &Sort::setParam(void *param){
  this->param=param;
  return *this;
}
Sort &Sort::setUnique(void *method){
  this->methodUnique=method;
  return *this;
}
/*=========================================================

=========================================================*/
Object &Sort::getParam(int x){
  void
    **p=(void**)param;
  int
    start=0;

  cGnuStart(&start);

  return *(Object*)p[start+x];
}
/*=========================================================

=========================================================*/
String &Sort::getParamString(int x){
  Object
    &obj=getParam(x);

  return *(String*)&obj;
}
/*=========================================================

=========================================================*/
Number &Sort::getParamNumber(int x){
  Object
    &obj=getParam(x);

  return *(Number*)&obj;
}
/*=========================================================

=========================================================*/
Object *Sort::getObj(void){
  return obj;
}
/*=========================================================

=========================================================*/
void *Sort::getMethodCompare(void){
  return methodCompare;
}
/*=========================================================

=========================================================*/
void Sort::sort(void *buffer,int regsize,int low,int hig){
  if(!buffer)
    return;
  sortConfigure();
  cQuicksort(buffer,hig+1,regsize,VOID(cSortSortImpl),this);
}
/*=========================================================

=========================================================*/
void Sort::sortImpl(void *buffer,int regsize,int low,int hig){
 int
    i=low,
    j=hig,
    p=(low+hig)/2;
  char
    *pbuffer=PCHAR(buffer),
    *tmp=cMemAllocChar(regsize),
    //*pivot=PCHAR(cMemCpy(&pbuffer[regsize*p],regsize,cMemAlloc(regsize))),
    *pivot=PCHAR(&pbuffer[regsize*p]),
    (Object::*compare)(Sort&,void*,void*)=0;

  *(void**)&compare=methodCompare;

  do{
    while(-1==(obj->*compare)(*(Sort*)this,&pbuffer[regsize*i],pivot)&&i<hig)
      i++;
    while(-1==(obj->*compare)(*(Sort*)this,pivot,&pbuffer[regsize*j])&&j>low)
      j--;
    if(i<=j){
      sortSwap(&pbuffer[regsize*i],&pbuffer[regsize*j],tmp,regsize);
      i++;
      j--;
    }
  }while(i<=j);

  if(low<j)
    sortImpl(buffer,regsize,low,j);
  if(i<hig)
    sortImpl(buffer,regsize,i,hig);

  cMemFree(tmp);
  //cMemFree(pivot);
}
/*=========================================================

=========================================================*/
void Sort::sortSwap(void *a,void *b,void *swap,int regsize){
  cMemCpy(a,regsize,swap);
  cMemCpy(b,regsize,a);
  cMemCpy(swap,regsize,b);
}
/*=========================================================

=========================================================*/
Sort *Sort::instance(void){
  return new(cMemAlloc(sizeof(Sort)))Sort();
}
/*=========================================================

=========================================================*/
void Sort::sortConfigure(void){
  if(!obj)
    obj=this;
  if(!methodCompare)
    methodCompare=METHOD(&Sort::compare);
}
/*=========================================================

=========================================================*/
void Sort::searchConfigure(void){
  if(!obj)
    obj=this;
  if(!methodSearch)
    methodSearch=METHOD(&Sort::compare);
}
/*=========================================================

=========================================================*/
int Sort::searchImpl(void *buffer,int regsize,int low,int hig){
  if(!buffer)
    return -1;
  if(hig<low||low>hig)
    return -1;
  {
    int
      m=(low+hig)/2;
    char
      *pbuffer=PCHAR(buffer),
      (Object::*search)(Sort&,void*)=0,
      c;

    *(void**)&search=methodSearch;

    c=(obj->*search)(*this,&pbuffer[m*regsize]);
    if(c==0){
      do{
        m-=1;
      }
      while(1
        &&m>=0
        &&0==(obj->*search)(*this,&pbuffer[m*regsize])
      );
      return m+1;
    }
    if(c==1)
      return searchImpl(buffer,regsize,m+1,hig);
    if(c==-1)
      return searchImpl(buffer,regsize,low,m-1);
    return -1;
  }
}
/*=========================================================

=========================================================*/
int Sort::search(void *buffer,int regsize,int low,int hig){
  if(!buffer)
    return -1;
  searchConfigure();
  return searchImpl(buffer,regsize,low,hig);
}
/*=========================================================

=========================================================*/
Sort &Sort::setArg(Object *arg){
  this->arg=arg;
  return *this;
}
Object *Sort::getArg(void){
  return arg;
}
char Sort::uniqueCompare(Sort &sort){
  return 0;
}
char Sort::unique(void){
  char
    (Object::*f)(Sort &sort)=0;

  if(!obj)
    obj=this;
  if(!methodUnique)
    methodUnique=METHOD(&Sort::uniqueCompare);
  *(void**)&f=methodUnique;
  return (obj->*f)(*this);
}
