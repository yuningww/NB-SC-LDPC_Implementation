#pragma once
#include "NBLdpcCodec.h"
#include "DnaChannel.h"

typedef /*std::pair<float, GFSymbol>*/double tLLRPair;
typedef std::pair<unsigned, unsigned> tIndPair;

class NBLdpcDecoder : public NBLdpcCodec
{
	unsigned m_NumOfComponents;

	tIndPair **m_ppVar2Checks;
	tIndPair **m_ppChecks2Var;
	unsigned *m_pCheckDegrees;
	unsigned *m_pVarDegrees;

	tLLRPair ***m_ppCheckInputs;
	tLLRPair ***m_ppCheckOutputs;
	tLLRPair ***m_ppVarInputs;
	tLLRPair ***m_ppVarOutputs;

	tLLRPair **m_ppInputLLRs;
public:
	NBLdpcDecoder(std::string specFile, unsigned NumOfRemainingLLRs);
	bool Decode(GFSymbol* pReceivedSymbols, const DnaChannel& channel, GFSymbol* pCodeword, unsigned NumOfIterations);

	virtual ~NBLdpcDecoder();
};


