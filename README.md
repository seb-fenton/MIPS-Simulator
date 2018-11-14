Lau Jun Kit Darrick
Sebastian Alexander William Fenton
EIE2 Computer Architecture MIPS Simulator

This readme serves to illustrate the design perspectives adopted with respect to ambiguous parts of the program specification. The various assumptions made in implementing them will be explained here.


I/O Specification:
Given the largely undefined nature of many aspects of the I/O behaviours of this project, a few points to note are as follows:
The simulated I/O devices, GetC and PutC, are accessed with valid memory calls to the 4-byte areas at 0x30000000 and 0x30000004 respectively.

Instruction         Granularity(bits)       Allowed Accesses(w.r.t. I/O)        Notes
LW                  32                      0x30000000                          Must be word aligned. Only one possible address.
LWL                 32                      ~                                   Must be word aligned. Only one possible address.
LWR                 32                      ~                                   Must be word aligned. Only one possible address.

LH(U)               16                      0x30000000 or 0x30000002            Odd addresses within I/O range should be unaligned reads.

LB(U)               8                       nil.                                Should not be allowed as there is no way to differentiate
                                                                                0xFF from -1(EOF)


SW                  32                      0x30000004                          Must be word aligned. Only one possible address.

SH                  16                      0x30000004 or 0x30000006            Odd addresses within I/O range should be unaligned reads.

SB                  8                       0x30000004 - 0x30000007             All writes trigger I/O, but the char value is only written for 
                                                                                the write to 0x30000007 (LSB)