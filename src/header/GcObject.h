/**********************************************************

**********************************************************/
class GcObject:public Gc{
  protected:
    void *free(void *ptr);
  public:
    GcObject();
    virtual ~GcObject();
};
