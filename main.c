// Programma chitaet dannye o 3d-kinematike iz udp soketa


#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <time.h>
#include <ncurses.h>
#include <math.h>
#include <string.h>

#include <sched.h>



#define NUMBER_OF_SENSORS 19
#define DIMENSIONS 9

#define ADDRESS "192.168.43.12"
#define ANDROID_PORT 3233
#define IN_PORT 50000
#define SIGNAL1_PORT 3231
#define SIGNAL2_PORT 3235

uint8_t buffer[1024];




//*****************************************
// Main
//*****************************************
int main(int argc, char *argv)
{

	buffer[0] = 0;
	char *gprmc_pointer = NULL;
	char *e_pointer = NULL;
	char *end_pointer = NULL;

	int rows, columns;

	initscr();   // start curses ***
	getmaxyx(stdscr, rows, columns);
	clear();
	noecho();
	cbreak();

	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	init_pair(4, COLOR_BLUE, COLOR_BLACK);
	init_pair(5, COLOR_YELLOW, COLOR_BLACK);

	attron(COLOR_PAIR(4));
	//attron(A_BOLD);                                                      	
	attron(A_DIM);                                                      	
	move(rows-1, 0);
	printw("%3d %3d");
	attroff(A_DIM);                                                      	
	
	beep();	
	
	
	struct timespec sleep_interval;

	sleep_interval.tv_sec = 0;
	sleep_interval.tv_nsec = 10000000;	// 10 miliseconds

	int i, j, k;
	
	//*****
	int receiver_socket;
	struct sockaddr_in receiver_address;
	int datagramm_length;
	int address_length;

	receiver_socket = socket(AF_INET, SOCK_DGRAM, 0);
	receiver_address.sin_family = AF_INET;
	receiver_address.sin_port = htons(IN_PORT);
	receiver_address.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(receiver_socket, (struct sockaddr *)&receiver_address, sizeof(receiver_address));
	//*****

	pthread_t udpReceiverThread;
	int threadId;
	int parameter = 0;
	
	// start threads 
	//pthread_create(&udpReceiverThread, NULL, udpThreadFunction, (void *)(&parameter));
	long counter = 0;
	i = 0;
	k = 0;
	while(1)
	{
		//clear();
		
	    move(0, 0);
	    //printw("GPS data");
		//move(1, 0);
		//printw("received %7ld", counter);
		//move(2,0);
		//printw("length = %7d", strlen(buffer));
		gprmc_pointer = strstr(buffer, "$GPRMC");
		if(gprmc_pointer != NULL)
		{
			//printw("%s", gprmc_pointer);
			//printw("%s", buffer);
			e_pointer = strstr(gprmc_pointer, "E");
			if(e_pointer != NULL)
			{
				move(0, 0); clrtoeol();
				int portion0 = e_pointer - gprmc_pointer + 2;
				for(i=0;i<portion0;i++)
					printw("%c", gprmc_pointer[i]);
				e_pointer += 2;
				end_pointer = strstr(e_pointer, "\r\n");
				if(end_pointer != 0)
				{
					int portion1 = end_pointer - e_pointer;
					move(1, 0); clrtoeol();
					for(i=0;i<portion1;i++)
						printw("%c", e_pointer[i]);
					//move(2, 0);
				}
			}
		}
		gprmc_pointer = strstr(buffer, "$GPGGA");
		if(gprmc_pointer != NULL)
		{
			//printw("%s", gprmc_pointer);
			//printw("%s", buffer);
			e_pointer = strstr(gprmc_pointer, "E");
			if(e_pointer != NULL)
			{
				move(2, 0); clrtoeol();
				int portion0 = e_pointer - gprmc_pointer + 2;
				for(i=0;i<portion0;i++)
					printw("%c", gprmc_pointer[i]);
				e_pointer += 2;
				end_pointer = strstr(e_pointer, "\r\n");
				if(end_pointer != 0)
				{
					int portion1 = end_pointer - e_pointer;
					move(3, 0); clrtoeol();
					for(i=0;i<portion1;i++)
						printw("%c", e_pointer[i]);
					move(4, 0);
				}
			}
		}
		refresh();

		for(i=0;i<1024;i++)
			buffer[i] = 0;
		datagramm_length = recvfrom(receiver_socket, buffer, 1024, 0, (struct sockaddr *)&receiver_address, &address_length);
		
		counter++;

	}// end while(1)

}// end main

//***************************************************
//***************************************************
//***************************************************
//***************************************************
//***************************************************
//***************************************************
