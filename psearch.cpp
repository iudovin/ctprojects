#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <vector>

const int BUF_SIZE = 4096;

// THAT WORKS!!!
void dfs(int * fd, int * fdd) {
    char buf2[BUF_SIZE] = {0};
    DIR *dir = opendir(".");
    for (auto rd = readdir(dir); rd != NULL; rd = readdir(dir)) {
        if (rd->d_name[0] == '.') continue;
        if (rd->d_type == DT_DIR) {
            chdir(rd->d_name);
            dfs(fd, fdd);
            chdir("..");
        }
        if (rd->d_type == DT_REG) {
            FILE * f;
            f = fopen(rd->d_name, "r");
            char str[BUF_SIZE] = {0};
            char c;
            int i = 0;
            while ((c = fgetc(f)) > 0) {
                str[i] = c;
                i++;
            }
            //strcat(str, "@");
            //strcat(str, rd->d_name);
            write(fd[1], str, sizeof(str));
            write(fdd[1], rd->d_name, sizeof(rd->d_name));
        }
    }
}

int kmp(char * s1, char * t) {  // s = string, t = text
	// returns number of entries of s in t
	// > 0 if s is a substring of t
    char s[BUF_SIZE] = {0};
    strcpy(s, s1);
    int m = strlen(s);
    strcat(s, "@");
    strcat(s, t);
    int n = strlen(s);
    int ans = 0;
    std::vector<int> pi(n);
    pi[0] = 0;
    for (int i = 1; i < n; ++i) {
        int j = pi[i-1];
        while (j > 0 && s[i] != s[j])
            j = pi[j-1];
        if (s[i] == s[j]) j++;
        pi[i] = j;
        if (pi[i] == m) {
            ans++;
        }
    }
    return ans;
}

int main() {
    char pattern[BUF_SIZE] = {0};
    scanf("%s", pattern);
    close(0);
    close(1);
    int fd[2];
    int fdd[2];
    pipe(fd);
    pipe(fdd);
    pid_t pid = fork();
    if (pid == 0) {
		close(fd[0]);
        close(fdd[0]);
        dfs(fd, fdd);
    } else {
		close(fd[1]);
        close(fdd[1]);
        int rd, rdd;
        char buf[BUF_SIZE] = {0};
        char name[BUF_SIZE] = {0};
        while ((rd = read(fd[0], buf, sizeof(buf))) > 0 && (rdd = read(fdd[0], name, sizeof(name))) > 0) {
            if (kmp(pattern, buf) > 0) {
				fprintf(stderr, "Pattetn found in %s\n", name);
			}
            fprintf(stderr, "reading:  %s\n", name);
        }
        fprintf(stderr, "pipe is read\n");
    }
}
