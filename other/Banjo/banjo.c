int main(){
	create_connect();

	printf("This program plays a song on the Create\n");
	gc_song_array[0][0]=9; //there are 9 notes in the song
	gc_song_array[0][1]=64;//E
	gc_song_array[0][2]=16;//.25 sec
	gc_song_array[0][3]=65;//F
	gc song array[0][4]=16;// ? Create notes: 64 65
	gc_song_array[0][5]=67;//G
	gc_song_array[0][6]=32;//.5 sec
	gc_song_array[0][7]=64;//E
	gc_song_array[0][8]=32;//.5 sec
	gc_song_array[0][9]=65;//F
	gc_song_array[0][10]=32;//.5 sec
	gc_song_array[0][11]=62;//D
	gc_song_array[0][12]=32;//.5 sec
	gc_song_array[0][13]=64;//E
	gc_song_array[0][14]=32;//.5 sec
	gc_song_array[0][15]=60;//C
	gc_song_array[0][16]=32;//.5 sec
	gc_song_array[0][17]=62;//D
	gc_song_array[0][18]=32;//.5 sec
	create_load_song(0);
	printf("Song is starting\n");
	create_play_song(0);
	printf("Song playing %d\n",get_create_song_number(.05));
	while(get_create_song_playing(.1)){} //wait until song
	printf("Song has finished playing\n"); // finishes
	return 0;
}
