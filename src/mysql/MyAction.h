/**********************************************************

**********************************************************/
class MyAction:public Object{
  public:
    MyAction(){}
    ~MyAction(){}
    virtual char action(uchar type,void **argv)=0;
    virtual void reset(void)=0;
    virtual char onError(MyByteArray &buffer){return 1;}
};
