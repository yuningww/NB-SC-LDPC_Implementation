#include <iostream>
#include "NBLdpcDecoder.h"
#include "NBLdpcBuilder.h"
#include "DnaChannel.h"
#include <algorithm>
#include <cctype>
#include <random>
#include <ctime>
#include <stdexcept>
#include <vector>

using namespace std;

int g_argc;
char **g_argv;

char *NextArgument()
{
	static int curArg = 1;
	return g_argv[curArg++];
}

void Usage()
{
	cout << "Usage:\n"
		<< "B VNsPerPos CNsPerPos CouplingWidth CheckDegreePerOffset ChainLength Extension specFileName\n"
		<< "T specFileName NumOfIterations\n"
		<< "D specFileName NumOfDecoderIterations NumOfRemainingVals ConfusionMatrixFile NumOfErrors NumOfIterations\n"
		<< "H specFileName\n"
		<< "E specFileName Message\n"
		<< "C specFileName ConfusionMatrixFile Message NumOfDecoderIterations\n";
}

// Nucleotide I/O (GF(4) only): symbol values 0,1,2,3 <-> A,C,G,T, matching
// the row/column order expected of confusion-matrix files (see DnaChannel.h).
const char kNucleotides[4] = { 'A', 'C', 'G', 'T' };

GFSymbol NucleotideToSymbol(char c)
{
	switch (toupper(static_cast<unsigned char>(c)))
	{
		case 'A': return 0;
		case 'C': return 1;
		case 'G': return 2;
		case 'T': return 3;
	}
	throw runtime_error(string("Invalid nucleotide character '") + c + "' (expected A/C/G/T)");
}

vector<GFSymbol> StringToSymbols(const string& s)
{
	vector<GFSymbol> out(s.size());
	for (size_t i = 0; i < s.size(); ++i)
		out[i] = NucleotideToSymbol(s[i]);
	return out;
}

string SymbolsToString(const GFSymbol* pSymbols, unsigned n)
{
	string s(n, 'A');
	for (unsigned i = 0; i < n; ++i)
		s[i] = kNucleotides[pSymbols[i]];
	return s;
}

void RequireGF4(unsigned maxFieldElement)
{
	if (maxFieldElement + 1 != 4)
		throw runtime_error("Nucleotide string I/O requires a GF(4) code; this spec is GF(" +
			to_string(maxFieldElement + 1) + ")");
}

int main(int argc, char **argv)
{
	g_argc = argc;
	g_argv = argv;
	static const int validArgcs[] = { 3, 4, 6, 8, 9 };
	if (find(begin(validArgcs), end(validArgcs), g_argc) == end(validArgcs))
	{
		Usage();
		return 0;
	}
	try
	{
	char mode = NextArgument()[0];
	unsigned Length, Extension;
	string specFileName;
	if(mode == 'B')
	{
		unsigned VNsPerPos = stoi(NextArgument());
		unsigned CNsPerPos = stoi(NextArgument());
		unsigned CouplingWidth = stoi(NextArgument());
		unsigned CheckDegreePerOffset = stoi(NextArgument());
		unsigned ChainLength = stoi(NextArgument());
		Extension = stoi(NextArgument());
		specFileName = NextArgument();
		NBLdpcBuilder::BuildSpatiallyCoupled(VNsPerPos, CNsPerPos, CouplingWidth,
			CheckDegreePerOffset, ChainLength, Extension, specFileName);
	}
	else if(mode == 'T')
	{
		specFileName = NextArgument();
		unsigned NumOfIterations = stoi(NextArgument());
		NBLdpcCodec codec(specFileName);

		GFSymbol *pData = new GFSymbol[codec.GetDimension()];
		GFSymbol *pEncoded = new GFSymbol[codec.GetLength()];
		mt19937 gen(time(nullptr));
		uniform_int_distribution<int> distr(1, codec.GetMaxFieldElement());
		int errNum = 0;
		for (unsigned i = 0; i < NumOfIterations; ++i)
		{
			transform(pData, pData + codec.GetDimension(), pData, [&](GFSymbol a) { return (GFSymbol)distr(gen); });
			codec.Encode(pData, pEncoded);
			bool result = codec.VerifyCodeword(pEncoded);
			if (!result)
			{
				cout << "Verification fail at iteration #" << i << endl;
				errNum++;
			}			
		}
		if (!errNum)
			cerr << "Verification successful" << endl;
		else
			cerr << "Verification failed" << endl;

		delete[] pData;
		delete[] pEncoded;
	}
	else if (mode == 'D')
	{
		specFileName = NextArgument();
		unsigned NumOfDecoderIterations = stoi(NextArgument());
		unsigned NumOfRemainingVals = stoi(NextArgument());
		string confMatrixFile = NextArgument();
		unsigned NumOfErrors = stoi(NextArgument());
		unsigned NumOfIterations = stoi(NextArgument());

		NBLdpcDecoder codec(specFileName, NumOfRemainingVals);
		Length = codec.GetLength();
		unsigned Dimension = codec.GetDimension();
		DnaChannel channel(confMatrixFile, codec.GetMaxFieldElement() + 1);
		mt19937 engine(0/*time(nullptr)*/);
		uniform_int_distribution<int> dataGen(0, codec.GetMaxFieldElement());
		GFSymbol *pData = new GFSymbol[Dimension];
		GFSymbol *pEncoded = new GFSymbol[Length];
		GFSymbol *pDecoded = new GFSymbol[Length];
		GFSymbol *pReceived = new GFSymbol[Length];
		unsigned currentErrors = 0;
		unsigned it = 0;
		unsigned symbolErrors = 0;
		unsigned curSymbolErrors;
		unsigned failures = 0;
		for (; it < NumOfIterations && currentErrors < NumOfErrors; ++it)
		{
			for (unsigned i = 0; i < Dimension; i++)
				pData[i] = dataGen(engine);
			codec.Encode(pData, pEncoded);
			for (unsigned i = 0; i < Length; i++)
				pReceived[i] = channel.Simulate(pEncoded[i], engine);
			bool flag = codec.Decode(pReceived, channel, pDecoded, NumOfDecoderIterations);
			if (!flag)
			{
				failures++;
				curSymbolErrors = Length / 2;
			}
			else
			{
				curSymbolErrors = 0;
				for (unsigned i = 0; i < Length; i++)
					if (pDecoded[i] != pEncoded[i])
						curSymbolErrors++;
			}
			if (curSymbolErrors)
				currentErrors++;

			symbolErrors += curSymbolErrors;
			if (it > 0 && it % 1000 == 0)
				cout << it << ' ' << (static_cast<float>(currentErrors) / it) <<  endl;
		}
		cerr << it << ' ' << (static_cast<float>(currentErrors) / it)
			<< ' ' << (static_cast<float>(symbolErrors) / (Length * it))
			<< ' ' << (static_cast<float>(failures) / it) <<  endl;
		delete[] pData;
		delete[] pEncoded;
		delete[] pDecoded;
		delete[] pReceived;
	}
	else if (mode == 'H')
	{
		specFileName = NextArgument();
		NBLdpcCodec codec(specFileName);
		codec.PrintCheckMatrix(cout);
	}
	else if (mode == 'E')
	{
		specFileName = NextArgument();
		string message = NextArgument();
		NBLdpcCodec codec(specFileName);
		RequireGF4(codec.GetMaxFieldElement());
		if (message.size() != codec.GetDimension())
			throw runtime_error("message length " + to_string(message.size()) +
				" does not match code dimension " + to_string(codec.GetDimension()));

		vector<GFSymbol> msgSyms = StringToSymbols(message);
		vector<GFSymbol> codeword(codec.GetLength());
		codec.Encode(msgSyms.data(), codeword.data());
		cout << "Codeword: " << SymbolsToString(codeword.data(), codec.GetLength()) << endl;
	}
	else if (mode == 'C')
	{
		specFileName = NextArgument();
		string confMatrixFile = NextArgument();
		string message = NextArgument();
		unsigned NumOfDecoderIterations = stoi(NextArgument());

		NBLdpcDecoder codec(specFileName, 4);
		RequireGF4(codec.GetMaxFieldElement());
		if (message.size() != codec.GetDimension())
			throw runtime_error("message length " + to_string(message.size()) +
				" does not match code dimension " + to_string(codec.GetDimension()));

		vector<GFSymbol> msgSyms = StringToSymbols(message);
		vector<GFSymbol> cleanCodeword(codec.GetLength());
		codec.Encode(msgSyms.data(), cleanCodeword.data());
		cout << "Message:        " << message << endl;
		cout << "Clean codeword: " << SymbolsToString(cleanCodeword.data(), codec.GetLength()) << endl;

		DnaChannel channel(confMatrixFile, codec.GetMaxFieldElement() + 1);
		mt19937 engine(0);
		vector<GFSymbol> received(codec.GetLength());
		for (unsigned i = 0; i < codec.GetLength(); ++i)
			received[i] = channel.Simulate(cleanCodeword[i], engine);
		cout << "Received noisy: " << SymbolsToString(received.data(), codec.GetLength()) << endl;

		vector<GFSymbol> decoded(codec.GetLength());
		bool success = codec.Decode(received.data(), channel, decoded.data(), NumOfDecoderIterations);
		cout << "Decoded:        " << SymbolsToString(decoded.data(), codec.GetLength()) << endl;
		cout << "Decode " << (success ? "converged (all parity checks satisfied)" : "FAILED to converge") << endl;

		vector<GFSymbol> recoveredMsg(codec.GetDimension());
		codec.ExtractMessage(decoded.data(), recoveredMsg.data());
		string recovered = SymbolsToString(recoveredMsg.data(), codec.GetDimension());
		cout << "Recovered msg:  " << recovered
			<< (recovered == message ? "  (matches original)" : "  (MISMATCH)") << endl;
	}
	}
	catch (const exception& e)
	{
		cerr << "Error: " << e.what() << endl;
		return 1;
	}
	return 0;
}