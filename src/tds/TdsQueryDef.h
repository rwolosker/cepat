/**********************************************************

**********************************************************/
#define TDS_LOGIN_PACKET   0x10 //7.0
#define TDS_ERROR_PACKET   0xAA
#define TDS_QUERY_PACKET   0x01
#define TDS_RESULT_DONE    0xFD
#define TDS_ENVIRONMENT    0xE3
#define TDS_RESULT         0x81
#define TDS_ROW            0xD1
#define TDS_ORDERBY        0xA9
#define TDS_SYBBIT         0x32
#define TDS_SYBINT1        0x30
#define TDS_SYBINT2        0x34
#define TDS_BIT            0x68
#define TDS_SYBINT4        0x38
#define TDS_INT            0x26
#define TDS_SMALLINT       TDS_INT
#define TDS_TINYINT        TDS_INT
#define TDS_BIGINT         TDS_INT
#define TDS_MONEY          0x6E
#define TDS_SMALLMONEY     TDS_MONEY
#define TDS_REAL           0x6D
#define TDS_FLOAT          TDS_REAL
#define TDS_NUMERIC        0x6C
#define TDS_DECIMAL        0x6A
#define TDS_SYBDATETIME    0x3D
#define TDS_DATE           0xE7
#define TDS_DATETIMEOFFSET TDS_DATE
#define TDS_DATETIME2      TDS_DATE
#define TDS_TIME           TDS_DATE
#define TDS_NVARCHAR       TDS_DATE //ta certo!!! 18/05/2021
#define TDS_DATETIME       0x6F
#define TDS_SMALLDATETIME  TDS_DATETIME
#define TDS_CHAR           0xAF
#define TDS_VARCHAR        0xA7
#define TDS_TEXT           0x23
#define TDS_NTEXT          0x63
#define TDS_NCHAR          0xEF

typedef struct PACK TDS_QUERY_DEF{
  String
    *name;
  int
    type,
    size,
    precicion,
    scale,
    cols;
}TDS_QUERY_DEF;


class TdsQueryDef:public TdsAction{
  protected:
    Table
      table;
    TableDef
      def;
    TDS_QUERY_DEF
      *tdsDef;
    int
      cols;
    GcObject
      gco;
    Gc
      gc;

    void   createTable(void);
    void   initTdsQueryDef(void);
    void   readCols(ByteArray &buffer);
    void   readRow(ByteArray &buffer);
    double tdsDateTime(int len,const ByteArray &buffer);

  public:
    TdsQueryDef();
    ~TdsQueryDef();
    void reset(void);
    char action(uchar type,void **arg);
    TableDef &getTableDef(void);
    Table     getTable(void);
    TDS_QUERY_DEF *getTdsDef(void);
};
