/**********************************************************

**********************************************************/
class MyPacketQuery:public Object{
  public:
    MyPacketQuery();
    ~MyPacketQuery();
    MyByteArray query(const String &sql);
};
