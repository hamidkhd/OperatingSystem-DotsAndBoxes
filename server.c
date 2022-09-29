#include "server.h"

#define MAX_PLAYER 50

int count_digit(int number, int digit)
{
    if (number)
        return count_digit(number/10, ++digit);
    return digit;
}

char *int_to_char_array(int number)
{
	int digit = count_digit(number, 0);

    char* array = (char*)malloc(digit);
    
    if (number == 0)
    {
        array[0] = '0'; 
        return (char*) array;
    }
    else 
    {
        for (int i = 0; i < digit; i++) 
        {
            array[digit-i-1] = number % 10 + 48;
            number /= 10;
        }
    }
    return (char*) array; 
}

int initial_socket(struct sockaddr_in serv_addr)
{
	int serv_socket, opt = 1, addr_len = sizeof(serv_addr); 

	if ((serv_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
	{ 
		write(1, "Failed to build server socket.\n", 32);
		exit(EXIT_FAILURE); 
	} 

    if (setsockopt(serv_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) 
	{ 
		write(1, "Failed to set socket opt.\n", 27);
		exit(EXIT_FAILURE); 
	} 

	if (bind(serv_socket, (struct sockaddr *)&serv_addr, addr_len) < 0) 
	{ 
		write(1, "Failed to bind.\n", 17);
		exit(EXIT_FAILURE); 
	} 

    if (listen(serv_socket, 3) < 0) 
	{ 
		write(1, "Failed to listen.\n", 19);
		exit(EXIT_FAILURE); 
	} 

	return serv_socket;
}

int main(int argc, char const *argv[]) 
{
    int val_read;
    int clients_socket[MAX_PLAYER];
    char buffer[1025]; 

	struct sockaddr_in serv_addr;
	int addr_len = sizeof(serv_addr); 
	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_addr.s_addr = INADDR_ANY; 
	serv_addr.sin_port = htons(atoi(argv[1])); 

	int serv_socket = initial_socket(serv_addr);

    for (int i = 0; i < MAX_PLAYER; i++) 
	    clients_socket[i] = 0; 


	int port = atoi(argv[1]);
	fd_set read_fds; 
	int new_socket;

	int port2, port3, port4;
	int flag2, flag3, flag4;
	int group2[2], group3[3], group4[4];

	char input_buf[1024];

    while (1)
	{ 
		FD_ZERO(&read_fds); 	
		FD_SET(serv_socket, &read_fds); 
			
        int max_sd = serv_socket;  
		int save_loc;

		for (int i = 0 ; i < MAX_PLAYER ; i++) 
		{ 
			if(clients_socket[i] == 0) 
				save_loc = i;

			if(clients_socket[i] > 0) 
				FD_SET(clients_socket[i], &read_fds); 
				
			if(clients_socket[i] > max_sd) 
				max_sd = clients_socket[i]; 
		} 
	
	
		if ((select(max_sd + 1 , &read_fds , NULL , NULL , NULL) < 0) && (errno != EINTR)) 
            write(1, "Select is fail.\n", 17);
			
		if (FD_ISSET(serv_socket, &read_fds)) 
		{ 
			if ((new_socket = accept(serv_socket, (struct sockaddr *)&serv_addr, (socklen_t*)&addr_len)) < 0) 
			{ 
                write(1, "Accept is fail.\n", 17);
				exit(EXIT_FAILURE); 
			} 

			read(new_socket, input_buf, 1024);

			if (atoi(input_buf) == 2)
			{
				if (flag2 == 0)
					port2 = ++port;
				
				flag2++;
				group2[flag2-1] = new_socket;

				if (flag2 == 2)
				{					
					flag2 = 0;
					char* _port2 = int_to_char_array(port2);
					char data2[6];

					for (int j = 0; j < 4; j++)
						data2[j] = _port2[j];

					data2[4] = *int_to_char_array(3);

					for (int i = 0; i < 2; i++)
					{
						data2[5] = *int_to_char_array(i+1);
						data2[6] = '\0';

						if(send(group2[i], data2, strlen(data2), 0) != strlen(data2)) 
							write(1, "Sending group 2 is fail.\n", 25);
					}
				}
			}

			if (atoi(input_buf) == 3)
			{
				if (flag3 == 0)
					port3 = ++port;
				
				flag3++;
				group3[flag3-1] = new_socket;

				if (flag3 == 3)
				{					
					flag3 = 0;
					char* _port3 = int_to_char_array(port3);
					char data3[6];

					for (int j = 0; j < 4; j++)
						data3[j] = _port3[j];

					data3[4] = *int_to_char_array(4);

					for (int i = 0; i < 3; i++)
					{
						data3[5] = *int_to_char_array(i+1);
						data3[6] = '\0';

						if(send(group3[i], data3, strlen(data3), 0) != strlen(data3)) 
							write(1, "Sending group 3 is fail.\n", 25);						
					}
				}
			}

			if (atoi(input_buf) == 4)
			{
				if (flag4 == 0)
					port4 = ++port;
				
				flag4++;
				group4[flag4-1] = new_socket;

				if (flag4 == 4)
				{					
					flag4 = 0;
					char* _port4 = int_to_char_array(port4);
					char data4[6];

					for (int j = 0; j < 4; j++)
						data4[j] = _port4[j];

					data4[4] = *int_to_char_array(5);

					for (int i = 0; i < 4; i++)
					{
						data4[5] = *int_to_char_array(i+1);
						data4[6] = '\0';

						if(send(group4[i], data4, strlen(data4), 0) != strlen(data4)) 
							write(1, "Sending group 4 is fail.\n", 25);						
					}
				}
			}
				
			for (int i = 0 ; i < MAX_PLAYER ; i++) 
			{ 
				if(clients_socket[i] == 0) 
				{
					clients_socket[i] = new_socket; 
					write(1, "This socket add to the list of sockets.\n", 41);	
					break;
				}
			} 					
		} 
			
		for (int i = 0; i < MAX_PLAYER; i++) 
		{ 				
			if (FD_ISSET(clients_socket[i], &read_fds)) 
			{ 
				if ((val_read = recv(clients_socket[i], buffer, 1024, 0)) < 0) 
				{ 
					char temp[1];
					temp[0] = clients_socket[i];
					temp[1] = '\0';
					write(1, "This socket disconnected:", 24);
					write(1, temp, 1);
					write(1, "\n", 2);
					close(clients_socket[i]); 
					clients_socket[i] = 0;                 
				} 
				else
				{ 
					buffer[val_read] = '\0'; 
					send(clients_socket[i], buffer, strlen(buffer), 0); 
				} 
			} 
		} 
	}                     
    return 0;
}