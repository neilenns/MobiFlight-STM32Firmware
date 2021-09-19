#include <mbed.h>

static BufferedSerial serial_port(USBTX, USBRX, 115200);

FileHandle *mbed::mbed_override_console(int fd)
{
  return &serial_port;
}

int main()
{
  printf("Hello world!\n");

  // put your setup code here, to run once:

  while (1)
  {
    // put your main code here, to run repeatedly:
  }
}