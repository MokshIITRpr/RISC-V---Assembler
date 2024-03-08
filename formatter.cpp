#include <bits/stdc++.h>
#include <fstream>
using namespace std;
#define ll long long
#define Start 0x10000000
ll size = 0;
// I format -- > imm : rs1 : func3 : rd : opcode
// U format -- >
string getOpcode(string inp, string s)
{
    // hello
    //  add the opcode for final output string of instruction
    if (inp == "add" || inp == "sll" || inp == "slt" || inp == "xor" || inp == "srl" || inp == "or" || inp == "and")
        s += "0110011";
    else if (inp == "addi" || inp == "andi" || inp == "ori")
        s += "0010011";
    else if (inp == "lb" || inp == "ld" || inp == "lw" || inp == "lh")
        s += "0000011";
    else if (inp == "jalr")
        s += "1100111";
    else if(inp=="beq" || inp=="bne" || inp=="bge" || inp=="blt")
        s+= "1100011";
    
    else if (inp == "sb" || inp == "sh" || inp == "sw" || inp == "sd")
        s += "0100011";
    else if (inp == "auipc")
        s += "0010111";

    else if (inp == "lui")
        s += "0110111";

    else
        s = "error"; // check for error here in s only
    return s;
}

string getRegister(string inp, string s)
{

    if (inp[0] != 'x')
    {
        s = "error";
        return s;
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
        return s;
    }
    ll get = stoi(neo);

    if (get >= 32 || get < 0)
    {
        s = "error";
        return s;
    }

    for (ll i = 4; i >= 0; i--)
    {
        if (get & (1ll << i))
            s += '1';
        else
            s += '0';
    }
    // adding the 5 bit register value ---> unsigned
    return s;
}

string getFunc7(string inp, string s)
{
    if (s == "error")
        return s;
    if (inp == "sub" || inp == "sra")
        s += "0100000";
    else if (inp == "add" || inp == "sll" || inp == "slt" || inp == "xor" || inp == "srl" || inp == "or" || inp == "and")
        s += "0000000";

    else if (inp == "mul" || inp == "div" || inp == "rem")
        s += "0000001";

    else
        s = "error";
    return s;
}

string getFunc3(string a, string s)
{

    if (a == "lb" || a == "jalr" || a == "addi")
    {
        s += "000";
    }
    else if (a == "srl")
    {
        s += "101";
    }
    else if (a == "sra")
    {
        s += "101";
    }
    else if (a == "xor" || a == "div")
    {
        s += "100";
    }
    else if (a == "and")
    {
        s += "111";
    }
    else if (a == "bge")
    {
        s += "101";
    }
    else if (a == "sd")
    {
        s += "011";
    }
    else if (a == "lw")
    {
        s += "010";
    }
    else if (a == "sll" || a == "sh")
    {
        s += "001";
    }
    else if (a == "ld")
    {
        s += "011";
    }
    else if (a == "bne")
    {
        s += "001";
    }
    else if (a == "or" || a == "rem")
    {
        s += "110";
    }
    else if (a == "add" || a == "sub" || a == "mul" || a == "sb")
    {
        s += "000";
    }
     else if (a == "lh")
    {
        s += "001";
    }
    else if (a == "slt" || a == "sw")
    {
        s += "010";
    }
    else if (a == "ori")
    {
        s += "110";
    }
    else if (a == "beq")
    {
        s += "000";
    }
    else if (a == "blt")
    {
        s += "100";
    }
    else if (a == "andi")
    {
        s += "111";
    }
    
    return s;
}

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

// copy till here ......................................................
//....................................................................
//...................................................................

string getImmediate(string inp, string s)
{
    // keep check for integer input only
    if (s == "error")
        return s;
    ll get = getData(inp);
    if (get < 0)
        get += (1ll << 12);
    if (get >= (1ll << 12) || get < 0)
    {
        s = "error";
        return s;
    }
    // get binary for the immediate to be added
    for (ll i = 11; i >= 0; i--)
    {
        if (get & (1ll << i))
            s += '1';
        else
            s += '0';
    }
    return s;
}

// add data to memory

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
// convert assebly to machine code
void getRformat(string txt)
{
    istringstream iss(txt); // create string stream
    string line;
    iss >> line;
    string keep = line;
    vector<string> ret(6, "");
    ret[0] = getOpcode(keep, ret[0]);
    iss >> line;
    // cout << line << endl;
    ret[1] = getRegister(line, ret[1]);

    iss >> line;
    if (line == ",")
        iss >> line;
    ret[3] = getRegister(line, ret[3]);
    iss >> line;
    if (line == ",")
        iss >> line;
    ret[4] = getRegister(line, ret[4]);
    ret[5] = getFunc7(keep, ret[5]);
    ret[2] = getFunc3(keep, ret[2]);
    string final;
    final += "0x";
    for (ll i = 5; i >= 0; i--)
        final += ret[i];
}

void getIformat(string txt)
{
    istringstream iss(txt); // create string stream
    string line;
    iss >> line;
    string keep = line;
    vector<string> ret(5, "");
    ret[0] = getOpcode(keep, ret[0]);
    iss >> line;
    // cout << line << endl;
    ret[1] = getRegister(line, ret[1]);

    iss >> line;
    if (line == ",")
        iss >> line;
    ret[3] = getRegister(line, ret[3]);
    iss >> line;
    if (line == ",")
        iss >> line;
    ret[4] = getImmediate(line, ret[4]);
    ret[2] = getFunc3(keep, ret[2]);
    string final = "0x";
    for (ll i = 4; i >= 0; i--)
        final += ret[i];
}

void assemble(string txt)
{
    istringstream iss(txt); // create string stream
    string line;
    iss >> line;

    if (line == "add" || line == "sub" || line == "xor" || line == "mul" || line == "div" || line == "rem" || line == "srl" || line == "sll" || line == "slt" || line == "or" || line == "and" || line == "sra")
    {
        getRformat(txt);
    }
    else if (line == "addi" || line == "andi" || line == "ori" || line == "lb" || line == "ld" || line == "lh" || line == "lw" || line == "jalr")
    {
        getIformat(txt);
    }
    else if (line == "sb" || line == "sw" || line == "sd" || line == "sh")
    {
    }
    else if (line == "beq" || line == "bne" || line == "bge" || line == "blt")
    {
    }
    else if (line == "jal")
    {
    }
    else if (line == "auipc" || line == "lui")
    {
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
            {
                getline(file, txt);
                break;
            }
            addMemory(label, txt, mem_arr);
        }
        display_memory(mem_arr);
    }
    if (txt != ".text")
        assemble(txt);
    /*while(getline(file,txt)){
        assemble(txt);
    }*/
    op.close();
    file.close();
    return 0;
}
