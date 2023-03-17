#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>

#define BUFFER_SIZE 200
#define RESULT_SIZE 200
char result_buffer[RESULT_SIZE];

void solve(const char *input, int n) {
    int last_found = -1;
    int count = 1;
    int size = strlen(input);
    
    for (int i = 0; i < size; i++) {
        if (i != 0) {
            if (input[i] > input[i - 1]) {
                count++;
            } else {
                count = 1;
            }
        }
        
        if (count >= n) {
            last_found = i + 1 - n;
        }
    }
    
    if (last_found != -1) {
        memset(result_buffer, 0, RESULT_SIZE);
        memcpy(result_buffer, input + last_found, n);
        result_buffer[n] = '\0';
    }
}

int main(int argc, char **argv) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    memset(result_buffer, 0, RESULT_SIZE);
    result_buffer[0] = -1;
    char name1[] = "a.fifo";
    char name2[] = "b.fifo";

    mknod(name1, S_IFIFO | 0666, 0);
    mknod(name2, S_IFIFO | 0666, 0);

    if (argc != 2) {
        printf("Incorrect number of command line arguments!\n");
        return 0;
    }

    int fd1 = open(name1, O_RDONLY);
    int fd2 = open(name2, O_WRONLY);
    
    int n = atoi(argv[1]);

    while (true) {
        int bytes_read = read(fd1, buffer, BUFFER_SIZE);
        
        if (bytes_read == 0) {
            break;
        }
        if (buffer[0] == -1) {
            break;
        }
        
        solve(buffer, n);
        memset(buffer, 0, BUFFER_SIZE);
    }
    
    if (result_buffer[0] == -1) {
        strncpy(result_buffer, "Not found", 10);
    }
    
    write(fd2, result_buffer, RESULT_SIZE);

    return 0;
}
