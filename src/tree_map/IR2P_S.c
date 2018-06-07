#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void s_bubblesort (char ** names, int count_files) {
		if (count_files == 0) {
			printf ("s_bubblesort: count_files = 0\n");
			exit (1);
		}
	int len_a, len_b, l, i, j, k;
	char * tmp_str = (char *) calloc (64, sizeof(char));
		if (tmp_str == NULL) {
			printf ("s_bubblesort: tmp_str calloc error\n");
			exit (2);
		}
		for (i=0; i<count_files; i++) {
			k = count_files - i - 1;
			for (j=0; j<k; j++)
				if (strcmp (names[j], names[j+1]) > 0) {
					len_a = strlen (names[j]);
					len_b = strlen (names[j+1]);
					for (l=0; l<len_a; l++)
						tmp_str[l] = names[j][l];
					for (l=0; l<len_b; l++)
						names[j][l] = names[j+1][l];
					names[j][l] = 0;
					for (l=0; l<len_a; l++)
						names[j+1][l] = tmp_str[l];
					names[j+1][l] = 0;
				}
		}
		free (tmp_str);
	return;
}	

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
	double lenght_top, lenght_left;
	int width, height;
} coor;

typedef struct step {
	double up_x, up_y, right_x, right_y;
} step;

list * List_init () {
	list * p = (list *) malloc (sizeof(list));
		if (p == NULL) {
			printf ("List_init: malloc error\n");
			exit (2);
		}
		p->head = NULL;
		p->tail = NULL;
		p->n = 0;
	return p;
}

void push_point (list * p, char * date, char * time, char * IMEI, double x, double y) {
		if (p == NULL) {
			printf ("push_point: list doesn't exist\n");
			exit (1);
		}
		if (date == NULL) {
			printf ("push_point: date doesn't exist\n");
			exit (1);
		}
		if (time == NULL) {
			printf ("push_point: time doesn't exist\n");
			exit (1);
		}
		if (IMEI == NULL) {
			printf ("push_point: IMEI doesn't exist\n");
			exit (1);
		}
	track_point * point = (track_point *) malloc (sizeof(track_point));
		if (point == NULL) {
			printf ("push_point: point malloc error\n");
			exit (2);
		}
		point->date = (char *) malloc (sizeof(char) * 32);
		if (point->date == NULL) {
			printf ("push_point: point->date malloc error\n");
			exit (2);
		}
		strcpy (point->date, date);
		point->time = (char *) malloc (sizeof(char) * 32);
		if (point->time == NULL) {
			printf ("push_point: point->time malloc error\n");
			exit (2);
		}
		strcpy (point->time, time);
		point->IMEI = (char *) malloc (sizeof(char) * 32);
		if (point->IMEI == NULL) {
			printf ("push_point: point->IMEI malloc error\n");
			exit (2);
		}
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
		if (p == NULL) {
			printf ("print_point: list doesn't exist\n");
			exit (1);
		}
		if (p->n == 0) {
			printf ("print_point: list is empty\n");
			exit (3);
		}
	int i;
	track_point * cur = p->head;
	int res = 0;
		for (i=1; i <= p->n; i++) {
			res += printf ("%d: %lf %lf %d %d\n", i, cur->x, cur->y, cur->pic_x, cur->pic_y);
			cur = cur->next;
		}
	return res;
}

coor * Coor_init (char * arg) {
	char * dir = (char *) calloc (60, sizeof(char));
		if (dir == NULL) {
			printf ("Coor_init: dir malloc error\n");
			exit (2);
		}
		strcat (dir, arg);
		strcat (dir, "/coor.txt");
	FILE * in = fopen (dir, "r");
		if (in == NULL) {
			printf ("Coor_init: coor.txt doesn't exist\n");
			exit (1);
		}
	coor * res = (coor *) malloc (sizeof(coor));
		if (res == NULL) {
			printf ("Coor_init: res malloc error\n");
			exit (2);
		}
		if (fscanf (in, "%d%d", &(res->width), &(res->height)) != 2) {
			printf ("Coor_init: fscanf error\n");
			exit (3);
		}
		if (fscanf (in, "%lf%lf%lf%lf%lf%lf%lf%lf", &(res->x1), &(res->y1), &(res->x2), &(res->y2), &(res->x3), &(res->y3), &(res->x4), &(res->y4)) != 8) {
			printf ("Coor_init: fscanf error\n");
			exit (3);
		} 
		res->lenght_left = sqrt ((res->x1 - res->x2)*(res->x1 - res->x2) + (res->y1 - res->y2)*(res->y1 - res->y2));
		res->lenght_top = sqrt ((res->x1 - res->x3)*(res->x1 - res->x3) + (res->y1 - res->y3)*(res->y1 - res->y3));
		//printf ("	lenght_top: %lf\n	lenght_left: %lf\n", res->lenght_top, res->lenght_left);
		free (dir);
	return res;
}

void point_to_bmp_coor (track_point * point, coor * Coor) {
		if (point == NULL) {
			printf ("point_to_bmp_coor: point doesn't exist\n");
			exit (1);
		}
		if (Coor == NULL) {
			printf ("point_to_bmp_coor: Coor doesn't exist\n");
			exit (1);
		}
	double l1 = sqrt ((Coor->x1 - point->x)*(Coor->x1 - point->x) + (Coor->y1 - point->y)*(Coor->y1 - point->y));
	double l2 = sqrt ((Coor->x2 - point->x)*(Coor->x2 - point->x) + (Coor->y2 - point->y)*(Coor->y2 - point->y));
	double l3 = sqrt ((Coor->x3 - point->x)*(Coor->x3 - point->x) + (Coor->y3 - point->y)*(Coor->y3 - point->y));
	double p1 = (Coor->lenght_left + l1 + l2)/2;
	double p2 = (Coor->lenght_top + l1 + l3)/2;
	double s1 = sqrt(p1*(p1 - Coor->lenght_left)*(p1 - l1)*(p1 - l2));
	double s2 = sqrt(p2*(p2 - Coor->lenght_top)*(p2 - l1)*(p2 - l3));
		point->pic_x = (2*s1/Coor->lenght_left)/Coor->lenght_top * Coor->width;
		point->pic_y = Coor->height - (2*s2/Coor->lenght_top)/Coor->lenght_left * Coor->height;
	return;
}

step * Step_init (coor * Coor) {
		if (Coor == NULL) {
			printf ("Step_init: Coor doesn't exist");
			exit (1);
		}
	step * Step = (step *) malloc (sizeof(step));
		if (Step == NULL) {
			printf ("Step_init: Step malloc error");
			exit (1);
		}
		Step->up_x = (Coor->x1 - Coor->x2) / Coor->height;
		Step->up_y = (Coor->y1 - Coor->y2) / Coor->height;
		Step->right_x = (Coor->x4 - Coor->x2) / Coor->width;
		Step->right_y = (Coor->y4 - Coor->y2) / Coor->width;
		printf ("\n	up_x: %lf	up_y: %lf\n	right_x: %lf	right_y: %lf\n\n", Step->up_x, Step->up_y, Step->right_x, Step->right_y);
	return Step;
}

void point_to_real_coor (track_point * point, coor * Coor, step * Step) {
		if (point == NULL) {
			printf ("point_to_real_coor: point doesn't exist\n");
			exit (1);
		}
		if (Coor == NULL) {
			printf ("point_to_real_coor: Coor doesn't exist\n");
			exit (1);
		}
		if (Step == NULL) {
			printf ("point_to_real_coor: Step doesn't exist\n");
			exit (1);
		}
		point->x = Coor->x2 + Step->up_x * point->pic_y + Step->right_x * point->pic_x;
		point->y = Coor->y2 + Step->up_y * point->pic_y + Step->right_y * point->pic_x;
	return;
}	

typedef struct FIELD {
	char * field;
	int Width, Height;
} FIELD;

typedef struct Color {
	char red, green, blue;
} Color;

char * malloc_char (int n) {
	char * mas = (char *) malloc (n * sizeof(char));
	int i;
		if(mas == NULL) {
			printf ("malloc_char: malloc error\n"); 
			exit(2);
		}
		for(i=0;i<n;i++) mas[i]=0;
	return mas;
}

FIELD * make_field (char * fbmp) {
	int height, width;
	FILE * in = fopen (fbmp, "r");
		if (in == NULL){
			printf ("%s: %s doesn't exist\n", __FUNCTION__, fbmp);
			exit(1);
		}
	unsigned char bm[54]; 
		fread((void *)bm, sizeof(char), 54, in);
		memcpy((void *)(&width), (void*) (bm + 18), 4);
		memcpy((void *)(&height), (void*) (bm + 22), 4);
	char * mas = malloc_char (width * height * 3);	
		fread((void *) mas, 1, width * height * 3, in);
		
	FIELD * field = (FIELD *) malloc (sizeof (FIELD));	
		if (field == NULL) {
			printf ("make_field: field malloc error\n");
			exit (2);
		}
		field->Width = width;
		field->Height = height;
		field->field = mas;
	return field;
}			
		
void data_to_bmp (char * fbmp1, char * fbmp2, FIELD * Field) {
	unsigned char bm[54];       //Массив для хранения заголовка		
	FILE * in = fopen (fbmp1, "rb"); 
	FILE * out = fopen (fbmp2, "wb");
			
			if (Field->Width % 4) {
				printf("data_to_bmp: Width некратна 4\n"); 
				exit(4);
			}
			
			if (in == NULL) {
				printf("data_to_bmp: %s doesn't exist\n", fbmp1); 
				exit(1);
			}
			if (out == NULL) {
				printf("data_to_bmp: %s doesn't exist\n", fbmp2); 
				exit(1);
			}

		//printf("%d * %d\n", Width, Height);
		fread ((void*) bm, 1, 54, in);
		fwrite ((void*) bm, 1, 54, out);
		fwrite ((void*) Field->field, 1, Field->Height * Field->Width * 3, out);
		fclose (in);
		fclose (out);
	return;
}

void Set_color (int x, int y, Color * color, FIELD * field) {
	if (x < 0 || x >= field->Width || y < 0 || y >= field->Height)
		return;
	int pos = 3 * (x + y * field->Width);
		field->field[pos] = color->blue;
		field->field[pos+1] = color->green;
		field->field[pos+2] = color->red;
	return;
}	

void Line (int x0, int y0, int x1, int y1, Color * color, FIELD * field) {
		if (color == NULL) {
			printf ("Line: color doesn't exist\n");
			exit (1);
		}
		if (field == NULL) {
			printf ("Line: field doesn't exist\n");
			exit (1);
		}
    int steep = 0, tmp;
		if (abs(x0-x1)<abs(y0-y1)) {
			tmp = x0;
			x0 = y0;
			y0 = tmp;
			tmp = x1;
			x1 = y1;
			y1 = tmp;
			steep = 1;
		}
		if (x0>x1) {
			tmp = x0;
			x0 = x1;
			x1 = tmp;
			tmp = y0;
			y0 = y1;
			y1 = tmp;
		}
    int dx = x1-x0;
    int dy = y1-y0;
    int derror2 = abs(dy)<<1;
    int error2 = 0;
    int y = y0;
		for (int x=x0; x<=x1; x++) {
			if (steep) {
				Set_color(y, x, color, field);
			} 
			else {
				Set_color(x, y, color, field);
			}
			error2 += derror2;

			if (error2 > dx) {
				y += (y1>y0?1:-1);
				error2 -= dx<<1;
			}
		}
	return;	
}

void Circle (int cent_x, int cent_y, double r, double widness, Color * color, FIELD * field) {
	int i, j, cur_x, cur_y, pos;
	int R = r + widness;
	double f_x, f_y;
	
		for (i = -R; i <= R; i++) {
			cur_x = cent_x + i;
			if (cur_x < field->Width && cur_x >= 0) {
				for (j = -R; j <= R; j++) {
					cur_y = cent_y + j;
					if (cur_y < field->Height && cur_y >= 0) {
						f_x = cur_x - cent_x;
						f_y = cur_y - cent_y;
						if (fabs (sqrt (f_x*f_x + f_y*f_y) - r) < widness) {
							Set_color (cur_x, cur_y, color, field);
							/*pos = cur_x + cur_y * field->Width;
							pos *= 3;
							field->field[pos] = color->blue;
							field->field[pos+1] = color->red;
							field->field[pos+2] = color->green;*/
						}
					}
				}
			}
		}
		
	return;
}

int main (int argc, char * argv[]) {
		if (argc != 2) {
			printf ("wrong count of args\n");
			return -2;
		}
	char * bmp_name = (char	*) calloc (30, sizeof(char));
		if (bmp_name == NULL) {
			printf ("main: bmp_name malloc error\n");
			return 2;
		}
	char * new_bmp_name = (char *) calloc (30, sizeof(char));
		if (new_bmp_name == NULL) {
			printf ("main: new_bmp_name malloc error\n");
			return 2;
		}
		
		strcat (bmp_name, argv[1]);
		strcat (bmp_name, "/test.bmp");
		printf ("%s\n", bmp_name);
		strcat (new_bmp_name, argv[1]);
		strcat (new_bmp_name, "/test_1.bmp");
	/*int argument = atoi (argv[1]);
		switch (argument) {
			case 1:
					strcpy (bmp_name, "./resources/map1/test.bmp");
					strcpy (new_bmp_name, "./resources/map1/test_1.bmp");
					strcpy (map_name, "map1");
				break;
			case 2:
					strcpy (bmp_name, "./resources/map2/test.bmp");
					strcpy (new_bmp_name, "./resources/map2/test_1.bmp");
					strcpy (map_name, "map2");
				break;
			case 3:
					strcpy (bmp_name, "./resources/map3/test.bmp");
					strcpy (new_bmp_name, "./resources/map3/test_1.bmp");
					strcpy (map_name, "map3");
				break;
			default:
					printf ("wrong arg\n");
				return -2;
		}*/
		
	FILE * tmp = fopen ("indexRequest.txt", "r");
		if (tmp == NULL) {
			printf("main: indexRequest.txt doesn't exist\n");
			return 1;
		}	
	int count_indexReq_str, i, j, count_files = 0;
	char * trash = (char *) malloc (sizeof(char) * 80);
		if (trash == NULL) {
			printf ("main: trash malloc error\n");
			return 2;
		}
	FILE * tmp2;	
		fscanf (tmp, "%d", &count_indexReq_str);
		for (i=0; i<count_indexReq_str; i++) {
			fscanf (tmp, "%s", trash);
			//printf ("trash: %s\n", trash);
			tmp2 = fopen (trash, "r");
			if (tmp2 != NULL) {
				count_files++;
				fclose (tmp2);
			}
		}
		fseek (tmp, 0 ,SEEK_SET);
		fscanf (tmp, "%d", &count_indexReq_str);
		//printf ("count_exist_files: %d\n", count_files);
	char ** full_directory = (char **) malloc (count_files * sizeof(char *));
		j = 0;
		for (i=0; i<count_indexReq_str; i++) {
			fscanf (tmp, "%s", trash);
			//printf ("trash: %s\n", trash);
			tmp2 = fopen (trash, "r");
			if (tmp2 != NULL) {
				full_directory[j] = (char *) malloc (sizeof(char) * 80);
				strcpy (full_directory[j], trash);
				//printf ("%s\n", full_directory[j]);
				j++;
				fclose (tmp2);
			}
		}
		fclose (tmp);
		s_bubblesort (full_directory, count_files);
	
	FILE * in;		
	char date[32];
	char time[32];
	char IMEI[32];
	double x, y;
	int l;
	list * points = List_init ();
	char ** string = (char **) malloc (sizeof(char *) * count_files);
		for (i=0; i<count_files; i++) {
			in = fopen (full_directory[i], "r");
			string[i] = (char *) malloc (sizeof(char) * 80);
			fscanf (in, "%s", string[i]);
			//printf ("%s\n", string[i]);
			l = strlen (string[i]);
			for (j=0; j<l; j++)
				if (string[i][j] == '_')
					string[i][j] = ' ';
			sscanf (string[i], "%s%s%s%lf%lf", IMEI, date, time, &x, &y);
			push_point (points, date, time, IMEI, x, y);
			fclose (in);
		}
		//print_point (points);
		//printf ("\n");
		coor * Coor = Coor_init (argv[1]);
		
	track_point * cur = points->head;
		for (i=0; i<count_files; i++) {
			point_to_bmp_coor (cur, Coor);
			cur = cur->next;
		}
		//print_point (points);

	FIELD * field = make_field (bmp_name);
		Color * color = (Color *) malloc (sizeof(Color));
		color->red = 250;
		color->green = 0;
		color->blue = 0;
		
		cur = points->head;
		Circle (cur->pic_x, cur->pic_y, 0.0, 3.0, color, field);
		for (i=1; i<count_files; i++) {
			Line (cur->pic_x, cur->pic_y, cur->next->pic_x, cur->next->pic_y, color, field);
			Circle (cur->next->pic_x, cur->next->pic_y, 0.0, 3.0, color, field);
			//printf ("line: from (%d;%d) to (%d;%d)\n", cur->pic_x, cur->pic_y, cur->next->pic_x, cur->next->pic_y);
			cur = cur->next;
		}
		data_to_bmp (bmp_name, new_bmp_name, field);	
		
		free (bmp_name);
		free (new_bmp_name);
		free (trash);	
		free (Coor);
	track_point * next = points->head;	
		for (i=0; i<count_files; i++) {
			cur = next;
			free (full_directory[i]);
			free (string[i]);
			free (cur->date);
			free (cur->time);
			free (cur->IMEI);
			next = cur->next;
			free (cur);
		}
		free (string);
		free (full_directory);
		free (points);
		free (field->field);
		free (field);
		free (color);
	return 0;
}	
