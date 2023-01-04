/**********************************************************

**********************************************************/
class MySend:public Object{
  public:
    MySend();
    ~MySend();
    char receive(Socket32 &socket,MyByteArray &buffer,int *rlen=0);
    char send(Socket32 &socket,MyByteArray &buffer);
};
