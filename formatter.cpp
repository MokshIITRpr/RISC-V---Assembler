#include <bits/stdc++.h>
#include <fstream>
using namespace std;
#define ll long long
#define Start 0x10000000
ll size = 0;
// I format -- > imm : rs1 : func3 : rd : opcode
// U format -- >
void getOpcode(string inp, string s)
{
    // hello
    //  add the opcode for final output string of instruction
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
    if (s == "error" || inp[0] != 'x')
    {
        s = "error";
        return;
    }

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
    if (get >= 32 || get < 0)
    {
        s = "error";
        return;
    }
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
    switch (inp[0])
    {
    case 'a':
        s += (inp == "add" || inp == "addi") ? "000" : "001";
        break;
    case 'b':
        s += (inp == "beq") ? "000" : ((inp == "bne") ? "001" : ((inp == "blt") ? "100" : ((inp == "bge") ? "101" : "")));
        break;
    case 'd':
        s += (inp == "ld" || inp == "sd") ? "011" : ((inp == "div") ? "100" : "");
        break;
    case 'l':
        s += (inp == "lb") ? "000" : ((inp == "lh") ? "001" : ((inp == "lw") ? "010" : ""));
        break;
    case 'm':
        s += (inp == "mul") ? "000" : "";
        break;
    case 'o':
        s += (inp == "or" || inp == "ori") ? "110" : "";
        break;
    case 'r':
        s += (inp == "sra" || inp == "srl") ? "101" : ((inp == "rem") ? "110" : "");
        break;
    case 's':
        s += (inp == "sb") ? "000" : ((inp == "sh") ? "001" : ((inp == "sw") ? "010" : ((inp == "sll") ? "001" : ((inp == "slt") ? "010" : ((inp == "sub") ? "000" : "")))));
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

// add data to memory
ll getData(string line)
{
    // supports ---> binary + hex + ascii + integer
    string final;
    ll ret = 0;
    if (line[0] == '0' && line[1] == 'x')
    {
        for (ll i = 2; i < min((ll)line.size(), 10ll); i++)
            final += line[i];
        ret = stoi(final, 0, 16);
    }
    else if (line[0] == '0')
    {
        for (ll i = 1; i < line.size(); i++)
            final += line[i];
        ret = stoi(final, 0, 2);
    }
    else if ((ll)(line[0] - '0') < 0 || (ll)(line[0] - '0') > 9)
    { // no intger detected
        for (ll i = 0; i < line.size(); i++)
            ret += (ll)line[i];
    }
    else
        ret = stoi(line);
    return ret;
}

void addMemory(map<string, ll> &label, string txt, ll mem_arr[200])
{

    istringstream iss(txt);
    string line;
    // first is label
    iss >> line;
    string temp = line;
    if (line[line.size() - 1] == ':')
    {
        temp = "";
        for (ll i = 0; i < line.size() - 1; i++)
            temp += line[i];
        line = temp;
    }
    label[temp] = Start + size; // marking for la
    if (line[line.size() - 1] != ':')
        iss >> line; // jump to next line
    iss >> line;
    if (line == ".byte")
    {
        while (iss >> line)
        {
            if (line == ",")
                continue;
            ll get = getData(line);
            mem_arr[size] = get;
            size++;
        }
    }
    else if (line == ".word")
    {
        while (iss >> line)
        {
            if (line == ",")
                continue;
            ll get = getData(line);

            // now we have data in integer format and need to break datat into 4 parts
            ll mask = 255; // extraxt 8 bits
            for (ll i = 0; i < 4; i++)
            {
                ll g = mask & get;
                get >>= 8;
                mem_arr[size] = g;
                size++;
                // we have added to one byte
            }
        }
    }
    else if (line == ".asciiz")
    {
        while (iss >> line)
        {
            if (line == ",")
                continue;
            ll get = 0;
            for (ll i = 0; i < line.size(); i++)
                get += (ll)line[i];
            mem_arr[size] = get;
            size++;
        }
    }
    return;
}
// dsiplay the memeory for user purposes
void display_memory(ll mem_arr[])
{
    for (ll i = 0; i < 200; i += 4)
    {
        for (ll j = i + 3; j >= i; j--)
        {
            ll val = mem_arr[j];
            string result = "00";
            ll add = val % 16;
            if (add >= 10)
                result[1] = 'A' + add - 10;
            else
                result[1] = '0' + add;
            val /= 16;
            add = val % 16;
            if (add >= 10)
                result[0] = 'A' + add - 10;
            else
                result[0] = '0' + add;
            cout << result << " ";
        }
        cout << endl;
    }
}

int main()
{
    ofstream op("output.txt");
    ifstream file("input.txt");
    string txt;
    ll mem_arr[200];
    for (ll i = 0; i < 200; i++)
        mem_arr[i] = 0;
    map<string, ll> label;
    ll flag = 0;
    ll memory = Start; // memory

    ll pc = 0; // program counter
    getline(file, txt);

    if (txt == ".data")
    {
        while (getline(file, txt))
        {

            if (txt == ".text")
                break;
            addMemory(label, txt, mem_arr);
        }
    }
    display_memory(mem_arr);

    op.close();
    file.close();
    return 0;
}
