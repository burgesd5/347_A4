#include "Song.h"

int writeSongRecord(char* filename, char* name, char* artist, int length);
int readSongRecords(char* filename);
int totalTimeOfSongs(char* filename);
int findSong(char* filename, char* songName, Song* song);
