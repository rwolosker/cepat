#define SWAP(a,b,size)\
do{\
  int __size=(size);\
  char *__a=(a),*__b=(b);\
  do{\
    char __tmp=*__a;\
    *__a++= *__b;\
    *__b++=__tmp;\
  }while(--__size>0);\
}while(0)
#define MAX_THRESH 4
typedef struct PACK stack_node{
    char *lo;
    char *hi;
}stack_node;

#define STACK_SIZE (8*sizeof(int))
#define PUSH(low,high) ((void)((top->lo=(low)),(top->hi=(high)),++top))
#define POP(low,high)  ((void)(--top,(low = top->lo),(high = top->hi)))
#define STACK_NOT_EMPTY (stack<top)
#define min(x, y) ((x)<(y)?(x):(y))
//https://sourceware.org/git/?p=glibc.git;a=blob;f=stdlib/qsort.c;hb=HEAD
void cQuicksort(void *pbase,int total_elems,int size,void *cmp,void *arg){
  const int
    max_thresh=MAX_THRESH*size;
  char
    *base_ptr=(char*)pbase;
  int
    (*pcmp)(void *a,void *b,void *arg)=0;

  *(void**)&pcmp=cmp;


  if(total_elems==0)
    return;
  if(total_elems>MAX_THRESH){
    char
      *lo=base_ptr,
      *hi=&lo[size*(total_elems-1)];
    stack_node
      stack[STACK_SIZE],
      *top=stack;

    PUSH(0,0);
    while(STACK_NOT_EMPTY){
      char
        *left_ptr,
        *right_ptr,
        *mid=lo+size*((hi-lo)/size>>1);

      if(pcmp(VOID(mid),VOID(lo),arg)<0)
        SWAP(mid,lo,size);
      if(pcmp(VOID(hi),VOID(mid),arg)<0)
        SWAP(mid,hi,size);
      else
        goto jump_over;
      if(pcmp(VOID(mid),VOID(lo),arg)<0)
        SWAP(mid,lo,size);
      jump_over:
      left_ptr=lo+size;
      right_ptr= hi-size;

      do{
        while(pcmp(VOID(left_ptr),VOID(mid),arg)<0)
          left_ptr+=size;
        while(pcmp(VOID(mid),VOID(right_ptr),arg)<0)
          right_ptr-=size;
        if(left_ptr < right_ptr){
          SWAP(left_ptr,right_ptr,size);
          if(mid==left_ptr)
            mid=right_ptr;
          else
            if(mid==right_ptr)
              mid=left_ptr;
          left_ptr+=size;
          right_ptr-=size;
        }
        else
          if(left_ptr==right_ptr){
            left_ptr+=size;
            right_ptr-=size;
            break;
          }
      }while(left_ptr<=right_ptr);
      if((int)(right_ptr-lo)<=max_thresh){
        if((int)(hi-left_ptr)<=max_thresh)
          POP(lo,hi);
        else
          lo=left_ptr;
      }
      else
        if((int)(hi-left_ptr)<=max_thresh)
          hi=right_ptr;
        else
          if((right_ptr-lo)>(hi-left_ptr)){
            PUSH(lo, right_ptr);
            lo=left_ptr;
          }
          else{
            PUSH(left_ptr,hi);
            hi=right_ptr;
          }
    }
  }
  {
    char
      *end_ptr=&base_ptr[size*(total_elems-1)],
      *tmp_ptr=base_ptr,
      *thresh=min(end_ptr, base_ptr + max_thresh),
      *run_ptr;

    for(run_ptr=tmp_ptr+size;run_ptr<=thresh;run_ptr+=size)
      if(pcmp(VOID(run_ptr),VOID(tmp_ptr),arg)<0)
        tmp_ptr=run_ptr;
    if(tmp_ptr!=base_ptr)
      SWAP(tmp_ptr,base_ptr,size);
    run_ptr = base_ptr + size;
    while((run_ptr+=size)<=end_ptr){
      tmp_ptr=run_ptr-size;
      while(pcmp(VOID(run_ptr),VOID(tmp_ptr),arg)<0)
        tmp_ptr-=size;
      tmp_ptr+=size;
      if(tmp_ptr!=run_ptr){
        char
          *trav=run_ptr+size;

        while(--trav>=run_ptr){
          char
            c=*trav,
            *hi,*lo;

          for(hi=lo=trav;(lo-=size)>=tmp_ptr;hi=lo)
            *hi=*lo;
          *hi=c;
        }
      }
    }
  }
}
#undef SWAP
#undef MAX_THRESH
#undef STACK_SIZE
#undef PUSH
#undef POP
#undef STACK_NOT_EMPTY
#undef min
