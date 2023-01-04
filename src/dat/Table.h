/**********************************************************

**********************************************************/
class Table:public Object{
  protected:
    int
      regSize,
      rows,
      bookMark;
    TableDef
      def;
    ByteArray
      stream;
    String
      name,
      fileName;

    Table &addRegImpl(void *p);
    Table &colByNameImpl(void *p);
    Table &grepImpl(void *p);
    void   initTable(void);
    String resolveName(const String &value);
    void   descritor(void);
    Table &updateRegImpl(void *p);
  public:
    Table();
    Table(const TableDef &def);
    Table(const Table &def);
    ~Table();

    Table     &addNew(void);
    Table     &addReg(...);
    Table     &debug(char dump=0);
    Table     &colByName(...);
    Table      copy(void);
    char       eof(void);
    char       fetch(void);
    int        find(int col,...);
    int        find(cchar *name,...);
    int        findEx(Object &obj,void *method,...);
    Table     &fromStream(ByteArray &buffer,int *erro=0);
    int        getCols(void);
    TableDef  &getDef(void);
    String     getFileName(void);
    String     getName(void);
    ByteArray &getStream(void);
    int        getRegSize(void);
    int        getRows(void);
    Table     &go(int recno);
    Table     &grep(...);
    Table     &last(void);
    int        len(void);
    int        recNo(void);
    Table     &reconfigureRows(void);
    Table     &remove(int recno);
    Table     &removeAll(int keepRecno=-1);
    Table     &reset(void);
    Table     &setName(cchar *name);
    Table     &setFileName(cchar *fileName);
    Table     &sort(int col);
    Table     &sort(cchar *name);
    Table     &sort(Object *obj,void *method);
    Table     &top(void);
    Table     &toStream(ByteArray &buffer);
    Table     &truncate(void);
    char       unique(Table &table,Object *pkey,...);
    Table     &updateReg(...);


    int     getInt(int col);
    int     getInt(cchar *name);
    double  getDbl(int col);
    double  getDbl(cchar *name);
    cchar  *getTxt(int col);
    cchar  *getTxt(cchar *name);
    char    getChr(int col);
    char    getChr(cchar *name);
    char    getBln(int col);
    char    getBln(cchar *name);
    Date    getDte(int col);
    Date    getDte(cchar *name);
    String  getStr(int col);
    String  getStr(cchar *name);
    Number  getNum(int col);
    Number  getNum(cchar *name);

    Table  &putInt(int col,int value);
    Table  &putInt(cchar *name,int value);
    Table  &putDbl(int col,double value);
    Table  &putDbl(cchar *name,double value);
    Table  &putTxt(int col,cchar *value);
    Table  &putTxt(cchar *name,cchar *value);
    Table  &putChr(int col,char value);
    Table  &putChr(cchar *name,char value);
    Table  &putBln(int col,char value);
    Table  &putBln(cchar *name,char value);
    Table  &putDte(int col,const Date &value);
    Table  &putDte(cchar *name,const Date &value);
    Table  &putStr(int col,const String &value);
    Table  &putStr(cchar *name,const String &value);
    Table  &putNum(int col,const Number &value);
    Table  &putNum(cchar *name,const Number&value);

    Table &operator=(const TableDef &def);
    Table &operator=(const Table &def);

    Table &open(File &f,int *erro=0);
    Table &save(File &f,int *erro=0);
    Table &save(int *erro=0);

    Table *instance(void);
};
