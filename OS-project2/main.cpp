#include <stdio.h>
#include <string.h>
#include <string>
#include <dirent.h>
#include "iostream"
#include <unistd.h>
#include <sys/wait.h>
#include "defs.h"

#define MAX_NAME_LENGTH 50
#define MAX_LINE_LENGTH 1024
#define MAX_INPUT_LENGTH 100
#define COUNTRY_EXECUTABLE "./country.out"
#define POSITION_EXECUTABLE "./position.out"
#define READ 0
#define WRITE 1

using namespace std;



struct {
    string folder[10];
    string csvFiles;
    int folder_count;
}files;

void print_all_pos(const char* filename) {
    FILE *csv_file = fopen(filename, "r");
    if (csv_file == NULL) {
        printf("Error opening file %s\n", filename);
        return;
    }

    char buffer[MAX_LINE_LENGTH];
    char name[MAX_NAME_LENGTH];
    int name_pos = 0;

    fgets(buffer, sizeof(buffer), csv_file);
    int len = strlen(buffer);
    for (int i = 0; i < len; i++) {
        if (buffer[i] == ',') {
            name[name_pos++] = ' ';
            name[name_pos++] = '-';
            name[name_pos++] = ' ';
        } else {
            name[name_pos++] = buffer[i];
        }
    }

    name[name_pos] = '\0';

    printf("%s", name);
    name_pos = 0;

    printf("%c",'\n');
    fclose(csv_file);
}

void split_input_into_pos(Pos &pos) {
    char input[MAX_INPUT_LENGTH];
    printf("Enter up to 11 positions separated by spaces:\n");
    fgets(input, MAX_INPUT_LENGTH, stdin);
    input[strcspn(input, "\n")] = '\0'; // Remove newline character from input

    int pos_count = 0;
    int input_len = strlen(input);
    int i = 0;

    while (i < input_len && pos_count < 11) {
        if (isspace(input[i])) {
            i++;
            continue;
        }

        if (input[i+1] == '\0') {
            printf("Invalid position: %c\n", input[i]);
            i++;
            continue;
        }

        pos.all_pos[pos_count*2] = input[i];
        pos.all_pos[pos_count*2+1] = input[i+1];
        pos_count++;
        i += 2;
    }
    pos.count = pos_count;
}


void list_of_files(string directory){
    int count = 0;

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (directory.c_str())) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir(dir)) != NULL) {
            string temp(ent->d_name);
            if (temp == "." || temp == "..")
                continue;
            if (temp.find(".csv") != string::npos)
                files.csvFiles = directory + "/" + temp;
            else {
                files.folder[count] = directory + "/" + temp;
                count++;
            }
        }
        files.folder_count = count;
        closedir(dir);
    }
}

int exec_new_process(const string& executable, const string& arg, int& write_pipe) {
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
    }
    int pid = fork();
    if (pid == 0) {
        // Child process
        dup2(pipe_fd[READ], STDIN_FILENO);
        close(pipe_fd[WRITE]);
        close(pipe_fd[READ]);
        execl(executable.c_str(), arg.c_str(), NULL);
        perror("execl");
        exit(1);
    } else if (pid > 0) {
        // Parent process
        close(pipe_fd[READ]);
        write_pipe = pipe_fd[WRITE];
    }else{
        perror("fork");
    }
    return pid;
}


int main(int argc, char *argv[]) {
//    if (argc != 2) {
//        printf("Usage: %s <csv_file>\n", argv[0]);
//        return 1;
//    }
    Pos pos;
    list_of_files("../clubs");
    print_all_pos(files.csvFiles.c_str());
    split_input_into_pos(pos);

    int countries = files.folder_count;
    int positions = pos.count;
    int child_pids[countries + positions];

    for (int i = 0; i < countries; ++i) {
        int country_write_pipe;
        int country_pid = exec_new_process(COUNTRY_EXECUTABLE, files.folder[i], country_write_pipe);
        write(country_write_pipe, pos.all_pos, strlen(pos.all_pos));
        close(country_write_pipe);
        child_pids[i] = country_pid;
    }

    for (int i = 0; i < positions; ++i) {
        int position_write_pipe;
        char buffer[2];
        pos.getPos(buffer, i);
        int position_pid = exec_new_process(POSITION_EXECUTABLE, POSITION_EXECUTABLE, position_write_pipe);
        write(position_write_pipe, buffer, strlen(buffer));
        close(position_write_pipe);
        child_pids[countries + i] = position_pid;
    }

    for(auto& pid : child_pids) {
        int status;
        waitpid(pid, &status, 0);
    }

    return 0;
}
