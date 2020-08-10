/* Created by Kent Nyberg, 2020 */




/****

Kolla upp vilka lägen som gäller för termostaten öppen/stängd samt för värmaren på/av.

* När reläet är på (1) så går ingen ström till termostaten och den är nere.


gpio write 4 0   För att sätta igång värmaren.
gpio write 4 1   För att stänga av den. 

*/

/* Saker att fixa.



	1.   Stäng av den under natten? Typ, 2-6?
	2.   Reglera om det blir för varmt. Typ, stäng av värmen helt eller delvis?
	3.   Reglera värmaren med algoritm. Dvs, blir det för varmt, så  öka vilotiden tills önskad värme nåtts?


*/


#include <unistd.h>
#include <stdio.h>
#include <wiringPi.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <curses.h>
#include <time.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>


int error=0;
char buf[200];
char *temp;
float value;
float wanted_value=32.0;
int fd=-1;
int intvalue;
int file_open_loop=0;
WINDOW * mainwin;
time_t begin;
time_t elapsed;
char time_running[200];
pthread_t thread1;
char slask[200];


#define termostat 0   		/* On when the termostat should open up to the entire system */
#define heater 4		/* Heater is on 4 */
#define heat_time_open 20	/* Time the heater is on, */
#define heat_time_closed 20	/* Time the heater is off */
#define temp_current		/* The current temp */



void INThandler(int);




static void *code_for_network(void *arg){

int socket_desc , client_sock , c , read_size;
struct sockaddr_in server , client;
	char client_message[2000];
char ok_response[200]="Command read and accepted. Will do!";
char fail_response[200]="Command read but not accepted.";
char slask2[3000];

//Create socket

	socket_desc = socket(AF_INET , SOCK_STREAM , 0);

	if (socket_desc == -1)

	{

		printf("Could not create socket");
return 1;
	}

server.sin_family = AF_INET;

	server.sin_addr.s_addr = INADDR_ANY;

	server.sin_port = htons( 8888 );
if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)

	{

		//print the error message

		perror("bind failed. Error");

		return 1;

	}

listen(socket_desc , 3);
c = sizeof(struct sockaddr_in);  //why?

//accept connection,
client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);

	if (client_sock < 0)

	{

		perror("accept failed");

		return 1;

	}

//echo message back,

while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )

	{

		//Send the message back to client

//		write(client_sock , client_message , strlen(client_message));
sprintf(slask2, "Recived command.\n");

write(client_sock , slask2 , strlen(slask2));
refresh();
mvaddstr(5, 0, slask2);
refresh();
wanted_value=atof(client_message);
//close(client_sock);  not here.
	}

if(read_size == 0)

	{

		puts("Client disconnected");

		fflush(stdout);

	}

	else if(read_size == -1)

	{

		perror("recv failed");

	}

return NULL;
}




int main (void){

begin = time(NULL);

signal(SIGINT, INThandler);

    /*  Initialize ncurses  */



    if ( (mainwin = initscr()) == NULL ) {

	fprintf(stderr, "Error initialising ncurses.\n");

	exit(EXIT_FAILURE);

    }



pthread_create(&thread1, NULL, code_for_network, NULL);


wiringPiSetup () ;

  pinMode (termostat, OUTPUT) ;
  pinMode (heater, OUTPUT) ;


    digitalWrite (termostat, LOW);
    digitalWrite (heater, HIGH);

//printf("Setting up system...\n");

//printf("Termostat: ");
if (digitalRead(termostat)){
//printf("HIGH\n");
}else{
//printf("LOW\n");
};

//printf("Heater: ");
if (digitalRead(heater)){
//	printf("HIGH\n");
}else{
printf("LOW\n");
};





while(error!=1){  // main loop

elapsed = time(NULL);

sprintf(time_running, "Total days  running: %ld, Total minutes running: %ld,", ((elapsed - begin)/86400), ((elapsed - begin)/60) );
// print string to string first, then just use mvaddstr with no arguments,
mvaddstr(0, 0, time_running);


if((fd = open("/sys/bus/w1/devices/28-0417039fa6ff/w1_slave",O_RDONLY)) < 0){
	//something
	}else{

	// something
	}


if(read(fd,buf,sizeof(buf)) < 0)

        {

//        printf("Error reading tempsensor. Shutting down system and stopping heater.\n");
	digitalWrite(heater, HIGH);

            return 1;
	/*

	Should turn of heater before quitting. 

	*/


        }
close(fd);
        // Returns the first index of 't'.

temp = strchr(buf,'t');

// Read the string following "t=".

sscanf(temp,"t=%s",temp);

// atof: changes string to float.

value = atof(temp)/1000;
intvalue = atoi(temp)/1000;
// printf(" temp : %3.3f °C\n",value);
// printf("%f\n",value);
// printf("%d\n",intvalue);

 


sprintf(slask, "Wanted temperature: %3.3f, Actual temperature: %3.3f",wanted_value, value  );
mvaddstr(2, 0, slask);

// Do we turn heat on?
if (value < wanted_value){		//If its 40 or hotter, do not turn it on.
	// Turn heat on,
//	printf("Setting heater on for %d seconds,,\n", heat_time_open);

   mvaddstr(1, 1, "Heater: On,");
    refresh();

	digitalWrite(heater, LOW);
	sleep(heat_time_open);
	}else{
   mvaddstr(1, 1, "Heater: Off,");

    refresh();

//	printf("To hot, skipping turning on heat for now. Temp: %3.3f °C.\n", value);
	}

//turn heat off?
if (value < wanted_value){
//	printf("Skipping turning it off, to cold. Going full power! Temp: %3.3f °C\n", value);
	}else{
//	printf("Setting heater off for %d seconds..\n", heat_time_closed);
	digitalWrite(heater, HIGH);
	sleep(heat_time_closed);
	}



}  // end of while



 delwin(mainwin);

    endwin();

    refresh();


return 0;
}







void INThandler(int sig){

signal(sig, SIG_IGN);
digitalWrite(heater, HIGH);
//printf("Ouch, turning off heater and quitting.. \n");

delwin(mainwin);
endwin();
refresh();



exit(0);

};
