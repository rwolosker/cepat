/*=========================================================

=========================================================*/
void ByteArray::init(void){
  str=0;
  pos=length=0;
}
ByteArray::ByteArray(){
  init();
}
ByteArray::~ByteArray(){
  reset();
}
ByteArray &ByteArray::reset(void){
  if(str)
    cMemFree(str);
  init();
  return *this;
}
ByteArray::ByteArray(const ByteArray &value){
  init();
  *this=value;
}
ByteArray::ByteArray(uchar c){
  init();
  set(c);
}
ByteArray::ByteArray(char c){
  init();
  set(c);
}
ByteArray::ByteArray(cchar *value){
  init();
  set(value);
}
ByteArray::ByteArray(int value){
  init();
  set(value);
}
ByteArray::ByteArray(ulong value){
  init();
  set(value);
}
ByteArray::ByteArray(short value){
  init();
  set(value);
}
ByteArray::ByteArray(double value){
  init();
  set(value);
}
/*=========================================================

=========================================================*/
ByteArray &ByteArray::fill(int c){
  cMemSet(c,length,str);
  return *this;
}
/*=========================================================

=========================================================*/
ByteArray &ByteArray::resize(int size){
  if(size==length)
    goto FIM;
  if(size>length)
    str=PUCHAR(cMemRealloc(str,length=size));
  else{
    uchar
      *tmp=PUCHAR(cMemCpy(str,size,cMemAlloc(size)));

    reset();
    str=tmp;
    length=size;
  }
  FIM:
  return *this;
}
/*=========================================================

=========================================================*/
ByteArray &ByteArray::debug(char dump){

  cPrintf("this  : [%X]\n",this);
  cPrintf("str   : [%X]\n",str);
  cPrintf("length: [%d]\n",length);
  cPrintf("pos...: [%d]\n",pos);
  if(dump)
    cHexdump(str,length);
  cPrintf("\n");
  return *this;
}
/*=========================================================

=========================================================*/
ByteArray &ByteArray::hexDump(void){
  cHexdump(str,length);
  return *this;
}
/*=========================================================

=========================================================*/
uchar *ByteArray::ptr(void){
  return str;
}
int ByteArray::len(void){
  return length;
}
int ByteArray::position(void){
  return pos;
}
/*=========================================================

=========================================================*/
ByteArray &ByteArray::set(uchar c){
  reset();
  resize(1);
  *str=c;
  inc(1);
  return *this;
}
ByteArray &ByteArray::set(char c){
  reset();
  resize(1);
  *str=c;
  inc(1);
  return *this;
}
ByteArray &ByteArray::set(cchar *value){
  reset();
  if(!value)
    goto FIM;
  length=cStrLen(value);
  str=PUCHAR(cMemCpy(VOID(value),length,cMemAlloc(length)));
  inc(length);
  FIM:
  return *this;
}
ByteArray &ByteArray::set(int value){
  *(int*)(reset().resize(4).ptr())=value;
  inc(4);
  return *this;
}
ByteArray &ByteArray::set(ulong value){
  *(ulong*)(reset().resize(4).ptr())=value;
  inc(4);
  return *this;
}
ByteArray &ByteArray::set(short value){
  *(short*)(reset().resize(2).ptr())=value;
  inc(2);
  return *this;
}
ByteArray &ByteArray::set(double value){
  *(double*)(reset().resize(8).ptr())=value;
  inc(8);
  return *this;
}
ByteArray &ByteArray::set(void *value,int size){
  reset();
  if(value&&size){
    str=PUCHAR(cMemCpy(value,size,cMemAlloc(length=size)));
    inc(size);
  }
  return *this;
}
/*=========================================================

=========================================================*/
ByteArray &ByteArray::setUtf16(cchar *value){
  reset();
  if(!value||!*value)
    goto FIM;
  else{
    int
      len=cStrLen(value);
    short
      *a=(short*)cMemAlloc(len*sizeof(short)),
      *pa=a;
    char
      *pv=PCHAR(value);

    while(*pv)
      *pa++=*pv++;

    str=PUCHAR(a);
    length=len*sizeof(short);
  }
  FIM:
  return *this;
}
/*=========================================================

=========================================================*/
ByteArray &ByteArray::operator=(const ByteArray &value){
  if(str!=value.str)
    set(value.str,value.length);
  return *this;
}
/*=========================================================

=========================================================*/
ByteArray &ByteArray::append(const ByteArray &value){
  needWrite(value.length);
  cMemCpy(value.str,value.length,str+pos);
  inc(value.length);
  return *this;
}
ByteArray &ByteArray::append(const ByteArray &value,int size){
  if(size>=value.length)
    size=value.length;
  needWrite(size);
  cMemCpy(value.str,size,str+pos);
  inc(size);
  return *this;
}
/*=========================================================

=========================================================*/
ByteArray ByteArray::copy(void){
  ByteArray
    copia;

  copia=*this;
  return copia;
}
/*=========================================================

=========================================================*/
String ByteArray::toString(void){
  return PCHAR(copy().append('\0').ptr());
}
/*=========================================================

=========================================================*/
ByteArray &ByteArray::writeChar(char value){
  return append(value);
}
ByteArray &ByteArray::writeShort(short value){
  return append(value);
}
ByteArray &ByteArray::writeInt(int value){
  return append(value);
}
ByteArray &ByteArray::writeDouble(double value){
  return append(value);
}
ByteArray &ByteArray::writePtr(void *value){
  if(!value)
    return writeInt(0);
  return writeInt((uint)value);
}
ByteArray &ByteArray::writeStr(const String &value,int len){
  if(!FOUND(len))
    return append(CSTRING(value).ptr());
  else{
    ByteArray
      b;

    cMemCpy(
      VOID(String(value).resize(len).ptr()),
      len,
      VOID(b.resize(len).ptr())
    );
    return append(b);
  }
}
ByteArray &ByteArray::writef(cchar *format,...){
  return writeStr(
    String().squeeze(cVsprintf(&format))
  );
}
/*=========================================================

=========================================================*/
ByteArray &ByteArray::write(void *buffer,int size){
  if(size<=0)
    return *this;
  else{
    ByteArray
      tmp;

    tmp.set(buffer,size);
    append(tmp);


    return *this;
  }
}
/*=========================================================

=========================================================*/
void ByteArray::inc(int size){
  pos+=size;
  if(pos>=length)
    pos=length;
}
/*=========================================================

=========================================================*/
char ByteArray::eof(void){
  return pos>=length;
}
/*=========================================================

=========================================================*/
void ByteArray::need(int value){
  if(value>=length)
    resize(value+1);
}
/*=========================================================

=========================================================*/
void ByteArray::needWrite(int size){
  int
    need=len()-pos-size;

  if(need>=0)
    return;
  need=abs(need);
  str=PUCHAR(cMemRealloc(str,length+need));
  length+=need;
}
/*=========================================================

=========================================================*/
ByteArray &ByteArray::seek(int pos){
  if(!length){
     pos=0;
     goto NEXT;
  }
  if(pos<0)
    pos=0;
  need(pos);
  this->pos=pos;
  NEXT:
  return *this;
}
/*=========================================================

=========================================================*/
short ByteArray::readShort(void){
  short
    value=*(short*)&str[pos];

  inc(2);
  return value;
}
/*=========================================================

=========================================================*/
void *ByteArray::readPointer(void){
  void
    *value=*(void**)&str[pos];

  inc(4);
  return value;
}
/*=========================================================

=========================================================*/
int ByteArray::readInt(int size){
  int
    value=0;

  if(size<0)
    size=0;
  cMemCpy(
    str+pos,
    size,
    &value
  );

  inc(size);
  return value;
}
/*=========================================================

=========================================================*/
double ByteArray::readDouble(void){
  double
    value=*(double*)&str[pos];

  inc(8);
  return value;
}
/*=========================================================

=========================================================*/
char ByteArray::readChar(void){
  char
    value=*(char*)&str[pos];

  inc(1);
  return value;
}
/*=========================================================

=========================================================*/
uchar ByteArray::readUchar(void){
  char
    value=*(char*)&str[pos];

  inc(1);
  return value;
}
/*=========================================================

=========================================================*/
String ByteArray::readStr(int size){
  String
    res;

  res.resize(size+1);
  cMemCpy(str+pos,size,res.ptr());
  inc(size);
  return res;
}
/*=========================================================

=========================================================*/
String ByteArray::readUtf16(int size){
  String
    res;
  short
    *item=(short*)(str+pos);

  res.resize(size+1);
  for(int i=0;i<size;i++)
    res[i]=(char)(item[i]);

  inc(size*2);
  return res;
}
/*=========================================================

=========================================================*/
int ByteArray::indexOf(void *buffer,int size){
  if(!length)
    return -1;
  else{
    uchar
      *p=PUCHAR(memmem(str,length,VOID(buffer),size));
    int
      x=!p?-1:p-str;

    return x;
  }
}
int ByteArray::indexOf(int start,void *buffer,int size){
  if(!length)
    return -1;
  else{
    uchar
      *p=PUCHAR(memmem(str+start,length-start,VOID(buffer),size));
    int
      x=!p?-1:p-str;

    return x;
  }
}
/*=========================================================

=========================================================*/
ByteArray ByteArray::mid(int start,int size){
  if(start>=len())
    return ByteArray();
  if(size==-1)
    size=length;
  if(start+size>=len())
    size=len()-start;

  {
    ByteArray
      res;

    cMemCpy(str+start,size,res.resize(size).ptr());
    return res;
  }
}
/*=========================================================

=========================================================*/
ByteArray &ByteArray::write(const ByteArray &value){
  return append(value);
}
/*=========================================================

=========================================================*/
char ByteArray::readLine(String &line){
  if(eof())
    return 0;
  else{
    int
      q=10,
      x=indexOf(pos,&q,1),
      len=FOUND(x)?x-pos:length-pos;

    if(len>line.size())
      line.resize(len+1);
    cMemCpy(str+pos,len,line.ptr());
    line[len]=0;
    if(len&&line[len-1]==13)
      line[len-1]=0;
    line.reLen();
    inc(len+1);
  }
  return 1;
}
/*=========================================================

=========================================================*/
uchar ByteArray::at(int x){
  if(x<0)
    x=0;
  need(x);
  return str[x];
}
/*=========================================================

=========================================================*/
ByteArray *ByteArray::instance(void){
  return new(cMemAlloc(sizeof(ByteArray)))ByteArray();
}
/*=========================================================

=========================================================*/
CharRef ByteArray::operator[](int x){
  CharRef
    ref;

  at(x);
  ref.set(PCHAR(&str[x]));
  return ref;
}
/*=========================================================

=========================================================*/
ByteArray ByteArray::readBuffer(int size){
  if(!FOUND(size))
    size=len();
  {
    int
      need=position()+size>len()?len()-position():size;
    ByteArray
      b=ByteArray().resize(need);

    cMemCpy(
      VOID(ptr()+position()),
      need,
      VOID(b.ptr())
    );
    inc(need);
    return b;
  }
}
