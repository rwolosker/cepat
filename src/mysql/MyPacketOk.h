/**********************************************************

**********************************************************/
class MyPacketOk:public MyAction{
  protected:
    int
      rowsAffected,
      lastInsertId;
    char
      moreResult;

    void initMyPacketOk(void);
  public:
    MyPacketOk();
    ~MyPacketOk();
    int  getRowsAffected(void);
    int  getLastInsertId(void);
    char getMoreResult(void);
    char action(uchar type,void **argv);
    void reset(void);
};
