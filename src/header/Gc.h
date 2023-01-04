/**********************************************************

**********************************************************/
class Gc:public Object{
  protected:
    void
      **list;
    int
      length,realSize;
    char
      otimized;
    
    void  init(void);
    virtual void *free(void *ptr);
  public:
    Gc();
    Gc(const Gc &gc);
    virtual ~Gc();
    virtual void *add(void *ptr);
    virtual void remove(void *ptr);
    void  reset(void);
    void debug(void);
    int len();
    void *get(int x);
    Gc &otimize(char otimized=0);
    Gc &operator=(const Gc &gc);
    char contains(void *ptr);
    int indexOf(void *ptr);
};
