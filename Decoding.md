The decoder logic behind this repo is built based on H. Wymeersch, H. Steendam, and M. Moeneclaey "Log-domain decoding of LDPC codes over GF(q)"

## Message been passed

Decoder in `NBLdpcDecoder::Decode` 

Every message passed on the Tanner graph is a length-q vector of LLRs, relative to symbol value 0:

```
L(a) = ln P(a) - ln P(0),   a = 0, 1, 2, 3
```

`L(0) = 0` if fixed. A positive `L(a)` means `a` is more likely than `0`, negative `L(a)` means `a` is less likely than `0`, and magnitude of `L(a)` is the confidence of this estimation w.r.t. `0`.


## Channel model

Substitution channel model. The channel is defined by a substitution probability matrix

```
Q[i][j] = P(received = j | sent = i) where i, j = 0, 1, 2, 3
```

read from file and stored as log-probabilities:

```
M[i][j] = ln(Q[i][j])
```

Channel input LLR (`m_ppInputLLRs[i]`, filled by `DnaChannel::ComputeLLRs`): given observed symbol y_i and substitution probability matrix,

```
L_i(a) = ln P(y_i | c_i=a) - ln P(y_i | c_i=0) = ln Q[a][y_i] - ln Q[0][y_i]
```
For a given variable node i, it receives an observed symbol y_i from the channel. Then for every candidate symbol a in GF(4), apply this formula to build the LLR vector message initialized at that VN.