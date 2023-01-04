/*=========================================================

=========================================================*/
void Factory::init(void){
  map=0;
  reflection=0;
}
Factory::Factory(){
  init();
}
Factory::~Factory(){
}
Map &Factory::getMap(void){
  return *map;
}
Reflection &Factory::getReflection(void){
  return *reflection;
}
void Factory::configure(Map &map,Reflection &reflection){
  this->map=&map;
  this->reflection=&reflection;
}
/*=========================================================
Toda classe deve ser herdada de Object.
Se houver mais de uma heranca, deve-se declarar Object primeiro!
verificado em 11/01/2021 19:11.
=========================================================*/
Object *Factory::getBean(cchar *name,cchar *clazz){
  Object
    *obj=0;
  
  if(name&&*name)
    obj=getMap().get(name).asObj();
  if(obj)
    return obj;
  if(!(obj=getReflection().instance(clazz)))
    return 0;
  if(name&&*name)
    getMap().setValue(name,obj,1);
  inject(obj);
  return obj;
}
/*=========================================================

=========================================================*/
Object *Factory::getBean(cchar *name,cchar *clazz,void *pobj){
  *(void**)pobj=getBean(name,clazz);
  return (Object*)*(void**)pobj;
}
/*=========================================================

=========================================================*/
Factory &Factory::inject(Object *obj){
  if(obj){
    injectMap(obj);
    injectFactory(obj);
    injectReflection(obj);
    injectConfigure(obj);
  }
  return *this;
}
/*=========================================================

=========================================================*/
void Factory::injectReflection(Object *obj){
  ExtReflection
    *e=dynamic_cast<ExtReflection*>(obj);

  if(e)
    e->setReflection(getReflection());
}
void Factory::injectMap(Object *obj){
  ExtMap
    *e=dynamic_cast<ExtMap*>(obj);
  
  if(e)
    e->setMap(getMap());
}
void Factory::injectFactory(Object *obj){
  ExtFactory
    *e=dynamic_cast<ExtFactory*>(obj);
  
  if(e)
    e->setFactory(*this);
}
void Factory::injectConfigure(Object *obj){
  ExtConfigure
    *e=dynamic_cast<ExtConfigure*>(obj);
  
  if(e)
    e->configure(getMap());
}
/*=========================================================

=========================================================*/
void *Factory::getMethod(cchar *name){
  return getReflection().methodAddress(name);
}
/*=========================================================

=========================================================*/
Factory &Factory::alias(cchar *name,cchar *alias){
  Object
    *obj=getMap().get(name).asObj();

  if(obj)
    getMap().setValue(alias,obj);

  return *this;
}
