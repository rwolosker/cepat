/**********************************************************

**********************************************************/
class TableDef:public Object{
  protected:
    GcObject
      gco,
      fields;
    Map
      map;

    void create(cchar *name,char type,int size);
  public:
    TableDef();
    TableDef(const TableDef &def);
    ~TableDef();
    TableDef &add(cchar *name,char type,int size=0);
    TableDef &add(cchar *name,cchar *type,int size=0);
    TableDef &add(Field &field);
    TableDef  copy(void);
    TableDef &debug(void);
    Field    &get(int x);
    Field    &get(cchar *name);
    int       len(void);
    TableDef &remove(int x);
    TableDef &remove(cchar *name);
    TableDef &rename(cchar *name,cchar *to);
    TableDef &reset(void);
    String    toDef(void);
    TableDef &fromDef(const String &value);
    Table     create(void);

    TableDef &operator=(const TableDef &def);
};
