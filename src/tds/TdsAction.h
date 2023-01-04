/**********************************************************

**********************************************************/
class TdsAction:public Object{
  public:
    TdsAction();
    ~TdsAction();
    virtual char action(uchar type,void **argv)=0;
    virtual void reset(void)=0;
};
