#include <iostream>
#include <cstdint>

#define ADDI   0x13
#define R_TYPE 0x33
#define LBU    0x3
#define SBU    0x23
#define BEQ    0x63
#define JAL    0x6F
#define JALR   0x67
#define LUI    0x37
#define AUIPC  0x17

int32_t registers[32]; // 32 general-purpose registers

void execute_instruction(uint32_t instruction) 
{
    
    // Extracting registers fields
    uint32_t rd  = (instruction >>  7) & 0x1F;
    uint32_t rs1 = (instruction >> 15) & 0x1F;
    uint32_t rs2 = (instruction >> 20) & 0x1F;

    // Extracting opcode, funct3 and funct7 fields
    uint32_t opcode = instruction & 0x7F;
    uint32_t funct3 = (instruction >> 12) & 0x7;
    uint32_t funct7 = (instruction >> 25) & 0x7F;

    // Immediate value generation logic
    int32_t imm_i = ((int32_t)instruction) >> 20;
    int32_t imm_s = (((int32_t)instruction) >> 20) | (((int32_t)instruction) & 0xFE000000);
    int32_t imm_b = (((int32_t)instruction) >> 19) & 0x1;
    imm_b |= (((int32_t)instruction) >> 20) & 0x7E;
    imm_b |= (((int32_t)instruction) << 3) & 0x80;
    imm_b |= (((int32_t)instruction) << 4) & 0x800;
    imm_b |= (((int32_t)instruction) << 19) & 0xFFFFF000;
    int32_t imm_u = ((int32_t)instruction) & 0xFFFFF000;
    int32_t imm_j = (((int32_t)instruction) >> 11) & 0x1;
    imm_j |= (((int32_t)instruction) >> 1) & 0x7FE;
    imm_j |= (((int32_t)instruction) >> 20) & 0x800;
    imm_j |= (((int32_t)instruction) << 11) & 0xFF000;
    imm_j |= ((int32_t)instruction) & 0x80000000;
    
    switch (opcode) 
    {
        case ADDI:
            registers[rd] = registers[rs1] + imm_i;
            break;
        case R_TYPE:
            if (funct3 == 0x0 && funct7 == 0x0)                     // ADD
                registers[rd] = registers[rs1] + registers[rs2];
            else if (funct3 == 0x0 && funct7 == 0x20)               // SUB
                registers[rd] = registers[rs1] - registers[rs2];
            else
                std::cout << "Unsupported instruction: " << std::hex << instruction << std::endl;
            break;
        case LBU:
            registers[rd] = (int8_t)(registers[rs1] + imm_i);
            break;
        case SBU:
            *((int8_t*)(registers[rs1] + imm_s)) = registers[rs2];
            break;
        case BEQ:
            if (registers[rs1] == registers[rs2]) 
                registers[0] = registers[0] + imm_b;
            break;
        case JAL:
            registers[rd] = registers[0] + 4;
            registers[0] = registers[0] + imm_j;
            break;
        case JALR:
            registers[rd] = registers[0] + 4;
            registers[0] = (registers[rs1] + imm_i) & ~0x1;
            break;
        case LUI:
            registers[rd] = imm_u;
            break;
        case AUIPC:
            registers[rd] = registers[0] + imm_u;
            break;
        case R_TYPE:
            if (funct3 == 0x7 && funct7 == 0x0) // AND
                registers[rd] = registers[rs1] & registers[rs2];
            else
                std::cout << "Unsupported instruction: " << std::hex << instr << std::endl;
            break;

        case R_TYPE:
            if (funct3 == 0x6 && funct7 == 0x0) // OR
                registers[rd] = registers[rs1] | registers[rs2];
            else
                std::cout << "Unsupported instruction: " << std::hex << instr << std::endl;
            break;

        case R_TYPE:
            if (funct3 == 0x1 && funct7 == 0x0) // SLL
                registers[rd] = registers[rs1] << registers[rs2];
            else
                std::cout << "Unsupported instruction: " << std::hex << instr << std::endl;
            break;

        case R_TYPE:
            if (funct3 == 0x5 && funct7 == 0x0) // SRL
                registers[rd] = registers[rs1] >> registers[rs2];
            else
                std::cout << "Unsupported instruction: " << std::hex << instr << std::endl;
            break;

        case R_TYPE:
            if (funct3 == 0x5 && funct7 == 0x20) // SRA
                registers[rd] = (int32_t)registers[rs1] >> registers[rs2];
            else
                std::cout << "Unsupported instruction: " << std::hex << instr << std::endl;
            break;
        default:
            std::cout << "Unsupported instruction: " << std::hex << instruction << std::endl;
            break;
    }
}

int main() 
{
    // Example program: addi t0, zero, 42
    uint32_t instruction = 0x00200093;    
    execute_instruction(instruction);
    std::cout << "Result: " << registers[5] << std::endl; // Printthe value of register t0
    // Example program: add t0, t1, t2
    instruction = 0x00A3033;    
    execute_instruction(instruction);
    std::cout << "Result: " << registers[5] << std::endl; // Print the value of register t0
    // Example program: sub t0, t1, t2
    instruction = 0x40A3033;    
    execute_instruction(instruction);
    std::cout << "Result: " << registers[5] << std::endl; // Print the value of register t0
    return 0;
}