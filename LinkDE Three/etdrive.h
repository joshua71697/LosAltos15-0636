/************************************* ET DRIVE **************************************
* Functions to drive parallel to pipes. Based on old drive libraries.                *
* Currently supports ET sensors mounted to the right side.                           *
* Requires an ET in front of wheel for forward and one behind the wheel for backward *
*************************************************************************************/
//ports
#define ET 1
#define ETb 2
#define MOT_LEFT 0
#define MOT_RIGHT 3
//forward constants
#define SPDl 80.
#define SPDr 80.
#define nLEFT .9
#define nRIGHT .9
#define THRESHOLD 1
//backwards constants
#define SPDlb 80.
#define SPDrb 80.
#define nLEFTb .8
#define nRIGHTb .8
#define THRESHOLDb 3
//forward sensative constants
#define SPDls 90.
#define SPDrs 90.
#define nLEFTs .5
#define nRIGHTs .5
#define THRESHOLDs 2
//backwards sensative constants
#define SPDlbs 80.
#define SPDrbs 80.
#define nLEFTbs .65
#define nRIGHTbs .65
#define THRESHOLDbs 4

void etforward(distance){
	int etdistance = avg_etdouble();
	long newdist;
	newdist = distance*INtoCM*CMtoBEMF;//conversion ratio
	long l = gmpc(MOT_LEFT)+newdist;
	long r = gmpc(MOT_RIGHT)+newdist;
		
	while(gmpc(MOT_LEFT) < l && gmpc(MOT_RIGHT) < r){
		printf("%d", etdistance);
		printf(" - %d",analog_et(ET));
		if(analog_et(ET)>etdistance+THRESHOLD)//if greater: robot is too close
		{
			motor(MOT_LEFT,SPDl*nLEFT);
			motor(MOT_RIGHT,SPDr);
			printf(" - TOO CLOSE\n");
		}
		else if(analog_et(ET)<etdistance-THRESHOLD)//if less: robot is too far
		{
			motor(MOT_LEFT,SPDl);
			motor(MOT_RIGHT,SPDr*nRIGHT);
			printf(" - TOO FAR\n");
		}
		else{
			motor(MOT_LEFT,SPDl);
			motor(MOT_RIGHT,SPDr);
			printf(" - STRAIGHT\n");
		}
	}
	drive_off();
}

void etbackward(distance){
	
	int etdistance = avg_etdoubleb();
	long newdist;
	newdist = distance*INtoCM*CMtoBEMF;//conversion ratio
	long l = gmpc(MOT_LEFT)-newdist;
	long r = gmpc(MOT_RIGHT)-newdist;
		
	while(gmpc(MOT_LEFT) > l && gmpc(MOT_RIGHT) > r){
		printf("%d", etdistance);
		printf(" - %d",analog_et(ETb));
		if(analog_et(ETb)>etdistance+THRESHOLDb)//if greater: robot is too close
		{
			motor(MOT_LEFT,-SPDlb*nLEFT);
			motor(MOT_RIGHT,-SPDrb);
			printf(" - TOO CLOSE\n");
		}
		else if(analog_et(ETb)<etdistance-THRESHOLDb)//if less: robot is too far
		{
			motor(MOT_LEFT,-SPDlb);
			motor(MOT_RIGHT,-SPDrb*nRIGHT);
			printf(" - TOO FAR\n");
		}
		else{
			motor(MOT_LEFT,-SPDlb);
			motor(MOT_RIGHT,-SPDrb);
			printf(" - STRAIGHT\n");
		}
	}
	drive_off();
}

void etforward_sens(distance){
	int etdistance = avg_etdouble();
	long newdist;
	newdist = distance*INtoCM*CMtoBEMF;//conversion ratio
	long l = gmpc(MOT_LEFT)+newdist;
	long r = gmpc(MOT_RIGHT)+newdist;
		
	while(gmpc(MOT_LEFT) < l && gmpc(MOT_RIGHT) < r){
		printf("%d", etdistance);
		printf(" - %d",analog_et(ET));
		if(analog_et(ET)>etdistance+THRESHOLDs)//if greater: robot is too close
		{
			motor(MOT_LEFT,SPDls*nLEFTs);
			motor(MOT_RIGHT,SPDrs);
			printf(" - TOO CLOSE\n");
		}
		else if(analog_et(ET)<etdistance-THRESHOLDs)//if less: robot is too far
		{
			motor(MOT_LEFT,SPDls);
			motor(MOT_RIGHT,SPDrs*nRIGHTs);
			printf(" - TOO FAR\n");
		}
		else{
			motor(MOT_LEFT,SPDls);
			motor(MOT_RIGHT,SPDrs);
			printf(" - STRAIGHT\n");
		}
	}
	drive_off();
}

void etbackward_sens(distance){
	
	int etdistance = avg_etdoubleb();
	long newdist;
	newdist = distance*INtoCM*CMtoBEMF;//conversion ratio
	long l = gmpc(MOT_LEFT)-newdist;
	long r = gmpc(MOT_RIGHT)-newdist;
		
	while(gmpc(MOT_LEFT) > l && gmpc(MOT_RIGHT) > r){
		printf("%d", etdistance);
		printf(" - %d",analog_et(ETb));
		if(analog_et(ETb)>etdistance+THRESHOLDbs)//if greater: robot is too close
		{
			motor(MOT_LEFT,-SPDlbs*nLEFTbs);
			motor(MOT_RIGHT,-SPDrbs);
			printf(" - TOO CLOSE\n");
		}
		else if(analog_et(ETb)<etdistance-THRESHOLDbs)//if less: robot is too far
		{
			motor(MOT_LEFT,-SPDlbs);
			motor(MOT_RIGHT,-SPDrbs*nRIGHTbs);
			printf(" - TOO FAR\n");
		}
		else{
			motor(MOT_LEFT,-SPDlbs);
			motor(MOT_RIGHT,-SPDrbs);
			printf(" - STRAIGHT\n");
		}
	}
	drive_off();
}
int avg_etdistance(){
	int i = 1;
	int sum = 0;
	int loops = 30;
	for(i;i<loops+1;i++){
		//printf("%d\n",analog_et(ET));
		sum = sum+analog_et(ET);
	}
	return sum/loops;
}

int avg_etdistanceb(){
	int i = 1;
	int sum = 0;
	int loops = 30;
	for(i;i<loops+1;i++){
		//printf("%d\n",analog_et(ETb));
		sum = sum+analog_et(ETb);
	}
	return sum/loops;
}
int avg_etdouble(){
	int first = avg_etdistance();
	int second = avg_etdistance();
	if(first-second>10||first-second<-10){
		panic();
		avg_etdouble();
	}
	else{
		return (first+second)/2;
	}
}
int avg_etdoubleb(){
	int first = avg_etdistanceb();
	int second = avg_etdistanceb();
	if(first-second>10||first-second<-10){
		panic();
		avg_etdoubleb();
	}
	else{
		return (first+second)/2;
	}
}
