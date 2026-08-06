#include "GFLinAlg.h"
#include <cstring>

//construct a nullspace basis for a given matrix. 
GFSymbol* Nullspace(GFSymbol*pMatrix  ///the matrix to be considered
	, unsigned Length ///number of columns in the matrix
	, unsigned& Dimension ///number of rows in the input matrix. On return, the dimension of the nullspace
	, unsigned* pPermutation ///preferred column permutation
	, GaloisField &gf
	)
{
	unsigned* pPerm;
	if (pPermutation)
		pPerm = pPermutation;
	else
	{
		pPerm = new unsigned[Length];
		for (unsigned i = 0; i<Length; i++)
			pPerm[i] = i;
	}
	Gauss(pMatrix, Dimension, Length, true, pPerm, gf);
	unsigned NumOfChecks = Length - Dimension;
	// Length/Dimension/NumOfChecks are 32-bit; their products routinely
	// exceed 2^32 for long codewords (e.g. 300000 * 269550 = 80.9e9), so
	// every flat-array size/index below must multiply in size_t, not
	// unsigned, or it silently wraps and corrupts memory.
	GFSymbol* pNullspace = new GFSymbol[(size_t)NumOfChecks * Length];
	memset(pNullspace, 0, sizeof(GFSymbol) * (size_t)Length * NumOfChecks);
	for (unsigned i = 0; i < NumOfChecks; i++)
	{
		pNullspace[(size_t)i * Length + pPerm[Dimension + i]] = 1;
		for (unsigned j = 0; j < Dimension; j++)
			pNullspace[(size_t)i * Length + pPerm[j]] = pMatrix[(size_t)j * Length + pPerm[Dimension + i]];
	}
	Dimension = Length - Dimension;
	if (!pPermutation)
		delete[]pPerm;
	return pNullspace;
}

void Gauss(GFSymbol* pMatrix
	, unsigned& NumOfRows
	, unsigned NumOfColumns
	, bool ReversePass
	, unsigned *pPermutation
	, GaloisField& gf)
{
	int gfDeg;
	for (unsigned i = 0; i < NumOfRows; i++)
	{
		//identify the leading column
		unsigned c = i;
		bool Success = false;
		for (; c < NumOfColumns; c++)
		{
			unsigned C = pPermutation[c];
			for (unsigned j = i; j < NumOfRows; j++)
			{
				if (pMatrix[(size_t)j * NumOfColumns + C] != 0)
				{
					Success = true;
					gfDeg = gf.pLogTable[pMatrix[(size_t)j * NumOfColumns + C]];
					for (unsigned k = 0; k < NumOfColumns; ++k)
						pMatrix[(size_t)j * NumOfColumns + k] = gf.divideConst(pMatrix[(size_t)j * NumOfColumns + k], gfDeg);
					if (j > i)
					{
						for (unsigned k = 0; k < NumOfColumns; ++k)
							pMatrix[(size_t)i * NumOfColumns + k] ^= pMatrix[(size_t)j * NumOfColumns + k];
					}
					break;
				}
			}
			if (Success)
			{
				if (c != i)
					std::swap(pPermutation[c], pPermutation[i]);
				break;
			}
		}
		if (!Success)
		{
			NumOfRows = i;
			break;
		}
		unsigned LoopStart = (ReversePass) ? 0 : (i + 1);
		unsigned C = pPermutation[i];
		for (unsigned j = LoopStart; j < NumOfRows; ++j)
		{
			if (j == i)
				continue;
			if (pMatrix[(size_t)j * NumOfColumns + C] != 0)
			{
				gfDeg = gf.pLogTable[pMatrix[(size_t)j * NumOfColumns + C]];
				for (unsigned k = 0; k < NumOfColumns; ++k)
					pMatrix[(size_t)j * NumOfColumns + k] ^= gf.multiplyConst(pMatrix[(size_t)i * NumOfColumns + k], gfDeg);
			}
		}
	}
}
