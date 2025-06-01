/*
# Name: Dylan Burges & Elijah Atienza
# Description: Threaded song record handler
# Date: 5/30/2025
*/
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

//Single Generic Struct
typedef struct {
    char filename[256]; // good upper bound with  NT
    char name[FIELD_BUFFER_SIZE]; // w / f
    char artist[FIELD_BUFFER_SIZE]; // w
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
    SR* args = (SR*)arg; // cast to struct ptr
    long* returnValue = malloc(sizeof(long)); // alloc early
    *returnValue = 0;

    // open for read
    FILE* a_file = fopen(args->filename, "r");
    // if file exists, 
    if (a_file != NULL) {
        // only alloc for entries that are taking space
        char name[FIELD_BUFFER_SIZE], artist[FIELD_BUFFER_SIZE], lengthStr[FIELD_BUFFER_SIZE];
        // while there is a next entry
        while (fgets(name, sizeof(name), a_file)) {
            // break immediately if entries are incomplete
            if (!fgets(artist, sizeof(artist), a_file) || !fgets(lengthStr, sizeof(lengthStr), a_file))
                break;
            // NT
            name[strcspn(name, "\n")] = 0;
            artist[strcspn(artist, "\n")] = 0;
            lengthStr[strcspn(lengthStr, "\n")] = 0;

            int length = atoi(lengthStr);
            if (strcmp(name, args->name) == 0 &&
                strcmp(artist, args->artist) == 0 &&
                length == args->length) {
                // Match found, skip write
                fclose(a_file);
                return returnValue;
            }
        }
        fclose(a_file);
    }

    // Open for append
    FILE* file = fopen(args->filename, "a");
    // no garbage writing
    if (file == NULL) {
        *returnValue = -1;
        return returnValue;
    }
    // append the song record
    fprintf(file, "%s\n", args->name);
    fprintf(file, "%s\n", args->artist);
    fprintf(file, "%d\n", args->length);
    fclose(file);

    *returnValue = 0;
    return returnValue;
}

// Using readSongRecordsThreadCallBack inside a thread
int readSongRecords(char* filename)
{
    long* tres; // stores thread return value;
    pthread_t main_thread;
    
    SR args;
    memset(&args, 0, sizeof(SR)); // zero for clean truct
    // copy filename, with NT space then start thread with encompassed method
    strncpy(args.filename, filename, sizeof(args.filename) - 1);
    pthread_create(&main_thread, NULL, readSongRecordsThreadCallback, &args);
    // grab output when thread finishes
    pthread_join(main_thread, (void**)&tres);
    int returnValue = *tres;
    free(tres);
    
    return returnValue; 
}

void* readSongRecordsThreadCallback(void* arg)
{
    SR* args = (SR*)arg; // ptr to struct
    FILE* file = fopen(args->filename, "r");
    long* returnValue = malloc(sizeof(long)); // alloc retval early 
    // if opening failed
    if (file == NULL) {
        *returnValue = -1; // error
        return returnValue;
    }
    // bufs
    char name[FIELD_BUFFER_SIZE], artist[FIELD_BUFFER_SIZE],length[FIELD_BUFFER_SIZE]; 

    // while there are attributes to red on each line
    while (fgets(name, sizeof(name), file))
    {
        if (!fgets(artist, sizeof(artist), file) ||
            !fgets(length, sizeof(length), file)) {
                // record is incomplete
                break;
            }
        // newline removal
        name[strcspn(name, "\n")] = 0;
        artist[strcspn(artist, "\n")] = 0;
        length[strcspn(length, "\n")] = 0;
        // display song record
        printf("Name: %s\n", name);
        printf("Artist: %s\n", artist);
        printf("Length: %s\n\n", length);
    }
    // close and return
    fclose(file);
    return returnValue;
}


int totalTimeOfSongs(char* filename)
{
    pthread_t time;
    SR args;
    memset(&args, 0, sizeof(SR)); // zero for clean struct
    // grab file name
    strncpy(args.filename, filename, sizeof(args.filename) - 1);
    long* threadRet; // hold return from thread

    // start thread and wait for result
    pthread_create(&time, NULL, totalTimeOfSongsThreadCallback, &args);
    pthread_join(time, (void**)&threadRet);

    // need to store and free
    int result = *threadRet;
    free(threadRet);
    return result;
}


void* totalTimeOfSongsThreadCallback(void* arg)
{
    SR* args = (SR*)arg;
    FILE* file = fopen(args->filename, "r");

    long* returnValue = malloc(sizeof(long)); // alloc ret early
    *returnValue = 0;

    // error if reading fails
    if (file == NULL) {
        *returnValue = -1;
        return returnValue;
    }
    char buf[FIELD_BUFFER_SIZE];
    int lineCount = 0;

    // while the current line contains info
    while (fgets(buf, sizeof(buf), file)) {
        lineCount++;// inc counter
        // check if we are on the 3rd line int he respective sequence
        if (lineCount % 3 == 0) {
            buf[strcspn(buf, "\n")] = 0;
            // add stripped digit to the retval
            *returnValue += atoi(buf);
        }
    }
    fclose(file);
    return returnValue;
}


int findSong(char* filename, char* songName, Song* song)
{
    pthread_t finder;
    SR args;
    memset(&args, 0, sizeof(SR)); // clear the struct

    // grab filename + song
    strncpy(args.filename, filename, sizeof(args.filename) - 1);
    strncpy(args.name, songName, sizeof(args.name) - 1);
    args.song = song; // ptr to struct
    // grab from thread
    long* tres;

    // create thread
    pthread_create(&finder, NULL, findSongThreadCallback, &args);
    // join thread
    pthread_join(finder, (void**)&tres);

    int result = *tres; // get int back
    free(tres); // free thread heap
    return result; // 1, 0, -1
}


void* findSongThreadCallback(void* arg)
{
    SR* args = (SR*)arg;
    FILE* file = fopen(args->filename, "r");
    long* returnValue = malloc(sizeof(long));
    *returnValue = 0; // default = not found

    if (file == NULL) {
        *returnValue = -1;
        return returnValue; // error
    }
    
    //bufs
    char name[FIELD_BUFFER_SIZE], artist[FIELD_BUFFER_SIZE], length[FIELD_BUFFER_SIZE];
    // while there is a name to read
    while (fgets(name, sizeof(name), file)) {
        // break if entries are incompleteß
        if (!fgets(artist, sizeof(artist), file) || !fgets(length, sizeof(length), file))
            break;

        // strip newlines
        name[strcspn(name, "\n")] = 0;
        artist[strcspn(artist, "\n")] = 0;
        length[strcspn(length, "\n")] = 0;

        // if we are at the specified song, grab info
        if (strcmp(name, args->name) == 0) {
            strncpy(args->song->name, name, FIELD_BUFFER_SIZE - 1);
            strncpy(args->song->artist, artist, FIELD_BUFFER_SIZE - 1);
            strncpy(args->song->length, length, FIELD_BUFFER_SIZE - 1);
            *returnValue = 1;
            break;
        }
    }
    fclose(file);
    return returnValue; 
}

