/*=========================================================

=========================================================*/
TableGrep::TableGrep(){
  initSort();
  initTableSort();
  initTableGrep();
}
TableGrep::~TableGrep(){
}
void TableGrep::initTableGrep(void){
  methodGrep=methodGrepString=0;
}
/*=========================================================

=========================================================*/
void TableGrep::grepConfigure(void){
  if(!obj)
    obj=this;
  if(!methodGrep)
    methodGrep=METHOD(&TableGrep::doGrep);
  if(!methodGrepString)
    methodGrepString=METHOD(&TableGrep::grepString);
}
/*=========================================================

=========================================================*/
void *TableGrep::getMethodGrep(void){return methodGrep;}
void *TableGrep::getMethodGrepString(void){return methodGrepString;}
void  TableGrep::setMethodGrep(void *method){methodGrep=method;}
void  TableGrep::setMethodGrepString(void *method){methodGrepString=method;}
/*=========================================================

=========================================================*/
Object &TableGrep::getParam(int x){
  int
    start=0;
  void
    **pparam=(void**)param;

  cGnuStart(&start);

  return *(Object*)pparam[start+x];
}
/*=========================================================

=========================================================*/
void TableGrep::grep(void){
  void
    (Object::*pgrep)(TableGrep &grep)=0;

  grepConfigure();
  *(void**)&pgrep=methodGrep;
  (getObj()->*pgrep)(*this);
}
/*=========================================================

=========================================================*/
void TableGrep::doGrep(TableGrep &grep){
  void
    (Object::*pgrep)(Table &table,String &out)=0;
  Table
    &table=getTable(),
    tmp=table;
  String
    expression=getParamString(0).normalize(),
    buffer;
  RegExp
    exp;

  table.top();
  if(!expression.len())
    return;
  *(void**)&pgrep=methodGrepString;
  exp.create(expression);
  tmp.truncate();

  table.top();
  while(table.fetch()){
   (getObj()->*pgrep)(table,buffer);
    if(exp.match(buffer.normalize()))
      tmp.addReg(table.me(),0);
  }
  table.truncate();
  table=tmp;
  table.top();
}
/*=========================================================

=========================================================*/
void TableGrep::grepString(Table &table,String &out){
  char
    *p;
  TableDef
    &def=table.getDef();
  int
    len=def.len();

  if(out.size()<4096)
    out.resize(4096);
  p=PCHAR(out.ptr());
  *p=0;
  for(int i=0;i<len;i++){
    Field
      &f=def.get(i);

    p+=sprintf(
      p,
      ".%s.%s",
      f.getName().ptr(),
      f.getText()
    );
  }
  *p++='.';
  *p++=0;
  out.reLen();
}
