#include "sptoto.h"
int serial;
sptoto toot(1);

void setup(){
  
  toot.sp_begin(0, 9600);    
}

void loop(){
 if(Serial.available()){
   serial = Serial.read();
  if (serial = 99)
  toot.pin_state(3);
  
 }
}
