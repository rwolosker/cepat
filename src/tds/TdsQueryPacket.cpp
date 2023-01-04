/*=========================================================

=========================================================*/
TdsQueryPacket::TdsQueryPacket(){

}
TdsQueryPacket::~TdsQueryPacket(){

}
/*=========================================================

=========================================================*/
TdsQueryPacket &TdsQueryPacket::setSql(const String &sql){this->sql=sql;return *this;}
String TdsQueryPacket::getSql(void){return sql;}
/*=========================================================

=========================================================*/
ByteArray TdsQueryPacket::create(void){
  ByteArray
    buffer;

  buffer.setUtf16(sql);
  return buffer;
}
