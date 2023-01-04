/**********************************************************

**********************************************************/
class TdsPacketExecute:public Object{
  protected:
    Gc
      gc;
    ByteArray
      action;

    void actionReset(void);
    void pkHexDump(ByteArray &buffer,int size,int skip=0);
    void pkAction(int type,void **arg);
    void pkAA(ByteArray &buffer);
    void pkAB(ByteArray &buffer);
    void pkAD(ByteArray &buffer);
    void pkD1(ByteArray &buffer);
    void pkFD(ByteArray &buffer);
    void pkE3(ByteArray &buffer);
    void pk81(ByteArray &buffer);
    void pkA9(ByteArray &buffer);
  public:
    TdsPacketExecute();
    ~TdsPacketExecute();
    TdsPacketExecute &actionAdd(uchar type,Object *bean);
    TdsPacketExecute &exec(ByteArray &buffer);
};
