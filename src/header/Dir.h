/**********************************************************

**********************************************************/
class Dir:public Object{
  protected:
    GcObject
      gco,
      gc;
  public:
    Dir();
    ~Dir();

    DirItem &item(int x);
    int      len(void);
    Dir     &open(cchar *name);
};
