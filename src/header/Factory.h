/**********************************************************

**********************************************************/
class Factory:public Object{
  protected:
    Map
      *map;
    Reflection
      *reflection;
    
    void init(void);
    void injectMap(Object *obj);
    void injectFactory(Object *obj);
    void injectConfigure(Object *obj);
    void injectReflection(Object *obj);

  public:
    Factory();
    ~Factory();

    Factory    &alias(cchar *name,cchar *alias);
    Map        &getMap(void);
    void       *getMethod(cchar *name);
    Reflection &getReflection(void);
    Object     *getBean(cchar *name,cchar *clazz);
    Object     *getBean(cchar *name,cchar *clazz,void *pobj);
    void        configure(Map &map,Reflection &reflection);
    Factory    &inject(Object *obj);
};
