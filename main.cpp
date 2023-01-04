#include "src/core.h"
#include "src/sk32/sk32.h"
#include "src/dat/dat.h"
#include "src/reflectionTable/reflection.h"
#include "src/mysql/mysql.h"
#include "src/tds/tds.h"
#include "src/webengine/engine.h"
#include "src/ldap/ldap.h"
#include "src/sqlite/SqLite.h"

void Main(int argc,char **argv){
}
int main(int argc,char **argv){
  Main(argc,argv);
  return cMemUsed();
}
