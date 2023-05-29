#include <stdio.h>
#include <string.h>
#include "iostream"
#include <string>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>
#include "defs.h"

#define CLUB_EXECUTABLE "./club.out"
#define READ 0
#define WRITE 1

using namespace std;



struct {
    string file[10];
    int count;
}csvFiles;

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
            csvFiles.file[count] = directory + "/" + temp;
            count++;
        }
        csvFiles.count = count;
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
    Pos pos;
    char buffer[50];
    int valread = read(STDIN_FILENO, buffer, sizeof(buffer));
    buffer[valread] = '\0';

    strcpy(pos.all_pos, buffer);
    pos.count = strlen(buffer)/2;

//    char ss[3];
//    pos.getPos(ss, sizeof(ss), 0);
//    cout<<ss<<endl;

    list_of_files(argv[0]);


    int clubs = csvFiles.count;
    int child_pids[clubs];

    for (int i = 0; i < clubs; ++i) {
        int club_write_pipe;
        int club_pid = exec_new_process(CLUB_EXECUTABLE, csvFiles.file[i], club_write_pipe);
        write(club_write_pipe, pos.all_pos, strlen(pos.all_pos));
        close(club_write_pipe);
        child_pids[i] = club_pid;
    }

    for(auto& pid : child_pids) {
        int status;
        waitpid(pid, &status, 0);
    }

    return 0;

}