/**********************************************************

**********************************************************/
class Reflection:public Object{
  protected:
    Map
      map;
    Map
      mSize;
  public:
    Reflection();
    ~Reflection();
    virtual String  className(Object *obj);
    virtual void    debugMap(void);
    virtual void    fileMap(cchar *file);
    virtual void   *free(Object *obj);
    virtual Map    &getMap(void);
    virtual Object *instance(cchar *clazz);
    virtual void    main(int argc,char **argv);
    virtual void   *methodAddress(cchar *name);
    virtual void   *vtable(cchar *name);
    virtual int     sizeOf(cchar *clazz);
};
