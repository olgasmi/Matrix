#include "matrix.h"
#include <memory.h> // memset
//#include <math.h>//pow
//#define _DEBUG_

void Complement( double** pTabO, double** pTabI, int nRow, int nCol, int nDim )
{

	for( int i = 0; i < nDim; i++, pTabI++)
	{
		

		if( i == nRow )
			continue;
		double* rowTabI = *pTabI; // wskaznik pomocniczny do wierszy
		double* rowTabO = *pTabO++;

		for( int j = 0; j < nDim; j++, rowTabI++)
		{
			if( j == nCol )
				continue;

			*rowTabO++ = *rowTabI;
		}
		//pTabO++;
	}
}
void ComplMatrix( double** pTabD, double** pTab, int nDim ) //macierz dopelnien
{
	double** Matrix = NULL;
	//CreateMatrix( &Matrix, nDim - 1 ); //tablica pomocnicza

	if( !CreateMatrix(&Matrix, nDim - 1) )
	{
		perror( "ERROR: CreateMatrix(ComplMatrix)!!" );
		return;
	}



	for( int nRow = 0; nRow < nDim; nRow++, pTabD++ )
	{
		
		int sign = ( nRow % 2 ) ? -1 : 1; // wyrazenie pytajnikowe, w zaleznosci do parzystosci -1 albo 1
		double* v = *pTabD; // pomocniczny wskaznik

		for( int nCol = 0; nCol < nDim; nCol++, v++ )
		{
			Complement( Matrix, pTab, nRow, nCol, nDim );
			*v = sign * Det( Matrix, nDim - 1 );
			sign = -sign;
		}
	}
	DeleteMatrix( &Matrix, nDim - 1 );
}
int CreateMatrix( double*** pTab, int nSize )
{
	*pTab = (double**)malloc( sizeof(double*) * nSize );// malloc - przydziela pamiec o wielkosci sizeof() elementow
	if( !pTab )
		return 0; // ERROR!!

	memset( *pTab, 0, sizeof(double*) * nSize ); // memset- wypelnia kolejne bajty w pamieci ustalona wartoscia
	// memset( adres poczatkowy, wypisywana wartosc, ile bajtow zapisac);

	double** p = *pTab;  // pomocniczy wskaznik do alokacji
	for( int i = 0; i < nSize; i++ )
	{
		*p = (double*)malloc(sizeof( double) * nSize );
		if( !*pTab ) return 0;

		memset( *p++, 0, sizeof(double) * nSize );
	}

	return 1; //OK
}
void DeleteMatrix( double*** pTab, int nSize )
{
	double** v = *pTab; // pomocniczy wskaznik
	for( int i = 0; i < nSize; i++ )
		free(*v++);
	free(*pTab);
	*pTab = NULL;
}

void TransMatrix( double** pTab, int nDim ) // bez pomocniczej, raz wolno indeksowac 
{
	//zamian wierszy na kolumny i kolumn na wiersze
	double** pRow = pTab;

	for( int i = 0; i < nDim - 1; i++)//, pTab++ ) 
	{
		double* pCol = *pRow++ + i + 1;

		for( int j = i + 1; j < nDim; j++, pCol++ ) // j=i+1 bo glowna przekatna bez zmian
		{
			//if ( i = j ) continue;// glowna przekatna bez zmian

			double tmp = *pCol;
			*pCol = pTab[j][i];
			pTab[j][i] = tmp;
		}
	}

}
void InverseMatrix( double** pInv, double** pTab, int nSize, double det )
{

	ComplMatrix( pInv, pTab, nSize ); // tworze macierz dopelnien
	
#ifdef _DEBUG_
	printf( "COMPLMATRIX! \n" );
	PrintMatrix( pInv, nSize );
#endif
	
	TransMatrix( pInv, nSize ); // transponuje macierz dopelnien
#ifdef _DEBUG_
	printf( "TRANSMATRIX!\n" );
	PrintMatrix( pInv, nSize );
#endif
	
	for( int nRow = 0; nRow < nSize; nRow++ )
	{
		double* v = *pInv++;
		for( int nCol = 0; nCol < nSize; nCol++, v++ )
		{
			*v /= det; // kazdy wyraz macierzy dziele przez wyznacznik 
		}
	}

/*#ifdef _DEBUG_
	printf( "INVERSE!\n" );
	PrintMatrix( pInv, nSize );
#endif    
*/
}
double Det( double** pTab, int nSize )// metoda LaPlace'a- rozwieniecie wzgledem i-tego wiersza
{


	if(nSize == 1)
		return **pTab;

	if(nSize == 2) // wyznacznik 2x2 obliczony z definicji
	{
		return **pTab * pTab[1][1] - pTab[1][0] * pTab[0][1];
	}

	// rozwniecie wzgledem 0-wego wiersza//rekurencja


	double** pTabD = NULL;

	if( !CreateMatrix(&pTabD, nSize - 1) )
	{
		perror( "ERROR: CreateMatrix!!" );
		return 1;
	}

	double* v = *pTab;
	int sign = 1;
	double DET = 0;

	for( int nCol = 0; nCol < nSize; nCol++, v++ )
	{
		Complement( pTabD, pTab, 0, nCol, nSize ); // wycinamy 0-wy wiersz i po kolei kolejne kolumny 

		DET += sign * (*v) * Det( pTabD, nSize - 1 );
		sign = -sign;

	}
	DeleteMatrix( &pTabD, nSize - 1 );
	return DET;

}
void LayoutEqu( double** pInv, double* pB, double* pRes, int nSize )
{
	// Ax = B->A ^ (-1) * B = x
	//A- pInv, x-pRes, B-pB
	//CreateMatrix((double***)pInv, nSize);//wykreowac macierz ukladu nDim x nDim //A

	for( int i = 0; i < nSize; i++, pRes++ )
	{
		double* v = *pInv++;
		for( int j = 0; j < nSize; j++ )
		{
			*pRes += *v++ * *pB++;//iloczyn skalarny
		}
	}

}
void PrintMatrix( double** pTab, int nSize )
{

	for( int i = 0; i < nSize; i++ )
	{
		double* v = *pTab++; // aby w nowym wierszu zawsze zaczynac od pierwszej kolumny

		for( int j = 0; j < nSize; j++ )
		{
			printf( "%lf", *v++ );
		}
		printf("\n");
	}
	printf("\n");
}
