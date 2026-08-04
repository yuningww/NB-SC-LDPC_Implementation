#include "NBLdpcBuilder.h"
#include "GF.h"
#include <random>
#include <fstream>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <vector>
#include <numeric>

using namespace std;

// Core building block, reused from BuildRegular: connect `NumVNs` VN-sockets
// to `NumCNs` check-node-sockets such that each check node gets exactly
// `SubDegree` edges and each VN-socket gets exactly 1 edge, via random
// column-partitioned permutation. Writes (col, value) pairs into rows
// [rowOffset, rowOffset + NumCNs), columns [colOffset, colOffset + NumVNs).
static void ConnectLayer(
    unsigned NumVNs, unsigned NumCNs, unsigned SubDegree,
    unsigned rowOffset, unsigned colOffset, unsigned Extension,
    mt19937 &gen, uniform_int_distribution<int> &distr,
    vector<vector<pair<unsigned, GFSymbol>>> &Rows)
{
    if (NumVNs != NumCNs * SubDegree)
    {
        cerr << "Inconsistent VNs/CNs for given sub-degree in coupled layer\n";
        return;
    }
    vector<unsigned> perm(NumCNs);
    for (unsigned g = 0; g < SubDegree; ++g)
    {
        unsigned base = g * NumCNs;
        iota(perm.begin(), perm.end(), base);
        shuffle(perm.begin(), perm.end(), gen);
        for (unsigned n = 0; n < NumCNs; ++n)
        {
            unsigned col = colOffset + perm[n];
            unsigned row = rowOffset + n;
            Rows[row].push_back({ col, (GFSymbol)distr(gen) });
        }
    }
}

// VNsPerPos, CNsPerPos : number of variable/check nodes at each position
// CouplingWidth (w)     : each VN sends exactly one edge to each of w
//                         consecutive positions -> per-VN degree dv = w
// CheckDegreePerOffset  : edges each check node receives FROM EACH of the w
//                         contributing positions -> total check degree
//                         dc = CheckDegreePerOffset * CouplingWidth
// ChainLength (L)       : number of variable-node positions; check-node
//                         positions run 0..L+w-2 (terminated, tapering
//                         check degree at the two ends, full VN degree
//                         everywhere)
void NBLdpcBuilder::BuildSpatiallyCoupled(
    unsigned VNsPerPos, unsigned CNsPerPos, unsigned CouplingWidth,
    unsigned CheckDegreePerOffset, unsigned ChainLength, unsigned Extension,
    std::string specFileName)
{
    if (VNsPerPos != CNsPerPos * CheckDegreePerOffset)
    {
        cerr << "VNsPerPos must equal CNsPerPos * CheckDegreePerOffset\n";
        return;
    }
    unsigned NumCheckPositions = ChainLength + CouplingWidth - 1;
    unsigned Length = VNsPerPos * ChainLength;
    unsigned Redundancy = CNsPerPos * NumCheckPositions;

    vector<vector<pair<unsigned, GFSymbol>>> Rows(Redundancy);
    mt19937 gen(time(nullptr));
    uniform_int_distribution<int> distr(1, (1 << Extension) - 1);

    for (unsigned z = 0; z < ChainLength; ++z)
    {
        unsigned colOffset = z * VNsPerPos;
        for (unsigned o = 0; o < CouplingWidth; ++o)
        {
            unsigned p = z + o;                 // target check position
            unsigned rowOffset = p * CNsPerPos;
            ConnectLayer(VNsPerPos, CNsPerPos, CheckDegreePerOffset,
                         rowOffset, colOffset, Extension, gen, distr, Rows);
        }
    }

    ofstream ofs(specFileName);
    ofs << Length << ' ' << Redundancy << ' ' << Extension << '\n';
    ofs << "SC" << ' ' << VNsPerPos << ' ' << CNsPerPos << ' '
        << CouplingWidth << ' ' << CheckDegreePerOffset << ' '
        << ChainLength << '\n';
    for (auto &row : Rows)
    {
        sort(row.begin(), row.end());
        for (auto &entry : row)
            ofs << entry.first << ' ' << (int)entry.second << ' ';
        ofs << '\n';
    }
}