#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>


/**
 ** from David Knight and Gordon Mallinson,
 **     "Visualizing Unstructured Flow Data Using Dual Stream Functions",
 **     Visualization and Computer Graphics, IEEE Computer Society,
 **     Vol 2, No 4, December 1996, pp 355-363.
 **/


inline float
Vx( float x, float y, float z )
{
	return (  -3 + 6.*x - 4.*x*(y+1.) - 4.*z  );
}

inline float
Vy( float x, float y, float z )
{
	return (  12.*x - 4.*x*x - 12.*z + 4.*z*z  );
}

inline float
Vz( float x, float y, float z )
{
	return (  3. + 4.*x - 4.*x*(y+1.) - 6.*z + 4.*(y+1.)*z  );
}




#define NUMS		256
#define NUMT		256

const float XMIN = { -1. };
const float XMAX = {  1. };
const float YMIN = { -1. };
const float YMAX = {  1. };
const float ZMIN = { -1. };
const float ZMAX = {  1. };


float		Texture[NUMT][NUMS][3];
float		Velx[NUMT][NUMS];
float		Vely[NUMT][NUMS];
float		Velz[NUMT][NUMS];

char *		Tex2dFile;


int	WriteTex2d( char * );
void	WriteInt( FILE *, unsigned int );
void	WriteFloat( FILE *, float );
void	WriteShort( FILE *, unsigned short int );
void	WriteByte( FILE *, unsigned char );


int
main( int argc, char *argv[] )
{
	Tex2dFile = NULL;

	for( int i = 1; i < argc; i++ )
	{
		if( argv[i][0] == '-' )
		{
			switch( argv[i][1] )
			{
				default:
					fprintf( stderr, "Unknown argument: '%s'\n", argv[i] );
			}
		}
		else
		{
			Tex2dFile = argv[i];
		}
	}

	if( Tex2dFile == NULL )
	{
		fprintf( stderr, "Usage: %s  [-s scale]  tex2dfilename\n", argv[0] );
		return 1;
	}



	float dx = ( XMAX - XMIN ) / (float)( NUMS - 1 );
	float dy = ( YMAX - YMIN ) / (float)( NUMT - 1 );
	float max = 0.;

	int   s, t;
	float x, y;
	float z = 0.;
	for( t = 0, y = YMIN; t < NUMT; t++, y += dy )
	{
		for( s = 0, x = XMIN; s < NUMS; s++, x += dx )
		{
			Velx[t][s] = 0;
			Vely[t][s] = x;
		//	Velx[t][s] = Vx( x, y, z );
			//Vely[t][s] = Vy( x, y, z );
			// Velz[t][s] = Vz( x, y, z );

			if(  fabs( Velx[t][s] ) > max  )	max = fabs( Velx[t][s] );
			if(  fabs( Vely[t][s] ) > max  )	max = fabs( Vely[t][s] );
			// if(  fabs( Velz[t][s] ) > max  )	max = fabs( Velz[t][s] );
		}
	}

	fprintf( stderr, "max = %8.2f\n", max );

	for( t = 0, y = YMIN; t < NUMT; t++, y += dy )
	{
		for( s = 0, x = XMIN; s < NUMS; s++, x += dx )
		{
			Texture[t][s][0] = Velx[t][s] / max;
			Texture[t][s][1] = Vely[t][s] / max;
			// Texture[t][s][2] = Velz[t][s] / max;
			Texture[t][s][2] = 0.;
		}
	}


	WriteTex2d( Tex2dFile );
}


int
WriteTex2d( char *filename )
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
