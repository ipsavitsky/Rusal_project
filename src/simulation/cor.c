#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<string.h>

int h=0;
int m=0;
int s=0;

char *name(){
	int i=rand();
	char *name=(char*)malloc(10*sizeof(char));
	i=i%10000;
	sprintf(name,"%d",i);
	return name;
}

void convertt(int c){
	h=(int)c/3600;
	m=(int)((c-h*3600)/60);
	s=c-h*3600-m*60;
}
	

int main(){
	double a;
	double b;
	int c;
	FILE *f;
	FILE *o;
	char* tmp = malloc(100*sizeof(char));
	f=fopen("inp","r");
	srand(time(NULL));
	while(fscanf(f,"%lf",&a)>=1){
		fscanf(f,"%lf",&b);
		fscanf(f,"%d",&c);
		tmp = name();
		strcat(tmp, ".txt");
		o=fopen(tmp,"w");
		convertt(c);
		fprintf(o,"111111111111111_30.05.2018_");
		if (h>=10){
			fprintf(o,"%d:",h);
		}else{
			fprintf(o,"0");
			fprintf(o,"%d:",h);
		}
		if (m>=10){
			fprintf(o,"%d:",m);
		}else{
			fprintf(o,"0");
			fprintf(o,"%d:",m);
		}
		if (s>=10){
			fprintf(o,"%d_",s);
		}else{
			fprintf(o,"0");
			fprintf(o,"%d_",s);
		}
		fprintf(o,"%lf_",b);
		fprintf(o,"%lf",a);

		fclose(o);
	}
}
	
	
	
	
	
	
	
