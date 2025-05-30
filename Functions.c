#pragma warning(disable : 4996)
//https://www.tutorialspoint.com/c_standard_library/c_function_fgets.htm

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include "Song.h"

//Function Prototypes
void* writeSongRecordThreadCallback(void* arg);
void* readSongRecordsThreadCallback(void* arg);
void* totalTimeOfSongsThreadCallback(void* arg);
void* findSongThreadCallback(void* arg);
int FBS = FIELD_BUFFER_SIZE;
//Single Generic Struct
typedef struct {
    char filename[256]; // good upper bound with  NT
    char name[FBS]; // w / f
    char artist[FBS]; // w
    int length; // w
    Song* song; // find
} SR; // args for the threads

int writeSongRecord(char* filename, char* name, char* artist, int length)
{
    pthread_t thread;
    SR args;
    // all fields in stuct initialized to 0
    memset(&args, 0, sizeof(SR));
    // copy filename, song name, artist, name in and set length
    strncpy(args.filename, filename, sizeof(args.filename) - 1); // space for NT on all 3 
    strncpy(args.name, name, sizeof(args.name) - 1);
    strncpy(args.artist, artist, sizeof(args.artist) - 1);
    args.length = length;
    long* returnValue;  // ptr for thread return val
    // creating thread for callback and pass ptr
    pthread_create(&thread, NULL, writeSongRecordThreadCallback, &args);
    // store value when thread is done
    pthread_join(thread, (void**)&returnValue);
    int finalRet = *returnValue; // deref to grab before freeing
    free(returnValue);

    return finalRet; 
}

void* writeSongRecordThreadCallback(void* arg)
{
    // recast to SR struct
    SR* args = (SR*)arg;
    
    // append for writing new song record
    FILE* file = fopen(args->filename, "a");
    
    // alloc return val
    long* returnValue = malloc(sizeof(long));
    // immediate error check
    if (file == NULL) {
        *returnValue = -1;
        return returnValue; 
    }
    
    

    // writing each field on 3 concurrent lines
    fprintf(file, "%s\n", args->name);
    fprintf(file, "%s\n", args->artist);
    fprintf(file, "%d\n", args->length);
    fclose(file); //close
    // set success if we made it here
    *returnValue = 0; // success
    return returnValue;
}

int readSongRecords(char* filename)
{
    long returnValue = 0;

    //Create thread here...
    
    return returnValue; 
}

void* readSongRecordsThreadCallback(void* arg)
{
   
}

int totalTimeOfSongs(char* filename)
{
    long returnValue = 0;

    //Create thread here...
    
    return returnValue; 
}

void* totalTimeOfSongsThreadCallback(void* arg)
{
    
}

int findSong(char* filename, char* songName, Song* song)
{
    int songFound = 0;

    //Create thread here...
    
    return songFound;
}

void* findSongThreadCallback(void* arg)
{
    

}

