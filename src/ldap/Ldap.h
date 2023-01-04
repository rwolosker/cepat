/**********************************************************
Por linha de comando.
A LIB do LDAP é muito instrusiva. A quantidade de DLLs
necessárias é a mesma da LIB.
**********************************************************/
class Ldap:public Object,public ExtConfigure{
  protected:
    String
      host,
      baseDn,
      dns,
      lib,
      path,
      erro,
      queryFormat;

    String
      ip,
      dnUser,
      dnPass;
    int
      port;

    void   initLdap(void);
    String cmdCreate(void);
    char   dnsResolve(String &out);
    char   result(ByteArray &buffer,Map &out);
  public:
    Ldap();
    ~Ldap();
    Ldap  &clear(void);
    Ldap  &configureImpl(const String &host,const String &baseDn,const String &dns,const String &lib,const String &path,const String &queryFormat,int port);
    void   configure(Map &map);
    String getError(void);
    char   login(cchar *user,cchar *pass);
    char   search(const String  &query,Map &out);
    char   cmdSearch(const String &query,Map &out);
    Ldap  &setQueryFormat(const String  &query);
};
