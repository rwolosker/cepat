/**********************************************************

**********************************************************/
class MyPacketError:public MyAction{
  protected:
    int
      code;
    String
      state,
      msg;

    void initMyPacketError(void);
  public:
    MyPacketError();
    ~MyPacketError();
    int getCode(void);
    String getState(void);
    String getMsg(void);
    char action(uchar type,void **argv);
    void reset(void);
};
