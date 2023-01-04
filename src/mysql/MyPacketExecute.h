/**********************************************************

**********************************************************/
class MyPacketExecute:public Object{
  protected:
    Gc
      gc;
    ByteArray
      action;
    MyAction
      *onError;


    void actionReset(void);
    void initMyPacketExecute(void);
    void pkHexDump(ByteArray &buffer,int size,int skip=0);
    void pkAction(int type,void **arg);
    void pk00(MyByteArray &buffer);
    void pk0A(MyByteArray &buffer);
    void pkFE(MyByteArray &buffer);
    void pkFF(MyByteArray &buffer);
  public:
    MyPacketExecute();
    ~MyPacketExecute();
    MyPacketExecute &setOnError(MyAction *onError);
    MyPacketExecute &actionAdd(uchar type,Object *bean);
    MyPacketExecute &exec(MyByteArray &buffer);
};
