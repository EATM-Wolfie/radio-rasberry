#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h> 
#include <time.h>
#include <wiringPi.h>


char * RandomFile();
#define LedPin 2

int main()
{

srand ( time(NULL) );
    printf("Hello, World! \n");
 

    printf ("----\n\n\n\n");
    char * fileName;
    fileName = RandomFile ();
    //RandomFile();
    // char outputName[1024];
    // strcpy (outputName, fileName);
    char cmd [512];
    sprintf(cmd,"mpg321 ./files/%s",fileName);
    printf ("Command to run: %s",cmd);
    
    printf ("file to pick is %s\n\n", fileName);
    if(wiringPiSetup() == -1) 
    { 
        //when initialize wiringPi failed, print message to screen
        printf("setup wiringPi failed !\n");
        return -1;
    }

    pinMode(LedPin, OUTPUT);
    digitalWrite(LedPin, LOW);   //led on
    printf("led on\n");
    system ("amixer set PCM 100%");
    //calculate the command line

    system(cmd);
    digitalWrite(LedPin, HIGH);  //led off
    printf("led off\n");
    printf("All Done\n");
    return 0;
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