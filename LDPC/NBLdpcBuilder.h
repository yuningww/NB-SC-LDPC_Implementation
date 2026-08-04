#include <string>

class NBLdpcBuilder
{
public:

	// Builds a terminated, spatially-coupled regular GF(2^Extension) LDPC code
	// and writes it to specFileName in the standard spec format.
	//   VNsPerPos, CNsPerPos   : number of variable/check nodes at each chain position
	//   CouplingWidth (w)      : each VN connects to w consecutive check-node positions,
	//                            so per-VN degree dv = CouplingWidth
	//   CheckDegreePerOffset   : edges each check node gets from each contributing
	//                            position, so total check degree dc = CheckDegreePerOffset * CouplingWidth
	//                            (checks near the two ends of the chain taper below dc)
	//   ChainLength (L)        : number of VN positions; check-node positions run 0..L+w-2
	static void BuildSpatiallyCoupled(unsigned VNsPerPos, unsigned CNsPerPos, unsigned CouplingWidth,
		unsigned CheckDegreePerOffset, unsigned ChainLength, unsigned Extension, std::string specFileName);
};