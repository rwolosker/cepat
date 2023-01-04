/**********************************************************

**********************************************************/
class TableJoin:public Object{
  public:
    TableJoin();
    ~TableJoin();
    Table join(Object *src,...);
};
