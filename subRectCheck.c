#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int setParams(char *s, int *dimention, int *height, int *width);

int main() {
    int firstRowIdx, dimention, height, width;
    char compatible = 1;
    char string[1000];

    /* starting pipe */
    char *myfifo = "/tmp/myfifo4";
    mkfifo(myfifo, 0666);
    int fd = open(myfifo, O_RDONLY);
    read(fd, string, 1000);
    close(fd);

    firstRowIdx = setParams(string, &dimention, &height, &width);

    /* creating a 2d array from the string */
    char table[100][100];
    int a = firstRowIdx;
    for (int b = 0; b < dimention; b++) {
        for (int c = 0; c < dimention; c++) table[b][c] = *(string + a + c);
        a += dimention + 1;  // skipping the '#'
    }

    /* checking each sub rectangle */
    int p, q;
    for (int m = 0; m < dimention; m += height)
        for (int n = 0; n < dimention; n += width) {
            for (int o = 0; o < height; o++) {
                for (int r = 0; r < width; r++) {
                    p = table[m + o][n + r];
                    for (int s = 0; s < height; s++) {
                        for (int t = 0; t < width; t++) {
                            if (p == table[m + s][n + t] && (s != o || t != r))
                                compatible = 0;
                        }
                    }
                }
            }
        }

    /* sending back the result */
    fd = open(myfifo, O_WRONLY);
    write(fd, &compatible, 1);
    close(fd);
    return 0;
}

int setParams(char *s, int *dimention, int *height, int *width) {
    int i = 0, j;
    char temp[10];
    while (*(s + i)) {
        if (*(s + i) == '*') break;
        i++;
    }

    strncpy(temp, s, i);
    *dimention = atoi(temp);

    j = i + 1;
    while (*(s + j)) {
        if (*(s + j) == '*') break;
        j++;
    }

    strncpy(temp, s + i + 1, j - i - 1);
    *width = atoi(temp);

    i = j + 1;

    while (*(s + i)) {
        if (*(s + i) < 48 || *(s + i) > 57) break;
        i++;
    }

    strncpy(temp, s + j + 1, i - j - 1);
    *height = atoi(temp);

    // first row index in buffer
    return i;
}