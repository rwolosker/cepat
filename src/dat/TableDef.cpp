/*=========================================================

=========================================================*/
TableDef::TableDef(){
}
TableDef::TableDef(const TableDef &def){
  *this=def;
}
TableDef::~TableDef(){
}
/*=========================================================

=========================================================*/
TableDef &TableDef::add(cchar *name,char type,int size){
  create(name,type,size);
  return *this;
}
TableDef &TableDef::add(cchar *name,cchar *type,int size){
  create(name,Type().type(type),size);
  return *this;
}
TableDef &TableDef::add(Field &field){
  return add(
    field.getName(),
    field.getType(),
    field.getSize()
  );
}
/*=========================================================

=========================================================*/
void TableDef::create(cchar *name,char type,int size){
  Field
    *field;

  switch(type){
    case C_INT:
      field=&FieldInt().instance();
      break;
    case C_DOUBLE:
      field=&FieldDouble().instance();
      break;
    case C_TEXT:
      field=&FieldText().instance();
      field->setSize(size);
      if(!field->getSize()){
        cPrintf("ERRO: TableDef.create TEXT size required.\n");
        exit(0);
      }
      break;
    case C_BOOL:
      field=&FieldBool().instance();
      break;
    case C_CHAR:
      field=&FieldChar().instance();
      break;
    case C_DATE:
      field=&FieldDate().instance();
      break;
    case C_DATETIME:
      field=&FieldDatetime().instance();
      break;
    default:
      cPrintf("ERRO: TableDef.create nao sei.\n");
      exit(0);
  }
  field->setName(name);
  fields.add(field);
  map.setValue(name,field);
}
/*=========================================================

=========================================================*/
TableDef &TableDef::debug(void){
  for(int i=0;i<len();i++){
    Field
      *f=(Field*)fields.get(i);

    cPrintf(
      "[%d]\n"
      "  name : %s\n"
      "  type : %s\n"
      "  size : %d\n"
      "  start: %d\n"
      "\n",
      i,
      f->getName().ptr(),
      Type().name(f->getType()).ptr(),
      f->getSize(),
      f->getStart()
    );
  }
  //map.debug();
  return *this;

}
/*=========================================================

=========================================================*/
int TableDef::len(void){
  return fields.len();
}
/*=========================================================

=========================================================*/
TableDef &TableDef::reset(void){
  fields.reset();
  return *this;
}
/*=========================================================

=========================================================*/
Field &TableDef::get(int x){
  if(x<0)
    x=0;
  if(x<len())
    return *(Field*)fields.get(x);
  else
    return *(Field*)gco.add(&FieldInt().instance());
}
Field &TableDef::get(cchar *name){
  MapItem
    &item=map.get(name);

  if(item.isNull())
    return *(Field*)gco.add(&FieldInt().instance());
  else
    return *(Field*)item.asObj();
}
/*=========================================================

=========================================================*/
TableDef &TableDef::remove(int x){
  Field
    &f=get(x);

  map.remove(f.getName());
  fields.remove(&f);
  return *this;
}
TableDef &TableDef::remove(cchar *name){
  Field
    &f=get(name);

  map.remove(f.getName());
  fields.remove(&f);
  return *this;
}
/*=========================================================

=========================================================*/
TableDef &TableDef::operator=(const TableDef &def){
  TableDef
    &p=*(TableDef*)&def;
  int
    start=0;

  reset();
  for(int i=0;i<p.len();i++){
    Field
      &f=p.get(i).clone();

    f.setStart(start);
    f.setCol(i);
    start+=f.getSize();

    map.setValue(f.getName(),&f);
    fields.add(&f);
  }
  return *this;
}
/*=========================================================

=========================================================*/
String TableDef::toDef(void){
  String
    res;

  for(int i=0;i<fields.len();i++)
    res.appendf("%s\n",get(i).toDef().ptr());
  return res;
}
/*=========================================================

=========================================================*/
TableDef &TableDef::fromDef(const String &value){
  ByteArray
    b=PSTRING(value).copy().replace('=',',').ptr();
  String
    linha;
  StringList
    item;
  int
    x;

  reset();
  b.seek(0);
  while(b.readLine(linha)){
    linha=linha.trim();
    if(!linha.len())continue;
    if(linha[0]=='#')continue;
    if(FOUND(x=linha.inStr('#')))
      linha[x]=0;
    item=linha.split(',');
    if((x=item.len())<2)continue;
    add(
      item[0].trim(),
      item[1].trim(),
      x==2?0:Number().parse(item[2]).asInt()
    );
  }
  return *this;
}
/*=========================================================

=========================================================*/
Table TableDef::create(void){
  Table
    table=*this;

  return table;
}
/*=========================================================

=========================================================*/
TableDef TableDef::copy(void){
  return *this;
}
/*=========================================================

=========================================================*/
TableDef &TableDef::rename(cchar *name,cchar *to){
  Field
    &f=get(name);

  if(f.getName().len()){
    map.remove(f.getName());
    f.setName(to);
    map.setValue(to,&f);
  }
  return *this;
}
