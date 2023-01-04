
#define BEAN(clazz,var,name)\
  clazz &var=*(clazz*)getFactory().getBean(name,""#clazz"");

class WeParam;

#include "header/WeCommand.h"
#include "header/WeParam.h"
#include "header/WeBean.h"
#include "header/WeTag.h"
#include "header/WeExec.h"
#include "header/WeParse.h"

#include "source/WeCommand.cpp"
#include "source/WeParam.cpp"
#include "source/WeBean.cpp"
#include "source/WeTag.cpp"
#include "source/WeExec.cpp"
#include "source/WeParse.cpp"
