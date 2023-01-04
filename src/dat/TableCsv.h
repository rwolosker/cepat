/**********************************************************

**********************************************************/
class TableCsv:public Object{
  public:
    TableCsv();
    ~TableCsv();
    String toCsv(Table &table);
};
