#include <stdio.h>

class instruction_set
{
	private:
		u_int32_t stack[MMS];
		u_int32_t main_memory[MMS];
		u_int32_t p[4];
		u_int32_t r[8];
		u_int32_t ip, flags, sp;
		char code[MCS];
		u_int32_t codesize;
	public:
		u_int32_t jump(u_int32_t op1, const char type);
		u_int32_t jumpzero(u_int32_t op1, const char type);
		u_int32_t compare(u_int32_t op1, u_int32_t op2, const char type);
		u_int32_t increase(u_int32_t op1, const char type);
		u_int32_t decrease(u_int32_t op1, const char type);
		u_int32_t input(u_int32_t op1, const char type);
		u_int32_t output(u_int32_t op1, const char type);
		u_int32_t move(u_int32_t op1, u_int32_t op2, const char type);
		u_int32_t swap(u_int32_t op1, u_int32_t op2); // IN PROGRESS
		u_int32_t logic_and(u_int32_t op1, u_int32_t op2, const char type);
		u_int32_t logic_not(u_int32_t op1, const char type);
		u_int32_t logic_xor(u_int32_t op1, u_int32_t op2, const char type);
		u_int32_t logic_or(u_int32_t op1, u_int32_t op2, const char type);
		instruction_set(void);
		u_int32_t loadcode(char* codefile);
		u_int32_t execute(u_int32_t ins);
        u_int32_t getoperand2value(u_int32_t op2, const char type);
        u_int32_t getoperand1value(u_int32_t op2, const char type);
};
instruction_set::instruction_set(void)
{
	fill(stack, stack + MSS, 0);
	fill(main_memory, main_memory + MMS, 0);
	ip = 0;
	sp = 0;
	flags = 0;
}
u_int32_t instruction_set::loadcode(char* codefile)
{
	codesize = readcode(code, codefile, MCS);
    return codesize;
}
u_int32_t instruction_set::getoperand1value(u_int32_t op1, const char type)
{
    u_int32_t temp;
    if ((type & 0xC0) == 0x00)
        return 0;
    if ((type & 0xC0) == 0x40)
    {
        if (op1 > MMS)
            return 14;
        temp = main_memory[op1];
    }
    if ((type & 0xC0) == 0x80)
        temp = op1;
    if ((type & 0xC0) == 0xC0)
    {
        if (op1 < 8)
            temp = r[op1];
        else
        if (op1 < 10)
            temp = main_memory[p[op1 - 8]];
        else
        if (op1 == 15)
            temp = sp;
        else
            return 14;
    }
    return temp;
}
u_int32_t instruction_set::getoperand2value(u_int32_t op2, const char type)
{
    u_int32_t temp;
    if ((type & 0x30) == 0x00)
        return 0;
    if ((type & 0x30) == 0x10)
    {
        if (op2 > MMS)
            return 14;
        temp = main_memory[op2];
    }
    if ((type & 0x30) == 0x20)
        temp = op2;
    if ((type & 0x30) == 0x30)
    {
        if (op2 < 8)
            temp = r[op2];
        else
        if (op2 < 10)
            temp = main_memory[p[op2 - 8]];
        else
        if (op2 == 15)
            temp = sp;
        else
            return 14;
    }
    return temp;
}

u_int32_t instruction_set::jump(u_int32_t op1, const char type)
{
    if ((type & 0x0C) == 0x00)
        return 0;
    if ((type & 0x0C) == 0x40)
        return 14;
    if ((type & 0x0C) == 0x80)
    {
        if ((op1 + 1) * 10 > codesize)
            return 14;
        ip = op1;
    }
    if ((type & 0x0C) == 0xC0)
        return 14;
	return 0;
}
u_int32_t instruction_set::jumpzero(u_int32_t op1, const char type)
{
    if (((type & 0x03) ^ (flags & 0x03)) == 0)
        return jump(op1, type);
	return 0;
}
u_int32_t instruction_set::compare(u_int32_t op1, u_int32_t op2, const char type)
{
    op1 = getoperand1value(op1, type);
    op2 = getoperand2value(op2, type);
    if (op1 > op2)
        flags |= 0x02;
    else
        flags &= 0xFFFFFFFD;
    if (op1 < op2)
        flags |= 0x01;
    else
        flags &= 0xFFFFFFFE;
	return 0;
}
u_int32_t instruction_set::increase(u_int32_t op1, const char type)
{
    if ((type & 0xC0) == 0x00)
        return 0;
    if ((type & 0xC0) == 0x40)
    {
        if (op1 > MMS)
            return 14;
        main_memory[op1]++;
    }
    if ((type & 0xC0) == 0x80)
       return 0;
    if ((type & 0xC0) == 0xC0)
    {
        if (op1 < 8)
            r[op1]++;
        else
        if (op1 < 10)
            main_memory[p[op1 - 8]]++;
        else
        if (op1 == 15)
            sp++;
        else
            return 14;
    }
    cout << "instruction_set::increase(" << main_memory[op1] << ")" << endl;
    return 0;
}
u_int32_t instruction_set::decrease(u_int32_t op1, const char type)
{
    if ((type & 0xC0) == 0x00)
          return 0;
    if ((type & 0xC0) == 0x40)
    {
        if (op1 > MMS)
            return 14;
        main_memory[op1]--;
    }
    if ((type & 0xC0) == 0x80)
    {
        return 0;
    }
    if ((type & 0xC0) == 0xC0)
    {
        if (op1 < 8)
            r[op1]--;
        else
        if (op1 < 10)
            main_memory[p[op1 - 8]]--;
        else
        if (op1 == 15)
            sp--;
        else
            return 14;
    }
    cout << "instruction_set::decrease(" << main_memory[op1] << ")" << endl;
	return 0;
}
u_int32_t instruction_set::input(u_int32_t op1, const char type)
{
    if ((type & 0xC0) == 0x00)
        return 0;
    if ((type & 0xC0) == 0x40)
    {
        if (op1 > MMS)
            return 14;
        cin >> main_memory[op1];
    }
    if ((type & 0xC0) == 0x80)
       return 0;
    if ((type & 0xC0) == 0xC0)
    {
        if (op1 < 8)
            cin >> r[op1];
        else
        if (op1 < 10)
            cin >> main_memory[p[op1 - 8]];
        else
            return 14;
    }
    cout << "instruction_set::input" << endl;
    return 0;
}
u_int32_t instruction_set::output(u_int32_t op1, const char type)
{
    if ((type & 0xC0) == 0x00)
        return 0;
    if ((type & 0xC0) == 0x40)
    {
        if (op1 > MMS)
            return 14;
        cout << main_memory[op1];
    }
    if ((type & 0xC0) == 0x80)
       return 0;
    if ((type & 0xC0) == 0xC0)
    {
        if (op1 < 8)
            cout << r[op1];
        else
        if (op1 < 10)
            cout << main_memory[p[op1 - 8]];
        else
            return 14;
    }
    cout << "instruction_set::output" << endl;
    return 0;
}
u_int32_t instruction_set::move(u_int32_t op1, u_int32_t op2, const char type)
{
    u_int32_t temp = getoperand2value(op2, type);

    if ((type & 0xC0) == 0x00)
        return 0;
    if ((type & 0xC0) == 0x40)
    {
        if (op1 > MMS)
            return 14;
        op1 = temp;
    }
    if ((type & 0xC0) == 0x80)
        return 14;
    if ((type & 0xC0) == 0xC0)
    {
        if (op1 < 8)
            r[op1] = temp;
        else
        if (op1 < 10)
            main_memory[p[op1 - 8]] = temp;
        else
        if (op1 == 15)
            sp = temp;
        else
            return 14;
    }
	return 0;
}
u_int32_t instruction_set::swap(u_int32_t op1, u_int32_t op2)
{
	return 0;
}
u_int32_t instruction_set::logic_and(u_int32_t op1, u_int32_t op2, const char type)
{
    u_int32_t temp = getoperand2value(op2, type);
    if ((type & 0xC0) == 0x00)
        return 0;
    if ((type & 0xC0) == 0x40)
    {
        if (op1 > MMS)
            return 14;
        op1 &= temp;
    }
    if ((type & 0xC0) == 0x80)
        return 14;
    if ((type & 0xC0) == 0xC0)
    {
        if (op1 < 8)
            r[op1] &= temp;
        else
        if (op1 < 10)
            main_memory[p[op1 - 8]] &= temp;
        else
            return 14;
    }
	return 0;
}
u_int32_t instruction_set::logic_not(u_int32_t op1, const char type)
{
    if ((type & 0xC0) == 0x00)
        return 0;
    if ((type & 0xC0) == 0x40)
    {
        if (op1 > MMS)
            return 14;
        op1 = ~op1;
    }
    if ((type & 0xC0) == 0x80)
        return 14;
    if ((type & 0xC0) == 0xC0)
    {
        if (op1 < 8)
            r[op1] = ~r[op1];
        else
        if (op1 < 10)
            main_memory[p[op1 - 8]] = ~main_memory[p[op1 - 8]];
        else
            return 14;
    }
	return 0;
}
u_int32_t instruction_set::logic_xor(u_int32_t op1, u_int32_t op2, const char type)
{
    u_int32_t temp = getoperand2value(op2, type);
    if ((type & 0xC0) == 0x00)
        return 0;
    if ((type & 0xC0) == 0x40)
    {
        if (op1 > MMS)
            return 14;
        op1 ^= temp;
    }
    if ((type & 0xC0) == 0x80)
        return 14;
    if ((type & 0xC0) == 0xC0)
    {
        if (op1 < 8)
            r[op1] ^= temp;
        else
        if (op1 < 10)
            main_memory[p[op1 - 8]] ^= temp;
        else
            return 14;
    }
	return 0;
}
u_int32_t instruction_set::logic_or(u_int32_t op1, u_int32_t op2, const char type)
{
    u_int32_t temp = getoperand2value(op2, type);
    if ((type & 0xC0) == 0x00)
        return 0;
    if ((type & 0xC0) == 0x40)
    {
        if (op1 > MMS)
            return 14;
        op1 |= temp;
    }
    if ((type & 0xC0) == 0x80)
        return 14;
    if ((type & 0xC0) == 0xC0)
    {
        if (op1 < 8)
            r[op1] |= temp;
        else
        if (op1 < 10)
            main_memory[p[op1 - 8]] |= temp;
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
			jumpzero(operand1, prefix);
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
