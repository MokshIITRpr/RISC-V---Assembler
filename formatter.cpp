#include <bits/stdc++.h>
using namespace std;
#define ll long long

// I format -- > imm : rs1 : func3 : rd : opcode
// U format -- >
void getOpcode(string inp, string s)
{
    // add the opcode for final output string of instruction
    if (inp == "addi" || inp == "andi" || inp == "ori")
        s += "0010011";
    else if (inp == "lb" || inp == "ld" || inp == "lw" || inp == "lh")
        s += "0000011";
    else if (inp == "jalr")
        s += "1100111";
    else if (inp == "R")
        s += "0110011";

    else if (inp == "S")
        s += "0100011";
    else if (inp == "auipc")
        s += "0010111";

    else if (inp == "lui")
        s += "0110111";

    else
        s = "error"; // check for error here in s only
}

void getRegister(string inp, string s)
{
    if (s == "error")
        return;
    string neo;
    for (ll i = 1; i < inp.size(); i++)
        neo += inp[i];
    // keep check for integer input only
    ll flag = 0;
    for (ll i = 0; i < neo.size(); i++)
    {
        ll check = neo[i] - '0';
        if (check > 9 || check < 0)
        {
            flag++;
            break;
        }
    }
    if (flag)
    {
        s = "error";
        return;
    }
    ll get = stoi(neo);
    for (ll i = 0; i < 5; i++)
    {
        if (get & (1 << i))
            s += '1';
        else
            s += '0';
    }
    // adding the 5 bit register value ---> unsigned
}

void getFunc7(string inp, string s)
{
    if (s == "error")
        return;
    if (inp == "sub" || inp == "sra")
        s += "0100000";
    else if (inp == "add" || inp == "sll" || inp == "slt" || inp == "xor" || inp == "srl" || inp == "or" || inp == "and")
        s += "0000000";

    else if (inp == "mul" || inp == "div" || inp == "rem")
        s += "0000001";

    else
        s = "error";
}

void getFunc3(string inp, string s)
{
    if (inp == "add" || inp == "sub" || inp == "mul" || inp == "sb")
        s += "000";

    else if (inp == "xor" || inp == "div")
        s += "100";

    else if (inp == "srl")
        s += "101";

    else if (inp == "sra")
        s += "101";

    else if (inp == "sll" || inp == "sh")
        s += "001";

    else if (inp == "slt" || inp == "sw")
        s += "010";

    else if (inp == "or" || inp == "rem")
        s += "110";

    else if (inp == "and")
        s += "111";

    else if (inp == "sd")
        s += "011";

    else if (inp == "lb" || inp == "jalr" || inp == "addi")
        s += "000";

    else if (inp == "lh")
        s += "001";

    else if (inp == "lw")
        s += "010";

    else if (inp == "ld")
        s += "011";

    else if (inp == "ori")
        s += "110";

    else if (inp == "andi")
        s += "111";

    else if (inp == "beq")
        s += "000";

    else if (inp == "bne")
        s += "001";

    else if (inp == "blt")
        s += "100";

    else if (inp == "bge")
        s += "101";
}

void getImmediate(string inp, string s)
{
    // keep check for integer input only
    if (s == "error")
        return;
    ll flag = 0;
    for (ll i = 1; i < inp.size(); i++)
    {
        ll check = inp[i] - '0';
        if (check > 9 || check < 0)
        {
            flag++;
            break;
        }
    }
    if (flag)
    {
        s = "error";
        return;
    }
    ll get = stoi(inp);
    if (get < 0)
        get += (1ll << 12);
    if (get < 0 || get >= (1ll << 12))
    {
        s = "error";
        return;
    }
    // get binary for the immediate to be added
    for (ll i = 0; i < 12; i++)
    {
        if (get & (1ll << i))
            s += '1';
        else
            s += '0';
    }
}

int main()
{
    return 0;
}