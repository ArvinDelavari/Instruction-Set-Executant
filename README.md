![License](https://img.shields.io/github/license/ArvinDelavari/CPP-Instruction-Set-Executant?color=dark-green) 
![Language](https://img.shields.io/badge/Language-C++-dark_green)
![Version](https://img.shields.io/badge/version-0.1-dark_green)

## C++ Instruction Set Executant 

<div align="justify">
  
This code is a C++ class called `instruction_set` that defines a set of methods and member variables for executing instructions. The code is implemented as a part of an instruction execution (sample) system like a compiler where instructions are loaded, executed, and operands are manipulated based on the given type.

- The class `instruction_set` is defined with private member variables, including arrays `stack`, `main_memory`, `p`, `r`, and `code`, as well as scalar variables `ip`, `flags`, `sp`, and `codesize`.
- The public section of the class contains several method declarations, including `jump`, `compare`, `input`, `output`, `move`, `logic_and`, `logic_not`, `logic_xor`, `logic_or` and etc. These methods perform various operations on operands and modify the state of the instruction execution.

> [!Note]\
> Please be aware that the code provided in this repository is designed for educational purpose and should not be considered suitable for professional applications. It is only offering examples and demonstrations of fundamental concepts. It is crucial to understand that this code may not meet the particular standards expected in a professional setting. Therefore, it is strongly advised to modify and enhance the code appropriately before utilizing it in any production or professional environment.

## RV32IM Instruction Set Executant 

This code repository contains an implementation of a simple RISC-V processor (Instruction Set Executant). The processor executes instructions from the `RV32IM` instruction set architecture. The `src` directory of this repository includes functions for reading and writing control and status registers (CSRs), as well as executing various instructions such as arithmetic operations, memory operations, branches, jumps, and system instructions.

The main components of the code are:
- Declaration and initialization of registers: The code declares an array of 32 general-purpose registers and an unordered map for CSR registers.
- Instruction execution: The `execute_instruction` function takes an instruction as input and performs the necessary operations based on the opcode and other fields of the instruction. It supports instructions for arithmetic operations, load/store operations, branches, jumps, and system instructions.
- CSR operations: The code includes functions for reading and writing CSR registers. These functions handle different types of CSR instructions, such as `CSRRW` (read/write), `CSRRS` (read/set), `CSRRC` (read/clear), `CSRRWI` (read/write immediate), `CSRRSI` (read/set immediate), and `CSRRCI` (read/clear immediate).
- Immediate value generation: The code includes logic for generating immediate values for different instruction types (I, S, B, U, J). The immediate values are used for operand calculations in instructions.

Overall, this code provides a basic implementation of a RISC-V processor that can execute a subset of the `RV32IM` instruction set.


This repository contains an open-source code under the [MIT license](https://opensource.org/license/mit/) and is free to use.

- Created By : [Arvin Delavari](https://github.com/ArvinDelavari) - arvin7807@gmail.com
- Iran University of Science and Technology, Electronics Research Center - October 2023
- Digital Design Research Lab, Super-Computing and Networking (SCaN) Research Lab

</div>
