/*=========================================================

=========================================================*/
Type::Type(){
}
Type::~Type(){
}
/*=========================================================

=========================================================*/
String Type::name(int type){
  switch (type){
    case C_VOID:return "VOID";
    case C_PCHAR:return "PCHAR";
    case C_NUMBER:return "NUMBER";
    case C_PTR:return "POINTER";
    case C_OBJ:return "OBJECT";
    case C_NULL:return "NULL";
    case C_INT:return "INT";
    case C_DOUBLE:return "DOUBLE";
    case C_FUNCTION:return "FUNCTION";
    case C_METHOD:return "METHOD";
    case C_EVALUATE:return "EVALUATE";
    case C_SET:return "SET";
    case C_CHAR:return "CHAR";
    case C_BOOL:return "BOOL";
    case C_DATE:return "DATE";
    case C_TEXT:return "TEXT";
    case C_DATETIME:return "DATETIME";
  }
  return "nao sei.";

}
/*=========================================================

=========================================================*/
int Type::type(cchar *name){
  String
    type=name;

  if(type.equals("METHOD",1))
    return C_METHOD;
  if(type.equals("VOID",1))
    return C_VOID;
  if(type.equals("INT",1))
    return C_INT;
  if(type.equals("INTEGER",1))
    return C_INT;
  if(type.equals("STR",1))
    return C_PCHAR;
  if(type.equals("PCHAR",1))
    return C_PCHAR;
  if(type.equals("NUMBER",1))
    return C_NUMBER;
  if(type.equals("POINTER",1))
    return C_PTR;
  if(type.equals("OBJECT",1))
    return C_OBJ;
  if(type.equals("EVAL",1))
    return C_EVALUATE;
  if(type.equals("SET",1))
    return C_SET;
  if(type.equals("CHAR",1))
    return C_CHAR;
  if(type.equals("BOOL",1))
    return C_BOOL;
  if(type.equals("BOOLEAN",1))
    return C_BOOL;
  if(type.equals("DATE",1))
    return C_DATE;
  if(type.equals("TEXT",1))
    return C_TEXT;
  if(type.equals("DATETIME",1))
    return C_DATETIME;
  if(type.equals("DOUBLE",1))
    return C_DOUBLE;
  return 0;
}
