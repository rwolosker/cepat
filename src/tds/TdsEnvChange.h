/**********************************************************

**********************************************************/
class TdsEnvChange:public TdsAction{
  protected:
    int
      packetSize;

    void initTdsEnvChange(void);
  public:
    TdsEnvChange();
    ~TdsEnvChange();
    void reset(void);
    char action(uchar type,void **arg);
    int getPacketSize(void);
};
