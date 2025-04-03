#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 33950  // Maximum line length
#define MAX_FIELDS 4  // Maximum number of fields per line X and Y components, s and t value
#define DELIMITER ","  // CSV delimiter
#define NUMT 170
#define NUMS 360
float Texture[NUMT][NUMS][3];
void readCSV(const char *);
int	WriteTex2d( const char * );
void	WriteInt( FILE *, unsigned int );
void	WriteFloat( FILE *, float );
void	WriteShort( FILE *, unsigned short int );
void	WriteByte( FILE *, unsigned char );
void readCSV(const char *filename) {
    FILE *file = fopen(filename, "r");  // Open file in read mode
    if (!file) {
        perror("Error opening file");
        return;
    }

    memset(Texture, 0, sizeof(Texture));
    char line[MAX_LINE];  // Buffer for reading each line
    while (fgets(line, sizeof(line), file)) {
        char *fields[MAX_FIELDS];  // Array to store parsed fields
        int fieldCount = 0;

        char *token = strtok(line, DELIMITER);
        while (token && fieldCount < MAX_FIELDS) {
            fields[fieldCount++] = token;
            token = strtok(NULL, DELIMITER);
        }
        int s = atoi( fields[2]);
        int t = atoi( fields[3]);
        float r = atof(fields[0]);
        float g = atof(fields[1]);
        Texture[t][s][0] = (r + 1.)/2.;
        Texture[t][s][1] = (g+ 1.)/2.;
        Texture[t][s][2] = 0.;
        // Print extracted fields
        /*
        for (int i = 0; i < fieldCount; i++) {
            printf("(%f,%f) ",r, g);
        }
        */
    }

    fclose(file);  // Close the file
}
int
WriteTex2d( const char *filename )
{
	FILE *fp;			// pointer to 2D texture file


	// try to create the file:

	fp = fopen( filename, "wb" );
	if( fp == NULL )
	{
		fprintf( stderr, "Cannot open Tex2D file '%s'\n", filename );
		return -1;
	}


	// write out the file header:

	WriteInt( fp, NUMS );
	WriteInt( fp, NUMT );


	// write the pixels:

	for( int t = 0; t < NUMT; t++ )
	{
		for( int s = 0; s < NUMS; s++ )
		{
			WriteFloat( fp, Texture[t][s][0] );
			WriteFloat( fp, Texture[t][s][1] );
			WriteFloat( fp, 0. );			// "alpha"
			WriteFloat( fp, 0. );			// "alpha"
		}
	}


	// end game:

	fflush( fp );
	fclose( fp );
	return 0;
}

void
WriteInt( FILE *fp, unsigned int i )
{
#ifdef OLDWAY
	fputc( (i>> 0) & 0xff, fp );
	fputc( (i>> 8) & 0xff, fp );
	fputc( (i>>16) & 0xff, fp );
	fputc( (i>>24) & 0xff, fp );
#endif
	fwrite( &i, 4, 1, fp );
}


void
WriteFloat( FILE *fp, float f )
{
	fwrite( &f, 4, 1, fp );
}


void
WriteShort( FILE *fp, unsigned short int i )
{
	fputc( (i>>0) & 0xff, fp );
	fputc( (i>>8) & 0xff, fp );
}


inline void
WriteByte( FILE *fp, unsigned char i )
{
	fputc( i & 0xff, fp );
}

int main() {
    const char *filename = "table.csv";  // Example CSV file
    readCSV(filename);
    WriteTex2d("current.tex");
    return 0;
}