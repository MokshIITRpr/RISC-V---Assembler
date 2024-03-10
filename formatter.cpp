#include <bits/stdc++.h>
#include <fstream>
using namespace std;
#define ll long long
#define err 1e18
ll Start = (1ll << 28) + 200;
map<string, ll> label;
ll pc = 0; // program counter
ll size1 = 0;
vector<string> store; // store the the binary format for hex
vector<ll> pc_count;
// I format -- > imm : rs1 : func3 : rd : opcode
// U format -- >
unordered_map<string, string> registers;

void initializeMap()
{
    pair<string, string> old_arr[] =
        {
            make_pair("x0", "x0"),
            make_pair("x1", "x1"),
            make_pair("x2", "x2"),
            make_pair("x3", "x3"),
            make_pair("x4", "x4"),
            make_pair("x5", "x5"),
            make_pair("x6", "x6"),
            make_pair("x7", "x7"),
            make_pair("x8", "x8"),
            make_pair("x9", "x9"),
            make_pair("x10", "x10"),
            make_pair("x11", "x11"),
            make_pair("x12", "x12"),
            make_pair("x13", "x13"),
            make_pair("x14", "x14"),
            make_pair("x15", "x15"),
            make_pair("x16", "x16"),
            make_pair("x17", "x17"),
            make_pair("x18", "x18"),
            make_pair("x19", "x19"),
            make_pair("x20", "x20"),
            make_pair("x21", "x21"),
            make_pair("x22", "x22"),
            make_pair("x23", "x23"),
            make_pair("x24", "x24"),
            make_pair("x25", "x25"),
            make_pair("x26", "x26"),
            make_pair("x27", "x27"),
            make_pair("x28", "x28"),
            make_pair("x29", "x29"),
            make_pair("x30", "x30"),
            make_pair("x31", "x31"),
            make_pair("zero", "x0"),
            make_pair("ra", "x1"),
            make_pair("sp", "x2"),
            make_pair("gp", "x3"),
            make_pair("tp", "x4"),
            make_pair("t0", "x5"),
            make_pair("t1", "x6"),
            make_pair("t2", "x7"),
            make_pair("s0", "x8"),
            make_pair("s1", "x9"),
            make_pair("a0", "x10"),
            make_pair("a1", "x11"),
            make_pair("a2", "x12"),
            make_pair("a3", "x13"),
            make_pair("a4", "x14"),
            make_pair("a5", "x15"),
            make_pair("a6", "x16"),
            make_pair("a7", "x17"),
            make_pair("s2", "x18"),
            make_pair("s3", "x19"),
            make_pair("s4", "x20"),
            make_pair("s5", "x21"),
            make_pair("s6", "x22"),
            make_pair("s7", "x23"),
            make_pair("s8", "x24"),
            make_pair("s9", "x25"),
            make_pair("s10", "x26"),
            make_pair("s11", "x27"),
            make_pair("t3", "x28"),
            make_pair("t4", "x29"),
            make_pair("t5", "x30"),
            make_pair("t6", "x31"),

        };

    int mppsize = (sizeof(old_arr) /
                   sizeof(old_arr[0]));

    unordered_map<string, string> mpp1(old_arr,
                                       old_arr + mppsize);
    registers = mpp1;
}

string BinaryToHex(string s)
{
    // we know its a string of length 32 exactly
    string ret = "0x";
    for (ll i = 0; i < 32; i += 4)
    {
        string temp;
        ll get = 0;
        for (ll j = i; j < i + 4; j++)
            s[j] == '0' ? get *= 2 : get = get * 2 + 1;
        if (get >= 10)
            ret += 'A' + get - 10;
        else
            ret += '0' + get;
    }
    return ret;
}

string getOpcode(string inp, string s)
{
    // hello
    //  add the opcode for final output string of instruction
    if (inp == "rem" || inp == "sra" || inp == "add" || inp == "sll" || inp == "slt" || inp == "xor" || inp == "srl" || inp == "or" || inp == "and" || inp == "sub" || inp == "mul" || inp == "div")
        s += "0110011";
    else if (inp == "addi" || inp == "andi" || inp == "ori")
        s += "0010011";
    else if (inp == "lb" || inp == "ld" || inp == "lw" || inp == "lh")
        s += "0000011";
    else if (inp == "jalr")
        s += "1100111";
    else if (inp == "beq" || inp == "bne" || inp == "bge" || inp == "blt")
        s += "1100011";

    else if (inp == "sb" || inp == "sh" || inp == "sw" || inp == "sd")
        s += "0100011";
    else if (inp == "auipc")
        s += "0010111";

    else if (inp == "lui")
        s += "0110111";
    else if (inp == "jal")
        s += "1101111";
    else
        s = "error"; // check for error here in s only
    return s;
}

string getRegister(string inp, string s)
{
    if (registers.find(inp) == registers.end())
    {
        return "error";
    }
    inp = registers[inp];
    // cout << registers[inp] << " ";
    if (inp[0] != 'x')
    {
        s = "error";
        return s;
    }

    string neo;
    ll sz = inp.size();
    if (inp[inp.size() - 1] == ',')
        sz = inp.size() - 1;
    else
        sz = inp.size();

    for (ll i = 1; i < sz; i++)
        neo += inp[i];
    // cout << inp << " " << sz << endl;
    //  keep check for integer input only
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
    if (a == "add" || a == "sub" || a == "mul" || a == "sb")
    {
        s += "000";
    }
    else if (a == "xor" || a == "div")
    {
        s += "100";
    }
    else if (a == "srl")
    {
        s += "101";
    }
    else if (a == "sra")
    {
        s += "101";
    }
    else if (a == "sll" || a == "sh")
    {
        s += "001";
    }
    else if (a == "slt" || a == "sw")
    {
        s += "010";
    }
    else if (a == "or" || a == "rem")
    {
        s += "110";
    }
    else if (a == "and")
    {
        s += "111";
    }
    else if (a == "sd")
    {
        s += "011";
    }
    else if (a == "lb" || a == "jalr" || a == "addi")
    {
        s += "000";
    }
    else if (a == "lh")
    {
        s += "001";
    }
    else if (a == "lw")
    {
        s += "010";
    }
    else if (a == "ld")
    {
        s += "011";
    }
    else if (a == "ori")
    {
        s += "110";
    }
    else if (a == "andi")
    {
        s += "111";
    }
    else if (a == "beq")
    {
        s += "000";
    }
    else if (a == "bne")
    {
        s += "001";
    }
    else if (a == "blt")
    {
        s += "100";
    }
    else if (a == "bge")
    {
        s += "101";
    }
    return s;
}

ll getData(string line)
{
    // supports ---> binary + hex + ascii + integer
    string final;
    ll ret = 0;
    if (line == "0")
        ret = 0;
    else if (line[0] == '0' && line[1] == 'x')
    {
        if (line.size() > 10)
            return (ll)err;
        for (ll i = 2; i < min((ll)line.size(), 10ll); i++)
            final += line[i];
        ret = stoi(final, 0, 16);
    }
    else if (line[0] == '0')
    {
        if (line.size() > 33)
            return err;
        for (ll i = 1; i < line.size(); i++)
            final += line[i];
        ret = stoi(final, 0, 2);
    }
    else if (line[0] == '-')
    {
        string temp;
        for (ll i = 1; i < line.size(); i++)
            temp += line[i];
        ret = stoi(temp);
        if (ret > (1ll << 32))
            return err;
        ret = -ret;
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

string getImmediate(string inp, string s)
{
    // keep check for integer input only
    if (s == "error")
        return s;
    ll get = getData(inp);
    if (get == err)
        return "error";

    if (get >= (1ll << 11) || get <= -(1ll << 11))
    {
        s = "error";
        return s;
    }
    if (get < 0)
        get += (1ll << 12);
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

string getImmediateSB(string inp, string s)
{
    // keep check for integer input only
    if (s == "error")
        return s;
    ll get = getData(inp);
    if (get == err)
        return "error";
    if (get < 0)
        get += (1ll << 13);
    if (get >= (1ll << 13) || get < 0)
    {
        s = "error";
        return s;
    }
    // get binary for the immediate to be added
    for (ll i = 12; i >= 0; i--)
    {
        if (get & (1ll << i))
            s += '1';
        else
            s += '0';
    }
    return s;
}

string getImmediateU(string inp, string s)
{
    // keep check for integer input only
    if (s == "error")
        return s;
    ll get = getData(inp);
    if (get == err)
        return "error";
    if (get < 0)
        get += (1ll << 12);
    get <<= 12;
    // get binary for the immediate to be added
    for (ll i = 31; i >= 12; i--)
    {
        if (get & (1ll << i))
            s += '1';
        else
            s += '0';
    }
    return s;
}

// add data to memory

void addMemory(string txt, ll mem_arr[200])
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
    }
    label[temp] = Start + size1; // marking for la
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
            mem_arr[size1] = get;
            size1++;
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
                mem_arr[size1] = g;
                size1++;
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
            {
                get += (ll)line[i];
                mem_arr[size1] = get;
                size1++; // assign to each charatcer one block
            }
        }
    }
    else if (line == ".half")
    {
        while (iss >> line)
        {
            if (line == ",")
                continue;
            ll get = getData(line);

            // now we have data in integer format and need to break datat into 2 parts
            ll mask = 255; // extraxt 8 bits
            for (ll i = 0; i < 2; i++)
            {
                ll g = mask & get;
                get >>= 8;
                mem_arr[size1] = g;
                size1++;
                // we have added to one byte
            }
        }
    }
    else if (line == ".dword")
    {
        while (iss >> line)
        {
            if (line == ",")
                continue;
            ll get = getData(line);

            // now we have data in integer format and need to break datat into 8 parts
            ll mask = 255; // extraxt 8 bits
            for (ll i = 0; i < 8; i++)
            {
                ll g = mask & get;
                get >>= 8;
                mem_arr[size1] = g;
                size1++;
                // we have added to one byte
            }
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
    string final;
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

    for (ll i = 5; i >= 0; i--)
    {
        final += ret[i];
        if (ret[i] == "error")
        {
            store.push_back("error");
            return;
        }
    }
    store.push_back(final);
    pc_count.push_back(pc);
}

void getIformat(string txt)
{
    string final;
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

    for (ll i = 4; i >= 0; i--)
    {
        final += ret[i];
        if (ret[i] == "error")
        {
            store.push_back("error");
            return;
        }
    }
    store.push_back(final);
    pc_count.push_back(pc);
}

void getNewIformat(string txt)
{
    string final;
    istringstream iss(txt); // create string stream
    string line;
    iss >> line;
    string keep = line;
    vector<string> ret(6, "");
    ret[0] = getOpcode(keep, ret[0]);
    iss >> line;
    if (line == ",")
        iss >> line;
    ret[1] = getRegister(line, ret[1]);
    ret[2] = getFunc3(line, ret[2]);
    iss >> line;
    if (line == ",")
        iss >> line;
    string off, reg;
    ll ind = 0;
    while (line[ind] != '(')
    {
        off += line[ind];
        ind++;
    }
    ind++;
    while (line[ind] != ')')
    {
        reg += line[ind];
        ind++;
    }

    ret[3] = getRegister(reg, ret[3]);
    ret[4] = getImmediate(off, ret[4]);
    for (ll i = 4; i >= 0; i--)
    {
        final += ret[i];
        if (ret[i] == "error")
        {
            store.push_back("error");
            return;
        }
    }
    store.push_back(final);
    pc_count.push_back(pc);
}

void getSformat(string txt)
{
    string final;
    istringstream iss(txt); // create string stream
    string line;
    iss >> line;
    string keep = line;
    vector<string> ret(6, "");
    ret[0] = getOpcode(keep, ret[0]);
    ret[2] = getFunc3(keep, ret[2]);
    iss >> line;
    if (line == ",")
        iss >> line;
    ret[4] = getRegister(line, ret[4]);

    iss >> line;
    if (line == ",")
        iss >> line;
    // now the line is of type offset(register)
    string off, reg;
    ll ind = 0;
    while (line[ind] != '(')
    {
        off += line[ind];
        ind++;
    }
    ind++;
    while (line[ind] != ')')
    {
        reg += line[ind];
        ind++;
    }

    ret[3] = getRegister(reg, ret[3]);
    ret[5] = getImmediate(off, ret[5]);
    // now we break the immediate field
    string temp = ret[5];
    ret[5] = "";
    for (ll i = 0; i < 7; i++)
        ret[5] += temp[i];
    for (ll i = 7; i <= 11; i++)
        ret[1] += temp[i];

    for (ll i = 5; i >= 0; i--)
    {
        final += ret[i];
        if (ret[i] == "error")
        {
            store.push_back("error");
            return;
        }
    }
    store.push_back(final);
    pc_count.push_back(pc);
}

void getSBformat(string txt)
{
    istringstream iss(txt); // create string stream
    string line;
    iss >> line;
    string keep = line;
    vector<string> ret(6, "");
    ret[0] = getOpcode(keep, ret[0]);
    ret[2] = getFunc3(keep, ret[2]);
    iss >> line;
    ret[3] = getRegister(line, ret[3]);
    iss >> line;
    if (line == ",")
        iss >> line;
    ret[4] = getRegister(line, ret[4]);
    iss >> line;
    if (line == ",")
        iss >> line;
    string final;
    bool flag = all_of(line.begin(), line.end(), ::isdigit); // not even integer in jump
    if (label.find(line) == label.end() && !flag)
    {
        store.push_back("error");
        return;
    }
    ll imm;
    if (label.find(line) != label.end())
        imm = label[line] - pc;
    else
        imm = stoi(line);
    if (imm % 4 != 0)
    {
        store.push_back("error");
        return;
    }
    // if(imm<0)imm+=
    string get = to_string(imm);
    ret[5] = getImmediateSB(get, ret[5]);
    string temp = ret[5];
    ret[5] = "";
    ret[5] += temp[0];
    for (ll i = 2; i <= 7; i++)
        ret[5] += temp[i];
    for (ll i = 8; i < 12; i++)
        ret[1] += temp[i];
    ret[1] += temp[1];
    for (ll i = 5; i >= 0; i--)
    {
        final += ret[i];
        if (ret[i] == "error")
        {
            store.push_back("error");
            return;
        }
    }
    store.push_back(final);
    pc_count.push_back(pc);
}

void getUformat(string txt)
{
    istringstream iss(txt); // create string stream
    string line;
    iss >> line;
    string keep = line;
    string final;
    vector<string> ret(3, "");
    ret[0] = getOpcode(line, ret[0]);
    iss >> line;
    if (line == ",")
        iss >> line;
    ret[1] = getRegister(line, ret[1]);
    iss >> line;
    if (line == ",")
        iss >> line;
    ret[2] = getImmediateU(line, ret[2]);
    for (ll i = 2; i >= 0; i--)
    {
        final += ret[i];
        if (ret[i] == "error")
        {
            store.push_back("error");
            return;
        }
    }
    store.push_back(final);
    pc_count.push_back(pc);
}

void getUJformat(string txt)
{
    istringstream iss(txt); // create string stream
    string line;
    iss >> line;
    string keep = line;
    string final;
    vector<string> ret(3, "");
    ret[0] = getOpcode(line, ret[0]);
    iss >> line;
    if (line == ",")
        iss >> line;
    ret[1] = getRegister(line, ret[1]);
    iss >> line;
    if (line == ",")
        iss >> line;
    bool flag = all_of(line.begin(), line.end(), ::isdigit); // not even integer in jump
    if (label.find(line) == label.end() && !flag)
    {
        store.push_back("error");
        return;
    }
    ll imm;
    if (label.find(line) != label.end())
        imm = label[line] - pc;
    else
        imm = stoi(line);
    if (imm % 4 != 0)
    {
        store.push_back("error");
        return;
    }
    ll jump = label[line] - pc;
    for (ll i = 20; i >= 1; i--)
    {
        if ((1ll << i) & jump)
            ret[2] += '1';
        else
            ret[2] += '0';
    }
    string temp = ret[2];
    ret[2] = "";
    ret[2] += temp[0];
    for (ll i = 10; i < 20; i++)
        ret[2] += temp[i];
    ret[2] += temp[9];
    for (ll i = 1; i <= 8; i++)
        ret[2] += temp[i];
    for (ll i = 2; i >= 0; i--)
    {
        final += ret[i];
        if (ret[i] == "error")
        {
            store.push_back("error");
            return;
        }
    }
    store.push_back(final);
    pc_count.push_back(pc);
}
void assemble(string txt)
{
    istringstream iss(txt); // create string stream
    string line;
    iss >> line;
    string keep = line;
    if (keep[keep.size() - 1] != ':')
        iss >> line;
    if (line == ":" || keep[keep.size() - 1] == ':')
    {
        return;
    }
    line = keep;
    if (line == "add" || line == "sub" || line == "xor" || line == "mul" || line == "div" || line == "rem" || line == "srl" || line == "sll" || line == "slt" || line == "or" || line == "and" || line == "sra")
    {
        getRformat(txt);
    }
    else if (line == "addi" || line == "andi" || line == "ori" || line == "jalr")
    {
        getIformat(txt);
    }
    else if (line == "lb" || line == "ld" || line == "lh" || line == "lw")
    {
        getNewIformat(txt);
    }
    else if (line == "sb" || line == "sw" || line == "sd" || line == "sh")
    {
        getSformat(txt);
    }
    else if (line == "beq" || line == "bne" || line == "bge" || line == "blt")
    {
        getSBformat(txt);
    }
    else if (line == "jal")
    {
        getUJformat(txt);
    }
    else if (line == "auipc" || line == "lui")
    {
        getUformat(txt);
    }
    else
    {
        store.push_back("Unknown operand type");
    }
    pc += 4; // after each operation
    return;
}
void read(string txt)
{
    istringstream iss(txt); // create string stream
    string line;
    iss >> line;
    string keep = line;
    if (keep[keep.size() - 1] != ':')
        iss >> line;
    ll sz = keep.size();
    if (line == ":" || keep[keep.size() - 1] == ':')
    {
        // cout << keep << endl;
        if (keep[keep.size() - 1] == ':')
            sz--;
        string tobe;
        for (ll i = 0; i < sz; i++)
            tobe += keep[i];
        label[tobe] = pc;
        return; // only label in that line
    }
    pc += 4;
}
int main()
{
    initializeMap();
    ofstream op("output.mc");
    ifstream file("input.asm");
    string txt;
    ll mem_arr[204];
    for (ll i = 0; i < 204; i++)
        mem_arr[i] = 0;
    ll flag = 0;
    ll memory = Start; // memory
    while (getline(file, txt))
    {
        if (txt == ".data")
        {

            while (getline(file, txt))
            {

                if (txt == ".text")
                {
                    // getline(file, txt);
                    break;
                }
                addMemory(txt, mem_arr);
            }
        }
        else if (txt == ".text")
            continue;
        else
            read(txt);
    }
    file.close();

    // we can get a read of the file beforehand as a cheat to get the labels
    // then we simply reset the program counter.

    ifstream file2("input.asm");

    pc = 0;

    while (getline(file2, txt))
    {
        if (txt == ".data")
        {
            while (getline(file2, txt))
            {
                if (txt == ".text")
                {
                    // getline(file2, txt);
                    break;
                }
            }
        }
        else if (txt == ".text")
            continue;
        else
            assemble(txt);
    }

    // for now print data here only for testing
    // cout << store.size() << endl;
    ll ct = 0;
    for (auto i : store)
    {
        if (i[0] == '0' || i[0] == '1')
        {
            op << "0x";
            op << hex << pc_count[ct] << "   ";
            op << BinaryToHex(i) << endl;
        }
        else
        {
            op << i << endl;
            // break;
        }
        ct++;
    }
    op << "\n\n";
    op << "------------------------------------------------------------------------------------------------------------------------\n\n";
    op << "Memory from 0x10000000 is displayed here\n\n";

    // end of printing

    for (ll i = 203; i >= 0; i -= 4)
    {
        op << "0x";
        op << hex << Start << "    ";
        for (ll j = i; j >= i - 3; j--)
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
            op << result << " ";
        }
        op << endl;
        Start -= 4;
    }
    /*for (auto i : label)
        cout << i.first << endl;*/
    op.close();
    file2.close();
    return 0;
}
