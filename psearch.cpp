#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>

const int BUF_SIZE = 4096;

// THAT WORKS!!!
void dfs(int * fd) {
    char buf2[BUF_SIZE] = {0};
    fprintf(stderr, "Now I'm in %s\n", getcwd(buf2, sizeof(buf2)));
    DIR *dir = opendir(".");
    for (auto rd = readdir(dir); rd != NULL; rd = readdir(dir)) {
        if (rd->d_type == DT_DIR) {
            if (rd->d_name[0] == '.') continue;
            fprintf(stderr, "I'm going to %s\n", rd->d_name);
            chdir(rd->d_name);
            dfs(fd);
            chdir("..");
        }
        if (rd->d_type == DT_REG) {
            fprintf(stderr, "%s ", rd->d_name);
            FILE * f;
            f = fopen(rd->d_name, "r");
            char str[BUF_SIZE] = {0};
            char c;
            int i = 0;
            while ((c = fgetc(f)) > 0) {
                str[i] = c;
                i++;
            }
            write(fd[1], str, sizeof(str));
        }
    }
}


int main() {
    close(0);
    close(1);
    int fd[2];
    pipe(fd);
    pid_t pid = fork();
    if (pid == 0) {
        dfs(fd);
    } else {
        int rd;
        char buf[BUF_SIZE] = {0};
        while ((rd = read(fd[0], buf, sizeof(buf))) > 0) {
            // KMP buf, t
            fprintf(stderr, "< reading\n");
        }
        fprintf(stderr, "pipe is read\n");
    }
    //int fd[2] = {0};
    //dfs(fd);
}
