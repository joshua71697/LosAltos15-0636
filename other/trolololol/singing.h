/*
To play a song:
1) Call initsong()
2) Call note(freq, time) for each note in the song (up to 255)
3) Call song() to load the song
4) Call play_song() to play the song in paralell (don't do ANYTHING with the create during that time)
	or you can call sing() to play and wait until the song stops
*/
#include "createDrive.h"

//NOTEb = NOTE flat
#define G 55
#define Ab 56
#define A 57
#define Bb 58 
#define B 59
#define C 60
#define Db 61
#define D 62
#define Eb 63
#define E 64
#define F 65
#define Gb 66
#define O +12//add one of these per octave up, subtract one per octave down
#define REST 0//can be anything outside of the range of 31-127

//define the tempo marks relative to the whole note (change the length of the whole note based on tempo)
//64 = 1 second, 32 = .5 seconds, etc
#define W 80 //whole note
#define H (W / 2)//half note
#define Q (W / 4)//quarter note
#define ETH (W / 8) //eighth note (note - we can't just use E)
#define TQ (H / 3)//triplet quarters
#define TE (Q / 3)//triplet eighth
#define DH (H + Q)//dotted half
#define DQ (Q + ETH)//dotted quarter
#define BR (W + H)//breve-ish thing (6 beats)
//if necessary, define lower levels of notes according to this pattern

//ceil(X) only works on positive values
int notes[255],times[255],notenum;
float waits[16];
void initsong()
{
	int i;
	notenum = 0;
	for(i=0;i<16;i++)
	{
		waits[i] = -1.;
	}
	for(i=0;i<255;i++)
	{
		notes[i] = -1;
		times[i] = -1;
	}
}
void note(int freq,int len)
{
	notes[notenum] = freq;
	times[notenum] = len;//(int)(len*64.);
	notenum++;
}
void song()
{
	int totnotes = 0,sets,finalset,set = 0,note;
	while(notes[totnotes] != -1)
	{
		totnotes++;
	}
	sets = totnotes/16;
	finalset = totnotes%16;
	while(set < sets)
	{
		create_write_byte(140);
		create_write_byte(set);
		create_write_byte(16);
		waits[set] = 0.;
		for(note = 0;note<16;note++)
		{
			waits[set] += (float)times[note+(set*16)]/64.;
			create_write_byte(notes[note+(set*16)]);
			create_write_byte(times[note+(set*16)]);
		}
		set++;
	}
	create_write_byte(140);
	create_write_byte(set);
	create_write_byte(finalset);
	waits[set] = 0.;
	for(note = 0;note<finalset;note++)
	{
		waits[set] += (float)times[note+(set*16)]/64.;
		create_write_byte(notes[note+(set*16)]);
		create_write_byte(times[note+(set*16)]);
	}
}

void sing()
{
	int i = 0;
	while(waits[i] != -1.)
	{
		create_write_byte(141);
		create_write_byte(i);
		create_block();//use this instead of create_wait_time to make sure it shuts down at the end
		msleep((int)(waits[i]*1000.));//if there's a gap between the songs, decrease this value slightly
		//create_wait_time((int)((waits[i]*10.)));
		//printf("%d,%d\n",i,(int)((waits[i]*10.)));
		i++;
	}
}

void play_song()//call this to play the song (shell function that creates a parallel process to play the song)
{
	thread play=thread_create(sing);
	thread_start(play);
}

void troll_intro()
{
	initsong();//clear variables to prepare
	
	//define notes here
	
	int pitches_0[16] = { C  ,  D  , E   , F   , G , G , C+O, G+O,  F , G+O,  C ,  D , E , G+O , E , D };
	int tempos_0[16]  = { ETH, ETH , ETH , ETH , Q,  Q , BR ,  DQ, ETH,  BR,  Q ,  Q , W , W , Q , DH  };
	
	for (i = 0; i < 16; i++)
		note(pitches_0[i], tempos_0[i]);
	song();	
}

void troll()//I do not approv btw
{
	int pitches_1[16] = { C, E, G+O, C+O, B+O, B+O, G+O, A+O, A+O, F, G+O, G, B, D, F, E};
	int tempos_1[16] =  { ETH , ETH , ETH , ETH, Q , ETH , ETH , Q , ETH , ETH , H , ETH , ETH , ETH , ETH , W };
	
	int pitches_2[16] = { C, E, G+O, C+O, B+O, B+O, G+O, A+O, A+O, F  , G+O, G+O, Ab+O, A+O, B+O, C+O };
	int tempos_2[16]  = { ETH, ETH, ETH, ETH, Q, ETH, ETH, Q, ETH, ETH, H, ETH, ETH, ETH, ETH, W };
	int i;
	
	for (i = 0; i < 16; i++)
		note(pitches_1[i], tempos_1[i]);
	song();
	for (i = 0; i < 16; i++)	
		note(pitches_2[i], tempos_2[i]);
	song();

	
	//load the song (play it when ready using play_song)	
	
}
