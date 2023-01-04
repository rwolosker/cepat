/**********************************************************

**********************************************************/
class StringList:public Object{
  protected:
    void
      **list;
    int
      length;
    GcObject
      gco;

    void init(void);
    char compare(Sort &sort,void *a,void *b);
    char compareSearch(Sort &sort,void *a);
    int  searchImpl(void *p);
  public:
    StringList();
    StringList(const StringList &list);
    ~StringList();

    StringList &add(const String &value);
    StringList &debug(void);
    String      join(cchar *de);
    int         len(void);
    String     &get(int x);
    StringList &remove(int x);
    StringList &reset(void);
    int         search(...);
    StringList &sort(void);
    StringList &unique(void);

    StringList &operator=(const StringList &list);
    String     &operator[](int x);
};
