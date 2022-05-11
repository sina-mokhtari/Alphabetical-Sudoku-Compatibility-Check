#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FIFO_FILE "/tmp/fifo_twoway"

void cipherDecode(char *s, int firstRowIdx);

int main() {
    int fd1;

    // FIFO file path
    char *myfifo = "/tmp/myfifo";

    // Creating the named file(FIFO)
    // mkfifo(<pathname>,<permission>)
    mkfifo(myfifo, 0666);

    char str1[1000], str2[1000], str3[1000] = "got it : \n";
    // while (1) {
    // First open in read only and read
    fd1 = open(myfifo, O_RDONLY);
    read(fd1, str1, 1000);

    // Print the read string and close
    // printf("User1: %s\n", str1);
    close(fd1);

    /*assumption : firstRowIdx 1 digit*/
    // sprintf(tmp, "%d", )
    // strcpy(tmp, str1[strlen(str1) - 1]);
    int i;
    for (i = 0; *(str1 + i); i++)
        if (*(str1 + i) == '@') break;
    i++;
    // printf("\n%s\n", str1);

    char *pChar = str1 + i;

    int firstRowIdx = atoi(pChar);

    str1[i - 1] = '\0';
    // printf("\n%s\n", str1);
    cipherDecode(str1, firstRowIdx);
    // printf("\n%s\n", str1);

    // strcat(str3, str1);
    // strcpy(str2, str1);
    // Now open in write mode and write
    // string taken from user.
    fd1 = open(myfifo, O_WRONLY);
    // fgets(str2, 80, stdin);
    write(fd1, str1, strlen(str1) + 1);
    close(fd1);
    // }
    return 0;
}

void cipherDecode(char *s, int firstRowIdx) {
    int i = firstRowIdx;
    while (*(s + i)) {
        if (*(s + i) != 35) {
            if (*(s + i) > 98 && *(s + i) < 123)
                *(s + i) -= 2;
            else if (*(s + i) == 98)
                *(s + i) = 122;
            else if (*(s + i) == 97)
                *(s + i) = 121;
        }
        i++;
    }
}