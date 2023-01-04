/********************************************************************

********************************************************************/
class Base64:public Object{
  protected:
    short resolve(short c,char *rem);
  public:
    Base64();
    ~Base64();

    String    encode(ByteArray &buffer);
    String    encode(String &buffer);
    ByteArray decode(const String &buffer);
    char      isBase64(const String &buffer);
    String    line(const String &buffer,int cols=76);
};
