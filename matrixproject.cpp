//#define _CRT_SECURE_NO_WARNINGS
#define _DEBUG_
#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>//fabs


void ReadData( FILE* fin, double** pMatrix, double* b, int nDim );
void PrintTab( double* pTab, int nSize );

int main( int argc, char* argv[] )
{

	if( argc != 2 )
	{
		printf( "Usage: %s <input_file>", argv[0] ); //mesg  "Usage: ukl <input_file>"  
		return 1;
	}

	FILE* fin = fopen( argv[1], "r" );// otworzyc plik do odczytu   -- argv[1] //fopen(nazwa pliku, co ma robic) tzn "r"-read//czyta plik
	if( fin == NULL ) //sprawdzam czy plik istnieje
	{
		printf( "ERROR: %s Plik nie istnieje\n", argv[1] );
		return 2;
	}

	int nDim = 0;
	fscanf( fin, "%d", &nDim ); // odczytuje rozmiar nDim

	double** pMatrix = NULL;

	
	if( !CreateMatrix(&pMatrix, nDim) ) // wykreowac macierz ukladu nDim x nDim
	{
		perror( "ERROR: pMatrix! \n" );
		return 3;
	}

	
	double* b = (double*)malloc( sizeof(double) * nDim );// kreuje wektor wyrazow wolnych


	if( !b )
	{
		perror( "ERROR: wektor wyrazow wolnych! \n" );
		return 4;
	}
	memset( b, 0, sizeof(double) * nDim ); // wypelniam 0 wolne bajty
	
	ReadData( fin, pMatrix, b, nDim ); // wczytuje dane
	fclose( fin ); // zamykam plik
	
#ifdef _DEBUG_

	printf( "Macierz:\n" );
	PrintMatrix( pMatrix, nDim );
	printf( "Wektor wyrazow wolnych :\n" );
	PrintTab( b, nDim );
	printf("\n\n\n");

#endif
	
	double det = Det( pMatrix, nDim );// obliczam wyznacznik
	
	
	if(fabs(det) < 1e-16)
	{
		perror( "ERROR: Macierz osobliwa!\n" );
		return 5;
	}
	
	double** pInverse = NULL;
	if( !CreateMatrix(&pInverse, nDim) )
	{
		perror( "ERROR!!! (CreateMatrix // pInverse )\n" );
		return 6;
	}
	
	InverseMatrix( pInverse, pMatrix, nDim, det ); // obracam macierz
	
#ifdef _DEBUG_

	printf( "Wyznacznik : %lf\n", det );
	printf( "Macierz odwrocona:\n" );
	PrintMatrix( pInverse, nDim );

#endif
	
	double* pRes = (double*)malloc( sizeof(double) * nDim );  //kreowanie wektora wynikowego
	if( !pRes )
	{
		perror( "ERROR!!! // ( WEKTOR WYNIKOWY)" );
		return 7;
	}
	memset( pRes, 0, sizeof(double) * nDim );
	
	
	LayoutEqu( pInverse, b, pRes, nDim );// rozwiaz uklad


	printf( "Wynik: \n" );
	PrintTab( pRes, nDim );

	
	DeleteMatrix( &pInverse, nDim ); // zwolnic pamiec  (usuwanie macierzy)!!!
	DeleteMatrix( &pMatrix, nDim );
	free(b);
	free(pRes);
	
	return 0;
}
void ReadData( FILE* fin, double** pMatrix, double* b, int nDim )
{

	for( int nRow = 0; nRow < nDim; nRow++ )
	{
		double* p = *pMatrix++;
		for( int nCol = 0; nCol < nDim; nCol++ )
		{
			fscanf( fin, "%lf", p++ );
		}

		fscanf( fin, "%lf", b++ ); // wyraz wolny jest pozniej 
	}
}
void PrintTab( double* pTab, int nSize ) // potrzebne do drukowania wektrow
{
	for( int i = 0; i < nSize; i++ )
	{ 
		//printf(" ");
		printf( " %lf", *pTab++ );
		//printf("\n");
	}
}