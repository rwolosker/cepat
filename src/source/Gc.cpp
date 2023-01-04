/*=========================================================

=========================================================*/
Gc::Gc(){
  init();
}
Gc::Gc(const Gc &gc){
  init();
}
Gc::~Gc(){
  reset();
}
/*=========================================================

=========================================================*/
Gc &Gc::operator=(const Gc &gc){
  return *this;
}
/*=========================================================

=========================================================*/
void Gc::init(void){
  length=0;
  list=0;
  otimized=0;
  realSize=0;
}
/*=========================================================

=========================================================*/
Gc &Gc::otimize(char otimized){
  if(otimized)
    this->otimized=otimized;
  return *this;
}
/*=========================================================

=========================================================*/
void Gc::reset(void){
  for(int i=0;i<length;i++)
    free(list[i]);
  cMemFree(list);
  init();
}
/*=========================================================

=========================================================*/
void *Gc::free(void *ptr){
  if(ptr)
    cMemFree(ptr);
  return 0;
}
/*=========================================================

=========================================================*/
void *Gc::add(void *ptr){
  if(!otimized){
    list=(void**)cMemRealloc(list,(length+=1)*sizeof(void*));
    list[length-1]=ptr;
  }
  else{
    if(!realSize){
      realSize=10;
      list=(void**)cMemRealloc(list,realSize*sizeof(void*));
      goto NEXT;
    }
    if(length+1==realSize){
      realSize*=2;
      list=(void**)cMemRealloc(list,realSize*sizeof(void*));
      goto NEXT;
    }
    NEXT:
      list[length++]=ptr;
  }
  return ptr;
}
/*=========================================================

=========================================================*/
void Gc::debug(void){
  for(int i=0;i<length;i++)
    cPrintf("%d: %X\n",i,list[i]);
}
/*=========================================================

=========================================================*/
int Gc::len(void){
  return length;
}
/*=========================================================

=========================================================*/
void *Gc::get(int x){
  return
    x>=0&&x<len()
      ?list[x]
      :0;
}
/*=========================================================

=========================================================*/
char Gc::contains(void *ptr){
  if(!ptr)
    return 0;
  else{
    char
      *plist=PCHAR(list),
      *query=PCHAR(cMemCpy(&ptr,4,cMemAlloc(SIZEOF_VOID))),
      *p=PCHAR(memmem(plist,cMemSize(list),query,SIZEOF_VOID)),
      ok=1&&p;

    cMemFree(query);
    return ok;
  }
}
/*=========================================================

=========================================================*/
int Gc::indexOf(void *ptr){
  if(!ptr)
    return -1;
  else{
    char
      *plist=PCHAR(list),
      *query=PCHAR(cMemCpy(&ptr,4,cMemAlloc(SIZEOF_VOID))),
      *p=PCHAR(memmem(plist,cMemSize(list),query,SIZEOF_VOID)),
      ok=1&&p;

    cMemFree(query);
    return ok?(p-plist)/4:-1;
  }
}

/*=========================================================

=========================================================*/
void Gc::remove(void *ptr){
  if(ptr){
    char
      *plist=PCHAR(list),
      *query=PCHAR(cMemCpy(&ptr,4,cMemAlloc(SIZEOF_VOID))),
      *p=PCHAR(memmem(plist,cMemSize(list),query,SIZEOF_VOID));
    
    if(p){
      char
        *update=cMemAllocChar((length-1)*SIZEOF_VOID);

      cMemCpy(plist,p-plist,update);
      cMemCpy(
        plist+(p-plist)+SIZEOF_VOID,
        (length-1)*SIZEOF_VOID-(p-plist),
        update+(p-plist)
      );
      cMemFree(list);
      list=(void**)update;
      length-=1;

      free(ptr);
    }
    cMemFree(query);
  }
}
