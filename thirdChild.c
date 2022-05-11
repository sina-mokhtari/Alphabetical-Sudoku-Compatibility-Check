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
    char *myfifo = "/tmp/myfifo3";

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
    for (j = 0; *(str1 + j); j++)
        if (*(str1 + j) == '$') break;
    j++;

    char tmp[10];
    strncpy(tmp, str1 + i, j - i - 1);

    char *pChar = str1 + j;

    int firstRowIdx = atoi(tmp);

    int dimention = atoi(pChar);

    str1[i - 1] = '\0';

    // write(fd, inputString, strlen(inputString) + 1);

    // close(fd);

    char table[100][100];
    int k = firstRowIdx;
    for (int l = 0; l < dimention; l++) {
        for (int m = 0; m < dimention; m++) table[l][m] = *(str1 + k + m);
        k += 10;  // skipping the '#'
    }


    for (int l = 0; l < dimention; l++)
        for (int m = 0; m < dimention; m++)
            for (int n = m + 1; n < dimention; n++)
                if (table[n][l] == table[m][l]) {
                    strcpy(compatible, "false");
                    break;
                }

    // strcat(str3, str1);
    strcpy(str2, compatible);
    // Now open in write mode and write
    // string taken from user.
    fd1 = open(myfifo, O_WRONLY);
    // fgets(str2, 80, stdin);
    write(fd1, str2, strlen(str2) + 1);
    close(fd1);
    return 0;
}
