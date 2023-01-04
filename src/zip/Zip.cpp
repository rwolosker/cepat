ZIP_LIB
  cZipLib={0};
/*=========================================================

=========================================================*/
void Zip::initZip(void){
  zip=0;
}
Zip::Zip(){
  initZip();
}
Zip::~Zip(){
  reset();
}
/*=========================================================

=========================================================*/
Zip &Zip::reset(void){
  if(zip)
    cZipLib.zipClose(zip,0);
  file.close();
  gc.reset();
  initZip();
  return *this;
}
/*=========================================================

=========================================================*/
Zip &Zip::create(void){
  reset();
  file.temp();
  zip=cZipLib.zipOpen(file.getName(),0);
  return *this;
}
/*=========================================================

=========================================================*/
Zip &Zip::append(cchar *name,ByteArray &buffer){
  ZIP_FILE_INFO
    info;
  int
    ok;
  Date
    agora=Date().now();

  cMemReset(&info,sizeof(ZIP_FILE_INFO));
  info.tmz_date.tm_sec=agora.getSecond();
  info.tmz_date.tm_min=agora.getMinute();
  info.tmz_date.tm_hour=agora.getHour();
  info.tmz_date.tm_mday=agora.getDay();
  info.tmz_date.tm_mon=agora.getMonth()-1;
  info.tmz_date.tm_year=agora.getYear();

  ok=cZipLib.zipOpenNewFileInZip(zip,name,&info,0,0,0,0,0,8,9);
  if(!ok&&buffer.len()){
    cZipLib.zipWriteInFileInZip(zip,buffer.ptr(),buffer.len());
    cZipLib.zipCloseFileInZip(zip);
  }
  return *this;
}
/*=========================================================

=========================================================*/
Zip &Zip::toStream(ByteArray &buffer){
  buffer.reset();
  cZipLib.zipClose(zip,0);
  zip=0;
  file.readAll(buffer);
  return reset();
}
/*=========================================================

=========================================================*/
UNZIP_FILE *Zip::fromStream(ByteArray &buffer){
  ZIP_GLOBAL_INFO
    ginfo;
  UNZIP_FILE_INFO
    info;
  String
    name;
  UNZIP_FILE
    *res=0;

  reset();
  file.temp();
  file.write(buffer);
  file.sync();
  zip=cZipLib.unzOpen(file.getName());
  cZipLib.unzGetGlobalInfo(zip,&ginfo);
  cZipLib.unzGoToFirstFile(zip);

  res=(UNZIP_FILE*)gc.add(cMemAlloc(
    sizeof(UNZIP_FILE)*ginfo.number_entry
  ));

  for(uint i=0;i<ginfo.number_entry;i++){
    cZipLib.unzGetCurrentFileInfo(zip,&info,0,0,0,0,0,0);
    cZipLib.unzGetCurrentFileInfo(zip,&info,name.reset().resize(info.size_filename+1).ptr(),info.size_filename,0,0,0,0);
    res[i].count=ginfo.number_entry;
    res[i].name=PCHAR(gc.add(cMemAddChar(name.ptr())));
    res[i].size=info.uncompressed_size;
    res[i].date=Date().set(
      info.tmu_date.tm_year,
      info.tmu_date.tm_mon+1,
      info.tmu_date.tm_mday,
      info.tmu_date.tm_hour,
      info.tmu_date.tm_min,
      info.tmu_date.tm_sec);
    cZipLib.unzOpenCurrentFile(zip);
    cZipLib.unzReadCurrentFile(
      zip,
      res[i].bin=gc.add(cMemAlloc(res[i].size)),
      res[i].size
    );
    cZipLib.unzCloseCurrentFile(zip);
    cZipLib.unzGoToNextFile(zip);
  }
  cZipLib.unzClose(zip);
  zip=0;
  return res;
}
/*=========================================================

=========================================================*/
Zip &Zip::init(cchar *zlib,cchar *ziplib){
  if(cZipLib.started)
    return *this;
  cZipLib.libz=cLibraryLoad(PCHAR(zlib));
  cZipLib.libzip=cLibraryLoad(PCHAR(ziplib));

  #define symbol(name)\
    *(void**)&cZipLib.name=cLibrarySymbol(cZipLib.libzip,#name);\
    if(!cZipLib.name){\
      cPrintf("ERRO: zip load: %s n",#name);\
      exit(1);\
    }

  symbol(zipOpen)
  symbol(zipClose)
  symbol(zipOpenNewFileInZip)
  symbol(zipWriteInFileInZip)
  symbol(zipCloseFileInZip)
  symbol(unzOpen)
  symbol(unzGetGlobalInfo)
  symbol(unzClose)
  symbol(unzGoToFirstFile)
  symbol(unzGetCurrentFileInfo)
  symbol(unzGoToNextFile)
  symbol(unzOpenCurrentFile)
  symbol(unzCloseCurrentFile)
  symbol(unzReadCurrentFile)


  #undef symbol
  cZipLib.started=1;
  return *this;
}
