#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

void solve(const char *input, int n, char *ouput) {
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
    
    
    if (last_found == -1) {
        strncpy(ouput, "Not found", 10);
    } else {
        memcpy(ouput, input + last_found, n);
        ouput[n] = '\0';
    }
}

const int BUFFER_SIZE = 10000;
const int RESULT_SIZE = 1000;

int main(int argc, char **argv) {
    char buffer[BUFFER_SIZE];
    int fd1[2], fd2[2];
    
    memset(buffer, 0, BUFFER_SIZE);
    
    pipe(fd1);
    pipe(fd2);

    if (argc != 4) {
        printf("Incorrect number of command line arguments!\n");
        return 0;
    }
    
    if (fork() != 0) {
        // Первый процесс
        close(fd1[0]);
        close(fd2[0]);
        close(fd2[1]);
        
        int input = open(argv[1], O_RDONLY);
        read(input, buffer, BUFFER_SIZE);
        write(fd1[1], buffer, BUFFER_SIZE);
    } else {
        if (fork() != 0) {
            // Второй процесс
            char result_buffer[RESULT_SIZE];
            close(fd1[1]);
            close(fd2[0]);
            memset(result_buffer, 0, RESULT_SIZE);
            
            int n = atoi(argv[3]);
            read(fd1[0], buffer, BUFFER_SIZE);
            solve(buffer, n, result_buffer);
            write(fd2[1], result_buffer, RESULT_SIZE);
        } else {
            // Третий процесс
            close(fd1[0]);
            close(fd1[1]);
            close(fd2[1]);
            
            int output = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
            read(fd2[0], buffer, BUFFER_SIZE);
            write(output, buffer, strlen(buffer));
        }
    }
    
    return 0;
}
