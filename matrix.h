#ifndef MATRIX_1
#define MATRIX_1
#include <stdlib.h>
#include <stdio.h>



int    CreateMatrix( double*** pTab, int nSize );
void   DeleteMatrix( double*** pTab, int nSize );
void   TransMatrix( double** pTab, int nDim );
void   InverseMatrix( double** pInv, double** pTab, int nSize, double det );
double Det( double** pTab, int nSize );
void   LayoutEqu( double** pInv, double* pB, double* pRes, int nSize );
void   PrintMatrix( double** pTab, int nSize );

#endif