/**********************************************************

**********************************************************/
class SSL32:public Object{
  protected:
    String
      key;
    SK32
      *sk;

    void initSsl32(void);
  public:
    SSL32();
    virtual ~SSL32();
    virtual char init(cchar *libcrypt,cchar *libssl);
    char handshake(Socket32 &s);
    char encrypt(cchar *key,ByteArray &buffer);
    char decrypt(cchar *key,ByteArray &buffer);
    char sha1(ByteArray &out);
    SK32 &getSk(void);
    void setKey(cchar *key);
};
