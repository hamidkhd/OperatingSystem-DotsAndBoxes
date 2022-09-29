#include "game.h"

char **make_map(int map_size)
{
    int print_size = 2 * map_size - 1;

    char **print_map = (char **)malloc(print_size * sizeof(char *));
    for (int i = 0; i < print_size; i++)
        print_map[i] = (char*)malloc(print_size * sizeof(char));

    for (int i = 0; i < print_size; i++)
        for(int j = 0; j < print_size; j++)
            if (i % 2 == 0 && j % 2 == 0)
                print_map[i][j] = '*';
            else 
                print_map[i][j] = ' ';

    return (char**) print_map;
}

void change_map(int row, int column)
{   
    int print_size = player.map_size * 2 - 1;
    if (row < 0 || row > print_size || column < 0 || column > print_size)
    {
        player.error = 1;
        write(1, "Your input coordinates is wrong! please enter a new coordinates:\n", 67);
    }

    else if (player.map[row][column] != ' ')
    {
        player.error = 1;
        write(1, "Your input coordinates is repetitive! please enter a new coordinates:\n", 72);
    }

    else if (row % 2 == 1)
        player.map[row][column] = '|';
    else
        player.map[row][column] = '-';
}

void print_map()
{
    int print_size = 2 * player.map_size - 1;

	for (int i = 0; i < print_size; i++)
    {
		for (int j = 0; j < print_size; j++)
        {
            char temp[1];
            temp[0] = player.map[i][j];
            temp[1] = '\0';
            write(1, temp, 1);
        }
		write(1, "\n", 1);
    }
}

void check_map() 
{
    player.award = 0; 
    int print_size = 2 * player.map_size - 1;
	for (int i = 0; i < print_size; i++) 
		if (i % 2 == 1) 
			for (int j = 0; j < print_size; j++) 
				if (player.map[i - 1][j] == '-' && player.map[i + 1][j] == '-' && 
                    player.map[i][j - 1] == '|' && player.map[i][j + 1] == '|' && player.map[i][j] == ' ') 
                {
                    player.map[i][j] = find_name(player.id);
                    player.level_num--;
                    if (player.turn == player.id)
                    {
                        player.score++;
                    }
                    player.table[player.id-1]++;
                    player.award = 1;
                }
}

char find_name(int id)
{
    if (id == 1)
        return 'A';
    else if (id == 2)
        return 'B';
    else if (id == 3)
        return 'C';
    else if (id == 4)
        return 'D';
}

void announce_winner() 
{
    write(1, "End of game !!!\n", 17);

    char temp[2];
    temp[1] = '\0';

    int winner = player.id;
    for (int i = 0; i < player.map_size-1; i++)
    {
        if (player.table[i] > player.table[winner-1])
            winner = i+1;

        write(1, "Player:", 7);

        temp[0] = find_name(i+1);
        write(1, temp, 1);

        write(1, "\n", 2);

        temp[0] = player.table[i] + '0';
        write(1, temp, 1);

        write(1, "\n", 2);
    }
    write(1, "Winner:\n", 10);
    temp[0] = winner + '0';
    write(1, temp, 1);
    write(1, "\n", 2);
}

int game(int row, int coulmn)
{
    printf("%d %d \n", row, coulmn);
    if (row != -1 && coulmn != -1)
    {
        change_map(row, coulmn);
        check_map();
        print_map();
        if (player.error == 0)
            write(1, "Done!\n", 7);
        if (player.level_num == 0)
        {
            write(1, "End of the game!\n", 18);
            announce_winner();
            return 1;
        }
        else
            return 0;
    }
}