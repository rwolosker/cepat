/**********************************************************

**********************************************************/
class TdsLoginAcKnow:public TdsAction{
  protected:
    int
      version;

    void initTdsLoginAcKnow(void);
  public:
    TdsLoginAcKnow();
    ~TdsLoginAcKnow();
    void reset(void);
    char action(uchar type,void **arg);
    int getVersion(void);
};
