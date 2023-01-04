ExtConfigure::ExtConfigure(){
}
ExtConfigure::~ExtConfigure(){
}
void ExtConfigure::configure(Map &map){
}
ExtFactory::ExtFactory(){
  initExtFactory();
}
ExtFactory::~ExtFactory(){
}
void ExtFactory::initExtFactory(void){
  factory=0;
}
Factory &ExtFactory::getFactory(){
  return *factory;
}
void ExtFactory::setFactory(Factory &factory){
  this->factory=&factory;
}
ExtMap::ExtMap(){
  initExpMap();
}
ExtMap::~ExtMap(){
}
void ExtMap::initExpMap(void){
  map=0;
}
Map &ExtMap::getMap(){
  return *map;
}
void ExtMap::setMap(Map &map){
  this->map=&map;
}
ExtReflection::ExtReflection(){
  initExtReflection();
}
ExtReflection::~ExtReflection(){
}
void ExtReflection::initExtReflection(void){
  reflection=0;
}
Reflection &ExtReflection::getReflection(){
  return *reflection;
}
void ExtReflection::setReflection(Reflection &reflection){
  this->reflection=&reflection;
}
