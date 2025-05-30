#pragma warning(disable : 4996)
//https://www.tutorialspoint.com/c_standard_library/c_function_fwrite.htm

//usr/bin/gcc -pthread Program.c -o Program 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Functions.h"
#include "Song.h"

#define SONG_FILE "Songs.txt"

int main()
{
	fprintf(stdout, "Running program...\n\n");

	writeSongRecord(SONG_FILE, "Black Dog", "Led Zeppelin", 220);
	writeSongRecord(SONG_FILE, "Purple Haze", "Jimi Hendrix", 240);

	readSongRecords(SONG_FILE);

	int totalTime = totalTimeOfSongs(SONG_FILE);

	fprintf(stdout, "Total time of songs: %d seconds\n\n", totalTime);

	char* searchSongName = "Black Dog";

	Song song = { 0 };

	int songFound = findSong(SONG_FILE, searchSongName, &song);

	if (songFound == 1)
	{
		fprintf(stdout, "Song found...%s\n\n", searchSongName);

		fprintf(stdout, "Name:   %s\n", song.name);
		fprintf(stdout, "Artist: %s\n", song.artist);
		fprintf(stdout, "Length: %s\n", song.length);
		fprintf(stdout, "\n");
	}
	else
	{
		fprintf(stdout, "Song not found...%s\n\n", searchSongName);
	}

	searchSongName = "Black Cat";

	songFound = findSong(SONG_FILE, searchSongName, &song);

	if (songFound == 1)
	{
		fprintf(stdout, "Song found...%s\n\n", searchSongName);

		fprintf(stdout, "Name:   %s\n", song.name);
		fprintf(stdout, "Artist: %s\n", song.artist);
		fprintf(stdout, "Length: %s\n", song.length);
		fprintf(stdout, "\n");
	}
	else
	{
		fprintf(stdout, "Song not found...%s\n\n", searchSongName);
	}
	
	fprintf(stdout, "Program complete...\n\n");

	return 0;

}
