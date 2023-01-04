/**********************************************************

**********************************************************/
#define SQLITE_OK          (0)
#define SQLITE_ROW         (100)
#define SQLITE_DONE        (101)
#define SQLITE_UTF8        (1)
#define SQLITE_NULL        (5)
#define SQLITE_TEXT        (3)
#define SQLITE_INTEGER     (1)
#define SQLITE_DOUBLE      (2)
#define SQLITE_OPEN_MEMORY (0x00000080)
#define SQLITE_RES_NUMBER  (1)
#define SQLITE_RES_TABLE   (2)

typedef struct PACK SQLITE_API{
    void
      *lib;
    int
      (*sqlite3_open)(cchar*,void**),
      (*sqlite3_close)(void*),
      (*sqlite3_prepare)(void*,cchar*,int,void**,cchar**),
      (*sqlite3_prepare_v2)(void*,cchar*,int,void**,char**),
      (*sqlite3_finalize)(void*),
      (*sqlite3_step)(void*),
      (*sqlite3_bind_parameter_count)(void*),
      (*sqlite3_bind_int)(void*,int,int),
      (*sqlite3_bind_double)(void*,int,double),
      (*sqlite3_bind_text)(void*,int,cchar*,int,void*),
      (*sqlite3_column_type)(void*,int),
      (*sqlite3_column_count)(void*),
      (*sqlite3_reset)(void*),
      (*sqlite3_exec)(void*,cchar*,void*,void*,char**),
      (*sqlite3_limit)(void*,int,int),
      (*sqlite3_open_v2)(cchar*,void**,int,cchar*),
      (*sqlite3_column_int)(void*,int),
      (*sqlite3_changes)(void*),
      (*sqlite3_declare_vtab)(void*,cchar*),
      (*sqlite3_create_module)(void*,char*,void*,void*),
      (*sqlite3_create_function)(void*,cchar*,int,int,void*,void*,void*,void*),
      (*sqlite3_value_int)(void*);
    void
      (*sqlite3_free)(void*),
      (*sqlite3_result_double)(void*,double),
      (*sqlite3_result_int)(void*,int),
      (*sqlite3_result_text)(void*,void*,int,void*),
     *(*sqlite3_user_data)(void*);
    char
      *(*sqlite3_errmsg)(void*),
      *(*sqlite3_column_text)(void*,int),
      *(*sqlite3_column_decltype)(void*,int),
      *(*sqlite3_column_name)(void*,int),
      *(*sqlite3_libversion)(void),
      (*sqlite3_value_text)(void*);
    double
      (*sqlite3_column_double)(void*,int),
      (*sqlite3_value_double)(void*);
}SQLITE_API;

typedef struct SQLITE_MODULE{
  int
    iVersion,
    (*xCreate)(void*,void*,int,char**,void**,char**),
    (*xConnect)(void*,void*,int,char**,void**,char**),
    (*xBestIndex)(void*,void*),
    (*xDisconnect)(void*),
    (*xDestroy)(void*),
    (*xOpen)(void*,void**),
    (*xClose)(void*),
    (*xFilter)(void*,int,char*,int,char**),
    (*xNext)(void*),
    (*xEof)(void*),
    (*xColumn)(void*,void*,int),
    (*xRowid)(void*,int64*),
    (*xUpdate)(void*,int,char**,int64*),
    (*xBegin)(void*),
    (*xSync)(void*),
    (*xCommit)(void*),
    (*xRollback)(void*),
    (*xFindFunction)(void*,int,cchar*,void*,void**),
    (*xRename)(void*,cchar*),
    (*xSavepoint)(void*,int),
    (*xRelease)(void*,int),
    (*xRollbackTo)(void*,int);
}SQLITE_MODULE;

typedef struct SQLITE_VTAB{
  SQLITE_MODULE *module;
  int nRef;
  char *zErrMsg;
  char *file;
  Table *table;
}SQLITE_VTAB;

typedef struct SQLITE_CURSOR{
  SQLITE_VTAB *vtab;
  Table *table;
}SQLITE_CURSOR;

typedef struct SQLITE_INFO{
  /* Inputs */
  int nConstraint;           /* Number of entries in aConstraint */
  struct sqlite3_index_constraint {
     int iColumn;              /* Column on left-hand side of constraint */
     unsigned char op;         /* Constraint operator */
     unsigned char usable;     /* True if this constraint is usable */
     int iTermOffset;          /* Used internally - xBestIndex should ignore */
  } *aConstraint;            /* Table of WHERE clause constraints */
  int nOrderBy;              /* Number of terms in the ORDER BY clause */
  struct sqlite3_index_orderby {
     int iColumn;              /* Column number */
     unsigned char desc;       /* True for DESC.  False for ASC. */
  } *aOrderBy;               /* The ORDER BY clause */
  /* Outputs */
  struct sqlite3_index_constraint_usage {
    int argvIndex;           /* if >0, constraint is part of argv to xFilter */
    unsigned char omit;      /* Do not code a test for this constraint */
  } *aConstraintUsage;
  int idxNum;                /* Number used to identify the index */
  char *idxStr;              /* String, possibly obtained from sqlite3_malloc */
  int needToFreeIdxStr;      /* Free idxStr using sqlite3_free() if true */
  int orderByConsumed;       /* True if output is already ordered */
  double estimatedCost;      /* Estimated cost of using this index */
}SQLITE_INFO;


class SqLite;
typedef struct SQLITE_AUX{
  SqLite
    *sqLite;
  Map
    *map;
  void
    *handle,
    *aux0;
}SQLITE_AUX;

extern SQLITE_API
  cSqliteApi;
extern char
  cSqliteApiStart;

int  cSqliteClose(void *cursor);
int  cSqliteColumn(void *cursor,void *ctx,int col);
int  cSqliteConnect(void *db,void *aux,int argc,char **argv,void **vtab,char **erro);
int  cSqliteCreate(void *db,void *aux,int argc,char **argv,void **vtab,char **erro);
int  cSqliteDisconnect(void *vtab);
int  cSqliteEof(void *cursor);
int  cSqliteFilter(void *cursor,int x,char *xstr,int argc,char **argv);
int  cSqliteIndex(void *vtab,void *info);
int  cSqliteNext(void *cursor);
int  cSqliteOpen(void *vtab,void **cursor);
int  cSqliteRowId(void *cursor,int64 *row);
void cSqliteFunctionSoma(void *ctx,int argc,void **argv);
void cSqliteModuleInit(SQLITE_MODULE &module);

#include "SqLiteItem.h"
#include "SqLiteResult.h"

class SqLite:public Object{
  protected:
    GcObject
      gco;
    String
      fileName,
      lastErro;
    char
      createBreakLine;
    void
      *handle,
      *stm;

    void initSqLite(void);
    void toSqlImpl(char drop,char create,Table &table,ByteArray &buffer);
    void toSqlDrop(char drop,Table &table,ByteArray &buffer);
    void toSqlCreate(char create,Table &table,ByteArray &buffer);
    void toSqlInsert(Table &table,ByteArray &buffer);
    void stmToTable(void *stm,int cols,Table &table);
  public:
    SqLite();
    ~SqLite();

    SqLite &close(void);
    char    init(cchar *lib);
    char    open(cchar *file,char memory=0);
    char    query(cchar *sql,SqLiteResult &result);
    SqLite &putInt(int col,int value);
    SqLite &putDbl(int col,double value);
    SqLite &putTxt(int col,const String &value);
    SqLite &resetStm(void);
    SqLite &toSql(int drop,int create,ByteArray &buffer,...);
    SqLite &shell(ByteArray &buffer,String &erro,cchar *ld="ld-linux.so.2",cchar *lib="lib",cchar *cmd="sqlite3 -batch -bail");
    SqLite &setCreateBreakLine(char createBreakLine);
    String  getError(void);
    SqLite &createModule(cchar *name,SQLITE_MODULE &module,SQLITE_AUX &aux);
    SqLite &createFunction(cchar *name,int count,void *pf,SQLITE_AUX &aux);
};

#include "SqLite.cpp"
