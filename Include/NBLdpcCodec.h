#pragma once

#include "GFLinAlg.h"
#include <algorithm>
#include <fstream>
#include <ostream>
#include <vector>

struct regular_params
{
	unsigned m_NumOfRowElements;
	unsigned m_NumOfColumnElements;
};

struct sc_params
{
	unsigned m_VNsPerPos;
	unsigned m_CNsPerPos;
	unsigned m_CouplingWidth;
	unsigned m_CheckDegreePerOffset;
	unsigned m_ChainLength;
};

// Degree of check row `rowIndex` in a terminated SC chain: all CNsPerPos
// rows at a given chain position share the same degree, which tapers near
// the two ends where fewer of the CouplingWidth offsets have a contributing
// VN position (see NBLdpcBuilder::BuildSpatiallyCoupled/ConnectLayer).
inline unsigned ComputeSCCheckDegree(const sc_params& params, unsigned rowIndex)
{
	unsigned pos = rowIndex / params.m_CNsPerPos;
	int oLo = std::max(0, (int)pos - (int)params.m_ChainLength + 1);
	int oHi = std::min((int)params.m_CouplingWidth - 1, (int)pos);
	unsigned count = (oHi >= oLo) ? (unsigned)(oHi - oLo + 1) : 0;
	return params.m_CheckDegreePerOffset * count;
}

class NBLdpcCodec
{
protected:
	enum ldpc_type_t {L_REGULAR, L_SC, L_OTHER};
	
	GaloisField m_GF;
	//Code params
	ldpc_type_t m_Type;
	unsigned m_Length;
	unsigned m_Dimension;
	unsigned m_NumOfChecks;
	GFSymbol *m_pGenMatrix;
	GFPair **m_ppCheckConstraints;
	unsigned *m_pMessagePositions; // codeword index holding message symbol i, for i in [0, m_Dimension)

	void *m_pAdditionalParams;

public:
	NBLdpcCodec(unsigned Length, unsigned NumOfChecks, unsigned Extension, GFSymbol* pCheckMatrix);

	explicit NBLdpcCodec(std::string specFile);

	void Encode(GFSymbol *pInfSymbols, GFSymbol *pCodeword)
	{
		GFSymbol *pRow = m_pGenMatrix;
		memset(pCodeword, 0, sizeof(GFSymbol) * m_Length);
		for (unsigned j = 0; j < m_Dimension; ++j)
		{	
			for (unsigned i = 0; i < m_Length; ++i)
				pCodeword[i] ^= m_GF.multiply(pInfSymbols[j], pRow[i]);
			pRow += m_Length;
		}
	}

	bool VerifyCodeword(GFSymbol* pCodeword);

	void ExtractMessage(const GFSymbol* pCodeword, GFSymbol* pMessage) const
	{
		for (unsigned i = 0; i < m_Dimension; ++i)
			pMessage[i] = pCodeword[m_pMessagePositions[i]];
	}

	void PrintCheckMatrix(std::ostream& os) const
	{
		std::vector<GFSymbol> row(m_Length);
		for (unsigned i = 0; i < m_NumOfChecks; ++i)
		{
			std::fill(row.begin(), row.end(), 0);
			for (GFPair* p = m_ppCheckConstraints[i]; p->second; ++p)
				row[p->first] = p->second;
			for (unsigned j = 0; j < m_Length; ++j)
				os << (unsigned)row[j] << (j + 1 < m_Length ? ' ' : '\n');
		}
	}

	virtual ~NBLdpcCodec()
	{
		delete[] m_pGenMatrix;
		for (unsigned i = 0; i < m_NumOfChecks; ++i)
			delete[] m_ppCheckConstraints[i];
		delete[] m_ppCheckConstraints;
		delete[] m_pMessagePositions;
		switch (m_Type)
		{
		case L_REGULAR:
			delete static_cast<regular_params*>(m_pAdditionalParams);
			break;
		case L_SC:
			delete static_cast<sc_params*>(m_pAdditionalParams);
			break;
		default:
			break;
		}
	}

	unsigned GetDimension() const
	{
		return m_Dimension;
	}
	unsigned GetFieldOrder() const
	{
		return m_GF.Extension;
	}
	unsigned GetLength() const
	{
		return m_Length;
	}
	unsigned GetMaxFieldElement() const
	{
		return m_GF.FieldSize_1;
	}
};
