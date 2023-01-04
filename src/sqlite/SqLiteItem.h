/**********************************************************

**********************************************************/
class SqLiteItem:public Object{
  protected:
    char
      type;
    Object
      *object;
    GcObject
      gco;
    void initSqLiteItem(void);
  public:
    SqLiteItem();
    ~SqLiteItem();
    SqLiteItem &set(char type,Object *object);
    Number &getNumber(void);
    Table &getTable(void);
    char getType(void);
    SqLiteItem *intance(void);
};
