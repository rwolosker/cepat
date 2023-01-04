/**********************************************************

**********************************************************/
class Tds:public TableProtocol{
  protected:
    int
      version,
      packetSize;

    void initTds(void);
    char execute(const String &sql,TdsQueryDef *def=0);
  public:
    Tds();
    ~Tds();

    virtual char connect(void);
    virtual char executeSql(const String &sql);
    virtual Table query(const String &sql);
};
