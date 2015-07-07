// Created on Sun July 5 2015

#include "notes.h"

#define NOTES_PER_SONG 16
void playSong(int songNum);

int main()
{
	create_connect();
	printf("trololololol\n");
	
	// int[] pitches = { C1, D1, E1, F1, G2, A2, B2, C2 };
	// int[] tempos =  { Q , E , E , Q , E , E , Q , H  };
	
	int pitches_0[10] = { C2, G2, F1, G2, C1, D1, E1, G2, E1, D1 };
	int tempos_0[10] =  { B , D , E , B , Q , Q , W , W , Q , D  };
	
	int pitches_1[NUM_NOTES_PER_SONG] = { C1, E1, G2, C2, B2, B2, G2, A2, A2, F1, G2, G1, B1, D1, F1, E1};
	int tempos_1[NUM_NOTES_PER_SONG] =  { E , E , E , E , Q , E , E , Q , E , E , H , E , E , E , E , W };
	
	int pitches_2[NUM_NOTES_PER_SONG] = { C1, E1, G2, C2, B2, B2, G2, A2, A2, F1, G2, G2, GS2, A2, B2, C };
	int tempos_2[NUM_NOTES_PER_SONG]  = { E , E , E , E , Q , E , E , Q , E , E , H , E , E , E , E , W };
	
	int songNum = 0;
	gc_song_array[songNum][0] = NOTES_PER_SONG;
	
	int k = 0;
	for (; k < NOTES_PER_SONG; k++)
	{
		gc_song_array[songNum][2*k+1] = pitches_1[k];
		gc_song_array[songNum][2*k+2] = tempos_1[k];
	}
	
	songNum++;
	gc_song_array[songNum][0] = NOTES_PER_SONG;
	
	for (k = 0; k < NOTES_PER_SONG; k++)
	{
		gc_song_array[songNum][2*k+1] = pitches_2[k];
		gc_song_array[songNum][2*k+2] = tempos_2[k];
	}
	
	playSong(0);
	playSong(1);
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
