// Created on Sun July 5 2015

#include "notes.h"
#include "singing.h"

#define NOTES_PER_SONG 16
void playSong(int songNum);

int main()
{
	create_connect();
	printf("trololololol\n");
	
	troll_intro();
	sing();
	troll();
	sing();
	sing();
}

void playSong(int songNum)
{
	create_load_song(songNum);
	printf("Song is starting\n");
	create_play_song(songNum);
	printf("Song playing %d\n",get_create_song_number(.05));
	
	while(get_create_song_playing(.1)){} //wait until song
	printf("Song has finished playing\n"); // finishes
}
