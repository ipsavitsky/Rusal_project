#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct track_point {
	double x, y;
	int pic_x, pic_y;
	char * date;
	char * time;
	char * IMEI;
	struct track_point * next;
} track_point;

typedef struct list {
	track_point * head;
	track_point * tail;
	int n;
} list;

typedef struct coor {								// структура 4 крайних точек карты
	double x1, x2, x3, x4, y1, y2, y3, y4;
	double lenght_top, lenght_right;
	int width, height;
} coor;

list * List_init () {
	list * p = (list *) malloc (sizeof(list));
		p->head = NULL;
		p->tail = NULL;
		p->n = 0;
	return p;
}

void push_point (list * p, char * date, char * time, char * IMEI, double x, double y) {
	track_point * point = (track_point *) malloc (sizeof(track_point));
		point->date = (char *) malloc (sizeof(char) * 32);
		strcpy (point->date, date);
		point->time = (char *) malloc (sizeof(char) * 32);
		strcpy (point->time, time);
		point->IMEI = (char *) malloc (sizeof(char) * 32);
		strcpy (point->IMEI, IMEI);
		point->x = x;
		point->y = y;
		point->pic_x = -1;
		point->pic_y = -1;
		point->next = NULL;
		if (p->n == 0) {
			p->head = point;
			p->tail = point;
			p->n = 1;
		}
		else {
			p->tail->next = point;
			p->tail = point;
			p->n++;
		}
	return;
}	

int print_point (list * p) {
		if (p->n == 0) {
			printf ("\nпустой лист\n");
			return -1;
		}
	int i;
	track_point * cur = p->head;
		for (i=1; i <= p->n; i++) {
			printf ("%d: %s %s %lf %lf %d %d\n", i, cur->date, cur->time, cur->x, cur->y, cur->pic_x, cur->pic_y);
			cur = cur->next;
		}
	return i;
}

coor * Coor_init () {
	FILE * in = fopen ("coor.txt", "r");
		coor * res = (coor *) malloc (sizeof(coor));
		fscanf (in, "%d%d", &(res->width), &(res->height));
		fscanf (in, "%lf%lf%lf%lf%lf%lf%lf%lf", &(res->y1), &(res->x1), &(res->y2), &(res->x2), &(res->y3), &(res->x3), &(res->y4), &(res->x4));
		res->lenght_top = sqrt ((res->x1 - res->x2)*(res->x1 - res->x2) + (res->y1 - res->y2)*(res->y1 - res->y2));
		res->lenght_right = sqrt ((res->x1 - res->x3)*(res->x1 - res->x3) + (res->y1 - res->y3)*(res->y1 - res->y3));
		printf ("	lenght_top: %lf\n	lenght_right: %lf\n", res->lenght_top, res->lenght_right);
	return res;
}

void point_to_bmp_coor (track_point * point, coor * Coor) {
	double l1 = sqrt ((Coor->x1 - point->x)*(Coor->x1 - point->x) + (Coor->y1 - point->y)*(Coor->y1 - point->y));
	double l2 = sqrt ((Coor->x2 - point->x)*(Coor->x2 - point->x) + (Coor->y2 - point->y)*(Coor->y2 - point->y));
	double l3 = sqrt ((Coor->x3 - point->x)*(Coor->x3 - point->x) + (Coor->y3 - point->y)*(Coor->y3 - point->y));
	double p1 = (Coor->lenght_right + l1 + l3)/2;
	double p2 = (Coor->lenght_top + l1 + l2)/2;
	double s1 = sqrt(p1*(p1 - Coor->lenght_right)*(p1 - l1)*(p1 - l3));
	double s2 = sqrt(p2*(p2 - Coor->lenght_top)*(p2 - l1)*(p2 - l2));
		//printf (" l1: %lf\n l2: %lf\n l3: %lf\n p1: %lf\n p2: %lf\n s1: %lf\n s2: %lf\n", l1, l2, l3, p1, p2, s1, s2);
		//point->pic_x = (2*s1/Coor->lenght_right) / Coor->lenght_top * Coor->width; 
		//point->pic_y = Coor->height - (2*s2/Coor->lenght_top) / Coor->lenght_right * Coor->height;
		point->pic_y = Coor->height - (2*s1/Coor->lenght_right) / Coor->lenght_top * Coor->height; 
		point->pic_x = (2*s2/Coor->lenght_top) / Coor->lenght_right * Coor->width;
	return;
}

int main () {
	int count_points, i, j, s, h, m;
	char date[32];
	char time[32];
	char IMEI[32];
	double x, y;
	
	FILE * in = fopen ("wad.txt", "r");				// файл с координатами траектории
	FILE * out = fopen ("bmp_xy.txt", "w");			// файл с обработанными точками траектории
		if (in == NULL || out == NULL) {
				printf ("\n	no such files\n");
			return -1;
		}
		
		fscanf (in, "%d", &count_points);
		printf ("%d\n", count_points);
	
	list * points = List_init ();
	char string [count_points][64];
		for (i=0; i<count_points; i++) {
			fscanf (in, "%s", string[i]);
			for (j=0; j<63; j++)
				if (string[i][j] == '_')
					string[i][j] = ' ';
			sscanf (string[i], "%s%s%s%lf%lf", IMEI, date, time, &y, &x);
			push_point (points, date, time, IMEI, x, y);
		}
		fclose (in);
		print_point (points);
		
		coor * Coor = Coor_init ();
	
	track_point * cur = points->head;
		for (i=0; i<count_points; i++) {
			point_to_bmp_coor (cur, Coor);
			cur = cur->next;
		}
		print_point (points);
		
		cur = points->head;
		fprintf (out, "%d\n", count_points);
		for (i=0; i<count_points; i++) {
			sscanf (cur->time, "%d:%d:%d", &h, &m, &s);
			//printf ("%d %d %d\n", h, m, s);
			s += h*3600 + m*60;
			fprintf (out, "%d %d %d\n", s, cur->pic_x, cur->pic_y);
			cur = cur->next;
		}
		fclose (out);
	return 0;
}	