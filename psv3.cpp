#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <cstring>
#include <vector>
#include <map>

const int BUF_SIZE = 4096;

void * func(void * someUselessInformation) {
  usleep(1000000);
  printf("a\n");
  return NULL;
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

std::map<const char*, bool> threads_in_dir;

void * dfs(void * fd) {
  char buf2[BUF_SIZE] = {0};
  DIR *dir = opendir(".");
  for (auto rd = readdir(dir); rd != NULL; rd = readdir(dir)) {
    if (rd->d_name[0] == '.') continue;
    if (rd->d_type == DT_DIR) {
      if (threads_in_dir[rd->d_name] != true) {
        chdir(rd->d_name);
        dfs(fd);
        chdir("..");
        threads_in_dir[rd->d_name] = true;
      }
    }
    if (rd->d_type == DT_REG) {
      printf("file: %s\n", rd->d_name);
      /*FILE * f;
      f = fopen(rd->d_name, "r");
      char str[BUF_SIZE] = {0};
      strcat(str, rd->d_name);
      strcat(str, "\n");
      char c;
      int i = 0;
      while ((c = fgetc(f)) > 0) {
        str[i] = c;
        i++;
      }
      write(fd[1], str, sizeof(str));*/
    }
  }
  return NULL;
}


int main(int argc, char ** argv) {
  int n = (argc > 1) ? atoi(argv[1]) : 1;
  std::vector<pthread_t> thrs(n);
  for (int i = 0; i < n; ++i) {
    pthread_create(&thrs[i], NULL, dfs, NULL);
  }
  for (int i = 0; i < n; ++i) {
    pthread_join(thrs[i], NULL);
  }
  for (auto it = threads_in_dir.begin(); it != threads_in_dir.end(); ++it)
    printf("%s\n", it->first);
}
