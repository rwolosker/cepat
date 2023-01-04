/**********************************************************

**********************************************************/
class SqLiteResult:public Object{
  protected:
    GcObject
      gco;
  public:
    SqLiteResult();
    ~SqLiteResult();
    void reset(void);
    int  len(void);
    void add(Number &number);
    void add(Table &table);
    SqLiteItem &get(int x);
};
