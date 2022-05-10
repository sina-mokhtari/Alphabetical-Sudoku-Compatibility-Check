#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FIFO_FILE "/tmp/fifo_twoway"

int main()
{
    printf("1st Child file | pid : %d\n", getpid());
    //-----------
      int fd;
   int end_process;
   int stringlen;
   int read_bytes;
   char readbuf[80];
   char end_str[5];
  // printf("FIFO_CLIENT: Send messages, infinitely, to end enter \"end\"\n");
   fd = open(FIFO_FILE, O_CREAT|O_RDWR);
   strcpy(end_str, "end");
   
   while (1) {
      
    /*  stringlen = strlen(readbuf);
      readbuf[stringlen - 1] = '\0';
      end_process = strcmp(readbuf, end_str);
      */
      //printf("end_process is %d\n", end_process);
    /*  if (end_process != 0) {
         write(fd, readbuf, strlen(readbuf));
         printf("FIFOCLIENT: Sent string: \"%s\" and string length is %d\n", readbuf, (int)strlen(readbuf));
         read_bytes = read(fd, readbuf, sizeof(readbuf));
         readbuf[read_bytes] = '\0';
         printf("FIFOCLIENT: Received string: \"%s\" and length is %d\n", readbuf, (int)strlen(readbuf));
      } else {
         write(fd, readbuf, strlen(readbuf));
         printf("FIFOCLIENT: Sent string: \"%s\" and string length is %d\n", readbuf, (int)strlen(readbuf));
         close(fd);
         break;
      }*/
      read_bytes = read(fd, readbuf, sizeof(readbuf));
      readbuf[read_bytes] = '\0';
      printf("FIFOSERVER: Received string: \"%s\" and length is %d\n", readbuf, (int)strlen(readbuf));
      sleep(1);
   }
    //-----------
    return 0;
}
