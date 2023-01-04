/**********************************************************

**********************************************************/
class MyLoginPacket:public Object{
  protected:
    String
      username,
      password,
      database;
    MyHandshakeV10
      *v10;
    SSL32
      *ssl;

    void initMyLoginPacket(void);
    ByteArray passwordGen(void);
  public:
    MyLoginPacket();
    ~MyLoginPacket();
    MyLoginPacket &setUser(const String &username);
    MyLoginPacket &setPass(const String &password);
    MyLoginPacket &setDatabase(const String &database);
    MyLoginPacket &setV10(MyHandshakeV10 &v10);
    MyLoginPacket &setSSL(SSL32 &ssl);
    MyByteArray    create(void);
    void test(cchar *senha,cchar *cpart1,cchar *cpart2,cchar *cres);




};
