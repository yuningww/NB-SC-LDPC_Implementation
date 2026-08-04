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

**3. 300-nt messages, 400-nt codewords**
`VNsPerPos`, `CNsPerPos` = `40`, `5`
`CouplingWidth` = `11`
`CheckDegreePerOffset` = `8`
`ChainLength` = `10`
Rate = (300/400) * 2 = 1.5 bits per channel use.
```
./build/ldpc B 40 5 11 8 10 2 build/gf4_300_1.ldspec
./build/ldpc D build/gf4_300_1.ldspec 20 4 LDPC/acc-true_passQ-true_forward.txt 50 2000

511 0.0978474 0.0489237 0.0978474
```

**3. 3000-nt messages, 4000-nt codewords**
`VNsPerPos`, `CNsPerPos` = `40`, `5`
`CouplingWidth` = `101`
`CheckDegreePerOffset` = `8`
`ChainLength` = `100`
Rate = (3000/4000) * 2 = 1.5 bits per channel use.
```
./build/ldpc B 40 5 101 8 100 2 build/gf4_3000.ldspec
./build/ldpc D build/gf4_3000.ldspec 20 4 LDPC/acc-true_passQ-true_forward.txt 50 2000

51 0.980392 0.490196 0.980392
```

**4. 3000-nt messages, 4000-nt codewords**
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

**5. 3000-nt messages, 3500-nt codewords**
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

**6. 3000-nt messages, 3512-nt codewords**
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

**7. 3000-nt messages, 3510-nt codewords**
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
(This takes long time to run.)

**Summary:**
1. As the code length grows, the code can approach capacity with smaller error rate (~ 0).
2. For a given rate and code length, lower coupling width and higher chain length can achieve lower error rate.



