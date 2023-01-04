/*=========================================================

=========================================================*/
void File::init(){
  f=openned=readOnly=temporary=0;
}
File::File(){
  init();
}
File::~File(){
  close();
}
/*=========================================================

=========================================================*/
char File::access(cchar *file,cchar *mask){
  char
    ok=0,
    *buffer=cMemAllocChar(1024);
  String
    bit=String(mask).lCase(),
    res;

  if(bit.len()!=3)
    goto FIM;
  if(!file||!*file)
    goto FIM;
  if(__xstat64(3,VOID(file),buffer))
    goto FIM;
  res=Number(*(int*)&buffer[16]).toBinary();
  ok=1
    &&(bit[0]=='-'||(bit[0]=='r'&&(res[7]=='1'||res[7+3]=='1'||res[7+6]=='1')))
    &&(bit[1]=='-'||(bit[1]=='w'&&(res[8]=='1'||res[8+3]=='1'||res[8+6]=='1')))
    &&(bit[2]=='-'||(bit[2]=='x'&&(res[9]=='1'||res[9+3]=='1'||res[9+6]=='1')))
  ;
  FIM:
  cMemFree(buffer);
  return ok;
}
/*=========================================================

=========================================================*/
char File::temp(void){
  close();
  name="/tmp/tmpXXXXXX";
  f=mkstemp(name);
  fchmod(f,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
  openned=temporary=1;
  return 1;
}
/*=========================================================

=========================================================*/
char File::open(cchar *file,char readOnly){
  close();
  f=readOnly
      ? ::open(file,O_RDONLY|O_LARGEFILE,0)
      : ::open(file,O_CREAT|O_RDWR|O_LARGEFILE,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);

  if(f>0){
    name=file;
    openned=1;
    this->readOnly=readOnly;
    return 1;
  }
  return 0;
}
/*=========================================================

=========================================================*/
Date File::modified(void){
  if(f>0){
    ByteArray
      buffer;
    int
      ok=fstat(f,buffer.resize(256).ptr()),
      *item=(int*)buffer.ptr();

    return ok?0:Date().fromUtc(item[16]).get();
  }
  return 0;

}
/*=========================================================

=========================================================*/
char File::isDir(void){
  if(f>0){
    ByteArray
      buffer;
    int
      ok=fstat(f,buffer.resize(256).ptr());

    if(ok)
      return 0;
    return (((int*)buffer.ptr())[4]&S_IFMT)==S_IFDIR;
  }
  return 0;
}
/*=========================================================

=========================================================*/
void File::close(void){
  if(f>0)
    ::close(f);
  if(temporary)
    remove(name);
  init();
}
/*=========================================================

=========================================================*/
char File::isOpen(cchar *file,char readOnly){
  if(!file)
    return openned;
  return open(file,readOnly);
}
/*=========================================================

=========================================================*/
char File::isReadOnly(void){
  return readOnly;
}
/*=========================================================

=========================================================*/
char File::sync(void){
  if(!isOpen()||isReadOnly())
    return 0;
  fsync(f);
  return 1;
}
/*=========================================================

=========================================================*/
int File::write(const String &buffer,int len){
  if(!isOpen()||isReadOnly())
    return -1;
  else{
    String
      &p=PSTRING(buffer);

    return ::write(f,p.ptr(),FOUND(len)?len:p.len());
  }
}
/*=========================================================

=========================================================*/
int File::read(String &buffer,int len){
  if(!isOpen())
    return -1;
  if(len<0)
    len=0;
  buffer.resize(len+1);
  buffer.reLen();
  return ::read(f,VOID(buffer.ptr()),len);
}
/*=========================================================

=========================================================*/
int File::read(cchar *buffer,int len){
  if(!isOpen())
    return -1;
  if(len<0)
    len=0;
  return ::read(f,VOID(buffer),len);
}
/*=========================================================

=========================================================*/
int File::position(void){
  return
    !isOpen()
      ?0
      :lseek(f,0,SEEK_CUR);
}
/*=========================================================

=========================================================*/
char File::seek(int pos){
  if(!isOpen())
    return 0;
  lseek(f,pos,SEEK_SET);
  return 1;
}
/*=========================================================

=========================================================*/
char File::truncate(int size){
  if(size<0)
    size=0;
  if(!isOpen()||isReadOnly())
    return 0;
  ftruncate(f,size);
  seek(size);
  return 1;
}
/*=========================================================

=========================================================*/
int File::size(void){
  if(!isOpen())
    return 0;
  else{
    ByteArray
      buffer;

    fstat(getF(),buffer.resize(256).ptr());
    return ((int*)buffer.ptr())[11];
  }
}
/*=========================================================
testado em 08/01/2022
so funciona se o outro processo chamar esta mesma funcao!
=========================================================*/
char File::lock(int timeout){
  if(!isOpen()||isReadOnly())
    return 0;
  else{
    FLOCK
      &lock=*(FLOCK*)cMemAlloc(sizeof(FLOCK));
    int
      ok=0,
      agora=time(0);

    lock.seek=SEEK_SET;
    lock.type=F_WRLCK;
    lock.start=0;
    lock.end=0;
    lock.pid=getpid();

    while(1){
      ok=!fcntl(f,F_SETLK,&lock);
      if(ok)
        break;
      if(!timeout)
        break;
      if(time(0)-agora>timeout)
        break;
      usleep(5);
    }

    cMemFree(&lock);
    return ok;
  }
}
/*=========================================================

=========================================================*/
char File::unlock(void){
  if(!isOpen()||isReadOnly())
    return 0;
  else{
    FLOCK
      &lock=*(FLOCK*)cMemAlloc(sizeof(FLOCK));
    int
      ok=0;

    lock.seek=SEEK_SET;
    lock.type=F_UNLCK;
    lock.start=0;
    lock.end=0;
    lock.pid=getpid();

    ok=!fcntl(f,F_SETLK,&lock);

    cMemFree(&lock);
    return ok;
  }
}
/*=========================================================

=========================================================*/
char File::readLine(String &buffer,int max){
  if(buffer.size()<max)
    buffer.resize(max);
  buffer[0]=0;

  if(!isOpen())
    return 0;
  else{
    uchar
      c;
    int
      i,
      lidos;

    for(i=0;(lidos=::read(f,&c,1))==1&&i<max;i++)
      switch(c){
        case '\n':
          goto NEXT;
        case '\r':
          continue;
        default:
          buffer[i]=c;
      }
    NEXT:
    buffer[i]=0;
    if(!lidos&&i)
      return 1;
    if(!lidos)
      return 0;
    return 1;
  }
}
/*=========================================================

=========================================================*/
char File::readAll(String &buffer){
  if(!isOpen())
    return 0;
  else{
    seek(0);
    return read(buffer.reset(),size()+1);
  }
}
/*=========================================================

=========================================================*/
char File::readAll(ByteArray &buffer){
  buffer.seek(0);
  if(!isOpen())
    return 0;
  else{
    int
      s=size();

    seek(0);
    return read(PCHAR(buffer.reset().resize(s).ptr()),s);
  }
}
/*=========================================================

=========================================================*/
int File::write(ByteArray &buffer,int len){
  if(!isOpen()||isReadOnly())
    return -1;
  else{

    return ::write(f,buffer.ptr(),FOUND(len)?len:buffer.len());
  }
}
/*=========================================================

=========================================================*/
int File::getF(void){
  return f;
}
/*=========================================================

=========================================================*/
String File::getName(char noPath){
  if(!noPath)
    return name;
  else
    return Split(name,'/').last();
}
/*=========================================================

=========================================================*/
void File::unlink(void){
  char
    ok=1
      &&openned
      &&!temporary
      &&!readOnly;

  if(ok){
    String
      name=getName();

    close();
    remove(name);
  }
}
