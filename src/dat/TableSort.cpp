/*=========================================================

=========================================================*/
TableSort::TableSort(){
  initSort();
  initTableSort();
}
TableSort::~TableSort(){
}
/*=========================================================

=========================================================*/
void   TableSort::setCol(int col){this->col=col;}
int    TableSort::getCol(void){return col;}
Table &TableSort::getTable(void){return *(Table*)arg;}
/*=========================================================

=========================================================*/
void TableSort::initTableSort(void){
  col=-1;
}
/*=========================================================

=========================================================*/
void TableSort::sortConfigure(void){
  if(!obj)
    obj=this;
  if(!methodCompare)
    methodCompare=methodCompareResolve();
}
/*=========================================================

=========================================================*/
void TableSort::searchConfigure(void){
  if(!obj)
    obj=this;
  if(!methodSearch)
    methodSearch=methodSearchResolve();
}
/*=========================================================

=========================================================*/
Object &TableSort::getParam(int x){
  void
    **p=(void**)param;
  int
    start=1;

  return *(Object*)p[start+x];
}
/*=========================================================

=========================================================*/
void *TableSort::methodCompareResolve(void){
  char
    type=getTable().getDef().get(col).getType();

  switch(type){
    case C_TEXT:
      return METHOD(&TableSort::compareText);
    default:
      return METHOD(&TableSort::compareDouble);
  }
}
/*=========================================================

=========================================================*/
void *TableSort::methodSearchResolve(void){
  char
    type=getTable().getDef().get(col).getType();

  switch(type){
    case C_TEXT:
      return METHOD(&TableSort::compareTextSearch);
    default:
      return METHOD(&TableSort::compareDoubleSearch);
  }
}
/*=========================================================

=========================================================*/
TableSort &TableSort::getTableSort(Sort &sort){
  return *(TableSort*)&sort;
}
/*=========================================================

=========================================================*/
int TableSort::recNo(void *reg){
  Table
    &table=getTable();
  int
    recno=PUCHAR(reg)-table.getStream().ptr();

  return recno/table.getRegSize();
}
/*=========================================================

=========================================================*/
Field &TableSort::getField(void *reg,cchar *col){
  return getField(
    reg,
    getTable().getDef().get(col).getCol()
  );
}
/*=========================================================

=========================================================*/
Field &TableSort::getField(void *reg,int col){
  return getTable().go(recNo(reg)).getDef().get(col);
}
/*=========================================================

=========================================================*/
char TableSort::compareText(Sort &sort,void *a,void *b){
  String
    sa=getField(a,col).getString(),
    sb=getField(b,col).getString();

  return sa.compare(sb);
}
/*=========================================================

=========================================================*/
char TableSort::compareDouble(Sort &sort,void *a,void *b){
  double
    da=getField(a,col).getDouble(),
    db=getField(b,col).getDouble();

  return da==db?0:da<db?-1:1;
}
/*=========================================================

=========================================================*/
char TableSort::compareTextSearch(Sort &sort,void *a){
  String
    sq=getParamString(0),
    sa=getField(a,col).getString();

  return sq.compare(sa);
}
/*=========================================================

=========================================================*/
char TableSort::compareDoubleSearch(Sort &sort,void *a){
  double
    dq=getParamNumber(0),
    da=getField(a,col).getDouble();

  return dq==da?0:dq<da?-1:1;
}
/*=========================================================

=========================================================*/
void TableSort::colByName(cchar *name,int *col){
  if(!*col)
    *col=getTable().getDef().get(name).getCol();
}
