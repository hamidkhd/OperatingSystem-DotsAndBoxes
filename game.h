#ifndef _Game_
#define _Game_ 

#include "client.h"

struct game_info
{
    int turn;
    int award;
    int error;
    int id;
    char name;
    int score;
    int level_num;
    int map_size;
    char** map;
    int table[4];
} player;

char **make_map(int map_size);
void change_map(int row, int column);
void print_map();
void check_map();
char find_name(int id);
int game(int row, int coulmn);
void announce_winner();

#endif