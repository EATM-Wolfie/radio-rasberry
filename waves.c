#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h> 
#include <time.h>
#include <wiringPi.h>

#define DATA_SIZE 1000


    /* Variable to store user content */
    char data[DATA_SIZE];

    /* File pointer to hold reference to our file */
    FILE * fPtr;

//the file names and total files
char  files [128] [64];
int filecount = 0;

char cmd [512];
void playATune();
void cls();         //clears the screen
void doHome();      //puts cursor at home position
void doRandomLoop();
void Logf ();

char * RandomFile();
#define LedPin 2     //GPIO2 Physical Pin 13. this goes LOW when active and HIGH when inactive. (so go from 5v (Physical Pin 2) via a 33 ohm resistor to pin 13 for a cct.)
#define triggerPin 4 //GPIO4 Physical pin 16. This has to be shorted to 5v to activate. Use Physical pin 2 for the 5v.
//contradictory information.
//Use PIN2 (5v and PIN 16 [which says GPIO23])
//you need to make sure that mpyg321 is installed

// this page shows the poutputs https://www.digikey.com/en/maker/blogs/2019/how-to-use-gpio-on-the-raspberry-pi-with-c
void GetDirContents()
{
    sprintf(data, "Getting Diecotry contents\n");
    Logf();
    
DIR *d;
    struct dirent *dir;
    d = opendir("./files");
    if (d)
    {
        while ((dir = readdir(d)) != NULL) 
        {

            if (dir->d_name[0] != '.')
            {
                if (filecount > 64) break;
                strcpy (files[filecount], dir->d_name);
                filecount++;

            }
            
        }
    }
    else
    {
        
        printf ("the ./files/ folder is missing");
    }
    //print a list to the user so they can see what is going on
    printf ("List of files generated\n");
    printf ("=======================\n");
    for (int i =0 ; i<filecount; i++)
    {
        sprintf (data, "File number %d is %s\n", i, files[i]);
        Logf ();
        printf ("File number %d is %s\n", i, files[i]);

    }
    printf ("-----------------------\n");


}
int main()
{
    sprintf (data, "TAXI RADIO STARTING");
    Logf();

    srand ( time(0) );
    //go get the array of files which can be played
    GetDirContents();
    sprintf(data, "doing wiringpisetup\n");
    Logf();
    if(wiringPiSetup() == -1) 
    { 
    sprintf(data, "setup wiringPi failed !\n");
    Logf();
        
      //when initialize wiringPi failed, print message to screen
      printf("setup wiringPi failed !\n");
      return -1;
    }
    sprintf(data, "got through wiringpi !\n");
    Logf();
    
    pinMode(triggerPin, INPUT);
    //a file what you put in to stop execution
    char fname [64];;
    
    sprintf(fname, "/home/pi/src/kill.me");
    
    //main loop    
    cls();
    while (1)
    {
        if( access( fname, F_OK ) != -1 ) 
        {
            // file exists - return.
            printf ("Leaving the Program\n");
            break;
        } 
       
         if (digitalRead (triggerPin) == 0)
         {
             cls();
             printf("Going to play a song\n");

             playATune();
             //cls();
             for (int i=60; i>0 ; i--)
             {
                 sprintf (data, "In wait for 60 secs loop\n");
                 Logf();
                 printf ("Just waiting %d secs before entering loop again  ", i);
                 fflush(stdout);
                 sleep (1);
                 doHome();
             }
             cls();
         }
         else
         {
             doHome();
             time_t rawtime;
             struct tm * timeinfo;
             time (&rawtime);
             timeinfo = localtime ( &rawtime);
             printf ("Waiting for movement\n\n\n%s\n", asctime(timeinfo));
             
         }
        sleep (1);
    }
    return 0;
}

void doHome()
{
    printf ("\033[H");
    fflush(stdout);
             
}

void cls()
{
             printf ("\033[2J\033[H");
             fflush(stdout);
}



void playATune()
{
    printf ("PIR Sensor pin triggered\n");
    digitalWrite(LedPin, LOW);   //led on
    system ("amixer set PCM 100%");
    char * fileName = RandomFile();
    printf ("Filename=%s", fileName);
                 sprintf  (data, "about to play %s\n", fileName);
             Logf();
    //calculate the command line
    sprintf(cmd,"mpg321 ./files/%s",fileName);
    //printf ("temp suspend plauing file: %s\n", cmd);
    system(cmd);
    digitalWrite(LedPin, HIGH);  //led off
    printf ("Leaving function that plays a song\n");
}


char * RandomFile ()
{
    //generate a random number
    int randomnumber =1;
    randomnumber = rand() % filecount;
    //get the number of files in the folder
    //now get the nth file and return the filename
    int tempNumber = 0;
    for (int i = 0; i<= randomnumber; i++)
    {
        tempNumber++;
        if (tempNumber >= filecount) tempNumber=0;
    }

    char * fName;
    fName = files [tempNumber];
    printf ("=====================================\n");
    printf ("Random Number = %d. Number picked = %d, name=%s\n", randomnumber, tempNumber, fName);
    return fName;

}


void Logf ()
{
    /* 
     * Open file in w (write) mode. 
     * "data/file1.txt" is complete path to create file
     */
    fPtr = fopen("./log.log", "a");


    /* fopen() return NULL if last operation was unsuccessful */
    if(fPtr == NULL)
    {
        /* File not created hence exit */
        printf("Unable to create file.\n");
        exit(EXIT_FAILURE);
    }


 


    /* Write data to file */
    fputs(data, fPtr);


    /* Close file to save file data */
    fclose(fPtr);


    /* Success message */
    printf("File created and saved successfully. :) \n");


}