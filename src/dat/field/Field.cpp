/*=========================================================

=========================================================*/
void Field::initField(void){
  size=type=start=col=0;
  stream=0;
}
Field::Field(){
  initField();
}
Field::~Field(){
}
/*=========================================================

=========================================================*/
void *Field::pstream(void){
  return stream
    ?VOID(stream->ptr()+stream->position()+start)
    :0;
}
/*=========================================================

=========================================================*/
Field &Field::reset(void){
  return *this;
}
/*=========================================================

=========================================================*/
Field &Field::setName(cchar *name){
  this->name=name;
  return *this;
}
/*=========================================================

=========================================================*/
String Field::getName(void){
  return name;
}
/*=========================================================

=========================================================*/
char Field::getType(void){
  return type;
}
/*=========================================================

=========================================================*/
Field &Field::setSize(int size){
  this->size=size<=0?0:size;
  return *this;
}
/*=========================================================

=========================================================*/
int Field::getSize(void){
  return size;
}
/*=========================================================

=========================================================*/
Field &Field::debug(void){
  cPrintf(
    "name : [%s]\n"
    "type : [%s]\n"
    "size : [%d]\n"
    "value: [%s]\n"
    "gco  : (%X)->size[%d]\n",
    name.ptr(),
    Type().name(type).ptr(),
    size,
    getString().ptr(),
    gco.me(),
    gco.len()
  );
  return *this;
}
/*=========================================================

=========================================================*/
Field &Field::clone(void){
  Field
    &f=this->instance();

  f.setName(getName());
  f.setSize(getSize());
  f.setStart(getStart());
  f.setCol(getCol());

  return f;
}
/*=========================================================

=========================================================*/
Field &Field::setStart(int start){
  this->start=start;
  return *this;
}
int Field::getStart(void){
  return start;
}
/*=========================================================

=========================================================*/
Field &Field::setCol(int col){
  this->col=col;
  return *this;
}
int Field::getCol(void){
  return col;
}
/*=========================================================

=========================================================*/
Field &Field::setStream(ByteArray &stream){
  this->stream=&stream;
  return *this;
}
/*=========================================================

=========================================================*/
String Field::toDef(void){
  return String().appendf(
    "%s=%s,%d",
    name.ptr(),
    Type().name(type).ptr(),
    size
  );
}
/*=========================================================

=========================================================*/
char *Field::getPtr(void){
  return PCHAR(pstream());
}
