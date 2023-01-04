/*=========================================================

=========================================================*/
char *cStrTrim(cchar *src,cchar *dst){
  int
    len=cStrLen(src);
  char
    *p0=PCHAR(src),
    *p=p0,
    *pn=p+len-1;

  if(len){
    int
      need;

    while(p&&*p&&(*p==32||*p==9))
      p++;
    while(p&&*p&&pn&&*pn&&pn>p0&&(*pn==32||*pn==9))
      pn--;

    cMemCpy(
      VOID(p),
     need=pn+1-p,
      VOID(dst)
    );
    PCHAR(dst)[need]=0;
  }
  return PCHAR(dst);
}
/*=========================================================

=========================================================*/
char *cStrReplace(cchar *src,cchar *query,cchar *replace,cchar *dst){
  int
    *index=(int*)cMemAlloc(1024*sizeof(int)),
    count=0,
    x,
    qlen=cStrLen(query),
    rlen=cStrLen(replace),
    slen=cStrLen(src);
  char
    *p0=PCHAR(src),
    *p=p0,
    *tmp=0;

  while(p&&*p&&FOUND(x=cStrIndexOf(p,query))){
    index[count]=(p+x)-p0;
    count+=1;
    p+=x+qlen;
  }

  if(!count)
    cStrCopy(src,PCHAR(dst));
  else{
    char
      *pt=tmp=cMemAllocChar(rlen*count*2+slen*2);

    for(int i=0;i<count;i++){
      int
        rpos;

      cStrMid(
        p0,
        !i?0:index[i-1]+qlen,
        rpos=!i?index[i]:index[i]-index[i-1]-qlen,
        pt
      );
      pt+=rpos;
      cMemCpy(VOID(replace),rlen,VOID(pt));
      pt+=rlen;
    }
    cStrCopy(p0+index[count-1]+qlen,pt);
    cStrCopy(tmp,PCHAR(dst));

    cStrReplace(dst,query,replace,dst);
  }
  cMemFreeAll(index,tmp,-1);
  return PCHAR(dst);
}
/*=========================================================

=========================================================*/
void *cLibrarySymbol(void *lib,char *name){
  if(!name||!*name)
    return 0;
  return dlsym(lib,name);
}
/*=========================================================

=========================================================*/
int cStrIndexOf(cchar *value,int query){
  char
    *p=strchr(value,query);

  return p?p-value:-1;
}
/*=========================================================

=========================================================*/
int cStrIndexOf(cchar *value,cchar *query){
  char
    *p=strstr(value,query);

  return p?p-value:-1;
}
/*=========================================================

=========================================================*/
char *cStrMid(char *src,int pos,int size,char *dst){
  if(!src&&!dst)
    return 0;
  if(!src&&dst){
    char
      *pd=dst;

    *pd=0;
    return pd;
  }
  {
    char
      *ps=src,
      *pd=dst;

    if(pos>=cStrLen(ps)){
      *pd=0;
      return pd;
    }
    else{
      char
        *p0=ps+=pos;

      while(*ps&&ps-p0<size)
        *pd++=*ps++;
      *pd=0;
      return dst;
    }
  }
}
/*=========================================================

=========================================================*/
void *cLibraryFree(void *lib){
  if(lib)
    dlclose(lib);
  return 0;
}
/*=========================================================

=========================================================*/
void *cLibraryLoad(char *file){
  return dlopen(file,0x00002|0x00008|0x00100);
}
/*=========================================================

=========================================================*/
void cDebug(char debug,cchar *format,...){
  if(!debug)
    return;
  else{
    char
      *tmp=cVsprintf(&format);

    cPrintf("%s\n",tmp);
    cMemFree(tmp);
  }
}
/*=========================================================

=========================================================*/
void cMemFreeAllImpl(void *ptr){
  void
    **item=(void**)ptr;
  int
    start=0;

  if(__GNUC__==4)
    start=5;
  if(__GNUC__==8)
    start=5;//em 01/12/2022
  #ifdef __clang__
    start=3;//em todos eles podia ser cGnuStart
  #endif

  for(int i=start;(long)item[i]!=-1;i++)
    cMemFree(item[i]);
}
/*=========================================================

=========================================================*/
void cMemFreeAll(...){
  char
    *ptr=0;
  
  cMemFreeAllImpl(&ptr);
}
/*=========================================================

=========================================================*/
void *cMemFree(void *ptr){
  if(!ptr)
    return 0;
  else{
    NODE
      *item=cMemNodePtr(ptr);
    
    if(!item)
      return 0;
    if(item->ptr)
      free(item->ptr);
    cMemNode=cNodeDel(item,cMemNode);
    return 0;
  }
}
/*=========================================================

=========================================================*/
NODE *cMemNodePtr(void *ptr){
  if(!ptr)
    return 0;
  else{
    NODE
      *item=((NODE*)*(void**)(PCHAR(ptr)-SIZEOF_VOID));
  
    return item;
  }
}
/*=========================================================

=========================================================*/
int cMemSize(void *ptr){
  NODE
    *item=cMemNodePtr(ptr);
  
  return item?item->size:0;
}
/*=========================================================

=========================================================*/
char *cMemAllocChar(int size){
  return PCHAR(cMemAlloc(size));
}
/*=========================================================

=========================================================*/
void *cMemAlloc(int size){
  if(size<=0)
    return 0;
  else{
    NODE
      *item=(NODE*)malloc(sizeof(NODE));
    char
      *ptr=PCHAR(calloc(1,size+SIZEOF_VOID));
    
    if(!ptr){
      printf("ERRO: cMemAlloc %d\n.",size);
      exit(1);
    }

    cMemReset(item,sizeof(NODE));
    *(void**)ptr=item;
    item->ptr=ptr;
    item->size=size;
    cMemNode=cNodeAdd(item,cMemNode);
    
    return ptr+SIZEOF_VOID;
  }
}
/*=========================================================

=========================================================*/
NODE *cNodeDel(NODE *item,NODE *node,void*(*pfree)(void*)){
  if(!pfree)
    pfree=free;
  if(!item)
    return node;
  if(item==node&&!item->next){
    pfree(item);
    return 0;
  }
  if(item==node){
    NODE
      *next=node->next;
    
    next->last=node->last;
    next->prev=0;
    pfree(item);
    return next;
  }
  if(item==node->last){
    NODE
      *prev=item->prev;
    
    prev->next=0;
    node->last=prev;
    pfree(item);
    return node;
  }
  item->prev->next=item->next;
  item->next->prev=item->prev;
  pfree(item);
  return node;
}
/*=========================================================

=========================================================*/
int cMemUsed(char real){
  if(real){
    C_CALL
      c=mallinfo();
    void
      **pc=(void**)&c;

    if(1){
      cPrintf("arena   : %d\n",(ulong)pc[0]);
      cPrintf("ordblks : %d\n",(ulong)pc[1]);
      cPrintf("smblks  : %d\n",(ulong)pc[2]);
      cPrintf("hblks   : %d\n",(ulong)pc[3]);
      cPrintf("hblkhd  : %d\n",(ulong)pc[4]);
      cPrintf("usmblks : %d\n",(ulong)pc[5]);
      cPrintf("fsmblks : %d\n",(ulong)pc[6]);
      cPrintf("uordblks: %d\n",(ulong)pc[7]);
      cPrintf("fordblks: %d\n",(ulong)pc[8]);
      cPrintf("keepcost: %d\n",(ulong)pc[9]);
    }
    return (ulong)pc[7];
  }
  else{
    NODE
      *p=cMemNode;
    int
      total=0;
    
    while(p){
      total+=p->size;
      p=p->next;
    }
    return total;
  }
}
/*=========================================================

=========================================================*/
void cNodeDebug(NODE *node){
  NODE
    *p=node;
  
  while(p){
    printf("%08X %08X %d\n",p,p->ptr,p->size);
    p=p->next;
  }
}
/*=========================================================

=========================================================*/
NODE *cNodeAdd(NODE *src,NODE *node){
  if(!node){
    src->last=src;
    return src;
  }
  node->last->next=src;
  src->prev=node->last;
  node->last=src;
  
  return node;
}
/*=========================================================

=========================================================*/
void cDateTzConfig(void){
  if(cDateTz)
    return;
  if(getenv("TZ"))
    tzset();
  else
    cDateTzChange(TZ_RECIFE);
  cDateTz=1;
}
/*=========================================================
$ cat /etc/timezone
$ grep -r  America  /usr/share/zoneinfo
=========================================================*/
void cDateTzChange(cchar *value){
  setenv("TZ",value,1);
  tzset();//precisa ter isso.
  cDateTz=1;
}
/*=========================================================

=========================================================*/
void cGnuStart(int *start,char isarray){
  *start=0;
  if(__GNUC__==4)
    *start=6;
  if(__GNUC__==8)
    *start=6;
  #ifdef __clang__
    *start=4;//2022-12-13
  #endif
  if(!isarray)
    *start*=4;
}
/*=========================================================

=========================================================*/
void cHexdump(void *ptr,int size,int width){
  void
    (*printf)(cchar*,...)=0;

  *(void**)&printf=cHexdumpPrint;

  if(!ptr)
    return;
  else{
    int
      lwidth=8+1+(width*2)+(width-1)+1+width+1,
      i;
    uchar
      *p=PUCHAR(ptr);
    char
      *buffer=PCHAR(malloc(lwidth+1)),
      *offset=PCHAR(malloc(lwidth+1)),
      *pb=buffer,
      *po=offset;
    
    for(i=0;i<size;i++){
      if(!(i%width)){
        if(i){
          printf("%s|%-*.*s|\n",buffer,width,width,offset);
        }
        pb=buffer;
        pb+=sprintf(pb,"%08d ",i);
      }
      pb+=sprintf(pb,"%02X ",p[i]);
      po[i%width]=isprint(p[i])?p[i]:'.';
    }
    if(size%width)
      for(i=size%width;i<width;i++)
        po[i]=' ';

    printf(
      "%-*.*s |%-*.*s|\n\n",
      lwidth-2-width,lwidth-2-width,
      //lwidth-4,lwidth,
      buffer,
      width,width,
      offset
    );
    free(buffer);
    free(offset);
  }
}
/*=========================================================

=========================================================*/
void cMemPrintUsed(char real){
  cPrintf("used: %d\n",cMemUsed(real));
}
/*=========================================================

=========================================================*/
void cMemDebug(void){
  cNodeDebug(cMemNode);
}
/*=========================================================

=========================================================*/
void *cMemReset(void *ptr,int size){
  if(!ptr)
    return ptr;
  return memset(ptr,0,size);
}
/*=========================================================

=========================================================*/
void *cMemSet(int c,int size,void *ptr){
  return memset(ptr,c,size);
}
/*=========================================================

=========================================================*/
void *cMemRealloc(void *ptr,int size){
  if(!ptr)
    return cMemAlloc(size);
  else{
    NODE
      *node=cMemNodePtr(ptr);
    char
      *p;
    int
      sizeOld;
    
    if(!node)
      return cMemRealloc(0,size);
    if(size<=node->size)
      return ptr;
    
    sizeOld=node->size;
    p=PCHAR(node->ptr=realloc(node->ptr,(node->size=size)+4));
    cMemReset(p+SIZEOF_VOID+sizeOld,size-sizeOld);
    return p+SIZEOF_VOID;
  }
}
/*=========================================================

=========================================================*/
int cStrLen(cchar *str){
  return !str||!*str?0:strlen(str);
}
/*=========================================================

=========================================================*/
char *cStrCopy(cchar *src,char *dst){
  if(!src)
    return dst;
  else{
    int
      slen=cStrLen(src);
    char
      *res;
    
    res=PCHAR(cMemCpy(
      VOID(src),
      slen,
      dst
        ?VOID(dst)
        :cMemAlloc(slen+1)
    ));
    res[slen]=0;
    return res;
  }
}
/*=========================================================

=========================================================*/
char *cMemAddChar(char *str){
  if(!str)
    return 0;
  else{
    NODE
      *node=(NODE*)malloc(sizeof(NODE));
    char
      *tmp=cStrCopy(str,0);
    
    cMemReset(node,sizeof(NODE));
    
    node->size=cStrLen(str)+1;
    node->ptr=realloc(
      0,
      node->size+1+SIZEOF_VOID
    );
    
    cStrCopy(tmp,PCHAR(node->ptr)+SIZEOF_VOID);
    *(void**)node->ptr=PCHAR(node);
    
    cMemNode=cNodeAdd(node,cMemNode);
    
    cMemFree(tmp);
    return PCHAR(node->ptr)+SIZEOF_VOID;
  }
}
/*=========================================================

=========================================================*/
void *cMemCpy(void *src,long size,void *dst){
  return memcpy(dst,src,size);
}
/*=========================================================

=========================================================*/
char *cVsprintf(void *pformat,int *size){
  void
    **p=(void**)pformat;
  char
    *buffer,
    *res=0;
  int
    lidos;
  
  if(!p||!*p)
    return 0;
  lidos=vasprintf(&buffer,PCHAR(p[0]),p+1);
  if(size)
    *size=lidos;
  res=PCHAR(cMemCpy(buffer,lidos,cMemAlloc(lidos+1)));
  free(buffer);
  return res;
}
/*=========================================================

=========================================================*/
void cPrintf(cchar *format,...){
  char
    *res=cVsprintf(&format);
  
  write(cStdout,res,cStrLen(res));
  if(cStdout!=1)
    fsync(cStdout);
  cMemFree(res);
}
/*=========================================================

=========================================================*/
void *cMethodAddressImpl(void *p){
  void
    **pp=(void**)p;
  int
    start=0;
  
  cGnuStart(&start);
  return pp[start-1];//-1 porque o ponteiro tem 8 bytes; 25/05/2022
}
/*=========================================================

=========================================================*/
void *cMethodAddress(...){
  char
    *p=0;
  

  return cMethodAddressImpl(&p);
}
/*=========================================================

=========================================================*/
double cProcessEnd(void *t1,char print){
  void
    *t2=cProcessStart();
  TIMESPEC
    &s1=*(TIMESPEC*)t1,
    &s2=*(TIMESPEC*)t2;
  double
    d1=s1.sec+s1.nsec/pow(10,9),
    d2=s2.sec+s2.nsec/pow(10,9);
  double
    d=d2-d1;

  cMemFree(t1);
  cMemFree(t2);

  if(print)
    cPrintf("time: %f\n",d);
  
  return d;
}
/*=========================================================

=========================================================*/
void *cProcessStart(void){
  TIMESPEC
    *t=(TIMESPEC*)cMemAlloc(sizeof(TIMESPEC));
  
  clock_gettime(0,t);
  return t;
}
/*=========================================================

=========================================================*/
char cIconvToUpper(uchar c){
  char
    *iconv=CONV_ISO_8859_15_UCASE;

  return iconv[c];
}
/*=========================================================

=========================================================*/
char cIconvToLower(uchar c){
  char
    *iconv=CONV_ISO_8859_15_LCASE;

  return iconv[c];
}
/*=========================================================

=========================================================*/
char cIconvNormalize(uchar c){
  char
    *iconv=CONV_NORMALIZE;

  return iconv[c];
}
/*=========================================================

=========================================================*/
char cStrCompare(cchar *a,cchar *b){
  if(a&&!b)return 1;
  if(!a&&b)return -1;
  if(!a&&!b)return 0;
  {
    int
      ok=strcmp(a,b);

    return !ok?0:ok>0?1:-1;
  }
}
/*=========================================================

=========================================================*/
char cStrCompareNormalize(cchar *a,cchar *b){
  if(a&&!b)return 1;
  if(!a&&b)return -1;
  if(!a&&!b)return 0;
  {
    char
      *pa=cMemAddChar(PCHAR(a)),
      *pb=cMemAddChar(PCHAR(b)),
      *p;
    int
      ok;

    p=pa;while(p&&*p){*p=cIconvNormalize(*p);p++;}
    p=pb;while(p&&*p){*p=cIconvNormalize(*p);p++;}
    ok=strcmp(pa,pb);
    cMemFreeAll(pa,pb,-1);

    return !ok?0:ok>0?1:-1;
  }
}
/*=========================================================
https://github.com/php/php-src/blob/master/ext/standard/string.c
https://stackoverflow.com/questions/1017599/how-do-i-remove-accents-from-characters-in-a-php-string
=========================================================*/
char *cStrTr(char *str,int len,char *from,char *to,int trlen){
  if(!trlen)
    return str;
  if(trlen==1){
    uchar
      cfrom=*from,
      cto=*to;

    for(int i=0;i< len;i++)
      if(str[i]==cfrom)
        str[i]=cto;
    return str;
  }
  {
    char
      *xlat=PCHAR(cMemAlloc(256));

    for(int i=0;i<trlen;i++)
      xlat[(int)(unsigned char)from[i]]=to[i]-from[i];
    for(int i=0;i<len;i++)
      str[i]+=xlat[(int)(unsigned char)str[i]];
    cMemFree(xlat);
  }
  return str;
}
/*=========================================================

=========================================================*/
double cDateParseFast(cchar *value){
  if(!value||!*value)
    return 0;
  else{
    int
      dia=0,mes=0,ano=0,hor=0,min=0,seg=0;
    char
      tmp[4];

    sscanf(
      value,
      "%d%[/-]%d%[/-]%d %d:%d:%d",
      &dia,tmp,
      &mes,tmp,
      &ano,
      &hor,&min,&seg
    );
    if(dia>100){/*yyyy-MM-dd*/
      int
        tmp=dia;

      dia=ano;
      ano=tmp;
      goto NEXT;
    }
    if(mes>12&&ano>100){/*MM-dd-YYYY*/
      int
        tmp=dia;

      dia=mes;
      mes=tmp;
      goto NEXT;
    }

    NEXT:
    return 0
      +double(dia*1.)
      +double(mes*100.)
      +double(ano*10000.)
      +double(hor/100.)
      +double(min/10000.)
      +double(seg/1000000.);
  }
}
/*=========================================================

=========================================================*/
double cNumberParse(cchar *value){
  char
    *p=PCHAR(value);
  double
    total=0;

  if(!p||!*p)
    return total;
  else{
    int
      len=cStrLen(p);

    for(int i=len-1,j=0;i>=0;i--)
      switch(p[i]){
        case '.':
          total/=pow(10,j);
          j=0;
          break;
        case '-':
          total*=-1;
          break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
          total+=(p[i]-'0')*pow(10,j++);
          break;
      }
  }
  return total;
}
