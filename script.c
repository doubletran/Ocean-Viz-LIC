#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define EPSILON 0.3f
#define MAX_LINE 33950  // Maximum line length
#define MAX_FIELDS 6  // Maximum number of fields per line X and Y components, s and t value
#define DELIMITER ","  // CSV delimiter
#define NUMT 168
#define NUMS 360
float Texture[NUMT][NUMS][4];
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
				int s = atoi( fields[0]);
        int t = atoi( fields[1]);
				float a = atof(fields[2]);
				float b = atof(fields[5]);
        float r = atof(fields[3]);
        float g = atof(fields[4]);
        Texture[t][s][0] = r;
        Texture[t][s][1] = g;
				Texture[t][s][2] = a;
        Texture[t][s][3] = b;
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
			WriteFloat( fp, Texture[t][s][2] );			// "alpha"
			WriteFloat( fp, Texture[t][s][3]  );			// "alpha"
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
int WriteNoiseTex(char* filename){
	FILE *fp = fopen(filename, "rb");
  if (fp == NULL) {
      perror("Error opening file");
      return 1;
  }

    // Read dimensions
    int nums, numt;
    fread(&nums, sizeof(int), 1, fp);
    fread(&numt, sizeof(int), 1, fp);

    printf("Texture Size: %d x %d\n", nums, numt);

    // Allocate memory for texture data
    float *textureData = (float *)malloc(nums * numt * 4 * sizeof(float));
    if (textureData == NULL) {
        perror("Memory allocation failed");
        fclose(fp);
        return 1;
    }

    // Read RGBA data
    fread(textureData, sizeof(float), nums * numt * 4, fp);
    fclose(fp);
		int t_ratio = NUMT /numt;
		int s_ratio = NUMS / nums;
		float step = 0.045;
		   // printf("Texture Data (RGBA values):\n");
			 for (int t = 0; t < numt; t++) {
        for (int s = 0; s < nums; s++) {
          int idx = (t * nums + s) * 4;
            float red   = textureData[(t * nums + s) * 4 + 0];
            float green = textureData[(t * nums + s) * 4 + 1];
            float blue  = textureData[(t * nums + s) * 4 + 2];
            if (( 1.0f - red) <EPSILON &&
            ( 1.0f - green) < EPSILON &&
            ( 1.0f - blue) < EPSILON) {

							//Find the pixel in the temperature texture
							float u = s / (float)nums;   // normalized horizontal position
							float v = t / (float)numt;   // normalized vertical position
							
							int s_tem = (int)(u * NUMS);     // scaled to Texture B
							int t_tem = (int)(v * NUMT);
						red = 0;
						green = 0;
						blue = 1;
						for (float i = Texture[t_tem][s_tem][3]; i > 0. ;i-= 0.01){
							if ((blue == 1.) && (green < 1.f)){
									green += step;
							}
							else if ((green >= 1.f) && (red < 1.f)) {
								if (blue > 0.0f) {
										blue -= step;
								} else {
										red += step;
								}
						}
						else if (red >= 1.f) {
								green -= step;
						}

            // Change the pixel data
            textureData[idx + 0] =red;   // R
            textureData[idx + 1] = green;   // G
            textureData[idx + 2] =blue;  // B
        } 
        }
    }
	}
		fp = fopen("tempNoise.tex", "wb");
    if (fp == NULL) {
        perror("Error opening file for writing");
        free(textureData);
        return 1;
    }

    // Write dimensions again
    fwrite(&nums, sizeof(int), 1, fp);
    fwrite(&numt, sizeof(int), 1, fp);

    // Write the modified texture data
    fwrite(textureData, sizeof(float), nums * numt * 4, fp);
    fclose(fp);

    // Free allocated memory
    free(textureData);
    return 0;
}
int main() {
    const char *filename = "table.csv";  // Example CSV file
    readCSV(filename);
    WriteTex2d("current.tex");
		WriteNoiseTex("vis2dtexture.tex");
    return 0;
}