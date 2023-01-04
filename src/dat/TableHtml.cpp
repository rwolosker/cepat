/*=========================================================

=========================================================*/
TableHtml::TableHtml(){
}
TableHtml::~TableHtml(){
}
/*=========================================================

=========================================================*/
String TableHtml::toHtml(Table &table){
  ByteArray
    buffer;
  TableDef
    &def=table.getDef();

  buffer.writeStr("<table><tr>");
  for(int i=0;i<def.len();i++)
    buffer
      .writeStr("<th>")
      .writeStr(def.get(i).getName())
      .writeStr("</th>");
  buffer.writeStr("</tr>");
  table.top();
  while(table.fetch()){
    buffer.writeStr("<tr>");
    for(int i=0;i<def.len();i++)
      buffer
        .writeStr("<td>")
        .writeStr(def.get(i).toString())
        .writeStr("</td>");
    buffer.writeStr("</tr>");
  }
  buffer.writeStr("</table>");

  return buffer.toString();
}
