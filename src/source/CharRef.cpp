/*=========================================================

=========================================================*/
CharRef::CharRef(){
  ptr=0;
}
CharRef::~CharRef(){
}
/*=========================================================

=========================================================*/
void CharRef::set(char *ptr){
  this->ptr=ptr;
}
CharRef::operator char(void){
  return *ptr;
}
char CharRef::operator=(char c){
  return *ptr=c;
}
char CharRef::operator==(char value){
  return *ptr==value;
}
char CharRef::operator!=(char value){
  return *ptr!=value;
}
char CharRef::operator>=(char value){
  return *ptr>=value;
}
char CharRef::operator>(char value){
  return *ptr>value;
}
char CharRef::operator<=(char value){
  return *ptr<=value;
}
char CharRef::operator<(char value){
  return *ptr<=value;
}
/*=========================================================

=========================================================*/
char CharRef::operator==(int value){
  return *ptr==value;
}
char CharRef::operator!=(int value){
  return *ptr!=value;
}
char CharRef::operator>=(int value){
  return *ptr>=value;
}
char CharRef::operator>(int value){
  return *ptr>value;
}
char CharRef::operator<=(int value){
  return *ptr<=value;
}
char CharRef::operator<(int value){
  return *ptr<=value;
}
/*=========================================================

=========================================================*/
char CharRef::get(void){
  return *ptr;
}
