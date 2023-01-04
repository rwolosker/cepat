/*=========================================================

=========================================================*/
void StringList::init(void){
  list=0;
  length=0;
}
StringList::StringList(){
  init();
}
StringList::~StringList(){
  reset();
}
/*=========================================================

=========================================================*/
StringList::StringList(const StringList &list){
  init();
  *this=list;
}
/*=========================================================

=========================================================*/
StringList &StringList::add(const String &value){
  String
    &n=*String().instance();

  n=value;
  gco.add(&n);
  list=(void**)cMemRealloc(list,(length+1)*sizeof(void*));
  list[length]=&n;
  length+=1;

  return *this;
}
/*=========================================================

=========================================================*/
StringList &StringList::debug(void){
  for(int i=0;i<len();i++)
    get(i).debug();

  return *this;
}
/*=========================================================

=========================================================*/
StringList &StringList::reset(void){
  gco.reset();
  cMemFree(list);
  init();
  return *this;
}
/*=========================================================

=========================================================*/
int StringList::len(void){
  return length;
}
/*=========================================================

=========================================================*/
String &StringList::get(int x){
  if(x<0)
    x=0;

  return x<len()
    ?*(String*)(list[x])
    :*(String*)gco.add(String().instance());
}
/*=========================================================

=========================================================*/
char StringList::compareSearch(Sort &sort,void *a){
  String
    &sa=*(String*)*(void**)a,
    &sb=sort.getParamString(0);

  return sb.compare(sa);
}
/*=========================================================

=========================================================*/
int StringList::searchImpl(void *p){
  return Sort()
    .setObj(this)
    .setSearch(METHOD(&StringList::compareSearch))
    .setParam(p)
    .search(list,SIZEOF_VOID,0,len()-1);
}
/*=========================================================

=========================================================*/
int StringList::search(...){
  char
    *p=0;

  return searchImpl(&p);
}
/*=========================================================

=========================================================*/
StringList &StringList::sort(void){
  Sort()
    .setObj(this)
    .setCompare(METHOD(&StringList::compare))
    .sort(list,SIZEOF_VOID,0,len()-1);

  return *this;
}
/*=========================================================

=========================================================*/
char StringList::compare(Sort &sort,void *a,void *b){
  String
    &sa=*(String*)*(void**)a,
    &sb=*(String*)*(void**)b;

  return sa.compare(sb);
}
/*=========================================================

=========================================================*/
StringList &StringList::remove(int x){
  if(x<0||x>=len())
    return *this;
  else{
    void
      **update=(void**)cMemAlloc((length-1)*SIZEOF_VOID);

      cMemCpy(list,(x)*SIZEOF_VOID,update);
      cMemCpy(
        PCHAR(list)+(x+1)*SIZEOF_VOID,
        (length-x-1)*SIZEOF_VOID,
        PCHAR(update)+(x)*SIZEOF_VOID
      );

      gco.remove(&get(x));
      cMemFree(list);

      list=update;
      length-=1;

      return *this;
  }
}
/*=========================================================

=========================================================*/
StringList &StringList::operator=(const StringList &list){
  StringList
    &p=*(StringList*)&list;

  if(p.list==this->list)
    return *this;
  reset();
  for(int i=0;i<p.len();i++)
    add(p.get(i));
  return *this;
}
/*=========================================================

=========================================================*/
String &StringList::operator[](int x){
  return get(x);
}
/*=========================================================

=========================================================*/
String StringList::join(cchar *de){
  if(!de)
    de="";
  if(!len())
    return String();
  if(len()==1)
    return get(0);
  {
    String
      res;

    for(int i=0;i<len();i++)
      res.append(de).append(get(i));

    return res.mid(cStrLen(de));
  }
}
/*=========================================================

=========================================================*/
StringList &StringList::unique(void){
  StringList
    list=*this;
  String
    string;

  reset();
  for(int i=0;i<list.len();i++)
    if(!list[i].equals(string))
      add(string=list[i].ptr());
  return *this;
}
