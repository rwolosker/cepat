/**********************************************************

**********************************************************/
class TableSelect:public Object{
  public:
    TableSelect();
    ~TableSelect();
    void select(Object *src,Object *dst,...);
};
