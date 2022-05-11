#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void setParams(char *string, int *dimention, int *firstRowIdx);

int main() {
    int firstRowIdx, dimention;
    char compatible = 1;
    char string[1000];

    char *myfifo = "/tmp/myfifo2";
    mkfifo(myfifo, 0666);
    int fd = open(myfifo, O_RDONLY);
    read(fd, string, 1000);
    close(fd);

    setParams(string, &dimention, &firstRowIdx);

    char table[100][100];
    int k = firstRowIdx;
    for (int l = 0; l < dimention; l++) {
        for (int m = 0; m < dimention; m++) table[l][m] = *(string + k + m);
        k += 10;  // skipping the '#'
    }

    for (int l = 0; l < dimention; l++)
        for (int m = 0; m < dimention; m++)
            for (int n = m + 1; n < dimention; n++)
                if (table[l][n] == table[l][m]) {
                    compatible = 0;
                    break;
                }

    fd = open(myfifo, O_WRONLY);
    write(fd, &compatible, 1);
    close(fd);
    return 0;
}

void setParams(char *string, int *dimention, int *firstRowIdx) {
    int i = 0, j = 0;
    char temp[100];

    while (*(string + i)) {
        if (*(string + i) == '*') break;
        i++;
    }

    strncpy(temp, string, i);
    *dimention = atoi(temp);

    while (*(string + j)) {
        if (*(string + j) > 96 && *(string + j) < 123) break;
        j++;
    }

    *firstRowIdx = j;
}