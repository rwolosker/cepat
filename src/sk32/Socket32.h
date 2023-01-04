/**********************************************************

**********************************************************/
class Socket32:public Object{
  protected:
    int
      lineSize,
      timeout,
      erro,
      lastWrite,
      verbose,
      lastRead;
    char
      nonblock,
      nodelay,
      isopen;
    Split
      headers;
    String
      dns;//serv1;serv2
    SK32
      *sk;
    GcObject
      gco;

    void  initSocket32(void);
    SK32 *instanceSK(void);
  public:
    Socket32();
    ~Socket32();

    Socket32 &close(void);
    Socket32 &debug(void);
    String    getDns(void);
    Split    &getHeaders(void);
    SK32     &getSk(void);
    int       getContentLength(void);
    int       getError(void);
    int       getLastWrite(void);
    int       getLastRead(void);
    int       getLineSize(void);
    int       getNodelay(void);
    int       getNonblock(void);
    int       getTimeout(void);
    char      headerResolve(void);
    char      isChunk(void);
    char      isOpen(void);
    char      open(const String &host,int port);
    char      read(ByteArray &buffer,int len);
    char      readChunk(ByteArray &buffer);
    Socket32 &reset(void);
    String    resolve(const String &host);
    Socket32 &setDebug(int debug);
    Socket32 &setDns(const String &dns);
    Socket32 &setLineSize(int lineSize);
    Socket32 &setNodelay(int nodelay);
    Socket32 &setNonblock(int nonblock);
    Socket32 &setTimeout(int timeout);
    char      write(const String &buffer);
    char      write(ByteArray &buffer);
};
