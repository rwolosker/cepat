/**********************************************************

**********************************************************/
typedef struct PACK REFLECTION_TABLE{
  char
    name[128],
    address[128];
}REFLECTION_TABLE;
char cReflectionDat(Table &table,char reload=0);
char cReflectionDatFind(void *arg,void *a);
class ReflectionTable:public Reflection{
  protected:
    Table
      table;
    char
      reload;
    GcObject
      gco;

    void initReflectionTable(void);
    REFLECTION_TABLE *getName(cchar *name);
  public:
    ReflectionTable();
    ~ReflectionTable();
    ReflectionTable &setReload(char reload);
    void fileMap(cchar *file);
    void main(int argc,char** argv);
    void *methodAddress(cchar *name);
    String className(Object *obj);
    Map &getMap(void);


};
