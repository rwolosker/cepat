/**********************************************************

**********************************************************/
class Split:public Object{
  protected:
    int
      length,
      minSize;
    char
      **var;

    void initSplit(void);
    int  searchImpl(void *p);
    void reorg(void);
  public:
    Split();
    Split(int minSize);
    Split(cchar *value,char de);
    Split(cchar *value,cchar *de);
    Split(const Split &value);
    ~Split();
    Split  &add(cchar *value);
    Split  &debug(char dump=0);
    String  get(int x);
    Split  *instance(void);
    String  join(const String &de);
    String  last(void);
    int     len(void);
    String  operator[](int x);
    Split  &operator=(const Split &value);
    char  **ptr();
    Split  &reset(void);
    int     search(...);
    char    contains(const String &value);
    Split  &setMinSize(int minSize);
    Split  &sort(void);
    Split  &sortCustom(Object *obj,void *method);
    Split  &split(cchar *value,cchar *de);
    Split  &split(cchar *value,char de);
    Split  &unique(void);
    Split  mid(int start,int size=0);
};
typedef Split& SplitRef;
