#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_NAME "file.fifo"
#define MAX_LENGTH 1000

int main()
{
  char str[MAX_LENGTH];
  int num, fd;

  /* create a FIFO special file with name FIFO_NAME */
  mkfifo(FIFO_NAME, 0666);
  fd = open(FIFO_NAME, O_WRONLY);

  /* open the FIFO file for writing. open() blocks for readers */
  printf("waiting for readers...");
  fflush(stdout);

  printf("got a reader !\n");
  
  printf("Enter text to write in the FIFO file: ");
  fgets(str, MAX_LENGTH, stdin);
  while(!(feof(stdin))){
    
    if ((num = write(fd, str, strlen(str))) == -1)
      perror("write");
    else
      printf("producer: wrote %d bytes\n", num);
    fgets(str, MAX_LENGTH, stdin);
  }
}
