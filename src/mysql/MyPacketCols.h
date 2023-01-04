/**********************************************************

**********************************************************/
class MySql;
class MyPacketCols:public MyAction{
  protected:
    int
      cols;
    MySend
      *send;
    MyPacketExecute
      *exe;
    Socket32
      *socket;
    Table
      table;
    MySql
      *my;

    void initMyCols(void);
    void readCols(MyByteArray &buffer);
    void readCol(MyByteArray &buffer,TableDef &def);
  public:
    MyPacketCols();
    ~MyPacketCols();
    void configure(MySend &send,MyPacketExecute &exe,Socket32 &socket);
    int getCols(void);
    char action(uchar type,void **argv);
    void reset(void);
    char onError(MyByteArray &buffer);
    void setMySql(MySql &my);
};
