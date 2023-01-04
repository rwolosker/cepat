MyPacketQuery::MyPacketQuery(){}
MyPacketQuery::~MyPacketQuery(){}
MyByteArray MyPacketQuery::query(const String &sql){
  MyByteArray
    buffer;

  buffer.writeFlInt(3,1);
  buffer.write(PSTRING(sql).ptr());
  return buffer;
}
