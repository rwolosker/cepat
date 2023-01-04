#define ZIP_CREATE 1
typedef struct PACK ZIP_LIB{
  void
    *libz,
    *libzip;
  char
    started;
  void
   *(*zipOpen)(cchar*,int),
   *(*unzOpen)(cchar*);
  int
    (*zipClose)(void*,cchar*),
    (*zipOpenNewFileInZip)(void*,cchar*,void*,void*,int,void*,int,cchar*,int,int),
    (*zipWriteInFileInZip)(void*,void*,int),
    (*zipCloseFileInZip)(void*),
    (*unzGetGlobalInfo)(void*,void*),
    (*unzClose)(void*),
    (*unzGoToFirstFile)(void*),
    (*unzGetCurrentFileInfo)(void*,void*,char*,long,void*,long,char*,long),
    (*unzGoToNextFile)(void*),
    (*unzOpenCurrentFile)(void*),
    (*unzCloseCurrentFile)(void*),
    (*unzReadCurrentFile)(void*,void*,int);
}ZIP_LIB;
typedef struct PACK ZIP_TIME{
  uint tm_sec;
  uint tm_min;
  uint tm_hour;
  uint tm_mday;
  uint tm_mon;//0-11
  uint tm_year;
}ZIP_TIME;
typedef struct PACK ZIP_FILE_INFO{
  ZIP_TIME
    tmz_date;
  ulong
    dosDate,
    internal_fa,
    external_fa;
}ZIP_FILE_INFO;
typedef struct PACK ZIP_GLOBAL_INFO{
  ulong number_entry;
  ulong size_comment;
}ZIP_GLOBAL_INFO;
typedef struct PACK UNZIP_FILE_INFO{
  ulong
    version,
    version_needed,
    flag,
    compression_method,
    dosDate,
    crc,
    compressed_size,
    uncompressed_size,
    size_filename,
    size_file_extra,
    size_file_comment,
    disk_num_start,
    internal_fa,
    external_fa;
  ZIP_TIME
    tmu_date;
}UNZIP_FILE_INFO;
typedef struct PACK UNZIP_FILE{
  int count;
  int size;
  char *name;
  double date;
  void *bin;
}UNZIP_FILE;


extern ZIP_LIB
  cZipLib;

class Zip:public Object{
  protected:
    void
      *zip;
    File
      file;
    Gc
      gc;
    void initZip(void);
  public:
    Zip();
    ~Zip();
    Zip &init(cchar *zlib,cchar *ziplib);
    Zip &reset(void);
    Zip &create(void);
    Zip &append(cchar *name,ByteArray &buffer);
    Zip &toStream(ByteArray &buffer);
    UNZIP_FILE *fromStream(ByteArray &buffer);
};

#include "Zip.cpp"
