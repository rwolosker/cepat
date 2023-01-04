/*=========================================================

=========================================================*/
Reflection::Reflection(){
  mSize.reset().configure(128);
}
Reflection::~Reflection(){}
/*=========================================================

=========================================================*/
void Reflection::fileMap(cchar *file){
  cReflectionMap(file);
}
/*=========================================================

=========================================================*/
void Reflection::main(int argc,char** argv){
  String
    file=argv[0],
    linha;
  File
    f;
  ByteArray
    b;

  map.reset();

  if(f.isOpen(file.append(".map"),1)){
    f.readAll(b);
    while(b.readLine(linha)){
      int
        x=linha.inStr(" ");
      char
        *p=linha.ptr();

      p[x]=0;
      map.setValue(p,p+x+1);
    }
  }
}
/*=========================================================

=========================================================*/
String Reflection::className(Object *obj){
  if(!obj)
    return String();
  else{
    Number
      addr=Number().fromPtr(*(void**)obj);

    return map.get((addr-=8).toHex()).asStr();
  }
}
/*=========================================================

=========================================================*/
void *Reflection::methodAddress(cchar *name){
  String
    res=map.get(name).asStr();

  if(!res.len())
    return 0;
  return Number()
    .fromHex(res)
    .asPtr();
}
/*=========================================================

=========================================================*/
void *Reflection::vtable(cchar *name){
  return Number()
    .fromHex(map.get(name).asStr())
    .asPtr();
}
/*=========================================================

=========================================================*/
int Reflection::sizeOf(cchar *clazz){
  int
    size=mSize.get(clazz).asInt();

  if(size)
    return size;
  else{
    String
      name=String().append(clazz).append("::").append(clazz);
    void
      *ptr=methodAddress(name);
    int
      need=REFLECTION_SIZEOF_MAX;

    if(!ptr)
      return 0;
    else{
      char
        *buffer=PCHAR(cMemSet(0xFF,need,cMemAlloc(need))),
        *query=PCHAR(cMemSet(0xFF,4,cMemAlloc(4)));
      Object
        *obj=0;
      void
        (Object::*fptr)(void)=0;
      int
        size;

      *(void**)&obj=buffer;
      *(void**)&fptr=ptr;

      (obj->*fptr)();
      size=PCHAR(memmem(buffer,need,query,4))-buffer;
      obj->~Object();

      mSize.setValue(clazz,size);

      cMemFree(query);
      cMemFree(buffer);

      return size;
    }
  }
}
/*=========================================================

=========================================================*/
Object *Reflection::instance(cchar *clazz){
  int
    size=sizeOf(clazz);
  String
    name=String().append(clazz).append("::").append(clazz);
  void
    *buffer=cMemAlloc(size),
    *ptr=methodAddress(name),
    (Object::*novo)(void)=0;
  Object
    *obj=0;

  if(!size)
    return 0;

  *(void**)&obj=buffer;
  *(void**)&novo=ptr;

  (obj->*novo)();

  return obj;
}
/*=========================================================

=========================================================*/
void *Reflection::free(Object *obj){
  if(obj){
    obj->~Object();
    cMemFree(obj);
  }
  return 0;
}
/*=========================================================

=========================================================*/
void Reflection::debugMap(void){
  cPrintf(
    "map  : %d\n"
    "mSize: %d\n",
    map.keys().len(),
    mSize.keys().len()
  );
}
/*=========================================================

=========================================================*/
Map &Reflection::getMap(void){
  return map;
}
