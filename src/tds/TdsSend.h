/**********************************************************

**********************************************************/
class TdsSend:public Object{
  public:
    TdsSend();
    ~TdsSend();
    char send(Socket32 &socket,ByteArray &buffer);
    char receive(Socket32 &socket,ByteArray &buffer);
};
