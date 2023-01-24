/*******************************************************************************
 * @file projectfinal.c
 * @brief This Code is a CPU simulator
 * @version: V1.09
 * @date: 24. january. 2023
 * @author: Paria Khanjan
 * \mainpage
 ******************************************************************************/

/***********
 * Include Files
 * @brief I've added "ctype.h" library for the function "toupper". The "Windows.h" library is added for changing system color in errors.
 ************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <Windows.h>

int S[32];     /* sabbats */
int sabbat[8]; /* sabbat vaziat */
int stack[100];

/**
 *parityFlag Funtion
 *@brief it checks whether the binary form of the result has an even number of '1' bits or not.
 *@param int result
 *@return void
 **/
void parityFlag(int result)
{
    int count = 0;
    while (result != 0)
    {
        if (result % 2)
            count++;
        result /= 2;
    }
    if (count % 2)
        sabbat[0] = 1;
    else
        sabbat[0] = 0;
}

/**
 *zeroFlag Funtion
 *@brief It turn sabbat[1] to 1 if the result is equal to zero.
 *@param int result
 *@return void
 **/
void zeroFlag(int result)
{
    if (!result)
        sabbat[1] = 1;
    else
        sabbat[1] = 0;
}

/**
 *signFlag Funtion
 *@brief It'll turn sabbat[2] to 1 if our result is negative.
 *@param int result
 *@return void
 **/
void signFlag(int result)
{
    if (result < 0)
        sabbat[2] = 1;
    else
        sabbat[2] = 0;
}

/**
 *overFlowFlagAdd Funtion
 *@brief if the result of an addition has overflowed, it turns sabbat[5] to 1.
 *@param int a, int b, int a+b
 *@return void
 **/
void overflowFlagAdd(int a, int b, int result)
{
    if (a > 0 && b > 0 && result < 0 || a < 0 && b < 0 && result > 0)
        sabbat[5] = 1;
    else
        sabbat[5] = 0;
}

/**
 *overFlowFlagSub Funtion
 *@brief if the result of an subtraction has overflowed, it turns sabbat[5] to 1.
 *@param int a, int b, int a-b
 *@return void
 **/
void overFlowFlagSub(int a, int b, int result)
{
    if (a > 0 && b < 0 && result < 0 || a < 0 && b > 0 && result > 0)
        sabbat[5] = 1;
    else
        sabbat[5] = 0;
}

/**
 *overFlowFlagAdd Funtion
 *@brief if the result of an multiplication has overflowed, it turns sabbat[5] to 1.
 *@param int a, int b, int a*b
 *@return void
 **/
void overFlowProduct(int a, int b, int result)
{
    if (a != 0 && result / a != b)
        sabbat[5] = 1;
    else
        sabbat[5] = 0;
}

/**
 *PUSH Funtion
 *@brief adds S[rs] to stack.
 *@param int index (rs)
 *@return void
 **/
void PUSH(int rs)
{
    for (int i = 0; i < 100; i++)
    {
        stack[i + 1] = stack[i];
    }
    stack[0] = S[rs];
}

/**
 *POP Funtion
 *@brief removes S[rs] from the stack and returns it to S[].
 *@param int index (rs)
 *@return void
 **/
void POP(int rs)
{
    S[rs] = stack[0];
    for (int i = 0; i < 100; i++)
    {
        stack[0] = stack[i + 1];
    }
}

/**
 *ADD Funtion
 *@brief Adds the values of two sabbats and passes the result to the third.
 *@param int index of the first sabbat (rd), int index of the second sabbat (rs), and int index of the sabbat we'll pass the result to (rt).
 *@return void
 **/
void ADD(int rd, int rs, int rt)
{
    S[rd] = S[rs] + S[rt];
    parityFlag(S[rd]);
    zeroFlag(S[rd]);
    signFlag(S[rd]);
    overflowFlagAdd(S[rs], S[rt], S[rd]);
}

/**
 *SUB Funtion
 *@brief Subtracts the value of the first sabbat from the second sabbat and passes the result to the third.
 *@param int index of the first sabbat (rd), int index of the second sabbat (rs), and int index of the sabbat we'll pass the result to (rt).
 *@return void
 **/
void SUB(int rd, int rs, int rt)
{
    S[rd] = S[rt] - S[rs];
    parityFlag(S[rd]);
    zeroFlag(S[rd]);
    signFlag(S[rd]);
    overFlowFlagSub(S[rd], S[rt], S[rs]);
}

/**
 *DIV Funtion
 *@brief Divides the first sabbat by the second one. Passes the quotient to the first sabbat and the remainder to the second one.
 *@param int index of the first sabbat (rt), int index of the second sabbat (rs).
 *@return void
 **/
void DIV(int rt, int rs)
{
    int quotient = S[rt] / S[rs];
    int remainder = S[rt] % S[rs];
    S[rt] = quotient;
    S[rs] = remainder;
    parityFlag(S[rt]);
    zeroFlag(S[rt]);
    signFlag(S[rt]);
}

/**
 *AND Funtion
 *@brief Performs a bitwise and on two sabbats and passes the result to the third.
 *@param int index of the first sabbat (rd), int index of the second sabbat (rs), and int index of the sabbat we'll pass the result to (rt).
 *@return void
 **/
void AND(int rd, int rs, int rt)
{
    S[rd] = S[rt] & S[rs];
    parityFlag(S[rd]);
    zeroFlag(S[rd]);
    signFlag(S[rd]);
}

/**
 *XOR Funtion
 *@brief Performs a bitwise xor on two sabbats and passes the result to the third.
 *@param int index of the first sabbat (rd), int index of the second sabbat (rs), and int index of the sabbat we'll pass the result to (rt).
 *@return void
 **/
void XOR(int rd, int rs, int rt)
{
    S[rd] = S[rt] ^ S[rs];
    parityFlag(S[rd]);
    zeroFlag(S[rd]);
    signFlag(S[rd]);
}

/**
 *OR Funtion
 *@brief Performs a bitwise or on two sabbats and passes the result to the third.
 *@param int index of the first sabbat (rd), int index of the second sabbat (rs), and int index of the sabbat we'll pass the result to (rt).
 *@return void
 **/
void OR(int rd, int rs, int rt)
{
    S[rd] = S[rt] | S[rs];
    parityFlag(S[rd]);
    zeroFlag(S[rd]);
    signFlag(S[rd]);
}

/**
 *MULL Funtion
 *@brief it multiplies two sabbats and passes the 4 most significant bits to the first sabbat and the 4 least significant bits to the second one.
 *@param int index of the first sabbat (rt), int index of the second sabbat (rs).
 *@return void
 **/
void MULL(int rt, int rs)
{
    int product = S[rt] * S[rs];
    S[rt] = product / 16;
    S[rs] = product % 16;
    parityFlag(product);
    zeroFlag(product);
    signFlag(product);
    overFlowProduct(S[rt], S[rs], product);
}

/**
 *ADDI Funtion
 *@brief Adds a sabbat and a constant number and passes the result to the second sabbat.
 *@param int index of the first sabbat (rt), integer constant number, and int index of the sabbat we'll pass the result to (rs).
 *@return void
 **/
void ADDI(int rt, int rs, int Imm)
{
    S[rt] = S[rs] + Imm;
    parityFlag(S[rt]);
    zeroFlag(S[rt]);
    signFlag(S[rt]);
    overflowFlagAdd(S[rs], Imm, S[rt]);
}

/**
 *SUBI Funtion
 *@brief Subtarcts a constant number from a sabbat and passes the result to the second sabbat.
 *@param int index of the first sabbat (rt), integer constant number, and int index of the sabbat we'll pass the result to (rs).
 *@return void
 **/
void SUBI(int rt, int rs, int Imm)
{
    S[rt] = S[rs] - Imm;
    parityFlag(S[rt]);
    zeroFlag(S[rt]);
    signFlag(S[rt]);
    overFlowFlagSub(S[rs], Imm, S[rt]);
}

/**
 *ANDI Funtion
 *@brief Performs a bitwise and on a sabbat and a constant integer and passes the result to the second sabbat.
 *@param int index of the first sabbat (rt), integer constant number, and int index of the sabbat we'll pass the result to (rs).
 *@return void
 **/
void ANDI(int rt, int rs, int Imm)
{
    S[rt] = S[rs] & Imm;
    parityFlag(S[rt]);
    zeroFlag(S[rt]);
    signFlag(S[rt]);
}

/**
 *XORI Funtion
 *@brief Performs a bitwise xor on a sabbat and a constant integer and passes the result to the second sabbat.
 *@param int index of the first sabbat (rt), integer constant number, and int index of the sabbat we'll pass the result to (rs).
 *@return void
 **/
void XORI(int rt, int rs, int Imm)
{
    S[rt] = S[rs] ^ Imm;
    parityFlag(S[rt]);
    zeroFlag(S[rt]);
    signFlag(S[rt]);
}

/**
 *ORI Funtion
 *@brief Performs a bitwise or on a sabbat and a constant integer and passes the result to the second sabbat.
 *@param int index of the first sabbat (rt), integer constant number, and int index of the sabbat we'll pass the result to (rs).
 *@return void
 **/
void ORI(int rt, int rs, int Imm)
{
    S[rt] = S[rs] | Imm;
    parityFlag(S[rt]);
    zeroFlag(S[rt]);
    signFlag(S[rt]);
}

/**
 *MOV Funtion
 *@brief Passes a constant integer to a sabbat.
 *@param integer constant number, and int index of the sabbat we'll pass the value to (rt).
 *@return void
 **/
void MOV(int rt, int Imm)
{
    S[rt] = Imm;
}

/**
 *SWP Funtion
 *@brief It swaps the values of two sabbats.
 *@param int index of the first sabbat (rt) and int index of the second sabbat.
 *@return void
 **/
void SWP(int rt, int rs)
{
    S[rt] = S[rt] + S[rs];
    S[rs] = S[rt] - S[rs];
    S[rt] = S[rt] - S[rs];
}

/**
 *DUMP_REGS Funtion
 *@brief prints the values of all sabbats and all bits of sabbat vaziat.
 *@param void
 *@return void
 **/
void DUMP_REGS()
{
    system("color 07");
    printf("Sabbats: ");
    for (int i = 0; i < 32; i++)
    {
        printf("%d ", S[i]);
    }
    printf("\nSabbat Vaziat: ");
    for (int i = 0; i < 8; i++)
    {
        printf("%d ", sabbat[i]);
    }
}

/**
 *DUMP_REGS_F Funtion
 *@brief passes the values of all sabbats and all bits of sabbat vaziat to a file named "regs.txt".
 *@param void
 *@return void
 **/
void DUMP_REGS_F()
{
    FILE *file;
    file = fopen("regs.txt", "w");
    if (file == NULL)
        printf("ERROR!!!!!");
    else
    {
        for (int i = 0; i < 32; i++)
            fprintf(file, "%d ", S[i]);
        for (int i = 0; i < 8; i++)
            fprintf(file, " %d", sabbat[i]);
    }
    fclose(file);
}

/**
 *ERROR Funtion
 *@brief checks whether the argument the user has entered is valid.
 *@param three integer indexes of sabbats, a string of the unstruction we want to perform.
 *@return returns 1 if an error has occured and returns 0 if the argument are valid.
 **/
int Error(int rd, int rs, int rt, char instruction[12])
{
    int check = 1;
    if (rd < 0 || rs < 0 || rt < 0)
    {
        system("color 04"); /* changes terminal's color to red. */
        printf("ERROR! You have a negative argument in %s.\n", instruction);
        check = 0;
    }
    if (rd > 31 || rs > 31 || rt < 0)
    {
        system("color 04"); /* changes terminal's color to red. */
        printf("ERROR! You have an argument larger than 31 in %s.\n", instruction);
        check = 0;
    }
    if (check != 1)
        return 1;
    else
        return 0;
}

/**
 *main Funtion
 *@param int argc, char *argv[]
 *@return int
 **/

int main(int argc, char *argv[])
{
    int index;
    int jumpCount = 0;
    int lineCounter = 0;
    char commands[100];
    FILE *inputs;
    if (argc < 2)
    /* if the user has input an invalid file name, it automatically opens "in.txt" */ 
	    inputs = fopen("in.txt", "r");
    else
        inputs = fopen(argv[1], "r");
    /* it opens user's input */ 
    while (fscanf(inputs, "%[^\n]\n", commands) != EOF)
    /* scans each line from the file and passes it to the array "commands" */
    {
        lineCounter++;
        /*it counts all the lines to the file you've opened*/
    }
    rewind(inputs); 
    /* returns the pointer to the beggining of the file */
    
    while (fscanf(inputs, "%[^\n]\n", commands) != EOF)
    {
        int rd, rs, rt, Imm;
        char instructions[12] = {'\0'};
        for (int i = 0; i < sizeof(commands); i++)
        {
        	/* makes sure all the letters in our input are uppercase. */
            commands[i] = toupper(commands[i]);
        }
        for (index = 0; commands[index] != ' '; index++)
        {
        	/*passes the first word of the line (which is suppossed to b3e the instruction) to the array "instructions" */
            instructions[index] = commands[index];
        }
        
        if (strcmp(instructions, "ADD") == 0)
        {
            sscanf(commands, "ADD S%d, S%d, S%d", &rd, &rs, &rt);
            if (!Error(rd, rs, rt, instructions))
                ADD(rd, rs, rt);
        }
        
        else if (strcmp(instructions, "SUB") == 0)
        {
            sscanf(commands, "SUB S%d, S%d, S%d", &rd, &rs, &rt);
            if (!Error(rd, rs, rt, instructions))
                SUB(rd, rs, rt);
        }
        
        else if (strcmp(instructions, "AND") == 0)
        {
            sscanf(commands, "AND S%d, S%d, S%d", &rd, &rs, &rt);
            if (!Error(rd, rs, rt, instructions))
                AND(rd, rs, rt);
        }
        
        else if (strcmp(instructions, "XOR") == 0)
        {
            sscanf(commands, "XOR S%d, S%d, S%d", &rd, &rs, &rt);
            if (!Error(rd, rs, rt, instructions))
                XOR(rd, rs, rt);
        }
        
        else if (strcmp(instructions, "OR") == 0)
        {
            sscanf(commands, "OR S%d, S%d, S%d", &rd, &rs, &rt);
            if (!Error(rd, rs, rt, instructions))
                OR(rd, rs, rt);
        }
        
        else if (strcmp(instructions, "DIV") == 0)
        {
            sscanf(commands, "DIV S%d, S%d", &rt, &rs);
            if (!Error(rd, rs, 0, instructions))
                DIV(rt, rs);
        }
        
        else if (strcmp(instructions, "MULL") == 0)
        {
            sscanf(commands, "MULL S%d, S%d", &rt, &rs);
            if (!Error(rd, rs, 0, instructions))
                MULL(rt, rs);
        }
        
        else if (strcmp(instructions, "ADDI") == 0)
        {
            sscanf(commands, "ADDI S%d, S%d, %d", &rd, &rs, &Imm);
            if (!Error(rd, rs, 0, instructions))
                ADDI(rd, rs, Imm);
        }
        
        else if (strcmp(instructions, "SUBI") == 0)
        {
            sscanf(commands, "SUBI S%d, S%d, %d", &rd, &rs, &Imm);
            if (!Error(rd, rs, 0, instructions))
                SUBI(rd, rs, Imm);
        }
        
        else if (strcmp(instructions, "ANDI") == 0)
        {
            sscanf(commands, "ANDI S%d, S%d, %d", &rd, &rs, &Imm);
            if (!Error(rd, rs, 0, instructions))
                ANDI(rd, rs, Imm);
        }
        
        else if (strcmp(instructions, "XORI") == 0)
        {
            sscanf(commands, "XORI S%d, S%d, %d", &rd, &rs, &Imm);
            if (!Error(rd, rs, 0, instructions))
                XORI(rd, rs, Imm);
        }
        
        else if (strcmp(instructions, "ORI") == 0)
        {
            sscanf(commands, "ORI S%d, S%d, %d", &rd, &rs, &Imm);
            if (!Error(rd, rs, Imm, instructions))
                ORI(rd, rs, Imm);
        }
        
        else if (strcmp(instructions, "MOV") == 0)
        {
        	/* checks if the second input for MOV function is an index or a constant integer. */
            if (commands[8] == 'S' || commands[9] == 'S')
            {
                sscanf(commands, "MOV S%d, S%d", &rt, &rs);
                if (!Error(rt, rs, 0, instructions))
                    MOV(rt, S[rs]);
            }
            else
            {
                sscanf(commands, "MOV S%d, %d", &rt, &Imm);
                if (!Error(rt, 0, 0, instructions))
                    MOV(rt, Imm);
            }
        }
        
        else if (strcmp(instructions, "SWP") == 0)
        {
            sscanf(commands, "SWP S%d, S%d", &rt, &rs);
            if (!Error(rt, rs, 0, instructions))
                SWP(rt, rs);
        }
        
        else if (strcmp(instructions, "JMP") == 0)
        {
            jumpCount++;
            if (jumpCount > 10)
            {
            	/* if we have backward jumps in our input, an infinit loop might happen. We'll count our jumps and if the number gets higher than 10, we'll ignore jump instructions. */
                system("color 04");
                printf("ERROR! Infinit loop has happened! We'll skip your jumps from now on!\n");
                fscanf(inputs, "%[^\n]\n", commands);
                /* scans the current line again so that the conditions below will be false and we'll skip a line. */
                jumpCount = 0;
            }
            else
            {
                int countLines = 1, countChars = 0;
                sscanf(commands, "JMP %d", &Imm);
                if (Imm <= 0)
                {
                    system("color 04");
                    printf("ERROR! You can't jump to zero or a negative line!\n");
                }
                else if (Imm > lineCounter)
                {
                    system("color 04");
                    printf("ERROR! You only have %d lines. You can't jump to a line you don't have! \n", lineCounter);
                }
                else
                {
                    rewind(inputs);
                    /* return pointer to the beggining of our file */
                    while (countLines != Imm)
                    {
                        countChars++;
                        if (fgetc(inputs) == '\n')
                            countLines++;
                    }
                    /* it counts the charcters of the file until it reaches JMP's argument.*/
                    fseek(inputs, countChars, SEEK_SET);
                    /* places the pointer to the line we want to jump to. */
                    fscanf(inputs, "%[^\n]\n", commands);
                    /* scans the current line again so that the conditions below will be false and we'll skip a line. */
                }
            }
        }
        
        else if (strcmp(instructions, "SKIE") == 0)
        {
            sscanf(commands, "SKIE S%d, S%d", &rt, &rs);
            if (!Error(rt, rs, 0, instructions))
            {
                if (S[rs] == S[rt])
                {
                    fscanf(inputs, "%[^\n]\n", commands);
                    /* scans the current line again so that the conditions below will be false and we'll skip a line. */
                }
            }
        }
        
        else if (strcmp(instructions, "PUSH") == 0)
        {
            sscanf(commands, "PUSH S%d", &rs);
            if (!Error(rs, 0, 0, instructions))
                PUSH(rs);
        }
        
        else if (strcmp(instructions, "POP") == 0)
        {
            sscanf(commands, "POP S%d", &rs);
            if (!Error(rs, 0, 0, instructions))
                POP(rs);
        }
        
        else if (strcmp(instructions, "DUMP_REGS") == 0)
        {
            DUMP_REGS();
        }
        
        else if (strcmp(instructions, "DUMP_REGS_F") == 0)
        {
            DUMP_REGS_F();
        }
        
        else if (strcmp(instructions, "INPUT") == 0)
        {
        	/* takes a constant integer from user an passes it to S[0] */
            scanf("%d", &Imm);
            S[0] = Imm;
        }
        
        else if (strcmp(instructions, "OUTPUT") == 0)
        {
        	/* prints the first sabbat */
            printf("%d\n", S[0]);
        }
        
        else if (strcmp(instructions, "EXIT") == 0)
        {
        	/* stops the cpu simulator's work. */
            break;
        }
        
        else
        {
            system("color 04");
            printf("ERROR! Wrong instruction! Please try again. Maybe you've made a typo.\n");
        }
    }

    fclose(inputs);

    return 0;
}