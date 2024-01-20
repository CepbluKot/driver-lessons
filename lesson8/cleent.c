#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>

int main()
{
  int fd;
  char buf[100];
  fd = open("/dev/exampl", O_RDWR);

  read(fd, buf, 100);

  while (1)
  {
    printf("GOT FROM DRIVER:  %s \n", buf);
  }
  close(fd);
}