#include <stdio.h>
#include <string.h>
#include <ctype.h>
int S[32];
int sabbat[8];
int stack[100];
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
void zeroFlag(int result)
{
    if (!result)
        sabbat[1] = 1;
    else
        sabbat[1] = 0;
}
void signFlag(int result)
{
    if (result < 0)
        sabbat[2] = 1;
    else
        sabbat[2] = 0;
}
void overflowFlagAdd(int a, int b, int result)
{
    if (a > 0 && b > 0 && result < 0 || a < 0 && b < 0 && result > 0)
        sabbat[5] = 1;
    else
        sabbat[5] = 0;
}
void overFlowFlagSub(int a, int b, int result)
{
    if (a > 0 && b < 0 && result < 0 || a < 0 && b > 0 && result > 0)
        sabbat[5] = 1;
    else
        sabbat[5] = 0;
}
void overFlowProduct(int a, int b, int result)
{
    if (a != 0 && result / a != b)
        sabbat[5] = 1;
    else
        sabbat[5] = 0;
}
void PUSH(int rs)
{
    for (int i = 0; i < 100; i++)
    {
        stack[i + 1] = stack[i];
    }
    stack[0] = S[rs];
}
void POP(int rs)
{
    S[rs] = stack[0];
    for (int i = 0; i < 100; i++)
    {
        stack[0] = stack[i + 1];
    }
}
void ADD(int rd, int rs, int rt)
{
    S[rd] = S[rs] + S[rt];
    parityFlag(S[rd]);
    zeroFlag(S[rd]);
    signFlag(S[rd]);
    overflowFlagAdd(S[rs], S[rt], S[rd]);
}
void SUB(int rd, int rs, int rt)
{
    S[rd] = S[rt] - S[rs];
    parityFlag(S[rd]);
    zeroFlag(S[rd]);
    signFlag(S[rd]);
    overFlowFlagSub(S[rd], S[rt], S[rs]);
}
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
void AND(int rd, int rs, int rt)
{
    S[rd] = S[rt] & S[rs];
    parityFlag(S[rd]);
    zeroFlag(S[rd]);
    signFlag(S[rd]);
}
void XOR(int rd, int rs, int rt)
{
    S[rd] = S[rt] ^ S[rs];
    parityFlag(S[rd]);
    zeroFlag(S[rd]);
    signFlag(S[rd]);
}
void OR(int rd, int rs, int rt)
{
    S[rd] = S[rt] | S[rs];
    parityFlag(S[rd]);
    zeroFlag(S[rd]);
    signFlag(S[rd]);
}
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
void ADDI(int rt, int rs, int Imm)
{
    S[rt] = S[rs] + Imm;
    parityFlag(S[rt]);
    zeroFlag(S[rt]);
    signFlag(S[rt]);
    overflowFlagAdd(S[rs], Imm, S[rt]);
}
void SUBI(int rt, int rs, int Imm)
{
    S[rt] = S[rs] - Imm;
    parityFlag(S[rt]);
    zeroFlag(S[rt]);
    signFlag(S[rt]);
    overFlowFlagSub(S[rs], Imm, S[rt]);
}
void ANDI(int rt, int rs, int Imm)
{
    S[rt] = S[rs] & Imm;
    parityFlag(S[rt]);
    zeroFlag(S[rt]);
    signFlag(S[rt]);
}
void XORI(int rt, int rs, int Imm)
{
    S[rt] = S[rs] ^ Imm;
    parityFlag(S[rt]);
    zeroFlag(S[rt]);
    signFlag(S[rt]);
}
void ORI(int rt, int rs, int Imm)
{
    S[rt] = S[rs] | Imm;
    parityFlag(S[rt]);
    zeroFlag(S[rt]);
    signFlag(S[rt]);
}
void MOV(int rt, int Imm)
{
    S[rt] = Imm;
}
void SWP(int rt, int rs)
{
    S[rt] = S[rt] + S[rs];
    S[rs] = S[rt] - S[rs];
    S[rt] = S[rt] - S[rs];
}
void DUMP_REGS()
{
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
void INPUT()
{
    int Imm;
    scanf("%d", &Imm);
    S[0] = Imm;
}
void OUTPUT()
{
    printf("%d", S[0]);
}
int main(int argc, char *argv[])
{
    int jumpCount = 0;
    int j;
    char commands[100];
    FILE *inputs;
    if (argc < 2)
        inputs = fopen("in.txt", "r");
    else
        inputs = fopen(argv[1], "r");
    while (fscanf(inputs, "%[^\n]\n", commands) != EOF)
    {
        jumpCount++;
        int rd, rs, rt, Imm;
        char instructions[12] = {'\0'};
        for (int i = 0; i < sizeof(commands); i++)
        {
            commands[i] = toupper(commands[i]);
        }
        for (j = 0; commands[j] != ' '; j++)
        {
            instructions[j] = commands[j];
        }
        if (strcmp(instructions, "ADD") == 0)
        {
            sscanf(commands, "ADD S%d, S%d, S%d", &rd, &rs, &rt);
            ADD(rd, rs, rt);
        }
        else if (strcmp(instructions, "SUB") == 0)
        {
            sscanf(commands, "SUB S%d, S%d, S%d", &rd, &rs, &rt);
            SUB(rd, rs, rt);
        }
        else if (strcmp(instructions, "AND") == 0)
        {
            sscanf(commands, "AND S%d, S%d, S%d", &rd, &rs, &rt);
            AND(rd, rs, rt);
        }
        else if (strcmp(instructions, "XOR") == 0)
        {
            sscanf(commands, "XOR S%d, S%d, S%d", &rd, &rs, &rt);
            XOR(rd, rs, rt);
        }
        else if (strcmp(instructions, "OR") == 0)
        {
            sscanf(commands, "OR S%d, S%d, S%d", &rd, &rs, &rt);
            OR(rd, rs, rt);
        }
        else if (strcmp(instructions, "DIV") == 0)
        {
            sscanf(commands, "DIV S%d, S%d", &rt, &rs);
            DIV(rt, rs);
        }
        else if (strcmp(instructions, "MULL") == 0)
        {
            sscanf(commands, "MULL S%d, S%d", &rt, &rs);
            MULL(rt, rs);
        }
        else if (strcmp(instructions, "ADDI") == 0)
        {
            sscanf(commands, "ADDI S%d, S%d, %d", &rd, &rs, &Imm);
            ADDI(rd, rs, Imm);
        }
        else if (strcmp(instructions, "SUBI") == 0)
        {
            sscanf(commands, "SUBI S%d, S%d, %d", &rd, &rs, &Imm);
            SUBI(rd, rs, Imm);
        }
        else if (strcmp(instructions, "ANDI") == 0)
        {
            sscanf(commands, "ANDI S%d, S%d, %d", &rd, &rs, &Imm);
            ANDI(rd, rs, Imm);
        }
        else if (strcmp(instructions, "XORI") == 0)
        {
            sscanf(commands, "XORI S%d, S%d, %d", &rd, &rs, &Imm);
            XORI(rd, rs, Imm);
        }
        else if (strcmp(instructions, "ORI") == 0)
        {
            sscanf(commands, "ORI S%d, S%d, %d", &rd, &rs, &Imm);
            ORI(rd, rs, Imm);
        }
        else if (strcmp(instructions, "MOV") == 0)
        {
            if (commands[8] == 'S' || commands[9] == 'S')
            {
                sscanf(commands, "MOV S%d, S%d", &rt, &rs);
                MOV(rt, S[rs]);
            }
            else
            {
                sscanf(commands, "MOV S%d, %d", &rt, &Imm);
                MOV(rt, Imm);
            }
        }
        else if (strcmp(instructions, "SWP") == 0)
        {
            sscanf(commands, "SWP S%d, S%d", &rt, &rs);
            SWP(rt, rs);
        }
        else if (strcmp(instructions, "JMP") == 0)
        {
            jumpCount++;
            if (jumpCount > 10)
            {
                printf("Error! Infinit loop has happened! We'll skip your jumps from now on!\n");
                fscanf(inputs, "%[^\n]\n", commands);
                jumpCount = 0;
            }
            else
            {
                int countLines = 1, countChars = 0;
                sscanf(commands, "JMP %d", &Imm);
                rewind(inputs);
                while (countLines != Imm)
                {
                    countChars++;
                    if (fgetc(inputs) == '\n')
                        countLines++;
                }
                fseek(inputs, countChars, SEEK_SET);
                fscanf(inputs, "%[^\n]\n", commands);
            }
        }
        else if (strcmp(instructions, "SKIE") == 0)
        {
            sscanf(commands, "SKIE S%d, S%d", &rt, &rs);
            if (S[rs] == S[rt])
            {
                fscanf(inputs, "%[^\n]\n", commands);
            }
        }
        else if (strcmp(instructions, "PUSH") == 0)
        {
            sscanf(commands, "PUSH S%d", &rs);
            PUSH(rs);
        }
        else if (strcmp(instructions, "POP") == 0)
        {
            sscanf(commands, "POP S%d", &rs);
            POP(rs);
        }
        else if (strcmp(instructions, "EXIT") == 0)
        {
            break;
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
            INPUT();
        }
        else if (strcmp(instructions, "OUTPUT") == 0)
        {
            OUTPUT();
        }
        else if (strcmp(instructions, "EXIT") == 0)
        {
            break;
        }
        else
        {
            printf("ERROR! Wrong instruction! Please try again. Maybe you've made a typo\n");
        }
    }
    fclose(inputs);
    return 0;
}