SQLITE_API
  cSqliteApi;
char
  cSqliteApiStart=0;

void SqLiteItem::initSqLiteItem(void){
  object=0;
  type=0;
}
SqLiteItem::SqLiteItem(){
  initSqLiteItem();
}
SqLiteItem::~SqLiteItem(){
}
/*=========================================================

=========================================================*/
SqLiteItem &SqLiteItem::set(char type,Object *object){
  this->type=type;
  this->object=object;
  gco.add(object);
  return *this;
}
/*=========================================================

=========================================================*/
Number &SqLiteItem::getNumber(void){return *(Number*)object;};
Table &SqLiteItem::getTable(void){return *(Table*)object;};
char SqLiteItem::getType(void){return type;}
/*=========================================================

=========================================================*/
SqLiteItem *SqLiteItem::intance(void){
  return new(cMemAlloc(sizeof(SqLiteItem)))SqLiteItem();
}
/*=========================================================

=========================================================*/
SqLiteResult::SqLiteResult(){
}
SqLiteResult::~SqLiteResult(){
}
/*=========================================================

=========================================================*/
void SqLiteResult::reset(void){
  gco.reset();
}
/*=========================================================

=========================================================*/
int SqLiteResult::len(void){
  return gco.len();
}
/*=========================================================

=========================================================*/
void SqLiteResult::add(Number &number){
  SqLiteItem
    &item=*SqLiteItem().intance();

  gco.add(VOID(
    &item.set(SQLITE_RES_NUMBER,&number)));
}
/*=========================================================

=========================================================*/
void SqLiteResult::add(Table &table){
  SqLiteItem
    &item=*SqLiteItem().intance();

  gco.add(VOID(
    &item.set(SQLITE_RES_TABLE,&table)));
}
/*=========================================================

=========================================================*/
SqLiteItem &SqLiteResult::get(int x){
  if(x>=0&&x<len())
    return *(SqLiteItem*)gco.get(x);
  else{
    SqLiteItem
      &item=*SqLiteItem().intance();

    gco.add(&item);
    return item;
  }
}
/*=========================================================

=========================================================*/
SqLite::SqLite(){
  initSqLite();
}
SqLite::~SqLite(){
  close();
}
/*=========================================================

=========================================================*/
void SqLite::initSqLite(void){
  handle=stm=0;
  createBreakLine=0;
}
/*=========================================================

=========================================================*/
SqLite &SqLite::setCreateBreakLine(char createBreakLine){
  this->createBreakLine=createBreakLine;
  return *this;
}
/*=========================================================

=========================================================*/
SqLite &SqLite::close(void){
  if(cSqliteApiStart&&stm)
    cSqliteApi.sqlite3_finalize(stm);
  if(cSqliteApiStart&&handle)
    cSqliteApi.sqlite3_close(handle);
  stm=handle=0;
  fileName.reset();
  return *this;
}
/*=========================================================

=========================================================*/
char SqLite::open(cchar *file,char memory){
  close();
  if(!cSqliteApiStart)
    return 0;
  else{
    int
      ok;
    SQLITE_API
      &api=cSqliteApi;

    if(!memory)
      ok=api.sqlite3_open(file,&handle);
    else
      ok=api.sqlite3_open_v2(file,&handle,SQLITE_OPEN_MEMORY,0);

    if(!ok)
      fileName=file;
    return !ok?1:0;
  }
}
/*=========================================================

=========================================================*/
String SqLite::getError(void){
  return lastErro;
}
/*=========================================================

=========================================================*/
SqLite &SqLite::resetStm(void){
  if(stm)
    cSqliteApi.sqlite3_reset(stm);
  gco.reset();
  return *this;
}
/*=========================================================

=========================================================*/
SqLite &SqLite::putInt(int col,int value){
  if(!stm)
    return *this;
  cSqliteApi.sqlite3_bind_int(stm,col+1,value);
  return *this;
}
/*=========================================================

=========================================================*/
SqLite &SqLite::putDbl(int col,double value){
  if(!stm)
    return *this;
  cSqliteApi.sqlite3_bind_double(stm,col+1,value);
  return *this;
}
/*=========================================================

=========================================================*/
SqLite &SqLite::putTxt(int col,const String &value){
  if(!stm)
    return *this;
  else{
    String
      &tmp=*String().instance();

    tmp=value;
    cSqliteApi.sqlite3_bind_text(stm,col+1,tmp.ptr(),tmp.len(),0);
    gco.add(&tmp);
  }
  return *this;
}
/*=========================================================

=========================================================*/
SqLite &SqLite::toSql(int drop,int create,ByteArray &buffer,...){
  void
    **p=(void**)&drop;

  buffer.reset();
  for(int i=3;p[i];i++){
    Table
      &table=*(Table*)p[i];

    toSqlImpl(drop,create,table,buffer);
  }
  return *this;
}
/*=========================================================

=========================================================*/
void SqLite::toSqlImpl(char drop,char create,Table &table,ByteArray &buffer){
  toSqlDrop(drop,table,buffer);
  toSqlCreate(create,table,buffer);
  toSqlInsert(table,buffer);
}
/*=========================================================

=========================================================*/
void SqLite::toSqlInsert(Table &table,ByteArray &buffer){
  int
    recno=table.recNo();
  TableDef
    &def=table.getDef();
  Split
    pagina;
  ByteArray
    cache;
  int
    total=0;

  table.top();
  while(table.fetch()){
    ByteArray
      tmp;

    for(int i=0;i<def.len();i++){
      Field
        &f=def.get(i);

      switch(f.getType()){
        case C_DATETIME:
        case C_DATE:
          tmp.writef("%f,",f.getDate().getJulian().julian);
          break;
        case C_DOUBLE:
          tmp.writef("%f,",f.getDouble());
          break;
        case C_BOOL:
        case C_INT:
          tmp.writef("%d,",f.getInt());
          break;
        default:
          tmp.writef(
            "\"%s\",",
            f.getString().ptr()==0
              ?""
              :f.getString().ptr()
          );
      }
    }
    tmp=ByteArray().writef(
      "(%s),\n",
      tmp.toString().mid(0,tmp.len()-1).ptr()
    );
    cache.write(tmp);

    total+=1;
    if(total==499){
      pagina.add(
        cache.toString().mid(0,cache.len()-2).ptr()
      );
      total=0;
      cache.reset();
    }
  }
  if(cache.len())
    pagina.add(
      cache.toString().mid(0,cache.len()-2).ptr()
    );

  for(int i=0;i<pagina.len();i++)
    buffer.writef(
      "insert into %s values\n%s;\n",
      table.getName().ptr(),
      pagina[i].ptr()
    );

  table.go(recno);
}
/*=========================================================

=========================================================*/
void SqLite::toSqlDrop(char drop,Table &table,ByteArray &buffer){
  if(!drop)
    return;
  else
    buffer.writef("drop table if exists %s;\n",table.getName().ptr());
}
/*=========================================================

=========================================================*/
void SqLite::toSqlCreate(char create,Table &table,ByteArray &buffer){
  if(!create)
    return;
  else{
    TableDef
      &def=table.getDef();
    ByteArray
      tmp;

    for(int i=0;i<def.len();i++){
      Field
        &f=def.get(i);
      String
        name=f.getName(),
        type;

      switch(f.getType()){
        case C_DATETIME:
        case C_DOUBLE:
          type="real";
          break;
        case C_DATE:
        case C_BOOL:
        case C_INT:
          type="integer";
          break;
        default:
          type="text";
      }
      tmp.writef("  %s %s,\n",name.ptr(),type.ptr());
    }
    tmp=ByteArray().writef(
      "create table %s(\n"
      "%s\n);\n",
      table.getName().ptr(),
      tmp.toString().mid(0,tmp.len()-2).ptr()
    );
    if(!createBreakLine)
      tmp=ByteArray().writef(
        "%s\n",
        tmp
          .toString()
          .replace("\n","")
          .replace("\r\n","")
          .replace("  ","")
          .ptr()
      );
    buffer.write(tmp);
  }
}
/*=========================================================

=========================================================*/
SqLite &SqLite::shell(ByteArray &buffer,String &erro,cchar *ld,cchar *lib,cchar *cmd){
  File
    tmp,out;
  Shell
    shell;
  String
    exec,res;

  erro.reset();
  out.temp();
  tmp.temp();
  tmp.write(buffer);
  tmp.write(""
    "\n"
    ".print FIM SQLITE SHELL\n"
    ".quit\n"
  );
  tmp.sync();
  exec=String().appendf(
    "%s/%s --library-path %s %s/%s %s < %s\n",
    lib,
    ld,
    lib,
    lib,
    cmd,
    fileName.ptr(),
    tmp.getName().ptr()
  );
  shell.exec(exec,out);
  out.readAll(erro);

  if(!erro.inStr("FIM SQLITE SHELL"))
    erro.reset();
  return *this;
}
/*=========================================================

=========================================================*/
void SqLite:: stmToTable(void *stm,int cols,Table &table){
  table.addNew();
  for(int i=0;i<cols;i++)
    switch(table.getDef().get(i).getType()){
      case C_DATE:
      case C_DATETIME:
      case C_DOUBLE:
        table.putDbl(i,cSqliteApi.sqlite3_column_double(stm,i));
        break;
      case C_TEXT:
        table.putTxt(i,cSqliteApi.sqlite3_column_text(stm,i));
        break;
      case C_INT:
      case C_BOOL:
        table.putInt(i,cSqliteApi.sqlite3_column_int(stm,i));
        break;
    }
}
/*=========================================================

=========================================================*/
char SqLite::query(cchar *sql,SqLiteResult &result){
  result.reset();
  if(!cSqliteApiStart){
    lastErro="ERRO: not lib.";
    return 0;
  }
  if(!handle){
    lastErro="ERRO: not open.";
    return 0;
  }
  {
    void
      *stm=0;
    char
      *psql=PCHAR(sql),
      *remain=0;

    while(1){
      int
        ok;

      if(stm){
        cSqliteApi.sqlite3_finalize(stm);
        stm=0;
      }
      ok=cSqliteApi.sqlite3_prepare_v2(handle,psql,-1,&stm,&remain);

      if(ok){
        lastErro=cSqliteApi.sqlite3_errmsg(handle);
        cSqliteApi.sqlite3_finalize(stm);
        return 0;
      }
      ok=cSqliteApi.sqlite3_step(stm);
      if(ok==SQLITE_DONE){
        Number
          &number=*Number().instance();

        number=cSqliteApi.sqlite3_changes(handle);
        result.add(number);
        goto NEXT;
      }
      if(ok==SQLITE_ROW){
        Table
          &table=*Table().instance();
        int
          cols=cSqliteApi.sqlite3_column_count(stm);

        result.add(table);
        if(cols){
          TableDef
            def;

          for(int i=0;i<cols;i++){
            String
              name=cSqliteApi.sqlite3_column_name(stm,i);
            int
              size=0,
              type=cSqliteApi.sqlite3_column_type(stm,i);

            switch(type){
              case SQLITE_INTEGER:
                type=C_INT;
                break;
              case SQLITE_DOUBLE:
                type=C_DOUBLE;
                break;
              case SQLITE_TEXT:
                type=C_TEXT;
                size=256;
                break;
              default:
                cPrintf("ERRO: SqLite.query type[%d].\n",type);
                exit(0);
            }
            def.add(name,type,size);
          }
          table=def;
          stmToTable(stm,cols,table);
          while(cSqliteApi.sqlite3_step(stm)==SQLITE_ROW)
            stmToTable(stm,cols,table);
        }
      }
      NEXT:
      if(remain&&*remain){
        psql=remain;
        continue;
      }
      break;
    }
    if(stm)
      cSqliteApi.sqlite3_finalize(stm);
  }
  return 1;
}
/*=========================================================

=========================================================*/
SqLite &SqLite::createModule(cchar *name,SQLITE_MODULE &module,SQLITE_AUX &aux){
  int
    ok=cSqliteApi.sqlite3_create_module(
      handle,
      PCHAR(name),
      &module,
      &aux
    );
  if(ok){
    cPrintf("%s\n",cSqliteApi.sqlite3_errmsg(handle));
    exit(1);
  }
  aux.sqLite=this;
  aux.handle=handle;
  return *this;
}
/*=========================================================

=========================================================*/
SqLite &SqLite::createFunction(cchar *name,int count,void *pf,SQLITE_AUX &aux){
  int
    ok=cSqliteApi.sqlite3_create_function(
      handle,
      name,
      count,
      0,
      &aux,
      pf,
      0,
      0
    );
  if(ok){
    cPrintf("%s\n",cSqliteApi.sqlite3_errmsg(handle));
    exit(1);
  }
  aux.sqLite=this;
  aux.handle=handle;
  return *this;
}
/*=========================================================

=========================================================*/
char SqLite::init(cchar *libfile){
  if(cSqliteApiStart)
    return 1;
  if(!File().access(libfile,"r--"))
    return 0;
  if(!(cSqliteApi.lib=cLibraryLoad(PCHAR(libfile))))
    return 0;

  #define LINK(metodo)\
  *(void**)&cSqliteApi.metodo=cLibrarySymbol(cSqliteApi.lib,""#metodo"");\
  if(!cSqliteApi.metodo){\
    cPrintf("ERRO: "#metodo"\n");\
    exit(0);\
  }
  cMemReset(&cSqliteApi,sizeof(SQLITE_API));

  LINK(sqlite3_open)
  LINK(sqlite3_close)
  LINK(sqlite3_prepare)
  LINK(sqlite3_finalize)
  LINK(sqlite3_step)
  LINK(sqlite3_bind_parameter_count)
  LINK(sqlite3_bind_int)
  LINK(sqlite3_column_type);
  LINK(sqlite3_bind_double)
  LINK(sqlite3_bind_text)
  LINK(sqlite3_reset)
  LINK(sqlite3_errmsg);
  LINK(sqlite3_free);
  LINK(sqlite3_exec);
  LINK(sqlite3_limit);
  LINK(sqlite3_open_v2);
  LINK(sqlite3_column_count);
  LINK(sqlite3_column_name);
  LINK(sqlite3_column_decltype);
  LINK(sqlite3_column_double);
  LINK(sqlite3_column_int);
  LINK(sqlite3_column_text);
  LINK(sqlite3_prepare_v2);
  LINK(sqlite3_changes);
  LINK(sqlite3_libversion);
  LINK(sqlite3_declare_vtab);
  LINK(sqlite3_create_module);
  LINK(sqlite3_result_double);
  LINK(sqlite3_result_int);
  LINK(sqlite3_result_text);
  LINK(sqlite3_create_function);
  LINK(sqlite3_user_data);
  LINK(sqlite3_value_text);
  LINK(sqlite3_value_int);
  LINK(sqlite3_value_double);

  #undef LINK
  return cSqliteApiStart=1;
}
/*=========================================================

=========================================================*/
int cSqliteConnect(void *db,void *aux,int argc,char **argv,void **vtab,char **erro){
  Map
    argm;
  String
    file;
  Table
    *table=Table().instance();
  File
    f;
  SQLITE_VTAB
    *tab;
  int
    err;
  SQLITE_AUX
    *paux=(SQLITE_AUX*)aux;
  ByteArray
    buffer;

  argm.loadArg(argc,argv);
  file=argm.get("file").asStr();
  if(!f.access(file,"rw-")){
    cPrintf("ERRO: cSqliteConnect %s\n",file.ptr());
    exit(1);
  }
  f.open(file,1);
  table->open(f,&err);
  if(err){
    cPrintf("ERRO: cSqliteConnect %s\n",file.ptr());
    exit(1);
  }
  paux->sqLite->toSql(0,1,buffer,table->copy().removeAll().me(),0);
  cSqliteApi.sqlite3_declare_vtab(
    db,
    buffer.toString()
  );
  tab=(SQLITE_VTAB*)cMemAlloc(sizeof(SQLITE_VTAB));
  tab->file=cMemAddChar(file.ptr());
  tab->table=table;

  *vtab=tab;
  return 0;
}
/*=========================================================

=========================================================*/
int cSqliteCreate(void *db,void *aux,int argc,char **argv,void **vtab,char **erro){
  return cSqliteConnect(db,aux,argc,argv,vtab,erro);
}
/*=========================================================

=========================================================*/
int cSqliteDisconnect(void *vtab){
  SQLITE_VTAB
    *tab=(SQLITE_VTAB*)vtab;

  tab->table->~Table();
  cMemFreeAll(
    tab->file,
    tab->table,
    tab,
    -1
  );
  return SQLITE_OK;
}
/*=========================================================

=========================================================*/
int cSqliteOpen(void *vtab,void **cursor){
  SQLITE_VTAB
    *tab=(SQLITE_VTAB*)vtab;
  SQLITE_CURSOR
    *c=(SQLITE_CURSOR*)cMemAlloc(sizeof(SQLITE_CURSOR));
  Table
    *table=Table().instance();

  table->operator=(*tab->table);
  table->go(0);
  c->table=table;
  *cursor=c;
  return SQLITE_OK;
}
/*=========================================================

=========================================================*/
int cSqliteColumn(void *cursor,void *ctx,int col){
  SQLITE_CURSOR
    *c=(SQLITE_CURSOR*)cursor;
  SQLITE_API
    &api=cSqliteApi;

  switch(c->table->getDef().get(col).getType()){
    case C_TEXT:
      api.sqlite3_result_text(ctx,VOID(c->table->getTxt(col)),-1,0);
      break;
    case C_DOUBLE:
    case C_DATETIME:
      api.sqlite3_result_double(ctx,c->table->getDbl(col));
      break;
    case C_INT:
    case C_BOOL:
    case C_CHAR:
    case C_DATE:
      api.sqlite3_result_int(ctx,c->table->getInt(col));
      break;
    default:
      cPrintf("ERRO: cSqliteColumn [%X]\n",c->table->getDef().get(col).getType());
      exit(1);
  }
  return SQLITE_OK;
}
/*=========================================================

=========================================================*/
int cSqliteNext(void *cursor){
  SQLITE_CURSOR
    *c=(SQLITE_CURSOR*)cursor;
  int
    recno=c->table->recNo(),
    rows=c->table->len();

  if(recno>=rows)
    return 1;
  else{
    c->table->go(recno+1);
    return 0;
  }
}
/*=========================================================

=========================================================*/
int cSqliteEof(void *cursor){
  SQLITE_CURSOR
    *c=(SQLITE_CURSOR*)cursor;
  return c->table->eof();
}
/*=========================================================

=========================================================*/
int cSqliteClose(void *cursor){
  SQLITE_CURSOR
    *c=(SQLITE_CURSOR*)cursor;

  c->table->~Table();
  cMemFreeAll(
    c->table,
    c,
    -1
  );
  return SQLITE_OK;
}
/*=========================================================

=========================================================*/
int cSqliteRowId(void *cursor,int64 *row){
  cPrintf("RowId\n");
  return SQLITE_OK;
}
/*=========================================================

=========================================================*/
int cSqliteFilter(void *cursor,int x,char *xstr,int argc,char **argv){
  SQLITE_CURSOR
    *c=(SQLITE_CURSOR*)cursor;

  c->table->go(0);
  return SQLITE_OK;
}
/*=========================================================

=========================================================*/
int cSqliteIndex(void *vtab,void *info){
  return SQLITE_OK;
}
/*=========================================================

=========================================================*/
void cSqliteFunctionSoma(void *ctx,int argc,void **argv){
  SqLite
    *sqt=(SqLite*)cSqliteApi.sqlite3_user_data(ctx);


  cPrintf("%d\n",argc);
  cPrintf("%f\n",cSqliteApi.sqlite3_value_double(argv[0]));
  cPrintf("%f\n",cSqliteApi.sqlite3_value_double(argv[1]));
  cPrintf("%X\n",sqt);

  cSqliteApi.sqlite3_result_double(ctx,1);
};
/*=========================================================

=========================================================*/
void cSqliteModuleInit(SQLITE_MODULE &module){
  cMemReset(&module,sizeof(SQLITE_MODULE));
  module.xBestIndex=cSqliteIndex;
  module.xClose=cSqliteClose;
  module.xConnect=cSqliteConnect;
  module.xCreate=cSqliteCreate;
  module.xDisconnect=cSqliteDisconnect;
  module.xDestroy=cSqliteDisconnect;
  module.xOpen=cSqliteOpen;
  module.xEof=cSqliteEof;
  module.xNext=cSqliteNext;
  module.xColumn=cSqliteColumn;
  module.xRowid=cSqliteRowId;
  module.xFilter=cSqliteFilter;
}
