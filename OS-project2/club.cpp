#include <stdio.h>
#include <string.h>
#include <string>
#include "iostream"
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "defs.h"

#define MAX_PLAYERS 22
#define MAX_NAME_LENGTH 50
#define MAX_POSITION_LENGTH 2

using namespace std;


struct Player {
    char name[20];
    char position[3];
    int age;
};

Player players[MAX_PLAYERS];
int num_players;

void read_csv(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: cannot open file %s\n", filename);
        exit(1);
    }

    char line[1024];
    int count = 0;

    // read each line of the file
    while (fgets(line, 1024, fp)) {
        if (count >= MAX_PLAYERS) {
            printf("Error: too many players in file %s\n", filename);
            exit(1);
        }

        int line_pos = 0;
        int player_pos = 0;
        while (line[line_pos] != ',')
            players[count].name[player_pos++] = line[line_pos++];
        players[count].name[player_pos] = '\0';

        player_pos = 0;
        line_pos++;

        while (line[line_pos] != ',')
            players[count].position[player_pos++] = line[line_pos++];
        players[count].position[player_pos] = '\0';

        player_pos = 0;
        line_pos++;

        char age_str[2];
        while (line_pos < strlen(line)-1) {
            age_str[player_pos++] = line[line_pos++];
        }

        players[count].age = atoi(age_str);

        count++;
    }

    fclose(fp);
    num_players = count;
}

void write_age_to_pipe(int age, string position) {
    char pipe_name[2];
    strcpy(pipe_name,position.c_str());

    int fd = open(pipe_name, O_WRONLY | O_NONBLOCK);

    while (fd == -1){
        fd = open(pipe_name, O_WRONLY | O_NONBLOCK);
    }

    write(fd, &age, sizeof(age));
    close(fd);
}

int main(int argc, char *argv[]) {
    Pos pos;
    char buffer[50];
    int valread = read(STDIN_FILENO, buffer, sizeof(buffer));
    buffer[valread] = '\0';

    strcpy(pos.all_pos, buffer);
    pos.count = strlen(buffer)/2;

    read_csv(argv[0]);

    for (int i = 0; i < num_players; i++) {
        for (int j = 0; j < pos.count; j++) {
            char position[2];
            pos.getPos(position, j);
            if (strcmp(players[i].position, position) == 0) {
                write_age_to_pipe(players[i].age, position);
            }
        }
    }

    return 0;

}