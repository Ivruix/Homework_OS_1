#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

const int BUFFER_SIZE = 200;
const int RESULT_SIZE = 200;

int main(int argc, char **argv) {
    char buffer[BUFFER_SIZE];
    char name1[] = "a.fifo";
    char name2[] = "b.fifo";
    
    memset(buffer, 0, BUFFER_SIZE);
    
    mknod(name1, S_IFIFO | 0666, 0);
    mknod(name2, S_IFIFO | 0666, 0);

    if (argc != 3) {
        printf("Incorrect number of command line arguments!\n");
        return 0;
    }

    int fd1 = open(name1, O_WRONLY);
    int fd2 = open(name2, O_RDONLY);
    
    int input = open(argv[1], O_RDONLY);

    int bytes_read = read(input, buffer, BUFFER_SIZE);
    while (bytes_read > 0) {
        write(fd1, buffer, BUFFER_SIZE);
        memset(buffer, 0, BUFFER_SIZE);
        bytes_read = read(input, buffer, BUFFER_SIZE);
    }
    memset(buffer, 0, BUFFER_SIZE);
    buffer[0] = -1;
    write(fd1, buffer, BUFFER_SIZE);

    int output = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0777);
    read(fd2, buffer, BUFFER_SIZE);
    write(output, buffer, strlen(buffer));
    
    return 0;
}
