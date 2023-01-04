/*=========================================================

=========================================================*/
TableSelect::TableSelect(){
}
TableSelect::~TableSelect(){
}
/*=========================================================

=========================================================*/
void TableSelect::select(Object *src,Object *dst,...){
  Table
    &tsrc=*(Table*)src,
    &tdst=*(Table*)dst;
  int
    cols=tsrc.getCols();
  Field
    **field=(Field**)cMemAlloc(cols*sizeof(void*));
  void
    **pp=(void**)&dst;
  Table
    table;
  TableDef
    def;

  for(int i=1;pp[i];i++){
    String
      &name=*(String*)pp[i];
    Field
      &f=tsrc.getDef().get(name);

    if(!f.getName().len()){
      cPrintf("ERRO: TableSelect.select [%s].\n",name.ptr());
      exit(0);
    }
    field[i-1]=&f;
    def.add(
      f.getName(),
      f.getType(),
      f.getSize()
    );
  }

  table=def.create();

  tsrc.top();
  while(tsrc.fetch()){
    table.addNew();
    for(int i=0,pos=0;i<cols;i++){
      cMemCpy(
        tsrc.getStream().ptr()+tsrc.getStream().position()+field[i]->getStart(),
        field[i]->getSize(),
        table.getStream().ptr()+table.getStream().position()+pos);
      pos+=field[i]->getSize();
    }
  }
  tdst=table;
  cMemFree(field);
}
