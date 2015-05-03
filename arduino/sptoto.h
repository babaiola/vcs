#include<Arduino.h>

#ifndef sptoto_h
#define sptoto_h

class sptoto{
  
public:
  
  int addr;
  int to;
  int a, b, c, d, e;
 
  sptoto(int);
  
  void sp_begin(int, int); 

  void pin_state(int);

  void sp_write(int, int, int, int, int);

};





#endif
