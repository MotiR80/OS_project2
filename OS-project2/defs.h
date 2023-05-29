#ifndef OS_PROJECT2_DEFS_H
#define OS_PROJECT2_DEFS_H
struct Pos{
    char all_pos[22] = {"\0"};
    int count;

    char* getPos(char buff[], int index){
        buff[0] = all_pos[2*index];
        buff[1] = all_pos[2*index+1];
        buff[2] = '\0';

        return buff;
    }
};
#endif //OS_PROJECT2_DEFS_H
