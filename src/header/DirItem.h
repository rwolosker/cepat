/**********************************************************

**********************************************************/
class DirItem:public Object{
  protected:
    void initDirItem(void);
  public:
    char
      type;
    String
      name,
      path;

    DirItem();
    ~DirItem();

    String   getAbsolutePath(void);
    DirItem &instance(void);
};
