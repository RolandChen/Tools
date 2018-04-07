// Console based CSV Parser Framework in C
// RolandChen 18.4.5
// RolandChen 18.4.7  THX:LionNatsu
// first version, do not recognize '\' as esc char. 

#define FILENAME "1.csv"
#define MAXCOL    100
#define MAXLENGTH 1000

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

void ERROR(int);
void Print(char content[MAXCOL][MAXLENGTH], int colmax, int chrmax[MAXCOL], int count);
char content[MAXCOL][MAXLENGTH];

int main(int argc, char const *argv[])
{
	FILE *fp;
	char c = 0, lastchar=0 ;   //only parse one line: content[col][chr]
	int  col=0, chr=0, colmax=0, chrmax[MAXCOL] = {0}; //index
	int  qflag = 0, count = 0;                //signals

#pragma region Init
	for (int i = 0; i<MAXCOL; i++)for (int j = 0; j<MAXLENGTH; j++)content[i][j] = 0; //content={0}
	system("color 0f"); 
	fp = (fopen(FILENAME, "r"));
	if (fp == NULL)ERROR(1);
#pragma endregion

	for (; fscanf(fp, "%c", &c) != EOF; )
	{
		if (c == '"' && qflag == 0)qflag = 1;
		else if (c == '"' && qflag == 1)qflag = 0;
		else if (c == ',' && qflag == 0)
		{
			chrmax[col] = chr;
			chr = 0;
			col++;
			if (col == MAXCOL)ERROR(2);
			colmax = col;
		}
		else if (c == '\n' && qflag == 0) //End of a line
		{
			if (lastchar == ',')colmax--;
			Print(content, colmax, chrmax, count);
			count++;
			for (col = 0; col<MAXCOL; col++)chrmax[col] = 0;
			col = 0; chr = 0, colmax=0;
			continue;
		}
		else
		{
			content[col][chr] = c;
			chr++;
			if (chr == MAXLENGTH)ERROR(3);
			chrmax[col] = chr;
		}
		lastchar = c;
	}
	return 0;
}


void Print(char content[MAXCOL][MAXLENGTH], int colmax, int chrmax[MAXCOL], int count)
{
	int col, chr;
	printf("%d %d\n", count, colmax + 1);
	for (col = 0; col <= colmax; col++)
	{
		for (chr = 0; chr<chrmax[col]; chr++)printf("%c", content[col][chr]);
		getchar();
	}
	system("cls");
}


void ERROR(int errnum) //ERROR processing func
{
	fprintf(stderr,"ERROR %d occured: ", errnum);
	switch (errnum)
	{
	  case 1 : fprintf(stderr,"Input file not found"); break;
	  case 2 : fprintf(stderr,"TOO MUCH Columns, more than %d",MAXCOL); break;
	  case 3 : fprintf(stderr,"TOO MUCH Chars in a cell, more than %d",MAXLENGTH); break;
	  default: fprintf(stderr,"unknown error");
	}
	printf("\n");
	system("pause");
	exit(errnum);
}
