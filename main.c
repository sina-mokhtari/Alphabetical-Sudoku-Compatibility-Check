#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FIFO_FILE "/tmp/fifo_twoway"

void removeExtras(char *s);
void toLower(char *s);
int setDHW(char *s, int *dimention, int *height, int *width);
void cipherDecode(char *s, int firstRowIdx);

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

        char arr1[1000], arr2[80] = "yeay";

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
        char *myfifo2 = "/tmp/myfifo2";

        mkfifo(myfifo2, 0666);

        fd = open(myfifo2, O_WRONLY);
        sprintf(tmp, "@%d$%d", firstRowIdx, dimention);
        strcat(arr1, tmp);
        write(fd, arr1, strlen(arr1) + 1);
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
        strcat(arr1, tmp);
        write(fd, arr1, strlen(arr1) + 1);
        close(fd);

char newStr3[1000];
        fd = open(myfifo3, O_RDONLY);
        read(fd, newStr3, sizeof(newStr3));
        close(fd);


        printf("User2: %s\n", arr1);
        printf("secondchild: %s\n",newStr);
        printf("thirdchild: %s\n",newStr3);

    }

    else if (getpid() == decoderChildPid) {
        //  char *args[] = {"./firstChild", NULL};
        // execv(args[0], args);
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

        char *pChar = str1 + i;

        int firstRowIdx = atoi(pChar);

        str1[i - 1] = '\0';

        cipherDecode(str1, firstRowIdx);

        // strcat(str3, str1);
        strcpy(str2, str1);
        // Now open in write mode and write
        // string taken from user.
        fd1 = open(myfifo, O_WRONLY);
        // fgets(str2, 80, stdin);
        write(fd1, str2, strlen(str2) + 1);
        close(fd1);
        // }
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