#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h> 
#include <time.h>
#include <wiringPi.h>

//hello

//the file names and total files
char  files [128] [64];
int filecount = 0;

char cmd [512];
void playATune();
void doRandomLoop();
char * RandomFile();
#define LedPin 2     //GPIO2 Physical Pin 13. this goes LOW when active and HIGH when inactive. (so go from 5v (Physical Pin 2) via a 33 ohm resistor to pin 13 for a cct.)
#define triggerPin 4 //GPIO4 Physical pin 16. This has to be shorted to 5v to activate. Use Physical pin 2 for the 5v.



// this page shows the poutputs https://www.digikey.com/en/maker/blogs/2019/how-to-use-gpio-on-the-raspberry-pi-with-c
void GetDirContents()
{
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
        printf ("File number %d is %s\n", i, files[i]);

    }
    printf ("-----------------------\n");


}
int main()
{


    srand ( time(0) );
    printf("Program Started!\n");
    //go get the array of files which can be played
    GetDirContents();
    if(wiringPiSetup() == -1) 
    { 
      //when initialize wiringPi failed, print message to screen
      printf("setup wiringPi failed !\n");
      return -1;
    }
   //set up the pins for input and output
    
   pinMode(LedPin, OUTPUT);
   pinMode(triggerPin, INPUT);

    //a file what you put in to stop execution
    char fname [64];;
    
    sprintf(fname, "/home/pi/src/kill.me");
    
    //main loop    
    while (1)
    {
        if( access( fname, F_OK ) != -1 ) 
        {
        // file exists - return.
        printf ("Leaving the Program");
        break;
        } 
        if (digitalRead (triggerPin) == HIGH)
        {
            playATune();
        }
        sleep (1);
    }
    return 0;
}




void playATune()
{
    printf ("PIR Sensor pin triggered\n");
    digitalWrite(LedPin, LOW);   //led on
    system ("amixer set PCM 100%");
    char * fileName = RandomFile();
    printf ("Filename=%s", fileName);
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