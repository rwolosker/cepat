/**********************************************************

**********************************************************/
typedef struct PACK REGEXP{
  short len;
  void *value;
  struct REGEXP *composition;
  char type;
  char res;
}REGEXP;

#define REG_EXP_OR  (0)
#define REG_EXP_AND (1)
#define REG_EXP_END (2)
#define REG_EXP_COM (3)

class RegExp:public Object{
  protected:
    REGEXP
      *regra;
    String
      source;

    void    initRegExp(void);
    void    dump(REGEXP *regra);
    REGEXP *free(REGEXP *regra);
    REGEXP *load(const String &value);
    void    matchComposite(REGEXP *regra,const String &value);
    char    matchProcess(REGEXP *regra);
  public:
    RegExp();
    ~RegExp();
    RegExp &create(const String &value);
    RegExp &debug(void);
    char    match(const String &value);
    RegExp &reset(void);
};
