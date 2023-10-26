#include <iostream>
#include <cstdint>

int32_t registers[32]; // 32 general-purpose registers

void execute_instruction(uint32_t instr) 
{
    uint32_t opcode = instr & 0x7F;
    uint32_t rd = (instr >> 7) & 0x1F;
    uint32_t rs1 = (instr >> 15) & 0x1F;
    uint32_t rs2 = (instr >> 20) & 0x1F;
    int32_t imm_i = ((int32_t)instr) >> 20;
    int32_t imm_s = (((int32_t)instr) >> 20) | (((int32_t)instr) & 0xFE000000);
    int32_t imm_b = (((int32_t)instr) >> 19) & 0x1;
    imm_b |= (((int32_t)instr) >> 20) & 0x7E;
    imm_b |= (((int32_t)instr) << 3) & 0x80;
    imm_b |= (((int32_t)instr) << 4) & 0x800;
    imm_b |= (((int32_t)instr) << 19) & 0xFFFFF000;
    int32_t imm_u = ((int32_t)instr) & 0xFFFFF000;
    int32_t imm_j = (((int32_t)instr) >> 11) & 0x1;
    imm_j |= (((int32_t)instr) >> 1) & 0x7FE;
    imm_j |= (((int32_t)instr) >> 20) & 0x800;
    imm_j |= (((int32_t)instr) << 11) & 0xFF000;
    imm_j |= ((int32_t)instr) & 0x80000000;
    
    switch (opcode) 
    {
        case 0x13: // addi
            registers[rd] = registers[rs1] + imm_i;
            break;
        case 0x33: // add
            registers[rd] = registers[rs1] + registers[rs2];
            break;
        case 0x3: // lb
            registers[rd] = (int8_t)(registers[rs1] + imm_i);
            break;
        case 0x23: // sb
            *((int8_t*)(registers[rs1] + imm_s)) = registers[rs2];
            break;
        case 0x63: // beq
            if (registers[rs1] == registers[rs2]) 
                registers[0] = registers[0] + imm_b;
            break;
        case 0x6F: // jal
            registers[rd] = registers[0] + 4;
            registers[0] = registers[0] + imm_j;
            break;
        case 0x67: // jalr
            registers[rd] = registers[0] + 4;
            registers[0] = (registers[rs1] + imm_i) & ~0x1;
            break;
        case 0x37: // lui
            registers[rd] = imm_u;
            break;
        case 0x17: // auipc
            registers[rd] = registers[0] + imm_u;
            break;
        default:
            std::cout << "Unsupported instruction: " << std::hex << instr << std::endl;
            break;
    }
}

int main() 
{
    // Example program: addi t0, zero, 42
    uint32_t instr = 0x00200093;    
    execute_instruction(instr);
    std::cout << "Result: " << registers[5] << std::endl; // Print the value of register t0
    return 0;
}