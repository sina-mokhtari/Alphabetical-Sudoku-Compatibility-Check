#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void removeExtras(char *s);
void toLower(char *s);
int setDHW(char *s, int *dimention, int *height, int *width);

int main() {
    int parentPid = getpid(), decoderChildPid = -10, rowCheckChildPid = -10,
        colCheckChildPid = -10, subRectCheckChildPid = -10;
    int temp;

    int pid = fork();

    if (pid != 0) {
        decoderChildPid = pid;
        pid = fork();
        if (pid != 0) {
            rowCheckChildPid = pid;
            pid = fork();
            if (pid != 0) {
                colCheckChildPid = pid;
                pid = fork();
                if (pid != 0) {
                    subRectCheckChildPid = pid;
                } else
                    subRectCheckChildPid = getpid();
            } else
                colCheckChildPid = getpid();
        } else
            rowCheckChildPid = getpid();
    } else
        decoderChildPid = getpid();

    if (getpid() == parentPid) {
        char *inputString = 0;
        long length;
        int dimention, height, width;
        int firstRowIdx;

        FILE *f = fopen("testcase1.txt", "rb");

        if (f) {
            fseek(f, 0, SEEK_END);
            length = ftell(f);
            fseek(f, 0, SEEK_SET);
            inputString = malloc(length);  // needs to be freed
            if (inputString) {
                fread(inputString, 1, length, f);
            }
            fclose(f);
        }

        removeExtras(inputString);

        toLower(inputString);

        firstRowIdx = setDHW(inputString, &dimention, &height, &width);

        int fd;

        // FIFO file path
        char *myfifo = "/tmp/myfifo";

        mkfifo(myfifo, 0666);

        char arr1[1000], arr2[80], arr3[1000], arr4[1000];

        fd = open(myfifo, O_WRONLY);

        char tmp[10];
        sprintf(tmp, "@%d", firstRowIdx);
        strcat(inputString, tmp);

        write(fd, inputString, strlen(inputString) + 1);

        close(fd);

        fd = open(myfifo, O_RDONLY);

        read(fd, arr1, sizeof(arr1));
        close(fd);

        // second child
        char myArr2[1000], myArr3[1000], myArr4[1000];
        // strcpy(myArr2, arr1);
        strcpy(myArr2, arr1);
        strcpy(myArr3, arr1);
        strcpy(myArr4, arr1);
        char *myfifo2 = "/tmp/myfifo2";

        mkfifo(myfifo2, 0666);
        fd = open(myfifo2, O_WRONLY);
        sprintf(tmp, "@%d$%d", firstRowIdx, dimention);
        strcat(myArr2, tmp);
        write(fd, myArr2, strlen(myArr2) + 1);
        close(fd);

        char newStr[1000];
        fd = open(myfifo2, O_RDONLY);
        read(fd, newStr, sizeof(newStr));
        close(fd);

        // third child
        char *myfifo3 = "/tmp/myfifo3";

        mkfifo(myfifo3, 0666);

        fd = open(myfifo3, O_WRONLY);
        sprintf(tmp, "@%d$%d", firstRowIdx, dimention);
        strcat(myArr3, tmp);
        write(fd, myArr3, strlen(myArr3) + 1);
        close(fd);

        char newStr3[1000];
        fd = open(myfifo3, O_RDONLY);
        read(fd, newStr3, sizeof(newStr3));
        close(fd);

        // fourth child
        char *myfifo4 = "/tmp/myfifo4";

        mkfifo(myfifo4, 0666);

        fd = open(myfifo4, O_WRONLY);
        sprintf(tmp, "@%d$%d!%d&%d", firstRowIdx, dimention, height, width);
        strcat(myArr4, tmp);
        write(fd, myArr4, strlen(myArr4) + 1);
        close(fd);

        char newStr4[1000];
        fd = open(myfifo4, O_RDONLY);
        read(fd, newStr4, sizeof(newStr4));
        close(fd);

        printf("User2: %s\n", arr1);
        printf("secondchild: %s\n", newStr);
        printf("thirdchild: %s\n", newStr3);
        printf("fourthchild: %s\n", newStr4);

    }

    else if (getpid() == decoderChildPid) {
        char *args[] = {"./firstChild", NULL};
        execv(args[0], args);

    } else if (getpid() == rowCheckChildPid) {
        char *args[] = {"./secondChild", NULL};
        execv(args[0], args);
    } else if (getpid() == colCheckChildPid) {
        char *args[] = {"./thirdChild", NULL};
        execv(args[0], args);
    } else if (getpid() == subRectCheckChildPid) {
        char *args[] = {"./fourthChild", NULL};
        execv(args[0], args);
    }

    //                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            printf("parentPid : %d | firstChildPid : %d | secondChidPid : %d || currentPid : %d\n", parentPid, firstChildPid, secondChildPid, getpid());
    return 0;
}

void removeExtras(char *s) {
    char *d = s;
    do
        while (*d == ' ' || *d == '\t' || *d == '\r' || *d == '\n') ++d;

    while (*s++ = *d++);
}

void toLower(char *s) {
    while (*s) {
        if (*s > 64 && *s < 91) *s += 32;
        s++;
    }
}

int setDHW(char *s, int *dimention, int *height, int *width) {
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
    *height = atoi(temp);

    i = j + 1;

    while (*(s + i)) {
        if (*(s + i) < 48 || *(s + i) > 57) break;
        i++;
    }

    strncpy(temp, s + j + 1, i - j - 1);
    *width = atoi(temp);

    // first row index in buffer
    return i;
}