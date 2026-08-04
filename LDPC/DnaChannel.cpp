#include "DnaChannel.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace
{
	const double kMinProbability = 1e-9;
}

DnaChannel::DnaChannel(const std::string& matrixFile, unsigned FieldSize)
	: m_FieldSize(FieldSize)
{
	std::ifstream ifs(matrixFile);
	if (!ifs)
		throw std::runtime_error("Cannot open confusion matrix file: " + matrixFile);

	unsigned q;
	ifs >> q;
	if (q != m_FieldSize)
		throw std::runtime_error("Confusion matrix size (" + std::to_string(q) +
			") does not match code field size (" + std::to_string(m_FieldSize) + ")");

	m_LogMatrix.assign(q, std::vector<double>(q));
	m_RowDistributions.reserve(q);
	for (unsigned i = 0; i < q; ++i)
	{
		std::vector<double> row(q);
		double sum = 0.;
		for (unsigned j = 0; j < q; ++j)
		{
			if (!(ifs >> row[j]))
				throw std::runtime_error("Confusion matrix file is missing entries in row " + std::to_string(i));
			sum += row[j];
		}
		if (std::fabs(sum - 1.0) > 1e-3)
			std::cerr << "Warning: confusion matrix row " << i << " sums to " << sum << ", expected 1.0\n";

		m_RowDistributions.emplace_back(row.begin(), row.end());
		for (unsigned j = 0; j < q; ++j)
			m_LogMatrix[i][j] = std::log(std::max(row[j], kMinProbability));
	}
}
