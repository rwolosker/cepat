TableStruct::TableStruct(){
}
TableStruct::~TableStruct(){
}
/*=========================================================

=========================================================*/
TableStruct &TableStruct::source(Table &table,ByteArray &out){
  ByteArray
    tmp;
  String
    nome=table.getName(),
    rename;
  Type
    type;

  if(!nome.len())
    return *this;
  for(int i=0;i<nome.len();i++){
    uchar
      c=nome[i];

    if(c>='A'&&c<='Z')
      rename.append('_');
    rename.append((char)c);
  }
  rename.append("_DAT");
  tmp.writef("typedef struct PACK %s{\n",rename.uCase().ptr());
  for(int i=0;i<table.getDef().len();i++){
    Field
      &f=table.getDef().get(i);

    switch(f.getType()){
      case C_TEXT:
        tmp.writef("  char %s[%d];\n",f.getName().ptr(),f.getSize());
        break;
      case C_BOOL:
        tmp.writef("  char %s;\n",f.getName().ptr());
        break;
      case C_INT:
      case C_DATE:
        tmp.writef("  int %s;\n",f.getName().ptr());
        break;
      case C_DOUBLE:
      case C_DATETIME:
        tmp.writef("  double %s;\n",f.getName().ptr());
        break;
      default:
        cPrintf("ERRO: cDatGenStruct. [%X].",f.getType());
        exit(1);
    }
  }
  tmp.writef("}%s;\n",rename.uCase().ptr());
  out.append(tmp);
  return *this;
}
/*=========================================================

=========================================================*/
TableStruct &TableStruct::stream(Table &table,void *pout,char readOnly){
  void
    **out=(void**)pout;

  if(!readOnly)
    *out=table.getStream().ptr();
  else{
    int
      size=table.len()*table.getRegSize();

    cMemCpy(
      table.getStream().ptr(),
      size,
      gc.add(*out=cMemAlloc(size))
    );
  }
  return *this;
}
/*=========================================================

=========================================================*/
TableStruct &TableStruct::replace(void *buffer,Table &table,char free){
  char
    same=buffer==table.getStream().ptr();

  if(!same){
    class Row:public Table{
      public:
      void _update(Table &table){
        char
          *pr=PCHAR(&rows),
          *p0=PCHAR(this),
          *pt=PCHAR(&table);

        *(int*)&pt[pr-p0]=table.getStream().len()/table.getRegSize();
        table.top();
      }
    };

    ByteArray
      &p=table.getStream();

    p.reset();
    if(buffer){
      p.write(buffer,cMemSize(buffer));
      if(free&&!gc.contains(buffer))
        cMemFree(buffer);
    }
    Row()._update(table);
  }
  return *this;
}
/*=========================================================

=========================================================*/
TableStruct &TableStruct::qsort(Table &table,void *pfcompare){
  ::qsort(
    table.getStream().ptr(),
    table.len(),
    table.getRegSize(),
    pfcompare
  );
  return *this;
}
/*=========================================================

=========================================================*/
int TableStruct::find(Table &table,void *arg,void *pfcompare,char forAdd){
  int
    regSize=table.getRegSize(),
    m,
    low=0,
    hig=table.len()-1;
  char
    *buffer=PCHAR(table.getStream().ptr()),
    (*pf)(void*,void*)=0,
    c;

  *(void**)&pf=pfcompare;
  if(!table.len())
    return -1;
  INICIO:
  if(forAdd&&hig<=low)
    return
      1==pf(
        arg,
        table.getStream().ptr()+low*table.getRegSize())
      ?low+1
      :low;
  if(!buffer||hig<low||low>hig)
    return -1;
  m=(low+hig)/2;
  c=(pf)(arg,&buffer[m*regSize]);
  if(c==0){
    do
      m-=1;
    while(m>=0&&0==(pf)(arg,&buffer[m*regSize]));
    return m+1;
  }
  if(c==1){
    low=m+1;
    goto INICIO;
  }
  if(c==-1){
    hig=m-1;
    goto INICIO;
  }
  return -1;
}
/*=========================================================

=========================================================*/
Table &TableStruct::addNewSort(Table &table,int recno){
  if(!FOUND(recno))
    return table.addNew();
  else{
    ByteArray
      update;
    int
      regSize=table.getRegSize(),
      len=table.len();

    if(!recno){
      update.resize(regSize).seek(regSize);
      update.write(table.getStream());
      goto NEXT;//BUG bytearray rev 3141. Quando len=0 nao aceita seek
    }
    update.write(
      table.getStream().ptr(),
      recno*regSize
    );
    update.seek(update.position()+regSize);
    update.write(
      table.getStream().ptr()+(recno*regSize),
      regSize*(len-recno)
    );
    NEXT:
    table.getStream()
      .reset()
      .write(update);

    return table.reconfigureRows().go(recno);
  }
}
