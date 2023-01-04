/**********************************************************

**********************************************************/
class MySql:public TableProtocol{
  protected:
    SSL32
      *ssl;
    GcObject
      table;

    void initMySql(void);
    char send(MySend &send,MyByteArray &buffer);
    char receive(MySend &send,MyByteArray &buffer);
  public:
    MySql();
    ~MySql();
    MySql &setSSL(SSL32 &ssl);
    char connect(void);
    char executeSql(const String &sql);
    Table query(const String &sql);
    Table getTable(int x);
    void  addTable(Table &table);
};
