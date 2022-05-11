#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void removeExtras(char *s);
void toLower(char *s);

int main() {
    int parentPid = getpid(), decoderChildPid, rowCheckChildPid,
        colCheckChildPid, subRectCheckChildPid;
    char strWithSuffix[1000];

    int pid = fork();  // creating first child (decoder)

    if (pid != 0) {
        decoderChildPid = pid;
        pid = fork();  // creating row check child
        if (pid != 0) {
            rowCheckChildPid = pid;
            pid = fork();  // creating col check child
            if (pid != 0) {
                colCheckChildPid = pid;
                pid = fork();  // creating sub rect child
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
        char *inputString, decodedString[1000];
        long length;

        /* converting file to string */
        FILE *f = fopen("testcase1.txt", "rb");

        if (f) {
            fseek(f, 0, SEEK_END);
            length = ftell(f);
            fseek(f, 0, SEEK_SET);
            inputString = malloc(length);
            if (inputString) {
                fread(inputString, 1, length, f);
            }
            fclose(f);
        }
        /* converting file to string */

        removeExtras(inputString);
        toLower(inputString);

        /*  decoder child communication begin */
        char *myfifo = "/tmp/myfifo";
        mkfifo(myfifo, 0666);
        int fd = open(myfifo, O_WRONLY);
        write(fd, inputString, strlen(inputString) + 1);
        close(fd);

        free(inputString);

        fd = open(myfifo, O_RDONLY);
        read(fd, decodedString, sizeof(decodedString));
        close(fd);
        /*  decoder child communication end */

        /*  row check child communication begin */
        char *myfifo2 = "/tmp/myfifo2";
        mkfifo(myfifo2, 0666);
        fd = open(myfifo2, O_WRONLY);
        write(fd, decodedString, strlen(decodedString) + 1);
        close(fd);

        char rowCompatible;
        fd = open(myfifo2, O_RDONLY);
        read(fd, &rowCompatible, 1);
        close(fd);
        /*  row check child communication end */

        /*  col check child communication begin */
        char *myfifo3 = "/tmp/myfifo3";
        mkfifo(myfifo3, 0666);
        fd = open(myfifo3, O_WRONLY);
        write(fd, decodedString, strlen(decodedString) + 1);
        close(fd);

        char colCompatible;
        fd = open(myfifo3, O_RDONLY);
        read(fd, &colCompatible, 1);
        close(fd);
        /*  col check child communication end */

        /*  sub rect child communication begin */
        char *myfifo4 = "/tmp/myfifo4";
        mkfifo(myfifo4, 0666);
        fd = open(myfifo4, O_WRONLY);
        write(fd, decodedString, strlen(decodedString) + 1);
        close(fd);

        char subRectCompatible;
        fd = open(myfifo4, O_RDONLY);
        read(fd, &subRectCompatible, 1);
        close(fd);
        /*  sub rect child communication end */

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
        char *args[] = {"./decode", NULL};
        execv(args[0], args);
    } else if (getpid() == rowCheckChildPid) {
        char *args[] = {"./rowCheck", NULL};
        execv(args[0], args);
    } else if (getpid() == colCheckChildPid) {
        char *args[] = {"./colCheck", NULL};
        execv(args[0], args);
    } else if (getpid() == subRectCheckChildPid) {
        char *args[] = {"./subRectCheck", NULL};
        execv(args[0], args);
    }
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