/**********************************************************

**********************************************************/
class File:public Object{
  protected:
    int
      f;
    char
      openned,
      readOnly,
      temporary;
    String
      name;

    void init();
  public:
    File();
    ~File();
    char access(cchar *file,cchar *mask);
    void close(void);
    int  getF(void);
    char isDir();
    char isOpen(cchar *file=0,char readOnly=0);
    char isReadOnly(void);
    Date modified(void);
    char open(cchar *file,char readOnly=0);
    int  position(void);
    char lock(int timeout=0);
    int  read(String &buffer,int len);
    int  read(cchar *buffer,int len);
    char readLine(String &buffer,int max=1024);
    char readAll(String &buffer);
    char readAll(ByteArray &buffer);
    char seek(int pos);
    int  size(void);
    char sync(void);
    char temp(void);
    char truncate(int size=0);
    char unlock(void);
    void unlink(void);
    int  write(const String &buffer,int len=-1);
    int  write(ByteArray &buffer,int len=-1);

    String getName(char noPath=0);
};
