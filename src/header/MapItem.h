/**********************************************************

**********************************************************/
class MapItem:public Object{
  protected:
    String
      name,
      strValue;
    Number
      numValue;
    char
      type,
      nulo,
      free,
      protect;
    int
      pos;
    NODE
      *node;
    void
      *ffree;

    void init(char keep=0);
  public:
    MapItem();
    ~MapItem();
    MapItem *instance(void);
    void     freePtr(void);

    void    *getFfree(void);
    char     getFree(void);
    String  &getName(void);
    NODE    *getNode(void);
    int      getPos(void);
    int      getType(void);

    void     setFfree(void *ffree);
    void     setFree(char free);
    void     setName(const String &name);
    void     setNull(char nulo);
    void     setNode(NODE *node);
    void     setPos(int pos);
    void     setType(char type);
    void     setProtect(char protect);

    void     setStrValue(const String &strValue);
    void     setNumValue(const Number &numValue);
    void     reset(char keepNode=0);

    String   asStr(void);
    int      asInt(void);
    Number   asNumber(void);
    void    *asPtr(void);
    Object  *asObj(void);
    void     pObj(void *pobj);

    char     isNull(void);
    char     isProtect(void);
    void     debug(void);
};
