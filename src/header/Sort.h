/**********************************************************

**********************************************************/
class Sort:public Object{
  protected:
    Object
      *arg,//objeto adicional. Table por exemplo.
      *obj;//bean dono do metodo.
    void
      *param,
      *methodCompare,
      *methodSearch,
      *methodUnique;

    void initSort();
    int  searchImpl(void *buffer,int regsize,int low,int hig);
    void sortImpl(void *buffer,int regsize,int low,int hig);
    void sortSwap(void *a,void *b,void *swap,int regsize);
  public:
    Sort();
    virtual ~Sort();
    virtual char compare(Sort &sort,void *a,void *b);
    Sort &setObj(Object *obj);
    Sort &setArg(Object *arg);
    Object *getArg(void);
    Sort &setCompare(void *method);
    Sort &setSearch(void *method);
    Sort &setParam(void *param);
    Sort &setUnique(void *param);
    Sort *instance(void);
    virtual Object &getParam(int x);
    String &getParamString(int x);
    Number &getParamNumber(int x);
    virtual void sortConfigure(void);
    virtual void searchConfigure(void);
    virtual void sort(void *buffer,int regsize,int low,int hig);
    int  search(void *buffer,int regsize,int low,int hig);
    Object *getObj(void);
    void   *getMethodCompare(void);
    char unique(void);
    virtual char uniqueCompare(Sort &sort);
};
