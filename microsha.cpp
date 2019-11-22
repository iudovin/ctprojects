#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>


const int BUF_SIZE = 4096;

enum CommandType {
    LS,
    CAT,
    CD,
    PWD,
    TIME,
    ECHO,
    SET,
    EXIT,
    HELP,
    UNKNOWN
};

struct Command {
    CommandType type;
    std::vector<std::string> args;
};

class Microsha {
public:
    void loop() {
        while (1) {
            std::string current_dir = get_current_dir();
            char invite_symbol = get_invite_symbol();
            printf("%s%c ", current_dir.c_str(), invite_symbol);
            Command command = get_command();
            if (command.type == EXIT)
                return;
            execute(command);
        }
    }
    
    
private:
    std::map<std::string, std::string> data;
    
    char get_invite_symbol() {
        return (getuid() == 0) ? '!' : '>';
    }
    
    std::string get_current_dir() {
        char buf[BUF_SIZE] = {0};
        getcwd(buf, sizeof(buf));
        // perror
        int n = sizeof(buf);
        int i = n;
        while (buf[i] != '/')
            i--;
        i++;
        std::string dir(buf + i, buf + n);
        return dir;
    }
    
    std::vector<std::string> get_split_line() {
        std::vector<std::string> v;
        char c;
        std::string s = "";
        while (1) {
            c = getchar();
            if (c == '\n') {
                if (s.size() > 0 || v.size() == 0)
                    v.push_back(s);
                break;
            } else if (c == ' ' || c == '\t') {
                if (s.size() > 0)
                    v.push_back(s);
                s = "";
            } else {
                s += c;
            }
        }
        return v;
    }
    
    Command get_command() {
        Command command;
        command.args = get_split_line();
        
        if (command.args[0] == "ls") {
            command.type = LS;
        } else if (command.args[0] == "cat") {
            command.type = CAT;
        } else if (command.args[0] == "cd") {
            command.type = CD;
        } else if (command.args[0] == "pwd") {
            command.type = PWD;
        } else if (command.args[0] == "time") {
            command.type = TIME;
        } else if (command.args[0] == "echo") {
            command.type = ECHO;
        } else if (command.args[0] == "set") {
            command.type = SET;
        } else if (command.args[0] == "exit") {
            command.type = EXIT;
        } else if (command.args[0] == "help") {
            command.type = HELP;
        } else if (command.args.size() > 0) {
            command.type = UNKNOWN;
        }
        
        return command;
    }
    
    void execute(Command command) {
        switch (command.type) {
            case LS: {
                break;
            } case CAT: {
                break;
            } case CD: {
                std::string dir = command.args[1];
                cd(dir);
                break;
            } case PWD: {
                pwd();
                break;
            } case TIME: {
                time(command.args);
                break;
            } case ECHO: {
                echo(command.args);
                break;
            } case HELP: {
                display_list_of_commands();
                break;
            } default: {
                if (command.args[0].size() > 0)
                printf("Unknown command: %s\ntype 'help' to see the list of commands.\n", command.args[0].c_str());
                break;
            }
        }
        return;
    }
    
    void ls(std::vector<std::string> args) {
        printf("executing: ls\n");
        return;
    }
    
    void cat(std::string file) {
        printf("executing: cat\n");
        return;
    }
    
    void cd(std::string dir) {
        chdir(dir.c_str());
        // perror
        return;
    }
    
    void pwd() {
        char buf[BUF_SIZE] = {0};
        getcwd(buf, sizeof(buf));
        // perror
        printf("%s\n", buf);
        return;
    }
    
    void time(std::vector<std::string> args) {
        printf("executing: time\n");
        return;
    }
    
    void echo(std::vector<std::string> args) {
        int n = args.size();
        for (int i = 1; i < n; ++i)
            printf("%s\n", args[i].c_str());
        return;
    }
    
    void set(std::string var, std::string value) {
        data[var] = value;
    }
    
    void display_list_of_commands() {
        printf("Welcome to Microsha! The following commands can be executed:\n\tcd   [arg]\n\tpwd\n\techo [arg1] [arg2] ...\n\ttime [arg1] [arg2] ...\n\tset  [VAR]=[value]\n\thelp\n\texit\n");
    }
};


int main() {
    
    Microsha msh;
    msh.loop();
    
    return 0;
}
