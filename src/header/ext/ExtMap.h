/**********************************************************

**********************************************************/
class ExtMap{
  protected:
    Map
      *map;
    void initExpMap(void);
  public:
    ExtMap();
    virtual ~ExtMap();
    Map &getMap();
    void setMap(Map &map);
};
