// Created on Sun July 5 2015

#include "notes.h"

#define NOTES_PER_SONG 16

int main()
{
	create_connect();
	printf("trololololol\n");
	
	// int[] pitches = { C1, D1, E1, F1, G2, A2, B2, C2 };
	// int[] tempos =  { Q , E , E , Q , E , E , Q , H  };
	
	int[] pitches = { C1, E1, G2, C2, B2, B2, G2, A2, A2, F1, G2, G1, B1, D1, F1, E1};
	int[] tempos =  { E , E , E , E , Q , E , E , Q , E , E , H , E , E , E , E , W };
	
	gc_song_array[0][0] = NOTES_PER_SONG;
	int songNum = 0;
	int k = 0;
	for (k = 0; k < NOTES_PER_SONG; k++)
	{
		gc_song_array[songNum][2*k] = pitches[k];
		gc_song_array[songNum][2*k+1] = tempos[k];
	}
	
	create_load_song(songNum);
	printf("Song is starting\n");
	create_play_song(songNum);
	printf("Song playing %d\n",get_create_song_number(.05));
	
	while(get_create_song_playing(.1)){} //wait until song
	printf("Song has finished playing\n"); // finishes
	
}
