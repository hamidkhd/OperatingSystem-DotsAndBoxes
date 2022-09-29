#include "client.h"
#include "game.h"

void set_player_name()
{
    if (player.turn == 1)
        player.name = 'A';
    else if (player.turn == 2)
        player.name = 'B';
    else if (player.turn == 3)
        player.name = 'C';
    else if (player.turn == 4)
        player.name = 'D';
}

void sig_handler(int signum)
{
	write(1, "Your time is up!\n", 18);
}

int main(int argc, char const *argv[]) 
{
    int opt = 1;
    int valread;
    char buffer[1024] = {0}; 
    char input_buff[1024] = {0}; 
    int broadcast = 1; 

    int sock_fd, _inet_pton, _connect, new_socket;
    struct sockaddr_in sock_addr;    
    int addr_len = sizeof(sock_addr); 

    if ((sock_fd = socket(AF_INET, SOCK_STREAM, IP_PROTOCOL)) < 0)
    {    
        write(1, "TCP client socket is fail.\n", 23);
		exit(EXIT_FAILURE); 
    }

	sock_addr.sin_family = AF_INET; 
	sock_addr.sin_port = htons(atoi(argv[1]));
	
	if (inet_pton(AF_INET, IP_ADDRESS, &sock_addr.sin_addr) <= 0)
    {
        write(1, "Address is fail.\n", 17);
		exit(EXIT_FAILURE); 
    }

	if (connect(sock_fd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0)
    {
        write(1, "Connection is fail.\n", 16);
		exit(EXIT_FAILURE); 
    } 

    write(1, "Enter the number of group members:\n", 36);
    read(0, input_buff, 1024);
	send(sock_fd, input_buff, strlen(input_buff),0);

    read(sock_fd , buffer, 1024); 

    char _port[4], _turn[1], _map_size[1];
    int port, turn, map_size;

    for (int i = 0; i < 6; i++)
    {
        if (i < 4)
            _port[i] = buffer[i];
        if (i == 4)
        {
            _port[i] = '\0';  
            port = atoi(_port); 
            _map_size[0] = buffer[i];
            _map_size[1] = '\0'; 
            map_size = atoi(_map_size);
        }
        if (i == 5)
        {
            _turn[0] = buffer[i];    
            _turn[1] = '\0';
            turn = atoi(_turn);
        }
    }           

    player.turn = turn;
    player.score = 0;
    player.award = 0;
    player.error = 0;
    player.table[0] = 0;
    player.table[1] = 0;
    player.table[2] = 0;
    player.table[3] = 0;
    player.map_size = map_size;
    player.level_num = (map_size-1) * (map_size-1);
    player.map = make_map(map_size);
    set_player_name();

    if ((new_socket = socket(AF_INET, SOCK_DGRAM, IP_PROTOCOL)) < 0)
    {
        write(1, "udp client socket failed.\n", 23);
	    exit(EXIT_FAILURE); 
    }
    
    if (setsockopt(new_socket, SOL_SOCKET, SO_BROADCAST, (char *)&broadcast, sizeof(broadcast)) < 0) 
    {
        write(1, "set socket opt1 failed.\n", 24);
	    exit(EXIT_FAILURE); 
    }

    if (setsockopt(new_socket, SOL_SOCKET, SO_REUSEPORT, (char *)&opt, sizeof(opt)) < 0) 
    {
        write(1, "set socket opt2 failed.\n", 24);
	    exit(EXIT_FAILURE); 
    }

    sock_addr.sin_family = AF_INET; 
    sock_addr.sin_port = htons(port);
    sock_addr.sin_addr.s_addr = inet_addr("255.255.255.255");

    if (bind(new_socket, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0) 
    { 
	    write(1, "Bind is fail.\n", 14);
	    exit(EXIT_FAILURE); 
    } 

    print_map();

    int counter = 1;
    while(1)
    {
        int status = 0;
        char send_buffer[1024] = "Time"; 
        char recieve_buffer[1024] = "Time"; 

        if (player.error == 1)
        {
            player.error = 0;
            counter--;
        }

        else if (player.award == 1)
            counter--;

        if ((counter % (player.map_size - 1)) == (player.turn % (player.map_size - 1)))
        {
            write(1, "Please select new line:\n", 25);

            siginterrupt(SIGALRM, 1);
	        signal(SIGALRM, sig_handler);
            alarm(20);
            read(0, send_buffer, 1024);
            alarm(0);

            if (strcmp(send_buffer, "Time") != 0)
            {
                send_buffer[3] = player.turn + '0';
                send_buffer[4] = '\0';
            }

            if (sendto(new_socket, send_buffer, 1024, 0, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0)
                write(1, "Send data is fail.\n", 19);
            
            if (recvfrom(new_socket, recieve_buffer, 1024, 0, NULL, 0) < 0)
                write(1, "Recieve data is fail.\n", 22);

            //write(1, "New line selected!\n", 20);
        }
        else
        {
            write(1, "Wait for select of other players!\n", 35);
            if (recvfrom(new_socket, recieve_buffer, 1024, 0, NULL, 0) < 0)
                write(1, "Recieve data is fail.\n", 22);

            //write(1, "New line from other player selected!\n", 38);
        }

        if (strcmp(recieve_buffer, "Time") != 0)
        {
            recieve_buffer[4] = '\0';
            int row = recieve_buffer[0]-'0', column = recieve_buffer[2]-'0';
            player.id = recieve_buffer[3]-'0';
            status = game(row, column);
        }
        if (status == 1)
            break;
        counter++;
    }
    return 0;
}