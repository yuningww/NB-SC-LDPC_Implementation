#pragma once

#include "GF.h"
#include <random>
#include <string>
#include <vector>

// Discrete memoryless substitution channel over GF(q), q = 2^FieldExtension.
// Symbol values 0..q-1 map 1:1 onto the nucleotide alphabet; the caller
// decides that mapping when writing the confusion matrix file.
class DnaChannel
{
	unsigned m_FieldSize;
	std::vector<std::vector<double>> m_LogMatrix; // m_LogMatrix[sent][received]
	mutable std::vector<std::discrete_distribution<int>> m_RowDistributions; // indexed by sent symbol; operator() is non-const but stateless for this distribution

public:
	// matrixFile format: first line q, then q rows of q space-separated
	// probabilities (row = transmitted symbol, column = received symbol).
	DnaChannel(const std::string& matrixFile, unsigned FieldSize);

	GFSymbol Simulate(GFSymbol trueSymbol, std::mt19937& rng) const
	{
		return static_cast<GFSymbol>(m_RowDistributions[trueSymbol](rng));
	}

	// Fills pLLRs[0..FieldSize_1] with log P(observed | j) - log P(observed | 0).
	void ComputeLLRs(GFSymbol observed, double* pLLRs, unsigned FieldSize_1) const
	{
		double ref = m_LogMatrix[0][observed];
		for (unsigned j = 0; j <= FieldSize_1; ++j)
			pLLRs[j] = m_LogMatrix[j][observed] - ref;
	}
};
