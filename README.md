### Lau Jun Kit Darrick

### Sebastian Alexander William Fenton

##### EIE2 Computer Architecture MIPS Simulator



#### Using the simulator
* make simulator
* make testbench
* (runs the simulator against every test binary in the catalogue) bin/mips_testbench bin/mips_simulator
* (for individual tests): bin/mips_simulator test/..../testbinary.bin
Directory names are self-explanatory. However, the output CSV file for running a simulator can be found in test/output.


#### The following write-ups illustrate the design perspectives adopted with respect to ambiguous parts of the program specification.

##### Input/Output
This was the group that tightened the specification for I/O (shown [here](https://github.com/m8pple/arch2-2018-cw/issues/42#issuecomment-437190800)), and will thus adopt that interpretation in our simulator. In short, any properly aligned load/store in the assigned memory space will trigger I/O, but only reads/writes that access the least significant byte (0x30000003 and 0x30000007) will properly process a given character value. EOFs, however, should be readable throughout the entire 32-bit memory space as 0xFF per byte.

##### Branch/Jumps
JALR: If unspecified, the link return address is stored by default in register $31, as per specification. However, specification sheets online state that an assembler might return this "unspecified" register rd as 0 in the instruction binary. We would thus consider a user calling JALR and specifying the link register to $0 to be undefined behaviour. As such, our implementation is such that if one tries to compile a binary involving "JALR $0, $5", for example, the link register would still be defaulted to register $31.
