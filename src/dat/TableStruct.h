/**********************************************************

**********************************************************/
class TableStruct:public Object{
  protected:
    Gc
      gc;
  public:
    TableStruct();
    ~TableStruct();
    TableStruct &source(Table &table,ByteArray &out);
    TableStruct &stream(Table &table,void *pout,char readOnly=1);
    TableStruct &replace(void *buffer,Table &table,char free=1);
    TableStruct &qsort(Table &table,void *pfcompare);
    int          find(Table &table,void *arg,void *pfcompare,char forAdd=0);
    Table       &addNewSort(Table &table,int recno);
};
