TableUnique::TableUnique(){}
TableUnique::~TableUnique(){}
char TableUnique::unique(Sort &sort){
  ByteArray
    &buffer=*(ByteArray*)sort.getArg();
  Table
    &copy=*(Table*)buffer.readPointer(),
    &reg=*(Table*)buffer.readPointer();
  String
    id=sort.getParamString(0),
    regName=sort.getParamString(1);
  int
    x;

  x=copy.sort(regName).find(
    regName,
    reg.getStr(regName).me()
  );
  if(!FOUND(x))
    return 1;
  copy.go(x);
  return copy.getStr(id).equals(reg.getStr(id));
}
