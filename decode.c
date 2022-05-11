#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void cipherDecode(char *s, int firstRowIdx);

int main() {
    int firstRowIdx;
    char string[1000];

    /* starting pipe */
    char *myfifo = "/tmp/myfifo";
    mkfifo(myfifo, 0666);
    int fd = open(myfifo, O_RDONLY);
    read(fd, string, 1000);
    close(fd);

    /* finding first row index */
    int j = 0;
    while (*(string + j)) {
        if (*(string + j) > 96 && *(string + j) < 123) break;
        j++;
    }

    firstRowIdx = j;
    cipherDecode(string, firstRowIdx);

    /* sending back the decoded string */
    fd = open(myfifo, O_WRONLY);
    write(fd, string, strlen(string) + 1);
    close(fd);

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