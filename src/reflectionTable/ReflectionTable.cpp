ReflectionTable::ReflectionTable(){
  initReflectionTable();
}
ReflectionTable::~ReflectionTable(){
}
void ReflectionTable::initReflectionTable(void){
  reload=0;
}
ReflectionTable &ReflectionTable::setReload(char reload){this->reload=reload;return *this;}
/*=========================================================

=========================================================*/
void ReflectionTable::fileMap(cchar *file){
}
/*=========================================================

=========================================================*/
void ReflectionTable::main(int argc,char** argv){
  cReflectionDat(table,reload);
}
/*=========================================================

=========================================================*/
void *ReflectionTable::methodAddress(cchar *name){
  REFLECTION_TABLE
    *p=getName(name);

  return !p
    ?0
    :Number().fromHex(p->address).asPtr();
}
/*=========================================================

=========================================================*/
String  ReflectionTable::className(Object *obj){
  if(!obj)
    return String();
  else{
    Number
      addr=Number().fromPtr(*(void**)obj);
    REFLECTION_TABLE
      *p=getName((addr-=8).toHex());

    return !p
      ?0
      :p->address;
  }
}
/*=========================================================

=========================================================*/
Map &ReflectionTable::getMap(void){
  Map
    *map=0;
  REFLECTION_TABLE
    *p;
  int
    len=table.len();

  if(gco.len())
    map=(Map*)gco.get(0);
  else
    gco.add(map=Map().instance());

  TableStruct().stream(table,&p,0);
  for(int i=0;i<len;i++)
    map->setValue(p[i].name,p[i].address);

  return *map;
}
/*=========================================================

=========================================================*/
REFLECTION_TABLE *ReflectionTable::getName(cchar *name){
  TableStruct
    ts;
  REFLECTION_TABLE
    *p;
  int
    x;

  ts.stream(table,&p,0);
  return FOUND(x=ts.find(table,VOID(name),VOID(cReflectionDatFind)))
    ?p+x
    :0;
}
/*=========================================================

=========================================================*/
char cReflectionDatFind(void *arg,void *a){
  REFLECTION_TABLE
    *ra=(REFLECTION_TABLE*)a;

  return strcmp(PCHAR(arg),ra->name);
}
/*=========================================================

=========================================================*/
char cReflectionDat(Table &table,char reload){
  Shell
    exe;
  File
    out;
  Split
    test,
    list;
  String
    linha,
    main,
    name,
    clazz,
    cmd,
    dat;
  ByteArray
    buffer;
  Number
    addr;
  int
    p,c,pc,pe;

  dat=(main=__progname_full).copy().append(".dat");
  if(!reload&&out.access(dat,"r--")){
    out.open(dat,1);
    table.open(out);
    return 1;
  }
  table=table
    .reset()
    .getDef()
    .add("name",C_TEXT,sizeof(REFLECTION_TABLE)/2)
    .add("address",C_TEXT,sizeof(REFLECTION_TABLE)/2)
    .create();

  out.temp();
  exe.exec("echo \"`which nm`|`which c++filt`\"",out);
  out.readAll(linha);
  test=Split(linha,'|');

  if(!test[0].len()||!test[1].len()){
    cPrintf("ERRO: install nm c++filt");
    exit(1);
  }
  cmd=String().sprintf(
    "%s %s | %s",
    test[0].ptr(),
    main.ptr(),
    test[1].ptr()
  );
  out.truncate();
  exe.exec(cmd,out);
  out.readAll(buffer);
  while(buffer.readLine(linha))
    switch(linha.at(9)){
      case 'T':
        addr=Number().fromHex(linha.mid(0,8));
        name=linha.mid(11);
        if(!name.inStr("non-virtual"))
          continue;
        if(FOUND(p=name.inStr('('))){
          pc=name[p+1]==')';
          c=name.inStr("::");
          pe=name.inStr(')');

          if(pe+1!=name.len())
            continue;
          name[p]=0;

          if(FOUND(c)){
            clazz=name.mid(0,c);
            name=name.mid(c+2);

            if(clazz.equals(name)&&!pc)
              continue;
            name=String().append(clazz).append("::").append(name);
          }
        }
        list.add(String().sprintf("%s %s",name.ptr(),addr.toHex().ptr()));
        break;
      case 'V':
        addr=Number().fromHex(linha.mid(0,8));
        if(FOUND(linha.inStr(" typeinfo ")))
          continue;
        name=linha.mid(22);
        if(FOUND(name.inStr("::")))
          continue;

        list.add(String().sprintf("%s %s",addr.toHex().ptr(),name.ptr()));
        list.add(String().sprintf("%s %s",name.ptr(),addr.toHex().ptr()));
        break;
    }

  list.sort().unique();
  for(int i=0;i<list.len();i++){
    String
      linha=list[i];
    int
      x=linha.inStr(" ");
    char
      *p=linha.ptr();

    p[x]=0;
    table
      .addNew()
      .putTxt(0,p)
      .putTxt(1,p+x+1);
  }
  out.open(dat);
  out.truncate();
  table.save(out);


  return 1;
}
