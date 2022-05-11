#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    int fd1;
    char compatible[6] = "true";

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
            p = table[m][n];
            q = table[m][n + 1];
            for (int o = 1; o < height; o++)
                if (table[m + o][n] == p || table[m + o][n] == q ||
                    table[m + o][n + 1] == p || table[m + o][n + 1] == q ||
                    p == q)
                    strcpy(compatible, "false");
        }
    // strcat(str3, str1);
    char temp[15];

    sprintf(temp, "%d,%d,%d\n\n", dimention, height, width);
    strcpy(str2, compatible);
    // Now open in write mode and write
    // string taken from user.
    fd1 = open(myfifo, O_WRONLY);
    // fgets(str2, 80, stdin);
    write(fd1, str2, strlen(str2) + 1);
    close(fd1);
    return 0;
}
