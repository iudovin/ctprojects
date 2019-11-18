#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <dirent.h>
#include "threadsafequeue.h"
#include <vector>
#include <string>


struct Data {
    int num_of_thrs;
    int thr_num;
    Queue<std::string> *q;
};

void * bfs(void * arg) {
    Data *data = (Data *) arg;
    int num_of_thrs = data->num_of_thrs;
    int thr_num = data->thr_num;
    Queue<std::string> *q = data->q;
    while (1) {
        std::string u = q->pop();
        usleep(100000);
        DIR *dir = opendir(u.c_str());
        for (auto rd = readdir(dir); rd != NULL; rd = readdir(dir)) {
            const char *name = rd->d_name;
            if (name[0] == '.') continue;
            if (rd->d_type == DT_DIR) {
                std::string v = u + "/" + name;
                q->push(v);
            }
            if (rd->d_type == DT_REG) {
                printf("\tfile: %s\n", name);
            }
        }
    }
}


int main(int argc, char ** argv) {
    chdir("..");
    
    const int n = (argc > 1) ? atoi(argv[1]) : 4;
    std::vector<pthread_t> thrs(n);
    std::vector<Data> args(n);
    
    Queue<std::string> q;
    q.push("TestDir");
    
    for (int i = 0; i < n; ++i) {
        args[i].num_of_thrs = n;
        args[i].thr_num = i;
        args[i].q = &q;
        pthread_create(&thrs[i], nullptr, bfs, &args[i]);
    }
    
    for (int i = 0; i < n; ++i)
        pthread_join(thrs[i], nullptr);
    
    return 0;
}
