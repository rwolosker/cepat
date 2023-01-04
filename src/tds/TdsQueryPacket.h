/**********************************************************

**********************************************************/
class TdsQueryPacket:public Object{
  protected:
    String
      sql;
  public:
    TdsQueryPacket();
    ~TdsQueryPacket();
    TdsQueryPacket &setSql(const String &sql);
    String getSql(void);
    ByteArray create(void);
};
