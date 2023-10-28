#ifndef RV32I_INSTRUCTION_SET_H
#define RV32I_INSTRUCTION_SET_H

#include <iostream>
#include <cstdint>
#include <unordered_map>

#define ADDI   0x13
#define R_TYPE 0x33
#define LBU    0x3
#define SBU    0x23
#define BEQ    0x63
#define JAL    0x6F
#define JALR   0x67
#define LUI    0x37
#define AUIPC  0x17
#define SYSTEM 0x73

// CSR instructions
#define CSRRW  0x1
#define CSRRS  0x2
#define CSRRC  0x3
#define CSRRWI 0x5
#define CSRRSI 0x6
#define CSRRCI 0x7

// CSR registers
#define CSR_MSTATUS  0x300
#define CSR_MIE      0x304
#define CSR_MTVEC    0x305
#define CSR_MSCRATCH 0x340
#define CSR_MEPC     0x341
#define CSR_MCAUSE   0x342
#define CSR_MTVAL    0x343
#define CSR_MIP      0x344

class RV32I_INSTRUCTION_SET 
{
public:
    RV32I_INSTRUCTION_SET();
    void execute_instruction(uint32_t instruction);

private:
    int32_t registers[32];
    std::unordered_map<uint32_t, uint32_t> csr_registers;

    uint32_t read_csr(uint32_t csr_addr);
    void write_csr(uint32_t csr_addr, uint32_t value);
    int32_t sign_extend(uint32_t imm, uint32_t size);
};

#endif  // RV32I_INSTRUCTION_SET_H