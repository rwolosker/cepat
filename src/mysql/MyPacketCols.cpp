/*=========================================================

=========================================================*/
void MyPacketCols::initMyCols(void){
  cols=0;
  send=0;
  exe=0;
  socket=0;
  my=0;
}
/*=========================================================

=========================================================*/
void MyPacketCols::setMySql(MySql &my){
  this->my=&my;
}
/*=========================================================

=========================================================*/
void MyPacketCols::configure(MySend &send,MyPacketExecute &exe,Socket32 &socket){
  this->send=&send;
  this->exe=&exe;
  this->socket=&socket;
}
/*=========================================================

=========================================================*/
MyPacketCols::MyPacketCols(){
  initMyCols();
}
MyPacketCols::~MyPacketCols(){
}
int MyPacketCols::getCols(void){return cols;}
/*=========================================================

=========================================================*/
char MyPacketCols::action(uchar type,void **argv){
  return 0;
}
/*=========================================================

=========================================================*/
void MyPacketCols::reset(void){
}
/*=========================================================

=========================================================*/
char MyPacketCols::onError(MyByteArray &buffer){
  if(!buffer.len())
    return 0;
  buffer.seek(0);
  cols=buffer.readLeInt();
  exe->setOnError(0);

  readCols(buffer);

  buffer.reset();
  return 1;
}
/*=========================================================

=========================================================*/
void MyPacketCols::readCol(MyByteArray &buffer,TableDef &def){
  String
    catalog=buffer.readLeString(),
    schema=buffer.readLeString(),
    table=buffer.readLeString(),
    tableOrg=buffer.readLeString(),
    name=buffer.readLeString(),
    nameOrg=buffer.readLeString();
  Number
    lenField=buffer.readLeInt(),
    charSet=buffer.readFlInt(2),
    len=buffer.readFlInt(4),
    type=buffer.readFlInt(1);

  switch(type.asInt()){
    case MYSQL_TYPE_TINY:
    case MYSQL_TYPE_SHORT:
    case MYSQL_TYPE_LONG:
    case MYSQL_TYPE_LONGLONG:
    case MYSQL_TYPE_INT24:
    case MYSQL_TYPE_YEAR:
      len=0;
      type=C_INT;
      break;
    case MYSQL_TYPE_DECIMAL:
    case MYSQL_TYPE_FLOAT:
    case MYSQL_TYPE_DOUBLE:
    case MYSQL_TYPE_NULL:
    case MYSQL_TYPE_NEWDECIMAL:
      len=0;
      type=C_DOUBLE;
      break;
    case MYSQL_TYPE_BIT:
      len=0;
      type=C_BOOL;
      break;
    case MYSQL_TYPE_DATE:
      len=0;
      type=C_DATE;
      break;
    case MYSQL_TYPE_TIME:
    case MYSQL_TYPE_TIMESTAMP:
    case MYSQL_TYPE_DATETIME:
    case MYSQL_TYPE_NEWDATE:
    case MYSQL_TYPE_VARCHAR:
    case MYSQL_TYPE_TIMESTAMP2:
    case MYSQL_TYPE_DATETIME2:
    case MYSQL_TYPE_TIME2:
      len=0;
      type=C_DATETIME;
      break;
    case MYSQL_TYPE_VAR_STRING:
    case MYSQL_TYPE_STRING:
      type=C_TEXT;
      len+=1;
      break;
    case MYSQL_TYPE_BLOB:
    case MYSQL_TYPE_LONG_BLOB:
      type=C_TEXT;
      len=1024;
      break;
    default:
      cPrintf("ERRO: MyPacketCols::readCol [%X]\n",type.asInt());
      exit(0);
  }
  def.add(name,(char)type,len.asInt());
}
/*=========================================================

=========================================================*/
void MyPacketCols::readCols(MyByteArray &buffer){
  TableDef
    def;

  table.reset();
  for(int i=0;i<cols;i++){
    if(!send->receive(*socket,buffer))
      return;
    readCol(buffer,def);
  }
  table=def;

  int
    fes=0,lidos;
  ByteArray
    tmp;

  while(1){
    if(!send->receive(*socket,buffer,&lidos)){
      table.reset();
      return;
    }
    if(buffer.at(0)==0xFE){
      exe->exec(buffer);
      fes+=1;
      if(fes==2)
        break;
      continue;
    }
    tmp.write(buffer.ptr(),lidos);
  }
  buffer.reset().append(tmp).seek(0);
  while(!buffer.eof()){
    table.addNew();
    for(int i=0;i<cols;i++)
      switch(table.getDef().get(i).getType()){
        case C_DATE:
        case C_DATETIME:
          table.putDbl(i,cDateParseFast(buffer.readLeString()));
          break;
        default:
          table.putTxt(i,buffer.readLeString());
          break;
      }
  }
  my->addTable(table);
}

