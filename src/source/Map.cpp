/*=========================================================

=========================================================*/
Map::Map(){
  init();
}
Map::~Map(){
  reset();
}
Map::Map(const Map &map){
  init();
}
void Map::init(void){
  hash=0;
  hashSize=0;
  protect=0;
}
/*=========================================================

=========================================================*/
int Map::getHashSize(void){
  if(!hashSize)
    configure();
  return hashSize;
}
/*=========================================================

=========================================================*/
char Map::isEmpty(void){
  return !hash;
}
/*=========================================================

=========================================================*/
void **Map::getHash(void){
  if(!hash)
    hash=(void**)cMemAlloc(SIZEOF_VOID*getHashSize());
  return hash;
}
/*=========================================================

=========================================================*/
NODE *Map::getRoot(int pos){
  return (NODE*)getHash()[pos];
}
/*=========================================================

=========================================================*/
void Map::setRoot(NODE *node,int pos){
  getHash()[pos]=node;
}
/*=========================================================

=========================================================*/
int Map::keyGen(cchar *key){
  int
    total=0;
  char
    *p=PCHAR(key);

  while(*p)
    total+='0'+*p++;
  return total%getHashSize();
}
/*=========================================================

=========================================================*/
Map &Map::configure(int hashSize){
  this->hashSize=hashSize;
  return *this;
}
/*=========================================================

=========================================================*/
MapItem *Map::getItem(cchar *name,int key,char create){
  if(!name||!*name){
    MapItem
      *item=(MapItem*)gco.add(MapItem().instance());

    item->setNull(1);
    return item;
  }
  else{
    NODE
      *root=getRoot(FOUND(key)?key:key=keyGen(name)),
      *busca=root;

    while(busca){
      MapItem
        *item=(MapItem*)busca->ptr;

      if(item->getName().equals(name))
        return item;
      busca=busca->next;
    }
    if(!create)
      return 0;
    else{
      NODE
        *novo=(NODE*)cMemAllocChar(sizeof(NODE));
      MapItem
        *item=MapItem().instance();

      novo->ptr=item;
      item->setNode(novo);
      item->setPos(key);
      setRoot(cNodeAdd(novo,root),key);

      return item;
    }
  }
}
/*=========================================================

=========================================================*/
Map &Map::remove(cchar *key){
  MapItem
    &item=get(key);

  if(!item.isNull()){
    int
      pos=item.getPos();
    NODE
      *node=getRoot(pos);

    node=cNodeDel(item.getNode(),node,cMemFree);

    //item.setNode(0);
    item.~MapItem();
    cMemFree(&item);
    setRoot(node,pos);
  }
  return *this;
}
/*=========================================================

=========================================================*/
Map &Map::reset(void){
  for(int i=0;i<getHashSize();i++){
    NODE
      *node=getRoot(i);

    while(node){
      MapItem
        *item=(MapItem*)node->ptr;
      NODE
        *next=node->next;

      item->~MapItem();
      cMemFree(item);
      cMemFree(node);

      node=next;
    }
  }
  cMemFree(hash);
  init();
  return *this;
}
/*=========================================================

=========================================================*/
void Map::forEach(Object *obj,void *method){
  void
    (Object::*ptr)(MapItem &item)=0;

  *(void**)&ptr=method;
  for(int i=0;i<getHashSize();i++){
    NODE
      *node=getRoot(i);

    while(node){
      (obj->*ptr)(*(MapItem*)node->ptr);
      node=node->next;
    }
  }
}
/*=========================================================

=========================================================*/
Split Map::keys(void){
  class DoIt:public Object{
    public:
      Split
        *list;
      void doIt(MapItem &item){
        list->add(item.getName());
      }
  };
  Split
    list;
  DoIt
    doIt;

  doIt.list=&list;
  forEach(&doIt,METHOD(&DoIt::doIt));

  list.sort();
  return list;
}
/*=========================================================

=========================================================*/
MapItem &Map::get(cchar *name){
  MapItem
    *item=getItem(name,-1,0);

  if(item)
    return *item;
  else{
    item=(MapItem*)gco.add(MapItem().instance());
    item->setNull(1);

    return *item;

  }
}
/*=========================================================

=========================================================*/
Map &Map::debug(void){
  Split
    list=keys();

  for(int i=0;i<list.len();i++){
    MapItem
      &item=get(list[i]);

    if(!item.isNull())
      item.debug();
  }
  return *this;
}
/*=========================================================

=========================================================*/
Map &Map::setValue(cchar *key,cchar *value){
  MapItem
    &item=*getItem(key,-1,1);

  if(protect&&item.isProtect())
    goto FIM;

  item.reset(1);
  item.setName(key);
  item.setStrValue(value);
  item.setType(C_PCHAR);

  FIM:
  return *this;
}
/*=========================================================

=========================================================*/
Map &Map::setValue(cchar *key,int value){
  MapItem
    &item=*getItem(key,-1,1);

  if(protect&&item.isProtect())
    goto FIM;

  item.reset(1);
  item.setName(key);
  item.setNumValue(value);
  item.setType(C_INT);

  FIM:
  return *this;
}
/*=========================================================

=========================================================*/
Map &Map::setValue(cchar *key,Object *value,char free,void *pfree){
  MapItem
    &item=*getItem(key,-1,1);

  if(protect&&item.isProtect())
    goto FIM;

  item.reset(1);
  item.setName(key);
  item.setNumValue((ulong)value);
  item.setType(C_OBJ);

  item.setFree(free);
  item.setFfree(pfree);

  FIM:
  return *this;
}
/*=========================================================

=========================================================*/
Map &Map::setValue(cchar *key,void *value,char free,void *pfree){
  MapItem
    &item=*getItem(key,-1,1);

  if(protect&&item.isProtect())
    goto FIM;

  item.reset(1);
  item.setName(key);
  item.setNumValue((ulong)value);
  item.setType(C_PTR);

  item.setFree(free);
  item.setFfree(pfree);

  FIM:
  return *this;
}
/*=========================================================

=========================================================*/
Map &Map::loadBuffer(cchar *buffer){
  File
    f;

  f.temp();
  f.write(buffer);
  return loadIni(f.getName());
}
/*=========================================================

=========================================================*/
Map &Map::loadIni(cchar *file){
  File
    f;

  if(f.isOpen(file,1)){
    String
      linha;

    while(f.readLine(linha))
      if((linha=linha.trim()).len()&&linha[0]!='#'){
        String
          key,
          value;
        int
          x;

        x=linha.inStr('=');
        if(!FOUND(x))
          continue;
        key=linha.mid(0,x);
        value=linha.mid(x+1);
        if(FOUND(x=value.inStr('#')))
          value[x]=0;

        setValue(key.trim(),value.trim());
      }
  }
  return *this;
}
/*=========================================================

=========================================================*/
Map &Map::loadArg(int argc,char **argv){
  int
    erros=0;

  for(int i=0;i<argc;i++){
    String
      arg=String(argv[i]);
    int
      x=arg.inStr('=');

    if(FOUND(x))
      setValue(
        arg.mid(0,x).trim(),
        arg.mid(x+1).trim()
      );
    else
      erros+=1;
  }
  if(argc==erros){
    setValue("arg0",argv[0]);
    for(int i=1;i<argc;i+=2)
      setValue(argv[i+0],argv[i+1]);
  }
  return *this;
}
/*=========================================================

=========================================================*/
Map &Map::operator=(const Map &map){
  return *this;
}
/*=========================================================

=========================================================*/
Map &Map::protectMode(char protect){
  this->protect=protect;
  return *this;
}
/*=========================================================

=========================================================*/
Map &Map::protectAll(void){
  class DoIt:public Object{
    public:
    void doIt(MapItem &item){
      item.setProtect(1);
    }
  };
  DoIt
    doIt;

  forEach(&doIt,METHOD(&DoIt::doIt));
  return *this;
}
/*=========================================================

=========================================================*/
Map *Map::instance(void){
  return new(cMemAlloc(sizeof(Map)))Map();
}



