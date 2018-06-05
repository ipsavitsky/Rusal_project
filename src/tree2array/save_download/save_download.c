#include<stdio.h>
#include<stdlib.h>

void save(char *mas, int k_coor, int k_y, char* fname);
void download(char **q, char* fname);
void make_menu(int a);
void zap_coor(char *mas, int *mas_years, int year, int month, int day, char* time);
void zap_day(char *mas, int *mas_years, int year, int month, int day, int k_coor);
void zap_month(char *mas, int *mas_years, int year, int month, int k_coor);
int* sled_day(char *mas, int *mas_years, int year, int month, int day, int k_coor);
void zap_year(char *mas, int *mas_years, int year, int k_coor);
void zap_all(char *mas, int *mas_years, int k_coor);

int main(){
	int k_y;
	int k_coor;
	int year, month, day;
	int years_t=0, month_t=0, day_t=0;
	int ch;
	int g;
	int smesch;
	double x;
	double y;
	double *pom_d;
	int *pom_i;
	char time[10];
	char *mas;
	int *mas_years;
	int i;
	int j;
	int t;
	char fname_zap[]="zapros.txt";
	char fname1[]="out_in.txt";
	//int dlina;
	int sec; 
	FILE *f;
	FILE*c;
	FILE*zap;
	c=fopen("coor.txt", "r");
	f=fopen("years.txt", "r");
									//заполняем массив с годами
	fscanf(f, "%d", &k_y);
	mas_years=malloc(k_y*sizeof(int));
	for(j=0;j<k_y;j++)
	{
		fscanf(f, "%d", &mas_years[j]);
	}
									//заполняем большой массив годами 
	
	fscanf(c, "%d", &k_coor);
	mas=malloc((k_coor*(sizeof(int)+2*sizeof(double))+(k_y+1)*sizeof(int)+12*31*k_y*sizeof(int))*sizeof(char));
	pom_i=(int*)(mas);
	*pom_i=k_y;
	for(j=1;j<=k_y;j++)
	{
		pom_i=(int*)(mas+j*sizeof(int));
		*pom_i=mas_years[j-1];
	}
									//заполняем дни 0
	for(j=0;j<31*12*k_y;j++)
	{
			pom_i=(int*)(mas+(k_y+1)*sizeof(int)+j*sizeof(int));
			*pom_i=0;
	}
									//заполняем большой массив координатами
	t=0;
	while(fscanf(c, "%d%d%d%s%lf%lf", &year, &month, &day, time, &x, &y)==6)
	{
		sec=(int)(time[0]-'0')*10*60*60 + (int)(time[1]-'0')*60*60 + ((int)time[3]-'0')*10*60 + ((int)time[4]-'0')*60 + ((int)time[6]-'0')*10 + ((int)time[7]-'0');
		pom_i=(int*)(mas + (k_y+1)*sizeof(int) + k_y*31*12*sizeof(int) + t*sizeof(int) + 2*t*sizeof(double));
		*pom_i=sec;
		pom_d=(double*)(mas + (k_y+1)*sizeof(int) + k_y*31*12*sizeof(int) + (t+1)*sizeof(int) + 2*t*sizeof(double));
		*pom_d=x;
		pom_d=(double*)(mas + (k_y+1)*sizeof(int) + k_y*31*12*sizeof(int) + (t+1)*sizeof(int) + 2*t*sizeof(double)+sizeof(double));
		*pom_d=y;
		if(year!=years_t || month!=month_t || day!=day_t)
		{
		years_t=year;
		month_t=month;
		day_t=day;
		ch=0;
		while(mas_years[ch]!=year)
		{
			ch++;
		}
		smesch=(k_y+1)*sizeof(int) + k_y*31*12*sizeof(int) + t*(sizeof(int)+2*sizeof(double));
		pom_i=(int*)(mas+(k_y+1)*sizeof(int)+ch*31*12*sizeof(int)+(month-1)*31*sizeof(int)+(day-1)*sizeof(int));
		*pom_i=smesch;
		}
		t++;
	}
									//не работает "заливка" и "вытаскивание" информации
	save(mas, k_coor, k_y, fname1);
	download(&mas, fname1);
	pom_i=(int*)(mas);
	mas_years=malloc((*pom_i)*sizeof(int));
	k_y=(*pom_i);
	for(i=0;i<k_y;i++)
	{
		pom_i=(int*)(mas+(i+1)*sizeof(int));
		mas_years[i]=(*pom_i);
	}
									//меню запросов
	make_menu(1);
	printf("\n Выберите команду - ");
	while(scanf("%d", &g)==1)
	{
		printf("\n");
		zap=fopen(fname_zap, "r");
		if(g==1)
		{
			printf("Результаты запроса координат\n");
			while(fscanf(zap, "%d%d%d%s", &year, &month, &day, time)==4)
			{	
				printf("%d\n", year);
				printf("%6d\n", month);
				printf("%8d\n", day);
				zap_coor(mas, mas_years, year, month, day, time);
			}
		}
		if(g==2)
		{
			printf("Результаты запроса данных по дню\n");
			while(fscanf(zap, "%d%d%d", &year, &month, &day)==3)
			{
				printf("%d\n", year);
				printf("%6d\n", month);
				printf("%8d\n", day);
				zap_day(mas, mas_years, year, month, day, k_coor);
			}
		}
		if(g==3)
		{
			printf("Результаты запроса данных по месяцу\n");
			while(fscanf(zap, "%d%d", &year, &month)==2)
			{
				printf("%d\n", year);
				printf("%6d\n", month);
				zap_month(mas, mas_years, year, month, k_coor);
			}
		}
		if(g==4)
		{
			printf("Результаты запроса данных по году\n");
			while(fscanf(zap, "%d", &year)==1)
			{
				printf("%d\n", year);
				zap_year(mas, mas_years, year, k_coor);
			}
		}
		if(g==5)
		{
			printf("Результаты запроса всех данных\n");
			zap_all(mas, mas_years, k_coor);
		}
		fclose(zap);
		printf("\n Выберите команду - ");
	}
	return 0;
}

void save(char *mas, int k_coor, int k_y, char* fname)
{
	int all=k_coor*(sizeof(int)+2*sizeof(double))+(k_y+1)*sizeof(int)+12*31*k_y*sizeof(int);
	FILE* out;
	out=fopen(fname, "wb");
	fwrite((void*)&(all), sizeof(int), 1, out);
	fwrite((void*)&(mas[0]), sizeof(char), all, out);
	fclose(out);
}

void download(char **q, char* fname)
{
	int i;
	int k;
	char* mas;
	FILE* out;
	out=fopen(fname, "rb");
	//printf("1\n");	
	fread((void*)&(k), sizeof(int), 1, out);
	mas = malloc(k*sizeof(char));
	fread((void*)&(mas[0]), sizeof(char), k, out);
	fclose(out);
	*q=mas;
}

void make_menu(int a)
{
	printf("**************Меню****************\n");
	printf("1 - запрос координат\n");
	printf("2 - запрос информации по дню\n");
	printf("3 - запрос информации по месяцу\n");
	printf("4 - запрос информации по году\n");
	printf("5 - запрос всей информации\n");
}

void zap_coor(char *mas, int *mas_years, int year, int month, int day, char* time)
{
	int sec=(int)(time[0]-'0')*10*60*60 + (int)(time[1]-'0')*60*60 + ((int)time[3]-'0')*10*60 + ((int)time[4]-'0')*60 + ((int)time[6]-'0')*10 + ((int)time[7]-'0');
	int* pom_i;
	int* day_adr;
	double* pom_d;
	int ch=0;
	int t=0;
	int k_y;
	while(mas_years[ch]!=year)
	{
		ch++;
	}
	pom_i=(int*)(mas);
	k_y=*pom_i;
	pom_i=(int*)(mas+(k_y+1)*sizeof(int)+ch*31*12*sizeof(int)+(month-1)*31*sizeof(int)+(day-1)*sizeof(int));
	day_adr=pom_i;
	while(*pom_i!=sec)
	{
		pom_i=(int*)(&mas[*day_adr]+sizeof(int)*t+2*sizeof(double)*t);
		t++;
	}
	t--;
	printf("%14d  ", *pom_i);
	pom_d=(double*)(&mas[*day_adr]+sizeof(int)+sizeof(int)*t+2*sizeof(double)*t);
	printf("%lf  ", *pom_d);
	pom_d=(double*)(&mas[*day_adr]+sizeof(int)+sizeof(double)+sizeof(int)*t+2*sizeof(double)*t);
	printf("%lf\n", *pom_d);
}

void zap_day(char *mas, int *mas_years, int year, int month, int day, int k_coor)
{
	int* pom_i;
	int* day_adr;
	int* day_adr_sled;
	int i=0;
	double* pom_d;
	int ch=0;
	int t;
	int k_y;
	FILE * zap=fopen("zapros.txt", "r");
	while(mas_years[ch]!=year)
	{
		ch++;
	}
	pom_i=(int*)(mas);
	k_y=*pom_i;
	pom_i=(int*)(mas+(k_y+1)*sizeof(int)+ch*31*12*sizeof(int)+(month-1)*31*sizeof(int)+(day-1)*sizeof(int));
	day_adr=pom_i;
	day_adr_sled=(int*)(mas+(k_y+1)*sizeof(int)+ch*31*12*sizeof(int)+(month-1)*31*sizeof(int)+(day)*sizeof(int));
	t=1;
	while(*day_adr_sled==0)
	{
		day_adr_sled=(int*)(mas+(k_y+1)*sizeof(int)+ch*31*12*sizeof(int)+(month-1)*31*sizeof(int)+(day+t)*sizeof(int));
		t++;
	}
	t=0;
	while( (&mas[*day_adr] + t*sizeof(int) + 2*t*sizeof(double))!=&mas[*day_adr_sled] && (&mas[*day_adr] + t*sizeof(int) + 2*t*sizeof(double))!=&mas[(k_y+1)*sizeof(int)+12*31*k_y*sizeof(int)+(k_coor)*(sizeof(int)+2*sizeof(double))])
	{
		pom_i=(int*)(&mas[*day_adr] + t*sizeof(int) + 2*t*sizeof(double));
		printf("%14d  ", *pom_i);
		pom_d=(double*)(&mas[*day_adr]+sizeof(int)+sizeof(int)*t+2*sizeof(double)*t);
		printf("%lf  ", *pom_d);
		pom_d=(double*)(&mas[*day_adr]+sizeof(int)+sizeof(double)+sizeof(int)*t+2*sizeof(double)*t);
		printf("%lf\n", *pom_d);
		t++;
	}
	fclose(zap);
}

int* sled_day(char *mas, int *mas_years, int year, int month, int day, int k_coor)
{
	int* pom_i;
	int* day_adr;
	int* day_adr_sled;
	int ch=0;
	int k_y;
	while(mas_years[ch]!=year)
	{
		ch++;
	}
	pom_i=(int*)(mas);
	k_y=*pom_i;
	pom_i=(int*)(mas+(k_y+1)*sizeof(int)+ch*31*12*sizeof(int)+(month-1)*31*sizeof(int)+(day-1)*sizeof(int));
	day_adr=pom_i;
	day_adr_sled=(int*)(mas+(k_y+1)*sizeof(int)+ch*31*12*sizeof(int)+(month-1)*31*sizeof(int)+(day)*sizeof(int));
	return day_adr_sled;
}

void zap_month(char *mas, int *mas_years, int year, int month, int k_coor)
{
	int* pom_i;
	int* day_adr;
	int* month_adr_sled;
	int i=0;
	int ch=0;
	int t;
	int k_y;
	while(mas_years[ch]!=year)
	{
		ch++;
	}
	pom_i=(int*)(mas);
	k_y=*pom_i;
	pom_i=(int*)(mas+(k_y+1)*sizeof(int)+ch*31*12*sizeof(int)+(month-1)*31*sizeof(int));
	day_adr=pom_i;
	t=1;
	while(*day_adr==0)
	{
		day_adr=(int*)(mas+(k_y+1)*sizeof(int)+ch*31*12*sizeof(int)+(month-1)*31*sizeof(int)+t*sizeof(int));
		t++;
	}
	int day=t;
	pom_i=(int*)(mas+(k_y+1)*sizeof(int)+ch*31*12*sizeof(int)+(month)*31*sizeof(int));
	month_adr_sled=pom_i;
	t=1;
	while(*month_adr_sled==0)
	{
		month_adr_sled=(int*)(mas+(k_y+1)*sizeof(int)+ch*31*12*sizeof(int)+(month)*31*sizeof(int)+t*sizeof(int));
		t++;
	}
	while(&mas[*day_adr]!=&mas[*month_adr_sled])
	{
		if(*day_adr!=0)
		{
			printf("%8d\n", day);
			zap_day(mas, mas_years, year, month, day, k_coor);
		}
		day_adr=sled_day(mas, mas_years, year, month, day, k_coor);
		day++;
		t++;
	}
}

void zap_year(char *mas, int *mas_years, int year, int k_coor)
{
	int* pom_i;
	int i=0;
	double* pom_d;
	int ch=0;
	int j;
	int s=0;
	int k_y;
	int m;
	while(mas_years[ch]!=year)
	{
		ch++;
	}
	pom_i=(int*)(mas);
	k_y=*pom_i;
	for(j=1; j<=12;j++)
	{
		s=0;
		for(m=0;m<31;m++)
		{
			pom_i=(int*)(mas+(k_y+1)*sizeof(int)+ch*31*12*sizeof(int)+(j-1)*31*sizeof(int)+m*sizeof(int));
			if(*pom_i!=0)
			{
				s++;
			}
		}
		if(s!=0)
		{
			printf("%6d\n", j);
			zap_month(mas, mas_years, year, j, k_coor);
		}
	}
}

void zap_all(char *mas, int *mas_years, int k_coor)
{
	int* pom_i;
	int i;
	int k_y;
	pom_i=(int*)(mas);
	k_y=*pom_i;
	for(i=0; i<k_y;i++)
	{
		printf("%d\n", mas_years[i]);
		zap_year(mas, mas_years, mas_years[i], k_coor);
	}
}

