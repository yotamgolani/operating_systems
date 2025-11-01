#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char* buf;
    ssize_t len;
} chars;

/* open source file (Read-only) */
int open_source(char* source){
    int fd = 0;
    fd = open(source, O_RDONLY);
    /* Handle syscall error */
    if (fd < 0){
        printf("Error opening source file: %s\n", strerror(errno));
        return -1;
    }
    printf("Opened source: %d\n", fd);
    return fd;
}

/* open target file (Write-only) */
int open_target(char* target){
    int fd = 0;
    /* check if file exists */
    fd = open(target, O_RDONLY);
    if (fd >= 0){
        printf("Error: Target file exists!\n");
        close(fd);
        return -1;
    }
    close(fd);
    /* create the target for writing */
    fd = open(target, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd < 0){
        printf("Error creating target file: %s\n", strerror(errno));
        return -1;
    }
    printf("Opened target: %d\n", fd);
    return fd;
}

void read_n_from_source(int fd, chars* curr, int n){
    curr->len = read(fd, curr->buf, n);
    if (curr->len < 0){
        printf("Error reading source file: %s\n", strerror(errno));
        return;
    }
    return;
}

void close_files(int fd1, int fd2){
    printf("Closing\n");
    close(fd1);
    close(fd2);
}

int main(int argc, char* argv[]){
    int source_fd = 0;
    int target_fd = 0;
    int n;
    chars curr = {NULL, 1};
    ssize_t written;
    if (argc != 4){
        printf("Incorrect number of arguments!\n");
        printf("Usage: copy SOURCE TARGET\n");
        return -1;
    }
    source_fd = open_source(argv[1]);
    target_fd = open_target(argv[2]);
    if (target_fd < 0){
        printf("Closing source\n");
        close(source_fd);
        return -1;
    }
    /* allocate n bytes */
    sscanf(argv[3], "%d", &n);
    if (n < 0){
        printf("Problem parsing 'n'\n");
        close_files(source_fd, target_fd);
        return -1;
    }
    curr.buf = (char*) malloc(n);
    if (curr.buf == NULL){
        printf("Error allocating memory!\n");
        close_files(source_fd, target_fd);
        return -1;
    }
    /* from this point the 2 fds are open */
    while(curr.len > 0){
        read_n_from_source(source_fd, &curr, n);
        /* handle error while reading */
        if (curr.len < 0){
            close_files(source_fd, target_fd);
            return -1;
        }
        written = write(target_fd, curr.buf, curr.len);
        /* handle error while writing */
        if (written < 0){
            printf("Error writing to target file: %s\n", strerror(errno));
            close_files(source_fd, target_fd);
            return -1;
        }
    }
    close_files(source_fd, target_fd);
    return 0;
}
