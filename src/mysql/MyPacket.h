/**********************************************************

**********************************************************/
class MyPacket:public Object{
  protected:
    int
      sequence;

    void initMyPacke(void);
  public:
    MyPacket();
    ~MyPacket();
    void sequencePlus(void);
    MyPacket &setSequence(int sequence);
    void create(MyByteArray &buffer);
};
