/*
lab 6: keyboard controller
made by L1ttl3S1st3r
*/

#include <dos.h>

struct DWORD
{
  unsigned char word1;
  unsigned char word2;
};

unsigned char attributesOfBits = 0x50;

/**********************************************************************************************
********************************** output functions *******************************************
**********************************************************************************************/
void showString(char* string)
{
  struct DWORD far * screenPointer = (struct DWORD far* ) MK_FP(0xb800, 0);

  screenPointer += 14 * 80;

  while (*string != '\0')
  {
    screenPointer->word1 = *string;
    screenPointer->word2 = attributesOfBits;

    string++;
    screenPointer++;
  }
}

void showError(char * errorText)
{
  struct DWORD far * screenPointer = (struct DWORD far* ) MK_FP(0xb800, 0);

  screenPointer += 15 * 80 + 6;

  while (*errorText != '\0')
  {
    screenPointer->word1 = *errorText;
    screenPointer->word2 = attributesOfBits;

    errorText++;
    screenPointer++;
  }
}

// shows byte in line in hex format
void showByteHex(const char line, unsigned char byte)
{
  unsigned char highPart = byte / 16;
  unsigned char lowPart = byte & 0x0F;

  struct DWORD far * screenPointer = (struct DWORD far* ) MK_FP(0xb800, 0);

  screenPointer += 15 * 80 + line * 3;

  screenPointer->word1 = highPart < 10? highPart + '0' : highPart - 10 + 'a';
  screenPointer->word2 = attributesOfBits;
  screenPointer++;

  screenPointer->word1 = lowPart < 10? lowPart + '0' : lowPart - 10 + 'a';
  screenPointer->word2 = attributesOfBits;
  screenPointer++;
}

// shows state of registers and ect
void refresh()
{
  showString("content of keyboard data register:");
  showByteHex(1, inp(0x60));
}

/***************************************************************************************************
******************************* keyboard controller functions **************************************
****************************************************************************************************/
enum KeyboardFunctionsResults
{
  OK = 0, TIMEOUT = 1, ERROR = 2
};

// stop execution, while keyboard buffer is not free
int waitFreeBuffer()
{
  long int timeout = 50000;
  unsigned int byte;

  // decrement timeout while keyboard buffer isn't free
  while(timeout --> 0)
  {
    byte = inp(0x64);

    if(byte & 0x02)
    {
      break;
    }
  }

  if (timeout == 0)
  {
    return TIMEOUT;
  }
  else
  {
    return OK;
  }
}

int lightCommand()
{
  outp(0x60, 0xed);
  
  if (inp(0x60) == 0xfe)
  {
    return ERROR;
  }
  else
  {
    return OK;
  }
}

// scroll, num, caps can be {0, 1}
// switchs on, of scroll, num, caps lights
int setLights(char scroll, char num, char caps)
{
  unsigned char mask = (scroll | (num << 1) | (caps << 2)) & 7;

  outp(0x60, mask);

  if (inp(0x60) == 0xfe)
  {
    return ERROR;
  }
  else
  {
    return OK;
  }
}

// makes caps, scroll and num lock blink
void makeLigthsBlink()
{
  char animation[12] = {1, 0, 0,
                        1, 1, 0, 
                        1, 1, 1, 
                        0, 0, 0};
  int attempts;
  int counter;

  for (counter = 0; counter < 10; counter += 3)
  {
    
    if(waitFreeBuffer() == TIMEOUT)
    {
      showError("error: keyboard timeout");
    }

    attempts = 3;

    do
    {
      if (lightCommand() == OK)
      {
        break;
      }
    } while (attempts --> 0);

    if (attempts == 0)    
    {
      showError("error: blink doesn\'t work");
      return;
    }

    if(waitFreeBuffer() == TIMEOUT)
    {
      showError("error: keyboard timeout");
    }

    refresh();

    attempts = 3;

    do
    {
      if(setLights(animation[counter], animation[counter + 1], animation[counter + 2]) == OK)
      {
        break;
      }
    } while (attempts --> 0);

    if (attempts == 0)    
    {
      showError("error: blink doesn\'t work");
      return;
    }

    refresh();
  
    delay(250);
  }
}

/************************************************************************************
****************************** interrupts *******************************************
************************************************************************************/
void interrupt (*old09Interrupt) (); // default keyboard interrupt

void interrupt mine09Interrupt (void) // new keyboard interrupt
{
  unsigned char code = inp(0x60); 

  attributesOfBits = 0x50 | (++attributesOfBits & 0x0F);

  // make lights blink if 'b' has pressed
  if (code == 0x30)
  {
    makeLigthsBlink();
  }

  refresh();

  old09Interrupt();
}

// inits interrupt controller
void initController()
{
  // read & save old interrupts vectors
  old09Interrupt = getvect(0x09);

  // set new interrupts vectors
  setvect(0x09, mine09Interrupt);

  // disable interrupt handling for a while
  _disable();

  // init master
  outp(0x20, 0x11); // ends with 1, because ICW4 will be used
  outp(0x21, 0x08); // address of new master base vector
  outp(0x21, 0x04); // 0x04 = 00000100b, slave is connected in IRQ2
  outp(0x21, 0x0d); // type of mp is 8086, eoi is handled by program,
		    // it's a slave, use bufferisation

  _enable();
}


int main(void)
{
  // for resident prog
  char far * fp;

  // init interrupt controller
  initController();

  FP_SEG(fp) = _psp;
  FP_OFF(fp) = 0x2c;
  _dos_freemem(*fp);
  _dos_keep(0, (_DS - _CS) + (_SP / 16) + 1);

  return 0;
}
