/*=========================================================

=========================================================*/
Base64::Base64(){
}
Base64::~Base64(){
}
/*=========================================================

=========================================================*/
String Base64::encode(String &buffer){
  ByteArray
    b;

  b.set(buffer);
  return encode(b);
}
/*=========================================================

=========================================================*/
String Base64::encode(ByteArray &buffer){
  int
    len=buffer.len();
  char
    *table="==ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/",
    *ps=PCHAR(buffer.ptr()),
    *target,
    *pt=target=cMemAllocChar(len*2),
    *pe=&ps[len],
    rem=0;

  while(ps<pe){
    unsigned char
      *a=(unsigned char*)ps,
      *p=(unsigned char*)pt;

    p[0]=table[2+((a[0]&0xfc)>>2                   )];
    p[1]=table[2+(((a[0]&0x03)<<4)+((a[1]&0xF0)>>4))];
    p[2]=table[2+(((a[1]&0x0F)<<2)+((a[2]&0xC0)>>6))];
    p[3]=table[2+(a[2]&0x3F                        )];
    p[4]=0;

    ps+=3;
    pt+=4;
  }
  rem=ps-pe;
  if(rem)
    cMemCpy(table,rem,&pt[cStrLen(pt)-rem]);

  return String().squeeze(target);
}
/*=========================================================

=========================================================*/
String Base64::line(const String &buffer,int cols){
  String
    tmp,
    res;
  int
    i,count,resto;

  tmp=PSTRING(buffer)
    .replace('\r','\0')
    .replace('\n','\0')
    .trim();
  count=tmp.len();
  resto=count%cols;
  count/=cols;
  for(i=0;i<count;i++)
    res
      .append(tmp.mid(i*cols,cols))
      .append("\n");
  if(resto)
    res.append(tmp.mid(i*cols,resto));

  return res;
}
/*=========================================================

=========================================================*/
short Base64::resolve(short c,char *rem){
  if(c=='='){
    *rem=1;
    return 0;
  }
  if(c>='A'&&c<='Z')
    return c-'A'+2;
  if(c>='a'&&c<='z')
    return c-'a'+2+26;
  if(c>='0'&&c<='9')
    return c-'0'+2+26+26;
  if(c=='+')
    return 2+26+26+10;
  if(c=='/')
    return 2+26+26+10+1;
  return 0;
}
/*=========================================================

=========================================================*/
ByteArray Base64::decode(const String &buffer){
  String
    tmp;
  ByteArray
    res;

  tmp=PSTRING(buffer)
    .replace('\r','\0')
    .replace('\n','\0')
    .trim();

  if(!tmp.len())
    return res;

  int
    len=tmp.len();
  char
    *ps=tmp.ptr(),
    *target,
    *pt=target=cMemAllocChar(len*2),
    *pe=&ps[len],
    rem=0;

  while(ps<pe){
    unsigned char
      *a=(unsigned char*)ps,
      *p=(unsigned char*)pt,
      c0=-2+resolve(a[0],&rem),
      c1=-2+resolve(a[1],&rem),
      c2=-2+resolve(a[2],&rem),
      c3=-2+resolve(a[3],&rem);


    p[0]=(c0<<2)+((c1&0x30)>>4);
    p[1]=((c1&0xF)<<4)+((c2&0x3C)>>2);
    p[2]=((c2&0x3)<<6)+c3;

    ps+=4;
    pt+=3;
  }
  if(rem)
    *(pt-1)=0;
  {
    int
      len=pt-target;

    pe-=1;
    while(*pe=='='){
      len-=1;
      pe--;
    }

    res.set(target,len);
    cMemFree(target);
  }
  return res;
}
/*=========================================================

=========================================================*/
char Base64::isBase64(const String &buffer){
  char
    *p=PCHAR(PSTRING(buffer).ptr());

  if(!p)
    return 0;
  if(cStrLen(p)%4)
    return 0;
  while(p&&*p){
    char
      ok=0
        ||(*p>='A'&&*p<='Z')
        ||(*p>='a'&&*p<='z')
        ||(*p>='0'&&*p<='9')
        ||(*p=='+')
        ||(*p=='/')
        ||(*p=='=');

    if(!ok)
      return 0;
    p++;
  }
  return 1;
}
