/**********************************************************

**********************************************************/
class HttpGet:public Object{
  protected:
    int
      timeOut,
      port;
    char
      noDelay,
      nonBlock;
    String
      host,
      dns,
      erro;
    Split
      header;
    Socket32
      socket;

    void initHttpGet(void);
    char sendUrl(const String &url);
    char headers(void);
    char download(ByteArray &buffer);


  public:
    HttpGet();
    ~HttpGet();
    HttpGet  &setTimeOut(int timeOut);
    HttpGet  &setPort(int port);
    HttpGet  &setNoDelay(char noDelay);
    HttpGet  &setNonBlock(char nonBlock);
    HttpGet  &setHost(const String &host);
    HttpGet  &setDns(const String &dns);

    HttpGet  &open(void);
    HttpGet  &download(const String &url,ByteArray &buffer);

    String getErro(void);




};
