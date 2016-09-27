int BoR;// 0 se sfera rossa, 1 se blu
int fase,sottofase;
float stato[12];
float statoAvv[12];
float vai[3];
float punta[3];
float posAvv[3];
float pos[3];
float facing[3];
float debug[7];//7 variabili di debug, da usare
float zona[4];//posizione e dimensione drop zone

float dist(float a[],float b[]){
    return sqrt(mathSquare(a[0]-b[0])+mathSquare(a[1]-b[1])+mathSquare(a[2]-b[2]));
}

void setV(float v[],float x,float y,float z){ //Definitivo
    v[0]=x;
    v[1]=y;
    v[2]=z;
}
void setV(float v[],float c[]){ //Definitivo
    v[0]=c[0];
    v[1]=c[1];
    v[2]=c[2];
}

void muovi(){
    api.setPositionTarget(vai);
}

void frena(){       //To do  --Utile per evitare parabola
    api.setPositionTarget(pos);
}

void ruota(){
    api.setAttRateTarget(punta);
}

bool oOB (float ptc[]){//outOfBounds, Definitivo, Eliminabile se serve spazio
    if(fabsf(ptc[0])>0.75||fabsf(ptc[1])>0.75||fabsf(ptc[2])>0.75)
        return true;
    return false;
}

void init(){
	for(int i=0;i<7;i++)debug[i]=0;
	for(int i=0;i<3;i++)vai[i]=punta[i]=0;
	api.getMyZRState(stato);
	BoR=stato[1]>0?0:1;//capisco quale sfera siamo
	fase=sottofase=0;
	game.getItemLoc(vai,0);
}

void inizio(){
    api.getMyZRState(stato);//ottengo dati attuali
	api.getOtherZRState(statoAvv);//ottengo dati avversario
	for(int i=0;i<3;i++){
	    pos[i]=stato[i];
	    facing[i]=stato[i+6];
	    posAvv[i]=statoAvv[i];
	} //ottengo posizione e direzione miei e posizione avversario (magari poi anche direzione avversario)
}

void fine(){
	api.setDebug(debug);
	ruota();//ordine di rotazione verso punta[]
	muovi();//ordine di spostamento verso vai[]
}

void loop(){
    inizio();
	switch(fase){
	    case 0://drop SPSs, i cosi per sapere dove si trova la zhohona
	        if (sottofase==0 && dist(vai,pos)<0.1){
	            game.dropSPS();
	            sottofase++;
	            game.getItemLoc(vai,1);
	        } else if (sottofase==1 && dist(vai,pos)<0.1){
	            game.dropSPS();
	            sottofase++;
	            game.getItemLoc(vai,2);
	        } else if(sottofase==2 && dist(vai,pos)<0.1){
	            game.dropSPS();
	            sottofase=0;
	            fase++;
	        }
	    break;
	    case 1://getZone
	        if(!game.getZone(zona))//se true, carica anche la posizione della sfera in zona
	            fase--;
			else
				setV(vai,zona);
	    break;
	    default:
	    DEBUG(("ERROR"));
	}
	fine();
}
