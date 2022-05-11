#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    int fd1;
    char compatible = 1;

    // FIFO file path
    char *myfifo = "/tmp/myfifo4";

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

    int i;
    for (i = 0; *(str1 + i); i++)
        if (*(str1 + i) == '@') break;
    i++;
    //    @1253$5125
    //    @i...$j...
    //     1    6
    int j;
    for (j = i; *(str1 + j); j++)
        if (*(str1 + j) == '$') break;
    j++;

    int k;
    for (k = j; *(str1 + k); k++)
        if (*(str1 + k) == '!') break;
    k++;

    int l;
    for (l = k; *(str1 + l); l++)
        if (*(str1 + l) == '&') break;
    l++;

    char tmp[10];

    strncpy(tmp, str1 + i, j - i - 1);
    int firstRowIdx = atoi(tmp);

    //
    strncpy(tmp, str1 + j, k - j - 1);
    int dimention = atoi(tmp);

    strncpy(tmp, str1 + k, l - k - 1);
    int height = atoi(tmp);

    char *pChar = str1 + l;
    int width = atoi(tmp);

    str1[i - 1] = '\0';

    // write(fd, inputString, strlen(inputString) + 1);

    // close(fd);

    char table[100][100];
    int a = firstRowIdx;
    for (int b = 0; b < dimention; b++) {
        for (int c = 0; c < dimention; c++) table[b][c] = *(str1 + a + c);
        a += 10;  // skipping the '#'
    }
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

    fd1 = open(myfifo, O_WRONLY);

    write(fd1, &compatible, 1);
    close(fd1);
    return 0;
}
