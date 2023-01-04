/**********************************************************

**********************************************************/
class ForwardSession:public Object{
  protected:
    SK32
      *src,
      *dst;
    int
      srcPort,
      dstPort;
    GcObject
      *session;

    void initForwardSession(void);
  public:
    ForwardSession();
    ~ForwardSession();


};


class Forward:public Object{
  protected:
    int
      localPort,
      remotePort;
    String
      remoteHost;
    GcObject
      session;
    Gc
      gc;
    SK32
      *sk;

    void initForward(void);
  public:
    Forward();
    ~Forward();

    int      getLocalPort(void);
    String   getRemoteHost(void);
    int      getRemotePort(void);
    Forward &setLocalPort(int localPort);
    Forward &setRemoteHost(const String &remoteHost);
    Forward &setRemotePort(int remotePort);
    void     main(void);
};

