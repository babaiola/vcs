#include "sptoto.h"

sptoto::sptoto(int address){
  addr = address;
}

void sptoto::sp_begin(int to_dev, int speedo){
  to = to_dev;  //address arduino
  Serial.begin(speedo);  //speed serial
}

void sptoto::pin_state(int pin){
  a=to;
  b=addr;
  c=1;
  d=pin;
  e=digitalRead(pin); 
  sp_write(a, b, c, d, e); 
}

void sptoto::sp_write(int A, int B, int C, int D, int E){
  Serial.write(A);
  Serial.write(B);
  Serial.write(C);
  Serial.write(D);
  Serial.write(E);
//  Serial.write(A+B+C+D+E);  
}
