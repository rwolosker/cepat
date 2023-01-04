/*=========================================================

=========================================================*/
void MapItem::init(char keep){
  name.reset();
  strValue.reset();
  numValue=0;
  type=nulo=free=0;
  ffree=0;
  protect=0;
  if(!keep){
    node=0;
    pos=0;
  }
}
MapItem::MapItem(){
  init();
}
MapItem::~MapItem(){
  reset();
}
String &MapItem::getName(void){return name;}
void    MapItem::setName(const String &name){this->name=name;}
void    MapItem::setStrValue(const String & strValue){this->strValue=strValue;}
int     MapItem::getType(void){return type;}
void    MapItem::setType(char type){this->type=type;}
char    MapItem::isNull(void){return nulo;}
void    MapItem::setNull(char nulo){this->nulo=nulo;}
NODE   *MapItem::getNode(void){return node;}
void    MapItem::setNode(NODE *node){this->node=node;}
void    MapItem::setNumValue(const Number &numValue){this->numValue=numValue;}
int     MapItem::getPos(void){return pos;}
void    MapItem::setPos(int pos){this->pos=pos;}
char    MapItem::getFree(void){return free;}
void    MapItem::setFree(char free){this->free=free;}
void   *MapItem::getFfree(void){return ffree;}
void    MapItem::setFfree(void *ffree){this->ffree=ffree;}
void    MapItem::setProtect(char protect){this->protect=protect;}
char    MapItem::isProtect(void){return protect;}
/*=========================================================

=========================================================*/
void MapItem::reset(char keepNode){
  freePtr();
  init(keepNode);
}
/*=========================================================

=========================================================*/
MapItem *MapItem::instance(void){
  return new(cMemAllocChar(sizeof(MapItem)))MapItem();
}
/*=========================================================

=========================================================*/
void MapItem::freePtr(void){
  void
    *(*pf)(void*)=0;

  if(type==C_PTR&&free){
    *(void**)&pf=ffree?ffree:VOID(cMemFree);
    if(numValue.asPtr())
      pf(numValue.asPtr());
    return;
  }
  if(type==C_OBJ&&free){
    *(void**)&pf=ffree?ffree:VOID(cMemFree);
    if(numValue.asPtr()){
      Object
        *obj=(Object*)numValue.asPtr();

      obj->~Object();
      pf(obj);
    }
    return;
  }
}
/*=========================================================

=========================================================*/
String MapItem::asStr(void){
  switch(type){
    case C_PCHAR:
      return strValue;
    case C_NUMBER:
      return String().sprintf("%f",numValue.get());
    case C_INT:
      return String().sprintf("%d",numValue.asInt());
    case C_DOUBLE:
      return String().sprintf("%f",numValue.asDouble());
    case C_PTR:
    case C_OBJ:
      return String().sprintf("%X",numValue.asPtr());
    default:
      return String();
  }
}
/*=========================================================

=========================================================*/
Number MapItem::asNumber(void){
  switch(type){
    case C_PCHAR:
      return Number(strValue);
    case C_NUMBER:
    case C_DOUBLE:
    case C_INT:
    case C_PTR:
    case C_OBJ:
      return numValue;
    default:
      return 0.;
  }
}
/*=========================================================

=========================================================*/
int MapItem::asInt(void){
  switch(type){
    case C_PCHAR:
      return Number().parse(strValue);
    case C_NUMBER:
    case C_INT:
    case C_PTR:
    case C_OBJ:
      return numValue;
    default:
      return 0;
  }
}
/*=========================================================

=========================================================*/
void *MapItem::asPtr(void){
  switch(type){
    case C_PCHAR:
      return strValue.ptr();
    case C_NUMBER:
      return 0;
    case C_PTR:
    case C_OBJ:
      return numValue.asPtr();
    default:
      return 0;
  }
}
/*=========================================================

=========================================================*/
Object *MapItem::asObj(void){
  switch(type){
    case C_PCHAR:
      return strValue.me();
    case C_NUMBER:
      return numValue.me();
    case C_PTR:
    case C_OBJ:
      return (Object*)numValue.asPtr();
    default:
      return 0;
  }
}
/*=========================================================

=========================================================*/
void MapItem::pObj(void *pobj){
  *(void**)pobj=asObj();
}
/*=========================================================

=========================================================*/
void MapItem::debug(void){
  cPrintf("%s=[(%s)->#%s#]\n",name.ptr(),Type().name(type).ptr(),asStr().len()?asStr().ptr():"");
}
/*=========================================================

=========================================================*/
