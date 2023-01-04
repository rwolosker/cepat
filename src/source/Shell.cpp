/*=========================================================

=========================================================*/
Shell::Shell(){
}
Shell::~Shell(){
}
/*=========================================================

=========================================================*/
char Shell::exec(const String &cmd,File &out){
  int
    pid,status;
  Split
    arg;

  arg.add("/bin/bash");
  arg.add("-c");
  arg.add(PSTRING(cmd));
  pid=status=0;
  pid=fork();
  if(pid==0){
    dup2(out.getF(),1);
    dup2(out.getF(),2);
    execv("/bin/bash",arg.ptr());
    exit(0);
  }
  else
    waitpid(pid,&status,0);
  out.seek(out.size());
  return 1;
}
