/*=========================================================

=========================================================*/
void Table::initTable(void){
  regSize=bookMark=rows=0;
}
Table::Table(){
  initTable();
}
Table::Table(const TableDef &def){
  *this=def;
}
Table::Table(const Table &table){
  *this=table;
}
Table::~Table(){
}
/*=========================================================

=========================================================*/
String Table::getName(void){
  return name;
}
/*=========================================================

=========================================================*/
Table &Table::setName(cchar *name){
  this->name=name;
  return *this;
}
/*=========================================================

=========================================================*/
Table &Table::setFileName(cchar *fileName){
  this->fileName=fileName;
  return *this;
}
/*=========================================================

=========================================================*/
String Table::resolveName(const String &value){
  String
    tmp=String(value).trim();

  if(!tmp.len())
    return "";
  else{
    StringList
      list=tmp.split('/');

    return list[list.len()-1].split('.')[0];
  }
}
/*=========================================================

=========================================================*/
void Table::descritor(void){
  for(int i=0;i<def.len();i++)
    def.get(i).setStream(stream);
}
/*=========================================================

=========================================================*/
Table &Table::operator=(const TableDef &def){
  reset();
  this->def=def;
  regSize=0
    +this->def.get(this->def.len()-1).getStart()
    +this->def.get(this->def.len()-1).getSize();
  descritor();
  return *this;
}
/*=========================================================

=========================================================*/
Table &Table::operator=(const Table &table){
  reset();
  *this=table.def;
  rows=table.rows;
  stream.append(table.stream);
  if(stream.len())
    stream.seek(0);
  fileName=table.fileName;
  name=table.name;
  go(0);
  return *this;
}
/*=========================================================

=========================================================*/
Table &Table::reset(void){
  stream.reset();
  fileName.reset();
  name.reset();
  def.reset();
  initTable();
  return *this;
}
/*=========================================================

=========================================================*/
Table &Table::truncate(void){
  return removeAll();
}
/*=========================================================

=========================================================*/
Table &Table::removeAll(int keepRecno){
  char
    tudo=0
      ||keepRecno<0
      ||keepRecno>=rows;

  bookMark=0;
  if(tudo){
    stream.reset();
    rows=0;
  }
  else{
    ByteArray
      b=stream.mid(keepRecno*regSize,regSize);

    stream.reset().append(b);
    rows=1;
  }
  return *this;
}
/*=========================================================

=========================================================*/
Table &Table::remove(int recno){
  if(recno<0||recno>=rows)
    goto FIM;
  else{
    int
      ate=recno*regSize,
      apo=ate+regSize;
    ByteArray
      a=stream.mid(0,ate),
      b=stream.mid(apo,-1);

    stream.reset().append(a).append(b);
    rows-=1;
  }
  FIM:
  return *this;
}
/*=========================================================

=========================================================*/
int Table::getRows(void){return rows;}
int Table::len(void){return getRows();}
int Table::getCols(void){return def.len();}
int Table::getRegSize(void){return regSize;}
TableDef &Table::getDef(void){return def;}
ByteArray &Table::getStream(void){return stream;}
/*=========================================================

=========================================================*/
char Table::eof(void){
  if(!rows)
    return 1;
  return bookMark==rows;
}
/*=========================================================

=========================================================*/
char Table::fetch(void){
  if(bookMark+1==rows)
    return 0;
  else{
    go(bookMark+1);
    return 1;
  }
}
/*=========================================================

=========================================================*/
int Table::recNo(void){
  return bookMark;
}
/*=========================================================

=========================================================*/
Table &Table::debug(char dump){
  cPrintf(
    "\n"
    "file   : %s\n"
    "name   : %s\n"
    "regSize: %d\n"
    "rows   : %d\n"
    "cols   : %d\n"
    "mark   : %d\n"
    "eof    : %d\n"
    "s.pos  : %d\n"
    "s.len  : %d\n"
    "\n",
    fileName.ptr(),
    name.ptr(),
    regSize,
    rows,
    def.len(),
    bookMark,
    eof(),
    stream.position(),
    stream.len()
  );
  def.debug();
  if(dump)
    stream.hexDump();
  return *this;
}
/*=========================================================

=========================================================*/
Table &Table::top(void){
  return go(-1);
}
/*=========================================================

=========================================================*/
Table &Table::go(int recno){
  bookMark=recno<-1?-1:recno;
  if(bookMark<rows&&bookMark>=0)
    stream.seek(bookMark*regSize);
  return *this;
}
/*=========================================================

=========================================================*/
Table &Table::addNew(void){
  rows+=1;
  stream.resize(regSize*rows);
  return go(rows-1);
}
/*=========================================================

=========================================================*/
Table &Table::putInt(int x,int value){
  if(!eof())
    def.get(x).setInt(value);
  return *this;
}
int Table::getInt(int x){
  return eof()
    ?0
    :def.get(x).getInt();
}
Table &Table::putInt(cchar *name,int value){
  if(!eof())
    def.get(name).setInt(value);
  return *this;
}
int Table::getInt(cchar *name){
  return eof()
    ?0
    :def.get(name).getInt();
}
/*=========================================================

=========================================================*/
Table &Table::putDbl(int x,double value){
  if(!eof())
    def.get(x).setDouble(value);
  return *this;
}
double Table::getDbl(int x){
  return eof()
    ?0
    :def.get(x).getDouble();
}
Table &Table::putDbl(cchar *name,double value){
  if(!eof())
    def.get(name).setDouble(value);
  return *this;
}
double Table::getDbl(cchar *name){
  return eof()
    ?0
    :def.get(name).getDouble();
}
/*=========================================================

=========================================================*/
Table &Table::putTxt(int x,cchar *value){
  if(!eof())
    def.get(x).setText(value);
  return *this;
}
cchar *Table::getTxt(int x){
  return eof()
    ?0
    :def.get(x).getText();
}
Table &Table::putTxt(cchar *name,cchar *value){
  if(!eof())
    def.get(name).setText(value);
  return *this;
}
cchar *Table::getTxt(cchar *name){
  return eof()
    ?0
    :def.get(name).getText();
}
/*=========================================================

=========================================================*/
Table &Table::putChr(int x,char value){
  if(!eof())
    def.get(x).setChar(value);
  return *this;
}
char Table::getChr(int x){
  return eof()
    ?0
    :def.get(x).getChar();
}
Table &Table::putChr(cchar *name,char value){
  if(!eof())
    def.get(name).setChar(value);
  return *this;
}
char Table::getChr(cchar *name){
  return eof()
    ?0
    :def.get(name).getChar();
}
/*=========================================================

=========================================================*/
Table &Table::putBln(int x,char value){
  if(!eof())
    def.get(x).setBool(value);
  return *this;
}
char Table::getBln(int x){
  return eof()
    ?0
    :def.get(x).getBool();
}
Table &Table::putBln(cchar *name,char value){
  if(!eof())
    def.get(name).setBool(value);
  return *this;
}
char Table::getBln(cchar *name){
  return eof()
    ?0
    :def.get(name).getBool();
}
/*=========================================================

=========================================================*/
Table &Table::putDte(int x,const Date &value){
  if(!eof())
    def.get(x).setDate(value);
  return *this;
}
Date Table::getDte(int x){
  return eof()
    ?Date(0)
    :def.get(x).getDate();
}
Table &Table::putDte(cchar *name,const Date &value){
  if(!eof())
    def.get(name).setDate(value);
  return *this;
}
Date Table::getDte(cchar *name){
  return eof()
    ?Date(0)
    :def.get(name).getDate();
}
/*=========================================================

=========================================================*/
Table &Table::putStr(int x,const String &value){
  if(!eof())
    def.get(x).setString(value);
  return *this;
}
String Table::getStr(int x){
  return eof()
    ?String(0)
    :def.get(x).getString();
}
Table &Table::putStr(cchar *name,const String &value){
  if(!eof())
    def.get(name).setString(value);
  return *this;
}
String Table::getStr(cchar *name){
  return eof()
    ?String()
    :def.get(name).getString();
}
/*=========================================================

=========================================================*/
Table &Table::putNum(int x,const Number &value){
  if(!eof())
    def.get(x).setNumber(value);
  return *this;
}
Number Table::getNum(int x){
  return eof()
    ?Number()
    :def.get(x).getNumber();
}
Table &Table::putNum(cchar *name,const Number &value){
  if(!eof())
    def.get(name).setNumber(value);
  return *this;
}
Number Table::getNum(cchar *name){
  return eof()
    ?Number()
    :def.get(name).getNumber();
}
/*=========================================================

=========================================================*/
Table &Table::sort(int col){
  TableSort
    sort;

  if(col<0||col>=def.len())
    goto FIM;
  sort.setArg(this);
  sort.setCol(col);
  sort.sort(
    stream.ptr(),
    regSize,
    0,rows-1
  );
  FIM:
  go(0);
  return *this;
}
/*=========================================================

=========================================================*/
Table &Table::sort(Object *obj,void *method){
  TableSort
    sort;

  sort
    .setArg(this)
    .setCompare(method)
    .setObj(obj);

  sort.sort(
    stream.ptr(),
    regSize,
    0,rows-1
  );

  go(0);
  return *this;
}
/*=========================================================

=========================================================*/
Table &Table::sort(cchar *name){
  return sort(def.get(name).getCol());
}
/*=========================================================

=========================================================*/
int Table::findEx(Object( &obj),void *method,...){
  TableSort
    sort;
  int
    x;

  sort.setArg(this);
  sort.setParam(&method);
  sort.setObj(&obj);
  sort.setSearch(method);
  x=sort.search(
    stream.ptr(),
    regSize,
    0,rows-1
  );
  return x;
}
/*=========================================================

=========================================================*/
int Table::find(int col,...){
  TableSort
    sort;
  int
    x=-1;

  if(col<0||col>=def.len())
    goto FIM;
  sort.setArg(this);
  sort.setCol(col);
  sort.setParam(&col);
  x=sort.search(
    stream.ptr(),
    regSize,
    0,rows-1
  );
  FIM:
  return x;
}
/*=========================================================

=========================================================*/
int Table::find(cchar *name,...){
  int
    (Table::*pfind)(int,...)=0,
    (Object::*pofind)(C_CALL)=0;
  C_CALL
    call;

  pfind=&Table::find;
  *(void**)&pofind=METHOD(pfind);
  cMemCpy(&name,64,&call);
  *(int*)&call=def.get(name).getCol();

  return (this->*pofind)(call);
}
/*=========================================================
colunas int8
regSize int16
rows    int32

coluna:
nome len int8
nome     char[] sem \0
tipo     int8
tamanho  int16

=========================================================*/
Table  &Table::toStream(ByteArray &buffer){
  buffer.reset();
  buffer
    .writeChar(def.len())
    .writeShort(regSize)
    .writeInt(rows);

  for(int i=0;i<def.len();i++)
    buffer
      .writeChar(def.get(i).getName().len())
      .writeStr(def.get(i).getName())
      .writeChar(def.get(i).getType())
      .writeShort(def.get(i).getSize());
  buffer.write(stream);
  return *this;
}
/*=========================================================

=========================================================*/
String Table::getFileName(void){
   return fileName;
}
/*=========================================================

=========================================================*/
Table &Table::save(int *erro){
  File
    f;

  f.open(fileName);
  return save(f,erro);
}
/*=========================================================

=========================================================*/
Table &Table::save(File &f,int *erro){
  int
    perro=1;
  ByteArray
    b;


  if(!f.isOpen())
    goto FIM;
  if(f.isReadOnly())
    goto FIM;
  if(!def.len())
    goto FIM;

  toStream(b);
  f.seek(0);
  f.write(b);
  f.truncate(b.len());
  f.sync();
  perro=0;

  FIM:
  if(erro)
    *erro=perro;
  return *this;
}
/*=========================================================

=========================================================*/
Table &Table::open(File &f,int *erro){
  int
    perro=1;
  ByteArray
    buffer;

  reset();
  if(!f.isOpen())
    goto FIM;
  f.readAll(buffer);
  fromStream(buffer,&perro);
  if(perro)
    goto FIM;

  this->fileName=f.getName();
  this->name=resolveName(f.getName());
  perro=0;
  FIM:
  if(erro)
    *erro=perro;
  return *this;
}
/*=========================================================

=========================================================*/
Table &Table::addRegImpl(void *p){
  void
    **pp=(void**)p;
  int
    istart=0,
    next=0;

  cGnuStart(&istart);

  addNew();
  for(int i=istart;pp[i];i++){
    Table
      &t=*(Table*)pp[i];
    ByteArray
      &b=t.getStream();

    cMemCpy(
      b.ptr()+b.position(),
      t.getRegSize(),
      getStream().ptr()+getStream().position()+next
    );
    next+=t.regSize;
  }
  return *this;
}
/*=========================================================

=========================================================*/
Table &Table::updateRegImpl(void *p){
  void
    **pp=(void**)p;
  int
    istart=0,
    next=0;

  cGnuStart(&istart);

  for(int i=istart;pp[i];i++){
    Table
      &t=*(Table*)pp[i];
    ByteArray
      &b=t.getStream();

    cMemCpy(
      b.ptr()+b.position(),
      t.getRegSize(),
      getStream().ptr()+getStream().position()+next
    );
    next+=t.regSize;
  }
  return *this;
}
/*=========================================================
...=table,table,0
=========================================================*/
Table &Table::addReg(...){
  char
    *p=0;

  return addRegImpl(&p);
}
/*=========================================================
...=table,table,0
=========================================================*/
Table &Table::updateReg(...){
  char
    *p=0;

  return updateRegImpl(&p);
}
/*=========================================================

=========================================================*/
Table &Table::last(void){
  if(len())
    go(len()-1);
  else
    go(0);
  return *this;
}
/*=========================================================

=========================================================*/
Table &Table::grep(...){
  char
    *p=0;

  return grepImpl(&p);
}
/*=========================================================

=========================================================*/
Table &Table::grepImpl(void *p){
  TableGrep
    grep;

  grep.setArg(this);
  grep.setParam(p);
  grep.grep();

  return *this;
}
/*=========================================================

=========================================================*/
Table &Table::colByNameImpl(void *p){
  int
    start=0;
  void
    **pp=(void**)p;

  cGnuStart(&start);

  for(int i=start;pp[i];i+=2){
    int
      *col=(int*)(pp[i+1]);

    if(!*col)
      *col=getDef().get(PCHAR(pp[i])).getCol();
  }
  return *this;
}
/*=========================================================

=========================================================*/
Table &Table::colByName(...){
  char
    *p=0;

  return colByNameImpl(&p);
}
/*=========================================================

=========================================================*/
Table Table::copy(void){
  return *this;
}
/*=========================================================

=========================================================*/
char Table::unique(Table &table,Object *pkey,...){
  TableUnique
    unique;
  Table
    copy=*this;
  ByteArray
    buffer;
  Sort
    sort;
  C_CALL
    call;
  int
    start=0;

  cGnuStart(&start,0);
  buffer.writePtr(&copy);
  buffer.writePtr(&table);
  buffer.seek(0);
  cMemReset(&call,sizeof(C_CALL));
  cMemCpy(
    &pkey,
    40,
    VOID(((char*)&call)+start)
  );

  sort.setObj(&unique);
  sort.setUnique(METHOD(&TableUnique::unique));

  sort.setArg(&buffer);
  sort.setParam(&call);
  return sort.unique();
}
/*=========================================================

=========================================================*/
Table &Table::fromStream(ByteArray &buffer,int *erro){
  int
    perro=1;
  int
    colunas,regsize,rows;
  TableDef
    def;

  reset();
  if(!buffer.len())
    goto FIM;
  if(buffer.len()<(1+2+4))
    goto FIM;

  buffer.seek(0);
  colunas=buffer.readUchar();
  regsize=buffer.readShort();
  rows=buffer.readInt();

  for(int i=0;i<colunas;i++){
    int
      len,tipo,tamanho;
    String
      nome;

    len=buffer.readUchar();
    nome=buffer.readStr(len);
    tipo=buffer.readUchar();
    tamanho=buffer.readShort();

    def.add(nome,tipo,tamanho);
  }
  *this=def;
  if(rows)
    stream=buffer.readBuffer(rows*regsize);
  if(stream.len()!=rows*regsize){
    reset();
    goto FIM;
  }

  this->rows=rows;
  this->regSize=regsize;
  if(rows)
    stream.seek(0);
  perro=0;
  FIM:
  if(erro)
    *erro=perro;
  return *this;
}
/*=========================================================

=========================================================*/
Table *Table::instance(void){
  return new(cMemAlloc(sizeof(Table)))Table();
}
/*=========================================================

=========================================================*/
Table &Table::reconfigureRows(void){
  rows=!def.len()||!stream.len()
    ?0
    :stream.len()/getRegSize();
  return *this;
}
