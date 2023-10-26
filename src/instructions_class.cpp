#include <stdio.h>
#include <fstream>
#include <iostream>

u_int32_t readcode(char* code, const char* codefile, const u_int32_t size)
{
    std::ifstream file(codefile, std::ios::binary);
    if (!file)
    {
        std::cerr << "Failed to open code file: " << codefile << std::endl;
        return 0;
    }
    // Read code from the file
    file.read(code, size);
    if (!file)
    {
        std::cerr << "Failed to read code from file: " << codefile << std::endl;
        return 0;
    }
    // Get the number of bytes read
    const u_int32_t bytesRead = static_cast<u_int32_t>(file.gcount());
    // Close the file
    file.close();
    return bytesRead;
}

class instruction_set
{
	private:
    u_int32_t stack[MMS];               // Stack memory : MMS = Main Memory Size
    u_int32_t main_memory[MMS];         // Main  memory : MMS = Main Memory Size
    u_int32_t p[4];                     // Pointers
    u_int32_t r[8];                     // Registers
    u_int32_t ip, flags, sp;            // Instruction pointer, flags, stack pointer
    char code[MCS];                     // Code buffer
    u_int32_t codesize;                 // Code size

    public:
    instruction_set(void);                          // Constructor
    u_int32_t load_code(char* codefile);             // Load code from file
    u_int32_t get_operand1_value(u_int32_t operand_1, const char type);   // Get value of operand 1
    u_int32_t get_operand2_value(u_int32_t operand_2, const char type);   // Get value of operand 2

    // Instruction functions
		u_int32_t jump(u_int32_t operand_1, const char type);
		u_int32_t jump_zero(u_int32_t operand_1, const char type);
		u_int32_t compare(u_int32_t operand_1, u_int32_t operand_2, const char type);
		u_int32_t increase(u_int32_t operand_1, const char type);
		u_int32_t decrease(u_int32_t operand_1, const char type);
		u_int32_t input(u_int32_t operand_1, const char type);
		u_int32_t output(u_int32_t operand_1, const char type);
		u_int32_t move(u_int32_t operand_1, u_int32_t operand_2, const char type);
		u_int32_t swap(u_int32_t operand_1, u_int32_t operand_2);
		u_int32_t logic_and(u_int32_t operand_1, u_int32_t operand_2, const char type);
		u_int32_t logic_not(u_int32_t operand_1, const char type);
		u_int32_t logic_xor(u_int32_t operand_1, u_int32_t operand_2, const char type);
		u_int32_t logic_or(u_int32_t operand_1, u_int32_t operand_2, const char type);

		instruction_set(void);
		u_int32_t load_code(char* codefile);
		u_int32_t execute(u_int32_t ins);
        u_int32_t get_operand1_value(u_int32_t operand_1, const char type);
        u_int32_t get_operand2_value(u_int32_t operand_2, const char type);
};

instruction_set::instruction_set(void)
{
	fill(stack, stack + MSS, 0);
	fill(main_memory, main_memory + MMS, 0);
	ip = 0;
	sp = 0;
	flags = 0;
}
u_int32_t instruction_set::load_code(char* codefile)
{
	codesize = readcode(code, codefile, MCS);
    return codesize;
}
u_int32_t instruction_set::get_operand1_value(u_int32_t operand_1, const char type)
{
    u_int32_t temp;
    if ((type & 0xC0) == 0x00)
        return 0;
    if ((type & 0xC0) == 0x40)
    {
        if (operand_1 > MMS)
            return 14;
        temp = main_memory[operand_1];
    }
    if ((type & 0xC0) == 0x80)
        temp = operand_1;
    if ((type & 0xC0) == 0xC0)
    {
        if (operand_1 < 8)
            temp = r[operand_1];
        else
        if (operand_1 < 10)
            temp = main_memory[p[operand_1 - 8]];
        else
        if (operand_1 == 15)
            temp = sp;
        else
            return 14;
    }
    return temp;
}
u_int32_t instruction_set::get_operand2_value(u_int32_t operand_2, const char type)
{
    u_int32_t temp;
    if ((type & 0x30) == 0x00)
        return 0;
    if ((type & 0x30) == 0x10)
    {
        if (operand_2 > MMS)
            return 14;
        temp = main_memory[operand_2];
    }
    if ((type & 0x30) == 0x20)
        temp = operand_2;
    if ((type & 0x30) == 0x30)
    {
        if (operand_2 < 8)
            temp = r[operand_2];
        else
        if (operand_2 < 10)
            temp = main_memory[p[operand_2 - 8]];
        else
        if (operand_2 == 15)
            temp = sp;
        else
            return 14;
    }
    return temp;
}

u_int32_t instruction_set::jump(u_int32_t operand_1, const char type)
{
    if ((type & 0x0C) == 0x00)
        return 0;
    if ((type & 0x0C) == 0x40)
        return 14;
    if ((type & 0x0C) == 0x80)
    {
        if ((operand_1 + 1) * 10 > codesize)
            return 14;
        ip = operand_1;
    }
    if ((type & 0x0C) == 0xC0)
        return 14;
	return 0;
}
u_int32_t instruction_set::jump_zero(u_int32_t operand_1, const char type)
{
    if (((type & 0x03) ^ (flags & 0x03)) == 0)
        return jump(operand_1, type);
	return 0;
}
u_int32_t instruction_set::compare(u_int32_t operand_1, u_int32_t operand_2, const char type)
{
    operand_1 = get_operand1_value(operand_1, type);
    operand_2 = get_operand2_value(operand_2, type);
    if (operand_1 > operand_2)
        flags |= 0x02;
    else
        flags &= 0xFFFFFFFD;
    if (operand_1 < operand_2)
        flags |= 0x01;
    else
        flags &= 0xFFFFFFFE;
	return 0;
}
u_int32_t instruction_set::increase(u_int32_t operand_1, const char type)
{
    if ((type & 0xC0) == 0x00)
        return 0;
    if ((type & 0xC0) == 0x40)
    {
        if (operand_1 > MMS)
            return 14;
        main_memory[operand_1]++;
    }
    if ((type & 0xC0) == 0x80)
       return 0;
    if ((type & 0xC0) == 0xC0)
    {
        if (operand_1 < 8)
            r[operand_1]++;
        else
        if (operand_1 < 10)
            main_memory[p[operand_1 - 8]]++;
        else
        if (operand_1 == 15)
            sp++;
        else
            return 14;
    }
    cout << "instruction_set::increase(" << main_memory[operand_1] << ")" << endl;
    return 0;
}
u_int32_t instruction_set::decrease(u_int32_t operand_1, const char type)
{
    if ((type & 0xC0) == 0x00)
          return 0;
    if ((type & 0xC0) == 0x40)
    {
        if (operand_1 > MMS)
            return 14;
        main_memory[operand_1]--;
    }
    if ((type & 0xC0) == 0x80)
    {
        return 0;
    }
    if ((type & 0xC0) == 0xC0)
    {
        if (operand_1 < 8)
            r[operand_1]--;
        else
        if (operand_1 < 10)
            main_memory[p[operand_1 - 8]]--;
        else
        if (operand_1 == 15)
            sp--;
        else
            return 14;
    }
    cout << "instruction_set::decrease(" << main_memory[operand_1] << ")" << endl;
	return 0;
}
u_int32_t instruction_set::input(u_int32_t operand_1, const char type)
{
    if ((type & 0xC0) == 0x00)
        return 0;
    if ((type & 0xC0) == 0x40)
    {
        if (operand_1 > MMS)
            return 14;
        cin >> main_memory[operand_1];
    }
    if ((type & 0xC0) == 0x80)
       return 0;
    if ((type & 0xC0) == 0xC0)
    {
        if (operand_1 < 8)
            cin >> r[operand_1];
        else
        if (operand_1 < 10)
            cin >> main_memory[p[operand_1 - 8]];
        else
            return 14;
    }
    cout << "instruction_set::input" << endl;
    return 0;
}
u_int32_t instruction_set::output(u_int32_t operand_1, const char type)
{
    if ((type & 0xC0) == 0x00)
        return 0;
    if ((type & 0xC0) == 0x40)
    {
        if (operand_1 > MMS)
            return 14;
        cout << main_memory[operand_1];
    }
    if ((type & 0xC0) == 0x80)
       return 0;
    if ((type & 0xC0) == 0xC0)
    {
        if (operand_1 < 8)
            cout << r[operand_1];
        else
        if (operand_1 < 10)
            cout << main_memory[p[operand_1 - 8]];
        else
            return 14;
    }
    cout << "instruction_set::output" << endl;
    return 0;
}
u_int32_t instruction_set::move(u_int32_t operand_1, u_int32_t operand_2, const char type)
{
    u_int32_t temp = get_operand2_value(operand_2, type);

    if ((type & 0xC0) == 0x00)
        return 0;
    if ((type & 0xC0) == 0x40)
    {
        if (operand_1 > MMS)
            return 14;
        operand_1 = temp;
    }
    if ((type & 0xC0) == 0x80)
        return 14;
    if ((type & 0xC0) == 0xC0)
    {
        if (operand_1 < 8)
            r[operand_1] = temp;
        else
        if (operand_1 < 10)
            main_memory[p[operand_1 - 8]] = temp;
        else
        if (operand_1 == 15)
            sp = temp;
        else
            return 14;
    }
	return 0;
}
u_int32_t instruction_set::swap(u_int32_t operand_1, u_int32_t operand_2)
{
    if (operand_1 >= 8 || operand_2 >= 8)
        return 14;

    u_int32_t temp = r[operand_1];
    r[operand_1] = r[operand_2];
    r[operand_2] = temp;

    return 0;
}
u_int32_t instruction_set::logic_and(u_int32_t operand_1, u_int32_t operand_2, const char type)
{
    u_int32_t temp = get_operand2_value(operand_2, type);
    if ((type & 0xC0) == 0x00)
        return 0;
    if ((type & 0xC0) == 0x40)
    {
        if (operand_1 > MMS)
            return 14;
        operand_1 &= temp;
    }
    if ((type & 0xC0) == 0x80)
        return 14;
    if ((type & 0xC0) == 0xC0)
    {
        if (operand_1 < 8)
            r[operand_1] &= temp;
        else
        if (operand_1 < 10)
            main_memory[p[operand_1 - 8]] &= temp;
        else
            return 14;
    }
	return 0;
}
u_int32_t instruction_set::logic_not(u_int32_t operand_1, const char type)
{
    if ((type & 0xC0) == 0x00)
        return 0;
    if ((type & 0xC0) == 0x40)
    {
        if (operand_1 > MMS)
            return 14;
        operand_1 = ~operand_1;
    }
    if ((type & 0xC0) == 0x80)
        return 14;
    if ((type & 0xC0) == 0xC0)
    {
        if (operand_1 < 8)
            r[operand_1] = ~r[operand_1];
        else
        if (operand_1 < 10)
            main_memory[p[operand_1 - 8]] = ~main_memory[p[operand_1 - 8]];
        else
            return 14;
    }
	return 0;
}
u_int32_t instruction_set::logic_xor(u_int32_t operand_1, u_int32_t operand_2, const char type)
{
    u_int32_t temp = get_operand2_value(operand_2, type);
    if ((type & 0xC0) == 0x00)
        return 0;
    if ((type & 0xC0) == 0x40)
    {
        if (operand_1 > MMS)
            return 14;
        operand_1 ^= temp;
    }
    if ((type & 0xC0) == 0x80)
        return 14;
    if ((type & 0xC0) == 0xC0)
    {
        if (operand_1 < 8)
            r[operand_1] ^= temp;
        else
        if (operand_1 < 10)
            main_memory[p[operand_1 - 8]] ^= temp;
        else
            return 14;
    }
	return 0;
}
u_int32_t instruction_set::logic_or(u_int32_t operand_1, u_int32_t operand_2, const char type)
{
    u_int32_t temp = get_operand2_value(operand_2, type);
    if ((type & 0xC0) == 0x00)
        return 0;
    if ((type & 0xC0) == 0x40)
    {
        if (operand_1 > MMS)
            return 14;
        operand_1 |= temp;
    }
    if ((type & 0xC0) == 0x80)
        return 14;
    if ((type & 0xC0) == 0xC0)
    {
        if (operand_1 < 8)
            r[operand_1] |= temp;
        else
        if (operand_1 < 10)
            main_memory[p[operand_1 - 8]] |= temp;
        else
            return 14;
    }
	return 0;
}

u_int32_t instruction_set::execute(u_int32_t ins)
{
	if (codesize < (ins + 1) * 10)
		return 14;
	char prefix = code[ins * 10 + 0];
	u_int32_t operand1 = ((u_int32_t)code[ins * 10 + 2] << 12) + ((u_int32_t)code[ins * 10 + 3] << 8) + ((u_int32_t)code[ins * 10 + 4] << 4) + (u_int32_t)code[ins * 10 + 5];
	u_int32_t operand2 = ((u_int32_t)code[ins * 10 + 6] << 12) + ((u_int32_t)code[ins * 10 + 7] << 8) + ((u_int32_t)code[ins * 10 + 4] << 4) + (u_int32_t)code[ins * 10 + 5];

	switch (code[ins * 10 + 1])
	{
		case 16:
			jump(operand1, prefix);
			break;
		case 17:
			jump_zero(operand1, prefix);
			break;
		case 19:
			compare(operand1, operand2, prefix);
			break;
		case 20:
			increase(operand1, prefix);
			break;
		case 21:
			decrease(operand1, prefix);
			break;
		case 22:
			input(operand1, prefix);
			break;
		case 23:
			output(operand1, prefix);
			break;
		case 26:
			move(operand1, operand2, prefix);
			break;
		case 27:
			swap(operand1, operand2);
			break;
		case 28:
		    logic_and(operand1, operand2, prefix);
			break;
		case 29:
			logic_not(operand1, prefix);
			break;
		case 30:
			logic_xor(operand1, operand2, prefix);
			break;
		case 31:
			logic_or(operand1, operand2, prefix);
			break;
        default:
            break;
	}
	return 0;
}
