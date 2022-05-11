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
    char tempStr[1000];

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

        // first child
        // FIFO file path
        char *myfifo = "/tmp/myfifo";

        mkfifo(myfifo, 0666);

        char decodedString[1000], arr2[80], arr3[1000], arr4[1000];

        fd = open(myfifo, O_WRONLY);

        char tmp[100];
        sprintf(tmp, "@%d$%d!%d&%d", firstRowIdx, dimention, height, width);
        strcat(inputString, tmp);

        write(fd, inputString, strlen(inputString) + 1);

        close(fd);

        fd = open(myfifo, O_RDONLY);

        read(fd, decodedString, sizeof(decodedString));
        close(fd);

        // second child

        strcpy(tempStr, decodedString);

        char *myfifo2 = "/tmp/myfifo2";

        mkfifo(myfifo2, 0666);
        fd = open(myfifo2, O_WRONLY);

        strcat(tempStr, tmp);
        write(fd, tempStr, strlen(tempStr) + 1);
        close(fd);

        char rowCompatible;
        fd = open(myfifo2, O_RDONLY);
        read(fd, &rowCompatible, 1);
        close(fd);

        // third child
        char *myfifo3 = "/tmp/myfifo3";

        mkfifo(myfifo3, 0666);

        fd = open(myfifo3, O_WRONLY);

        write(fd, tempStr, strlen(tempStr) + 1);
        close(fd);

        char colCompatible;
        fd = open(myfifo3, O_RDONLY);
        read(fd, &colCompatible, 1);
        close(fd);

        // fourth child
        char *myfifo4 = "/tmp/myfifo4";

        mkfifo(myfifo4, 0666);

        fd = open(myfifo4, O_WRONLY);

        write(fd, tempStr, strlen(tempStr) + 1);
        close(fd);

        char subRectCompatible;
        fd = open(myfifo4, O_RDONLY);
        read(fd, &subRectCompatible, 1);
        close(fd);

        /*printf("User2: %s\n", decodedString);
        printf("Row Compatible: %s\n", rowCompatible ? "yes" : "no");
        printf("Column Compatible: %s\n", colCompatible ? "yes" : "no");
        printf("Sub Rectangle Compatible: %s\n",
               subRectCompatible ? "yes" : "no");*/

        printf((rowCompatible && colCompatible && subRectCompatible)
                   ? "Sudoku Puzzle constraints satisfied\n"
                   : "Sudoku Puzzle is Wrong\n");

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