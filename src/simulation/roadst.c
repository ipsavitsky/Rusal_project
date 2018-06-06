#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

struct obj{
	int i;
	double pos;
	double v;
	int *aim;
	double x;
	double y;
};
//func change pos between points by tic
void move(struct obj*car,int **rd, int **twn){
	if (rd[(car->aim)[car->i]][2]<car->pos+car->v){
		car->pos=car->pos+car->v-rd[(car->aim)[car->i]][2];
		car->i+=1;
	}else{
		car->pos+=car->v;
	}
}
//recounting pos -> x y
void change (struct obj* car, int **twn, int **rd){
	car->x=((twn[rd[car->aim[car->i]][1]-1][0]-twn[rd[car->aim[car->i]][0]-1][0])*car->pos)/rd[car->aim[car->i]][2]+twn[rd[car->aim[car->i]][0]-1][0];
	car->y=((twn[rd[car->aim[car->i]][1]-1][1]-twn[rd[car->aim[car->i]][0]-1][1])*car->pos)/rd[car->aim[car->i]][2]+twn[rd[car->aim[car->i]][0]-1][1];
}		
//correcting default cordinates
void correct1 (int *n){
	srand(time(NULL));
	int i=rand();
	i=i%21-10;
	*n=*n+i;
}
//making files names
char *name(){
	srand(time(NULL));
	int i=rand();
	char *name=(char*)malloc(10*sizeof(char));
	i=i%10000;
	sprintf(name,"%d",i);
	return name;
}
	

int main(){
	int tm=0;
	int n;
	int i;
	int *nas;
	int *mas;
	int **rd;
	int ** twn;
	FILE *f;
	FILE *o;
	f=fopen("inputt.txt","r");
	o=fopen(name(),"w");
	fscanf(f,"%d",&n);
	nas=(int *)malloc(n*2*sizeof(int));
	twn=(int **)malloc(n*sizeof(int *));
	//initialisation
	for (i=0;i<n;i++){
		twn[i]=&nas[i*2];
	}
	for (i=0;i<n;i++){
		fscanf(f,"%d",&twn[i][0]);
		correct1(&twn[i][0]);
		fscanf(f,"%d",&twn[i][1]);
		correct1(&twn[i][1]);
	}
	printf("%d\n",n);
	for (i=0;i<n;i++){
		printf("%d %d\n",twn[i][0],twn[i][1]);
	}
	fscanf(f,"%d",&n);
	mas=(int *)malloc(n*3*sizeof(int));
	rd=(int **)malloc(n*sizeof(int *));
	for (i=0;i<n;i++){
		rd[i]=&mas[i*3];
	}
	for (i=0;i<n;i++){
		fscanf(f,"%d",&rd[i][0]);
		fscanf(f,"%d",&rd[i][1]);
	}
	printf("%d\n",n);
	for (i=0;i<n;i++){
		printf("%d %d\n",rd[i][0],rd[i][1]);
	}
	//convert x y -> length between points
	for (i=0;i<n;i++){
		rd[i][2]=(int)sqrt((pow(twn[rd[i][0]-1][0]-twn[rd[i][1]-1][0],2)+pow(twn[rd[i][0]-1][1]-twn[rd[i][1]-1][1],2)));
		printf(">%d %d| %d %d| %d<",twn[rd[i][0]-1][0],twn[rd[i][1]-1][0],twn[rd[i][0]-1][1],twn[rd[i][1]-1][1],rd[i][2]);
	}
	int k;
	fscanf(f,"%d",&k);
	printf("%d",k);
	//filling obj car
	struct obj* car;
	car=(struct obj*)malloc(sizeof(struct obj));
	car->aim=(int *)malloc(k*sizeof(int));
	for (i=0;i<k;i++){
		fscanf(f,"%d",&car->aim[i]);
		printf("%d",car->aim[i]);
	}							
	fscanf(f,"%lf",&car->v);
	printf("%lf",car->v);
	fscanf(f,"%lf",&car->pos);
	printf("%lf",car->pos);	
	car->i=0;
	fscanf(f,"%lf",&car->x);
	printf("%lf",car->x);
	fscanf(f,"%lf",&car->y);
	printf("%lf",car->y);
	//moving process
	while (car->i<k-1){
		fprintf(o,"%f %f %d\n",car->x,car->y,tm);
		tm+=1;
		move(car,rd,twn);
		change (car,twn,rd);
	}
}
