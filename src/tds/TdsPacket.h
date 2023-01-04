/**********************************************************

**********************************************************/
typedef struct PACK TDS_PACKET{
  char  type;
  char  last;
  short size;
  int   unknown;
}TDS_PACKET;

class TdsPacket:public Object{
  protected:
    int
      packetSize;

    void initTdsPacket(void);
  public:
    TdsPacket();
    ~TdsPacket();

    TdsPacket &setPacketSize(int packetSize);
    void create(char type,ByteArray &buffer);
};
