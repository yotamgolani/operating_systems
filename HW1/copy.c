#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#define N 1

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
    /* create the target for writing */
    fd = open(target, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd < 0){
        printf("Error creating target file: %s\n", strerror(errno));
        return -1;
    }
    return fd;
}

chars read_n_from_source(int fd){
    char buf[N];
    ssize_t len = read(fd, buf, N);
    if (len < 0){
        printf("Error reading source file: %s\n", strerror(errno));
        return (chars){NULL, len};
    }
    return (chars){buf, len};
}

int remaining_data(int fd){
    char c[1];
    ssize_t len;
    len = read(fd, c, 1);
    return len;
}

void close_files(int fd1, int fd2){
    close(fd1);
    close(fd2);
}

int main(int argc, char* argv[]){
    int source_fd = 0;
    int target_fd = 0;
    chars curr;
    ssize_t written;
    if (argc != 3){
        printf("Incorrect number of arguments!\n");
        printf("Usage: copy SOURCE TARGET");
    }
    source_fd = open_source(argv[1]);
    target_fd = open_target(argv[2]);
    return 0;
    /* from this point the 2 fds are open */
    while(remaining_data(source_fd)){
        curr = read_n_from_source(source_fd);
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
    return 0;
}
