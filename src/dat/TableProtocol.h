/**********************************************************

**********************************************************/
class TableProtocol:public Object{
  protected:
    char
      isopen;
    int
      timeout,
      port,
      lasterror,
      affectedInsert,
      affectedDelete,
      affectedUpdate;
    String
      error,
      dns,
      host,
      username,
      password,
      database;
    Socket32
      socket;

    void initTableProtocol(void);
    char openSocket(void);
  public:
    TableProtocol();
    ~TableProtocol();

    int            getAffectedDelete(void);
    int            getAffectedInsert(void);
    int            getAffectedUpdate(void);
    String         getDatabase(void);
    String         getDns(void);
    String         getHost(void);
    String         getPassword(void);
    int            getPort(void);
    Socket32      &getSocket(void);
    String         getUsername(void);
    String         getError(void);
    char           isOpen(void);
    int            lastError(void);
    TableProtocol &setDatabase(const String &database);
    TableProtocol &setDns(const String &dns);
    TableProtocol &setHost(const String &host);
    TableProtocol &setPassword(const String &password);
    TableProtocol &setPort(int port);
    TableProtocol &setTimeout(int timeout);
    TableProtocol &setUsername(const String &username);

    virtual char  connect(void)=0;
    virtual char  executeSql(const String &sql)=0;
    virtual Table query(const String &sql)=0;
};
