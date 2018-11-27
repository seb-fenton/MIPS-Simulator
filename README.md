### EIE2 Computer Architecture MIPS Simulator
##### Lau Jun Kit Darrick
##### Sebastian Alexander William Fenton


#### Using the simulator
* make simulator
* make testbench
* (runs the simulator against every test binary in the catalogue) bin/mips_testbench bin/mips_simulator
* (for individual tests): bin/mips_simulator test/..../testbinary.bin
* (to clean) make clean 
Directory names are self-explanatory. However, the output CSV file for running a simulator can be found in test/output.

#### Extending the testbench
To add tests to the testbench, simply place a valid .txt file in whichever file is semantically appropriate out of: test/(test_instruction_src, test_io_instruction_src_input, test_io_instruction_src_output, test_simulator_instruction_src) 

All the above files are compiled to binaries and run automatically by the testbench, given that they include the following metadata in the first 4 lines of the .txt: 
* #expectedOutcome
* #testId   (which should also be the name of the binary)
* #instruction
* #message

The message is recommended to follow the current syntax: #message about what it's doing || Dependencies: command1 command2 command3 

The author of the tests is found by the $user bash script command.

test/test_io_instruction_src_manual is not automatically looped through; this is the folder designated for tests that need custom bash script. In this case, there is also a designated region in the bash script itself where the user may choose to implement the bash code.

test/test_text_files is the folder for any text files needed for tests, i.e. an empty eof file.

#### The following write-ups illustrate the design perspectives adopted with respect to ambiguous parts of the program specification.

##### Input/Output
This was the group that tightened the specification for I/O (shown [here](https://github.com/m8pple/arch2-2018-cw/issues/42#issuecomment-437190800)), and will thus adopt that interpretation in our simulator. In short, any properly aligned load/store in the assigned memory space will trigger I/O, but only reads/writes that access the least significant byte (0x30000003 and 0x30000007) will properly process a given character value. EOFs, however, should be readable throughout the entire 32-bit memory space as 0xFF per byte.

##### Branch/Jumps
JALR: If unspecified, the link return address is stored by default in register $31, as per specification. However, specification sheets online state that an assembler might return this "unspecified" register rd as 0 in the instruction binary. We would thus consider a user calling JALR and specifying the link register to $0 to be undefined behaviour. As such, our implementation is such that if one tries to compile a binary involving "JALR $0, $5", for example, the link register would still be defaulted to register $31.

#### LWL/LWR
LWL and LWR, in this implementation, have the same access privileges as LB would. We made this choice so that the simulator would have as much functionality as possible, and to reflect what we thought would be a realistic representation of hardware. 
