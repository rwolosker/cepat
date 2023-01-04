/*=========================================================

=========================================================*/
TableCsv::TableCsv(){
}
TableCsv::~TableCsv(){
}
/*=========================================================

=========================================================*/
String TableCsv::toCsv(Table &table){
  ByteArray
    buffer;

  buffer.resize(table.getRows()*table.getRegSize()*2);
  table.top();
  for(int i=0;i<table.getDef().len();i++)
    buffer
      .append(table.getDef().get(i).getName().ptr())
      .append(";");
  buffer.append("\r\n");
  while(table.fetch()){
    for(int i=0;i<table.getDef().len();i++)
      buffer
        .append(table.getDef().get(i).toString().ptr())
        .append(";");
    buffer.append("\r\n");
  }

  return buffer.toString();
}
