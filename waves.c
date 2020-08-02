#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h> 
#include <time.h>
#include <wiringPi.h>

//hello


char cmd [512];
void playATune();
void doRandomLoop();
char * RandomFile();
#define LedPin 2



// this page shows the poutputs https://www.digikey.com/en/maker/blogs/2019/how-to-use-gpio-on-the-raspberry-pi-with-c

int main()
{

    srand ( time(NULL) );
    printf("Hello, World! \n");
   // if(wiringPiSetup() == -1) 
   // { 
   //     //when initialize wiringPi failed, print message to screen
   //     printf("setup wiringPi failed !\n");
   //     return -1;
   // }

    printf ("Down here");
    doRandomLoop();
    return 0;
}

void doRandomLoop()
{
    char * fileName;


    while ( 1 == 1)
    {
                fileName = RandomFile ();
    sprintf(cmd,"mpg321 ./files/%s",fileName);
    printf ("Command to run: %s",cmd);
        char * fname;
        sprintf(fname, "/home/pi/src/kill.me");
        if( access( fname, F_OK ) != -1 ) 
        {
        // file exists - return.
        return;
        } 
      //  for (int i =0 ; i< 10; i++)
        {
            if (digitalRead (2) == 1)
            {
                printf ("someone have done pin %d", 2);
                //playATune(); 
            }
        }
        sleep(5);
    }
}

void playATune()
{
    printf ("Ooh SOmeone must have stepped in fromt of the PIR\r\nI'm going to play a song");
    pinMode(LedPin, OUTPUT);segmentation fault checker online
    digitalWrite(LedPin, LOW);   //led on
    printf("led on\n");
    system ("amixer set PCM 100%");
    //calculate the command line

    system(cmd);
    digitalWrite(LedPin, HIGH);  //led off
    printf("led off\n");
    printf("All Done\n");
}

char  * RandomFile ()
{
    //generate a random number
    int randomnumber;
    randomnumber = rand() % 100;

    //get the number of files in the folder
    int filecount;
    filecount =0;
    DIR *d;
    struct dirent *dir;
    d = opendir("./files");
    if (d)
    {
        while ((dir = readdir(d)) != NULL) 
        {
            //printf("FILE: %d = %s\n", filecount, dir->d_name);
            filecount++;
        }
    }
    else
    {
        printf ("the ./files/ folder is missing");
    }
    
    filecount = filecount -2; //for the . and .. directories which show
    int fileNumber;
    fileNumber = randomnumber % filecount;
    closedir(d);
    d=opendir("./files");

    //now get the nth file and return the filename
    for (int i = 0; i<= fileNumber; i++)
    {
        dir = readdir(d);

    }


    char * fName;
    fName = dir->d_name;
    closedir(d);
  



    return fName;

}