/*=========================================================

=========================================================*/
Dir::Dir(){

}
Dir::~Dir(){
}
/*=========================================================

=========================================================*/
int Dir::len(void){
  return gco.len();
}
/*=========================================================

=========================================================*/
Dir &Dir::open(cchar *name){
  String
    path=String(name).trim();
  void
    *dir=opendir(name);

  if(path[path.len()-1]=='/')
    path[path.len()-1]=0;
  gco.reset();
  if(!dir)
    goto FIM;
  else{
    uchar
      *entry=PUCHAR(cMemAlloc(1024)),
      *pe=0;

    while(!readdir_r(dir,entry,(void**)&pe)&&pe){
      char
        type=pe[10];

      if(type==DT_DIR||type==DT_REG){
        DirItem
          &item=DirItem().instance();

        item.path=path;
        item.name=PCHAR(&pe[11]);
        item.type=type==DT_DIR?C_DIR:C_FILE;


        gco.add(&item);
      }
    }
    cMemFree(entry);
  }
  FIM:
    if(dir)
      closedir(dir);
  return *this;
}
/*=========================================================

=========================================================*/
DirItem &Dir::item(int x){
  return x>=0&&x<len()
    ?*(DirItem*)gco.get(x)
    :*(DirItem*)gc.add(&DirItem().instance());
}
