/**********************************************************

**********************************************************/
class TableSort:public Sort{
  protected:
    int
      col;

    void  initTableSort(void);
    void *methodCompareResolve(void);
    void *methodSearchResolve(void);
  public:
    TableSort();
    virtual ~TableSort();

    void            colByName(cchar *name,int *col);
    virtual char    compareText(Sort &sort,void *a,void *b);
    virtual char    compareTextSearch(Sort &sort,void *a);
    virtual char    compareDouble(Sort &sort,void *a,void *b);
    virtual char    compareDoubleSearch(Sort &sort,void *a);
    int             getCol(void);
    Field          &getField(void *reg,int col);
    Field          &getField(void *reg,cchar *col);
    virtual Object &getParam(int x);
    Table          &getTable(void);
    TableSort      &getTableSort(Sort &sort);
    int             recNo(void *reg);
    void            setCol(int col);
    virtual void    sortConfigure(void);
    virtual void    searchConfigure(void);
};
