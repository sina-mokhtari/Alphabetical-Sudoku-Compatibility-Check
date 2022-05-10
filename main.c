#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FIFO_FILE "/tmp/fifo_twoway"

void reverse_string(char *);

int main() {
    int parentPid = getpid(), firstChildPid = -10, secondChildPid = -10,
        thirdChildPid = -10, fourthChildPid = -10;
    int temp;

    //-----------------------------

    //------------------------------

    int pid = fork();

    if (pid != 0) {
        firstChildPid = pid;
        pid = fork();
        if (pid != 0) {
            secondChildPid = pid;
            pid = fork();
            if (pid != 0) {
                thirdChildPid = pid;
                pid = fork();
                if (pid != 0) {
                    fourthChildPid = pid;
                } else
                    fourthChildPid = getpid();
            } else
                thirdChildPid = getpid();
        } else
            secondChildPid = getpid();
    } else
        firstChildPid = getpid();

    // char *args[15];
    // temp = getpid();
    if (getpid() == parentPid) {
        int fd;

        // FIFO file path
        char *myfifo = "/tmp/myfifo";

        mkfifo(myfifo, 0666);

        char arr1[80], arr2[80]="yeay";
        //while (1) {
            // Open FIFO for write only
            fd = open(myfifo, O_WRONLY);

            write(fd, arr2, strlen(arr2) + 1);
            close(fd);

            fd = open(myfifo, O_RDONLY);

            read(fd, arr1, sizeof(arr1));

            printf("User2: %s\n", arr1);
            close(fd);
       // }
    }

    else if (getpid() == firstChildPid) {
        //  char *args[] = {"./firstChild", NULL};
        // execv(args[0], args);
        int fd1;

        // FIFO file path
        char *myfifo = "/tmp/myfifo";

        // Creating the named file(FIFO)
        // mkfifo(<pathname>,<permission>)
        mkfifo(myfifo, 0666);

        char str1[80], str2[80], str3[80] = "got it : ";
       // while (1) {
            // First open in read only and read
            fd1 = open(myfifo, O_RDONLY);
            read(fd1, str1, 80);

            // Print the read string and close
           // printf("User1: %s\n", str1);
            close(fd1);
            strcat(str3, str1);
            strcpy(str2, str3);
            // Now open in write mode and write
            // string taken from user.
            fd1 = open(myfifo, O_WRONLY);
            //fgets(str2, 80, stdin);
            write(fd1, str2, strlen(str2) + 1);
            close(fd1);
       // }
    } else if (getpid() == secondChildPid) {
        char *args[] = {"./secondChild", NULL};
        execv(args[0], args);
    } else if (getpid() == thirdChildPid) {
        char *args[] = {"./thirdChild", NULL};
        execv(args[0], args);
    } else if (getpid() == fourthChildPid) {
        char *args[] = {"./fourthChild", NULL};
        execv(args[0], args);
    }

    //                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            printf("parentPid : %d | firstChildPid : %d | secondChidPid : %d || currentPid : %d\n", parentPid, firstChildPid, secondChildPid, getpid());
    return 0;
}

void mainProcess() {
    int fd;
    char readbuf[80];
    char end[10];
    int to_end;
    int read_bytes;

    /* Create the FIFO if it does not exist */
    mkfifo(FIFO_FILE, S_IFIFO | 0640);
    strcpy(end, "end");
    fd = open(FIFO_FILE, O_RDWR);
    // while (1) {
    /*read_bytes = read(fd, readbuf, sizeof(readbuf));
    readbuf[read_bytes] = '\0';
    printf("FIFOSERVER: Received string: \"%s\" and length is %d\n",
           readbuf, (int)strlen(readbuf));
    to_end = strcmp(readbuf, end);

    if (to_end == 0) {
        close(fd);
        break;
    }
    reverse_string(readbuf);
    printf("FIFOSERVER: Sending Reversed String: \"%s\" and length is %d\n",
           readbuf, (int)strlen(readbuf));*/
    write(fd, readbuf, strlen(readbuf));
    /*
    sleep - This is to make sure other process reads this, otherwise this
    process would retrieve the message
    */
    sleep(2);
    //}
    return;
}

void reverse_string(char *str) {
    int last, limit, first;
    char temp;
    last = strlen(str) - 1;
    limit = last / 2;
    first = 0;

    while (first < last) {
        temp = str[first];
        str[first] = str[last];
        str[last] = temp;
        first++;
        last--;
    }
    return;
}