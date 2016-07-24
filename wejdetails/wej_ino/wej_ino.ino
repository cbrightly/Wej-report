#include <SPI.h>
#include "Wej.h"

Wej *wej;
void setup()
{
  wej = new Wej;
  wej->start();
}


void loop()
{
  wej->task();  
}


