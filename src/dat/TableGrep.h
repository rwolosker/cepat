/**********************************************************

**********************************************************/
class TableGrep:public TableSort{
  protected:
    void
      *methodGrep,
      *methodGrepString;

    void initTableGrep(void);
  public:
    TableGrep();
    ~TableGrep();
    virtual void doGrep(TableGrep &grep);
    void grep(void);
    virtual void grepConfigure(void);
    virtual Object &getParam(int x);
    void *getMethodGrep(void);
    void *getMethodGrepString(void);
    void  setMethodGrep(void *method);
    void  setMethodGrepString(void *method);
    virtual void grepString(Table &table,String &out);
};
