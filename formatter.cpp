#include <bits/stdc++.h>
using namespace std;
#define ll long long
#include <fstream>
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

void getFunc3(string inp, string s) {
    switch (inp[0]) {
        case 'a':
            s += (inp == "add" || inp == "addi") ? "000" : "001";
            break;
        case 'b':
            s += (inp == "beq") ? "000" : ((inp == "bne") ? "001" : ((inp == "blt") ? "100" : ((inp == "bge") ? "101" : "")));
            break;
        case 'd':
            s += ( inp == "ld" || inp == "sd") ? "011" : ((inp == "div")?"100":"");
            break;
        case 'l':
            s += (inp == "lb") ? "000" : ((inp == "lh") ? "001" : ((inp == "lw")?"010":""));
            break;
        case 'm':
            s += (inp == "mul") ? "000" : "";
            break;
        case 'o':
            s += (inp == "or" || inp == "ori") ? "110" : "";
            break;
        case 'r':
            s += (inp == "sra" || inp == "srl") ? "101" : ((inp == "rem")?"110":"");
            break;
        case 's':
            s += (inp == "sb") ? "000" : ((inp == "sh")?"001":((inp == "sw") ? "010" : ((inp == "sll") ? "001" : ((inp == "slt") ? "010" : ((inp == "sub") ? "000" : "")))));
            break;
        case 'x':
            s += (inp == "xor") ? "100" : "";
            break;
        default:
            s += "111";
            break;
    }
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
    ofstream op("output.txt");
    ifstream ip("input.txt");
    string txt;
    while (getline (ip, txt)) {
        
    }
    op.close();
    ip.close();
    return 0;
}
