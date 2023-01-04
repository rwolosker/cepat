/**********************************************************

**********************************************************/
class Map:public Object{
  protected:
    void
      **hash;
    int
      hashSize;
    GcObject
      gco;
    char
      protect;

    void   **getHash(void);
    int      getHashSize(void);
    MapItem *getItem(cchar *name,int key=-1,char create=0);
    NODE    *getRoot(int pos);
    void     init(void);
    int      keyGen(cchar *key);
    void     setRoot(NODE *node,int pos);
  public:
    Map();
    Map(const Map &map);
    ~Map();

    Map       &configure(int hashSize=2048);
    Map       &debug(void);
    void       forEach(Object *obj,void *method);
    MapItem   &get(cchar *name);
    Split      keys(void);
    Map       &remove(cchar *key);
    Map       &reset(void);
    Map       &loadIni(cchar *file);
    Map       &loadBuffer(cchar *buffer);
    Map       &loadArg(int argc,char **argv);
    Map       &setValue(cchar *key,cchar *value);
    Map       &setValue(cchar *key,int value);
    Map       &setValue(cchar *key,Object *value,char free=0,void *pfree=0);
    Map       &setValue(cchar *key,void *value,char free=0,void *pfree=0);
    Map       &operator=(const Map &map);
    Map       &protectMode(char protect);
    Map       &protectAll(void);
    char       isEmpty(void);

    Map *instance(void);
};
#define MAPREF(clazz,var,map,key)\
  clazz &var=*(clazz*)map.get(key).asObj();
