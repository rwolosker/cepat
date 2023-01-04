typedef struct PACK GString{
  char
    *str;
  int
    len,
    size;
}GString;


extern int glib_major_version;
extern int glib_micro_version;
extern int glib_minor_version;

extern"C"{
  void  *g_date_new(void);
  void   g_date_set_day(void*,int);
  void   g_date_set_month(void*,int);
  void   g_date_set_year(void*,int);

  int    g_date_strftime(char*,int,cchar*,void*);

  void  *g_date_time_add_days(void*,int);
  void  *g_date_time_format(void*,cchar*);
  void  *g_date_time_new(void*,int,int,int,int,int,double);
  void  *g_date_time_unref(void*);
  void   g_free(void*);
  void   g_strfreev(char**);
  char **g_strsplit(cchar*,cchar*,int);
  int    g_strv_length(char**);
  char  *g_time_zone_get_identifier(void*);
  int    g_time_zone_get_offset(void*,int);
  void  *g_time_zone_new_local(void);//setenv TZ
  void   g_time_zone_unref(void*);
}
