/* Created by Kent Nyberg, 2020 */




/****



gpio write 4 0   To set heater on..
gpio write 4 1   To set heater off. 

*/



#include <unistd.h>
#include <stdio.h>
#include <wiringPi.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>


int error=0;
char buf[200];
char *temp;
float value;
float wanted_value=36.0;
int fd=-1;
int intvalue;
int file_open_loop=0;

#define termostat 0   		/* On when the termostat should open up to the entire system */
#define heater 4		/* Heater is on 4 */
#define heat_time_open 20	/* Time the heater is on, */
#define heat_time_closed 20	/* Time the heater is off */
#define temp_current		/* The current temp */



void INThandler(int);



int main (int argc, char *argv[]){

int opt;

while((opt = getopt(argc, argv, “help”)) != -1)

    {

        switch(opt)

        {

            case ‘help’:

                printf(“help... %c\n”, opt);
                break;

        }

    }




signal(SIGINT, INThandler);

  wiringPiSetup () ;

  pinMode (termostat, OUTPUT) ;
  pinMode (heater, OUTPUT) ;


    digitalWrite (termostat, LOW);
    digitalWrite (heater, HIGH);

printf("Setting up system...\n");

printf("Termostat: ");
if (digitalRead(termostat)){
printf("Should be low, is HIGH.\n");
}else{
printf("LOW.\n");
};

printf("Heater: ");
if (digitalRead(heater)){
	printf("HIGH\n");
}else{
printf("Should be high, is LOW.\n");
};





while(error!=1){  // main loop


if((fd = open("/sys/bus/w1/devices/28-0417039fa6ff/w1_slave",O_RDONLY)) < 0){
	//something
	}else{

	// something
	}


if(read(fd,buf,sizeof(buf)) < 0)

        {

        printf("Error reading tempsensor. Shutting down system and stopping heater.\n");
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
 printf(" temp : %3.3f °C\n",value);
// printf("%f\n",value);
// printf("%d\n",intvalue);

 




// Do we turn heat on?
if (value < wanted_value){		//If its 40 or hotter, do not turn it on.
	// Turn heat on,
	printf("Setting heater on for %d seconds,,\n", heat_time_open);
	digitalWrite(heater, LOW);
	sleep(heat_time_open);
	}else{
	printf("To hot, skipping turning on heat for now. Temp: %3.3f °C.\n", value);
	}

//turn heat off?
if (value < wanted_value){
	printf("Skipping turning it off, to cold. Going full power! Temp: %3.3f °C\n", value);
	}else{
	printf("Setting heater off for %d seconds..\n", heat_time_closed);
	digitalWrite(heater, HIGH);
	sleep(heat_time_closed);
	}



}  // end of while






return 0;
}







void INThandler(int sig){

signal(sig, SIG_IGN);
digitalWrite(heater, HIGH);
printf("Ouch, turning off heater and quitting.. \n");
exit(0);

};
