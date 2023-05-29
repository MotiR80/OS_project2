#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include "iostream"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <random>

#define MAX_POSITION_LENGTH 10
#define MAX_AGES 100

using namespace std;

int read_from_pipe(string position, int ages[]) {
    char pipe_name[100];
    strcpy(pipe_name,position.c_str());

    // create named pipe
    mkfifo(pipe_name, 0666);

    int fd = open(pipe_name, O_RDONLY);

    if (fd == -1) {
        perror("Error opening pipe in position.cpp");
        exit(1);
    }

    int count = 0;
    int age;
    ssize_t bytes_read;

    while ((bytes_read = read(fd, &age, sizeof(age))) > 0) {
        ages[count++] = age;
        usleep(50000);
    }

//    printf("Read %d ages from pipe %s\n", count, pipe_name);
    close(fd);

    // remove named pipe
    unlink(pipe_name);

    return count;
}

void calculate_age_stats(int ages[], int num_ages, string position) {
    int min_age = ages[0];
    int max_age = ages[0];
    float sum = 0;

    for (int i = 0; i < num_ages; i++) {
        if (ages[i] < min_age) {
            min_age = ages[i];
        }
        if (ages[i] > max_age) {
            max_age = ages[i];
        }
        sum += ages[i];
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(10, 400);
    usleep(dis(gen));

    printf("%s min age : %d\n", position.c_str(), min_age);
    printf("%s max age : %d\n", position.c_str(), max_age);
    printf("%s avg age : %0.2f\n", position.c_str(), sum / num_ages);
    printf("%s count : %d\n\n", position.c_str(), num_ages);

}

int main() {
    char buffer[MAX_POSITION_LENGTH];
    int valread = read(STDIN_FILENO, buffer, MAX_POSITION_LENGTH);
    buffer[valread] = '\0';


    int ages[MAX_AGES];
    int num_ages = read_from_pipe(buffer, ages);

    calculate_age_stats(ages, num_ages, buffer);

    return 0;
}

