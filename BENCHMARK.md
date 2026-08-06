**1. 150-nt messages, 240-nt codewords**
`VNsPerPos`, `CNsPerPos` = `40`, `10`
`CouplingWidth` = `4`
`CheckDegreePerOffset` = `4`
`ChainLength` = `6`
Rate = (150/240) * 2 = 1.25 bits per channel use.

```
./build/ldpc B 40 10 4 4 6 2 build/gf4_150.ldspec
./build/ldpc D build/gf4_150.ldspec 20 4 LDPC/acc-true_passQ-true_forward.txt 50 2000

1000 0.002
2000 0.002 0.001 0.002
```

**2. 300-nt messages, 480-nt codewords**
`VNsPerPos`, `CNsPerPos` = `80`, `20`
`CouplingWidth` = `4`
`CheckDegreePerOffset` = `4`
`ChainLength` = `6`
Rate = (300/480) * 2 = 1.25 bits per channel use.
```
./build/ldpc B 80 20 4 4 6 2 build/gf4_300.ldspec
./build/ldpc D build/gf4_300.ldspec 20 4 LDPC/acc-true_passQ-true_forward.txt 50 2000

1000 0
2000 0 0 0
```

**3. 3000-nt messages, 4000-nt codewords**
`VNsPerPos`, `CNsPerPos` = `200`, `40`
`CouplingWidth` = `6`
`CheckDegreePerOffset` = `5`
`ChainLength` = `20`
Rate = (3000/4000) * 2 = 1.5 bits per channel use.
```
./build/ldpc B 200 40 6 5 20 2 build/gf4_3000_1.ldspec
./build/ldpc D build/gf4_3000_1.ldspec 20 4 LDPC/acc-true_passQ-true_forward.txt 50 2000

1000 0
2000 0 0 0
```

**4. 3000-nt messages, 3500-nt codewords**
`VNsPerPos`, `CNsPerPos` = `500`, `50`
`CouplingWidth` = `4`
`CheckDegreePerOffset` = `10`
`ChainLength` = `7`
Rate = (3000/3500) * 2 = 1.71 bits per channel use.
```
./build/ldpc B 500 50 4 10 7 2 build/gf4_3000_2.ldspec
./build/ldpc D build/gf4_3000_2.ldspec 20 4 LDPC/acc-true_passQ-true_forward.txt 50 2000

1000 0.041
1195 0.041841 0.0209205 0.041841
```

**5. 3000-nt messages, 3512-nt codewords**
`VNsPerPos`, `CNsPerPos` = `28`, `4`
`CouplingWidth` = `4`
`CheckDegreePerOffset` = `7`
`ChainLength` = `125`
Rate = (3000/3512) * 2 = 1.71 bits per channel use.
```
./build/ldpc B 28 4 4 7 125 2 build/gf4_3000_512.ldspec
./build/ldpc D build/gf4_3000_512.ldspec 20 4 LDPC/acc-true_passQ-true_forward.txt 50 2000

52 0.961538 0.480769 0.961538
```

**6. 3000-nt messages, 3510-nt codewords**
`VNsPerPos`, `CNsPerPos` = `35`, `5`
`CouplingWidth` = `3`
`CheckDegreePerOffset` = `7`
`ChainLength` = `100`
Rate = (3000/3510) * 2 = 1.71 bits per channel use.
```
./build/ldpc B 35 5 3 7 100 2 build/gf4_3000_510.ldspec
./build/ldpc D build/gf4_3000_510.ldspec 20 4 LDPC/acc-true_passQ-true_forward.txt 50 2000

50 1 0.490011 0.98
```

**7. 15000-nt messages, 18450-nt codewords**
`VNsPerPos`, `CNsPerPos` = `450`, `75`
`CouplingWidth` = `6`
`CheckDegreePerOffset` = `6`
`ChainLength` = `41`
Rate = (15000/18450) * 2 = 1.63 bits per channel use.
```
./build/ldpc B 450 75 6 6 41 2 build/gf4_15000_3450.ldspec
./build/ldpc D build/gf4_15000_3450.ldspec 20 4 LDPC/acc-true_passQ-true_forward.txt 50 2000

1000 0
2000 0 0 0
```

Other results: (with rate = symbol/channel use = 2 bits/channel use)
``` bash
VNsPerPos  CNsPerPos  CouplingWidth  CheckDegreePerOffset  ChainLength  spec_file                    length  dimension  redundancy  rate    trials_run  codeword_error_rate  symbol_error_rate  decode_failure_rate
900        150        4              6                     17           build/gf4_12300_3000.ldspec  15300   12300      3000        0.8039  1961        0                    0                  0
900        150        4              6                     36           build/gf4_26550_5850.ldspec  32400   26550      5850        0.8194  926         0                    0                  0
900        150        4              6                     56           build/gf4_41550_8850.ldspec  50400   41550      8850        0.8244  595         0                    0                  0
900        150        6              6                     36           build/gf4_26250_6150.ldspec  32400   26250      6150        0.8102  926         0                    0                  0
900        150        6              6                     56           build/gf4_41250_9150.ldspec  50400   41250      9150        0.8185  595         0                    0                  0
900        150        8              6                     36           build/gf4_25950_6450.ldspec  32400   25950      6450        0.8009  926         0                    0                  0
900        150        8              6                     56           build/gf4_40950_9450.ldspec  50400   40950      9450        0.8125  595         0                    0                  0
900        150        10             6                     56           build/gf4_40650_9750.ldspec  50400   40650      9750        0.8065  595         0                    0                  0
1050       150        4              7                     60           build/gf4_53550_9450.ldspec  63000   53550      9450        0.85    500         0                    0                  0
```

Note that the final line shows a 1.7 bits per channel use SC-LDPC configuration, and the corresponding code structure is [gf4_53550_9450.ldspec](gf4_53550_9450.ldspec).

**Summary:**
1. As the code length grows, the code can approach capacity with smaller error rate (~ 0).
2. For a given rate and code length, lower coupling width and higher chain length can achieve lower error rate.
3. Larger VNs and CNs at each position along the chain can have better performance.