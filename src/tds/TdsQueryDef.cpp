/*=========================================================

=========================================================*/
TdsQueryDef::TdsQueryDef(){
  initTdsQueryDef();
}
TdsQueryDef::~TdsQueryDef(){
}
/*=========================================================

=========================================================*/
void TdsQueryDef::reset(void){
  def.reset();
  table.reset();
  gco.reset();
  gc.reset();
  initTdsQueryDef();
}
/*=========================================================

=========================================================*/
TableDef &TdsQueryDef::getTableDef(void){
  return def;
}
/*=========================================================

=========================================================*/
Table TdsQueryDef::getTable(void){
  return table;
}
/*=========================================================

=========================================================*/
void TdsQueryDef::initTdsQueryDef(void){
  cols=0;
  tdsDef=0;
}
/*=========================================================

=========================================================*/
TDS_QUERY_DEF *TdsQueryDef::getTdsDef(void){
  return tdsDef;
}
/*=========================================================

=========================================================*/
void TdsQueryDef::createTable(void){
  def.reset();
  for(int i=0;i<cols;i++)
    switch(tdsDef[i].type){
      case TDS_BIT:
      case TDS_SYBBIT:
        def.add(tdsDef[i].name->ptr(),C_BOOL);
        break;
      case TDS_INT:
      case TDS_SYBINT1:
      case TDS_SYBINT2:
      case TDS_SYBINT4:
        def.add(tdsDef[i].name->ptr(),C_INT);
        break;
      case TDS_MONEY:
      case TDS_REAL:
      case TDS_NUMERIC:
      case TDS_DECIMAL:
        def.add(tdsDef[i].name->ptr(),C_DOUBLE);
        break;
      case TDS_DATETIME:
      case TDS_SYBDATETIME:
        def.add(tdsDef[i].name->ptr(),C_DATETIME);
        break;
      case TDS_DATE:
      case TDS_CHAR:
      case TDS_VARCHAR:
      case TDS_NCHAR:
        def.add(tdsDef[i].name->ptr(),C_TEXT,tdsDef[i].size+1);
        break;
      case TDS_TEXT:
      case TDS_NTEXT:
        def.add(tdsDef[i].name->ptr(),C_TEXT,tdsDef[i].size+1);
        break;
      default:
        cPrintf("ERRO: TdsQueryDef::createTable  TYPE %X\n",tdsDef[i].type);
        exit(0);
    }
  table=def;
}
/*=========================================================

=========================================================*/
void TdsQueryDef::readCols(ByteArray &buffer){
  cols=buffer.readShort();
  tdsDef=(TDS_QUERY_DEF*)cMemAlloc(cols*sizeof(TDS_QUERY_DEF));
  gc.add(tdsDef);

  for(int i=0;i<cols;i++){
    int
      type=0,pre=0,size=0,len=0,dec=0;
    String
      &name=*String().instance();

    gco.add(&name);
    buffer.readBuffer(2+2);
    type=buffer.readUchar();
    switch(type){
      case TDS_SYBINT1:
      case TDS_SYBINT2:
      case TDS_SYBINT4:
        len=buffer.readUchar();
        name=buffer.readUtf16(len);
        break;
      case TDS_SYBDATETIME:
        len=buffer.readUchar();
        name=buffer.readUtf16(len);
        break;
      case TDS_SYBBIT:
        len=buffer.readUchar();
        name=buffer.readUtf16(len);
        break;
      case TDS_INT:
      case TDS_DATETIME:
      case TDS_BIT:
      case TDS_MONEY:
      case TDS_REAL:
        size=buffer.readUchar();
        len=buffer.readUchar();
        name=buffer.readUtf16(len);
        break;
      case TDS_VARCHAR:
      case TDS_CHAR:
      case TDS_DATE:
      case TDS_NCHAR:
        size=buffer.readShort();
        buffer.readBuffer(5);
          //informacao de collate
          //https://www.freetds.org/tds.html#t129
        len=buffer.readUchar();
        name=buffer.readUtf16(len);
        break;
      case TDS_NUMERIC:
      case TDS_DECIMAL:
        size=buffer.readUchar();
        pre=buffer.readUchar();
        dec=buffer.readUchar();
        len=buffer.readUchar();
        name=buffer.readUtf16(len);
        break;
      case TDS_TEXT:
      case TDS_NTEXT:
        size=1024;
        buffer.readBuffer(19);
        len=buffer.readUchar();
        name=buffer.readUtf16(len);
        break;
      default:
        cPrintf("ERRO TdsQueryDef::action: TYPE %X\n",type);
        buffer.debug(1);
        exit(0);
    }
    tdsDef[i].name=&name;
    tdsDef[i].type=type;
    tdsDef[i].size=size;
    tdsDef[i].precicion=pre;
    tdsDef[i].scale=dec;
    tdsDef[i].cols=cols;
  }
}
/*=========================================================

=========================================================*/
void TdsQueryDef::readRow(ByteArray &buffer){
  int
    len;

  table.addNew();
  for(int i=0;i<cols;i++)
    switch(tdsDef[i].type){
      case TDS_SYBINT4:
        table.putInt(i,buffer.readInt());
        break;
      case TDS_INT:
        len=buffer.readInt(1);
        table.putInt(i,buffer.readInt(len));
        break;
      case TDS_VARCHAR:
        len=buffer.readInt(2);
        table.putStr(i,buffer.readStr(len==0xFFFF?0:len));
        break;
      case TDS_DATETIME:
        len=buffer.readInt(1);
        table.putDbl(i,tdsDateTime(len,buffer.readBuffer(len)));
        break;
      default:
        cPrintf("ERRO TdsQueryDef::readRow: TYPE %X\n",tdsDef[i].type);
        buffer.debug(1);
        exit(0);
    }
}

/*=========================================================

=========================================================*/
double TdsQueryDef::tdsDateTime(int len,const ByteArray &buffer){
  Date
    data;
  int
    dia,segundo;

  switch(len){
    case 4:
      dia=PBYTEARRAY(buffer).readInt(2);
      segundo=PBYTEARRAY(buffer).readInt(2);
      if(!dia&&!segundo)
        return 0;
      data
        .fromDouble(19000101)
        .addDay(dia)
        .addSecond(segundo*60);
      break;
    case 8:
      dia=PBYTEARRAY(buffer).readInt(4);
      segundo=PBYTEARRAY(buffer).readInt(4);
      if(!dia&&!segundo)
        return 0;
      data
        .fromDouble(19000101)
        .addDay(dia)
        .addSecond(segundo/300);
      break;
  }

  return data;
}
/*=========================================================

=========================================================*/
char TdsQueryDef::action(uchar type,void **arg){
  ByteArray
    &buffer=*(ByteArray*)arg[0];

  switch(type){
    case TDS_RESULT:
      readCols(buffer);
      createTable();
      break;
    case TDS_ROW_RESULT:
      readRow(buffer);
      break;
  }
  return 1;
}
