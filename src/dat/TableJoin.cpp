/*=========================================================

=========================================================*/
TableJoin::TableJoin(){
}
TableJoin::~TableJoin(){
}
/*=========================================================

=========================================================*/
Table TableJoin::join(Object *src,...){
  void
    **p=(void**)&src;
  TableDef
    def;

  for(int i=0;p[i];i++){
    TableRef
      table=*(Table*)p[i];

    for(int i=0;i<table.getDef().len();i++){
      Field
        &field=table.getDef().get(i);

      def.add(
        table.getName().append('.').append(field.getName()),
        field.getType(),
        field.getSize()
      );
    }
  }
  return def;
}
