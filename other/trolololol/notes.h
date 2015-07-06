// Created on Mon July 6 2015


/* define the tempo marks
	E = eighth
	Q = quarter
	H = half
	W = whole
	T = triplet (btw quarter and half)
	all relative to "whole" which can be changed
	Notes:
		.5 sec = 32
		.25 sec = 16
*/

#define W 64
#define H (W / 2)
#define Q (W / 4)
#define E (W / 8) 

/* define the notes
	two octaves from the G below middle C
		octave 1 has each number with a 1, and octave 2 has a 2
	Let "s" after a letter denote sharp
	( I will not use flats :P )
	
	middle C = 261.6, and note 60
	
	Notes:
		62 is a D
		64 is an E
		65 is an F
*/

#define G1 55
#define G#1 56
#define A1 57
#define A#1 58 
#define B1 59
#define C1 60
#define C#1 61
#define D1 62
#define D#1 63
#define E1 64
#define F1 65
#define F#1 66

#define G2 67
#define G#2 68
#define A2 69
#define A#2 70
#define B2 71
#define C2 72 
#define C#2 73
#define D2 74
#define D#2 75
#define E2 76
#define F2 77
#define F#2 78

 
