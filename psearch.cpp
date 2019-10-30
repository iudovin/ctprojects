#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <vector>

const int BUF_SIZE = 4096;

void dfs(int * fd) {
    char buf2[BUF_SIZE] = {0};
    DIR *dir = opendir(".");
    for (auto rd = readdir(dir); rd != NULL; rd = readdir(dir)) {
        if (rd->d_name[0] == '.') continue;
        if (rd->d_type == DT_DIR) {
            chdir(rd->d_name);
            dfs(fd);
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
            strcat(str, "@");
            strcat(str, rd->d_name);
            write(fd[1], str, sizeof(str));
        }
    }
}

std::vector<std::pair<int, int> > kmp(char * s1, char * t) {
    char s[BUF_SIZE] = {0};
    strcpy(s, s1);
    int m = strlen(s);
    strcat(s, "@");
    strcat(s, t);
    int n = strlen(s);
    std::vector<std::pair<int, int> > ans;
    std::vector<int> pi(n);
    pi[0] = 0;
    int line = 1, pos = 0;
    for (int i = 1; i < n; ++i) {
        pos++;
        if (s[i-1] == '\n') {
            line++;
            pos = 1;
        }
        int j = pi[i-1];
        while (j > 0 && s[i] != s[j])
            j = pi[j-1];
        if (s[i] == s[j]) j++;
        pi[i] = j;
        if (pi[i] == m) {
            std::pair<int, int> lp = std::make_pair(line, pos);
            ans.push_back(lp);
        }
    }
    return ans;
}

int main() {
    char pattern[BUF_SIZE] = {0};
    scanf("%s", pattern);
    int fd[2];
    pipe(fd);
    pid_t pid = fork();	
    if (pid == 0) {
		close(fd[0]);
        dfs(fd);
    } else {
		close(fd[1]);
        int rd;
        char buf[BUF_SIZE] = {0};
        while ((rd = read(fd[0], buf, sizeof(buf))) > 0) {
            std::vector<std::pair<int, int> > k = kmp(pattern, buf);
            int m = k.size();
            if (m > 0) {
				printf("Pattetn found in:\n");
                int n = strlen(buf);
                int i = n;
                while (buf[i-1] != '@') {
                    i--;
                }
                char name[BUF_SIZE] = {0};
                int j = 0;
                while (i < n) {
                    name[j] = buf[i];
                    i++;
                    j++;
                }
                printf(" %s\n", name);
                for (j = 0; j < m; ++j)
                    printf("  line: %d pos: %d\n", k[j].first, k[j].second);
			}
        }
    }
}
