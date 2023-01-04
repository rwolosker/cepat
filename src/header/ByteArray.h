/**********************************************************

**********************************************************/
class ByteArray:public Object{
  protected:
    uchar
      *str;
    int
      length,
      pos;

    void init(void);
    void need(int value);
    void needWrite(int value);
    void inc(int size);
  public:
    ByteArray();
    ByteArray(const ByteArray &value);
    ByteArray(uchar c);
    ByteArray(char c);
    ByteArray(cchar *value);
    ByteArray(int value);
    ByteArray(ulong value);
    ByteArray(short value);
    ByteArray(double value);
    ~ByteArray();
    uchar      at(int x);
    ByteArray &reset(void);
    uchar     *ptr(void);
    int        len(void);
    int        position(void);
    char       eof(void);
    ByteArray &fill(int c);
    ByteArray &resize(int size);
    ByteArray &seek(int pos);
    ByteArray &set(uchar c);
    ByteArray &set(char c);
    ByteArray &set(cchar *value);
    ByteArray &set(int value);
    ByteArray &set(ulong value);
    ByteArray &set(short value);
    ByteArray &set(double value);
    ByteArray &set(void *value,int size);
    ByteArray &setUtf16(cchar *value);
    ByteArray &debug(char dump=0);
    ByteArray &hexDump(void);
    ByteArray &append(const ByteArray &value);
    ByteArray &append(const ByteArray &value,int size);
    ByteArray &operator=(const ByteArray &value);
    ByteArray  copy(void);
    ByteArray  mid(int start,int size);
    String     toString(void);
    char       readLine(String &line);
    ByteArray &write(void *buffer,int size);
    ByteArray &write(const ByteArray &value);
    ByteArray &writeChar(char value);
    ByteArray &writeShort(short value);
    ByteArray &writeInt(int value);
    ByteArray &writeDouble(double value);
    ByteArray &writePtr(void *value);
    ByteArray &writeStr(const String &value,int len=-1);
    ByteArray &writef(cchar *format,...);

    short      readShort(void);
    int        readInt(int size=4);
    char       readChar(void);
    uchar      readUchar(void);
    double     readDouble(void);
    String     readStr(int size);
    ByteArray  readBuffer(int size=-1);
    String     readUtf16(int size);
    void      *readPointer(void);

    int        indexOf(void *buffer,int size);
    int        indexOf(int start,void *buffer,int size);

    CharRef operator[](int x);
    ByteArray *instance(void);
};
