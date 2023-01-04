/*=========================================================

=========================================================*/
void cAppMain(int argc,char **argv,Map &map){
  Reflection
    *reflection=new(cMemAlloc(sizeof(Reflection)))Reflection();
  Factory
    *factory=new(cMemAlloc(sizeof(Factory)))Factory();
  GcObject
    *gco=new(cMemAlloc(sizeof(GcObject)))GcObject();
  String
    main=String(argv[0]).append(".map");

  reflection->fileMap(main);
  reflection->main(argc, argv);
  factory->configure(map,*reflection);

  map.setValue("app.map",&map);
  map.setValue("app.factory",factory,1);
  map.setValue("app.reflection",reflection,1);
  map.setValue("app.gco",gco,1);
}
/*=========================================================

=========================================================*/
void cReflectionObjDump(cchar *file){
  File
    f;
  String
    linha,
    File;
  ByteArray
    buffer;
  Shell
    exe;
  Split
    list;

  File=String(file).mid(0,cStrLen(file)-4);

  f.temp();
  f.truncate();
  linha=String().sprintf(
    "objdump -tC %s | grep --color=never .text",
    File.ptr()
  );
  exe.exec(linha,f);
  f.readAll(buffer);
  while(buffer.readLine(linha)){
    Number
      hex=Number().fromHex(linha.mid(0,8));
    String
      txt;
    int
      x=(txt=linha.mid(45)).inStr('(');

    if(FOUND(x))
      txt[x]=0;

    list.add(String().sprintf("%s %X",txt.ptr(),hex.asPtr()));
  }

  f.truncate();
  linha=String().sprintf(
    "objdump -tC %s | grep --color=never vtable | grep --color=never .rodata",
    File.ptr()
  );
  exe.exec(linha,f);
  f.readAll(buffer);
  while(buffer.readLine(linha)){
    Number
      hex=Number().fromHex(linha.mid(0,8));
    String
      txt=linha.mid(58);

    list.add(String().sprintf("%X %s",hex.asPtr(),txt.ptr()));
    list.add(String().sprintf("%s %X",txt.ptr(),hex.asPtr()));
  }
  list.sort().unique();
  buffer.reset();
  buffer.write("#passed true\n");
  for(int i=0;i<list.len();i++)
    buffer.writef("%s\n",list[i].ptr());

  f.open(file);
  f.truncate();
  f.write(buffer);
}
/*=========================================================

=========================================================*/
void cReflectionMap(cchar *file){
  File
    f;
  ByteArray
    b;
  String
    buffer,linha,endereco,nome,classe,metodo;
  Number
    hex;
  Split
    list;
  int
    x,p,c,pc,pe;

  if(!f.access(file,"rw"))
    cReflectionObjDump(file);

  f.open(file);
  f.readAll(b);
  b.readLine(buffer);
  if(buffer.trim().equals("#passed true"))
    return;
  b.seek(0);
  while(b.readLine(buffer)){
    linha=buffer.trim();

    if(strstr(linha,"0x00")!=linha.ptr())
      continue;
    if(strpbrk(linha,".="))
      continue;
    if(strstr(linha,"typeinfo"))
      continue;
    linha=linha.replace("  "," ");
    if(!FOUND(x=linha.inStr(' ')))
      continue;
    endereco=linha.mid(0,x);
    nome=linha.mid(x+1);
    if(nome[0]=='0')
      continue;
    if(strstr(nome,"_"))
      continue;

    hex.fromHex(endereco);

    if(strstr(nome,"vtable for")){
      nome=nome.mid(11);
      list.add(String().sprintf("%X %s",hex.asPtr(),nome.ptr()));
      list.add(String().sprintf("%s %X",nome.ptr(),hex.asPtr()));
      continue;
    }
    if(!FOUND(p=nome.inStr('(')))
      continue;
    pc=nome[p+1]==')';
    c=nome.inStr("::");
    pe=nome.inStr(')');

    if(pe+1!=nome.len())
      continue;

    nome[p]=0;

    if(!FOUND(c)){
      nome[p]=0;
      list.add(String().sprintf("%s %X",nome.ptr(),hex.asPtr()));
      continue;
    }
    classe=nome.mid(0,c);
    metodo=nome.mid(c+2);

    if(classe==metodo&&!pc)
      continue;
    list.add(String().sprintf("%s %X",nome.ptr(),hex.asPtr()));
  }

  list.sort().unique();

  b.reset();
  b.writef("#passed true\n");
  for(int i=0;i<list.len();i++)
    b.writef("%s\n",list[i].ptr());

  f.truncate();
  f.write(b);
}
/*=========================================================

=========================================================*/
int cSortSortImpl(void *a,void *b,void *arg){
  char
    (Object::*compare)(Sort&,void*,void*)=0;
  Sort
    *sort=(Sort*)arg;
  Object
    *obj=sort->getObj();

  *(void**)&compare=sort->getMethodCompare();

  return (obj->*compare)(*sort,a,b);
}
/*=========================================================

=========================================================*/
void cCppInclude(String &fileName,String &linha,ByteArray &buffer){
  linha=linha.trim();
  if(linha[0]!='#')
    return;
  linha=linha
    .replace("#include","")
    .replace('"',"")
    .replace(" ","")
    .trim();
  {
    int
      x;
    String
      path,name;
    Split
      item;

    x=fileName.inStrRev('/');
    path=fileName
      .mid(0,x)
      .append("/")
      .append(linha);
    x=path.inStrRev('/');
    name=path.mid(x+1);
    path[x]=0;
    item.split(name,'.');

    if(item[1].equals("h"))
      cCpp(path,name,buffer);
  }
}
/*=========================================================

=========================================================*/
void cCpp(String &path,String &name,ByteArray &buffer){
  String
    fileName=String().sprintf("%s/%s",path.ptr(),name.ptr()),
    linha;
  File
    f;

  if(!f.isOpen(fileName,1)){
    cPrintf("ERRO: cpp: %s\n",fileName.ptr());
    exit(1);
  }
  while(f.readLine(linha)){
    if(FOUND(linha.inStr("#include"))){
      cCppInclude(fileName,linha,buffer);
      continue;
    }
    buffer.writeStr(linha.append("\n"));
  }
}
/*=========================================================

=========================================================*/
void cCppMain(cchar *file){
  String
    path,name;
  ByteArray
    buffer;

  path=getcwd(String().resize(1024),1024);
  name=file;
  cCpp(path,name,buffer);
  cPrintf("%s\n",buffer.toString().ptr());
};
