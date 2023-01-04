/**********************************************************

**********************************************************/
class TdsError:public TdsAction{
  protected:
    int
      msgNo,
      line;
    String
      msg;

    void initTdsError(void);
  public:
    TdsError();
    ~TdsError();
    void reset(void);
    char action(uchar type,void **arg);
    int getMsgNo(void);
    int getLine(void);
    String getMsg(void);

};
