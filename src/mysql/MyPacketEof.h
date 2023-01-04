/**********************************************************

**********************************************************/
class MyPacketEof:public MyAction{
  protected:
    char
      moreResult;

    void initMyPacketEof(void);
  public:
    MyPacketEof();
    ~MyPacketEof();
    char getMoreResult(void);
    char action(uchar type,void **argv);
    void reset(void);
};
