/**********************************************************
https://dev.mysql.com/doc/internals/en/integer.html#fixed-length-integer
**********************************************************/
class MyByteArray:public ByteArray{
  public:
  MyByteArray();
  ~MyByteArray();
  void writeFlInt(int64 value,int size);
  int64 readFlInt(int size);
  void writeLeInt(int64 value);
  int64 readLeInt(void);
  void writeFlString(const String &value,int size);
  String readFlString(int size);
  void writeNuString(const String &value);
  String readNuString(void);
  void writeLeString(const String &value);
  String readLeString(void);
  String readEofString(int len=0);
};
