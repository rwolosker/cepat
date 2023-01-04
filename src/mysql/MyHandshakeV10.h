/**********************************************************

**********************************************************/
class MyHandshakeV10:public MyAction{
  protected:
    ByteArray
      part1,
      part2;

    void initMyHandshakeV10(void);
  public:
    MyHandshakeV10();
    ~MyHandshakeV10();
    ByteArray getPart1(void);
    ByteArray getPart2(void);
    void setPart1(ByteArray &part1);
    void setPart2(ByteArray &part2);
    char action(uchar type,void **argv);
    void reset(void);
};
