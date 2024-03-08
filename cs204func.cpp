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
    }
    store.push_back(final);
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
    }
    store.push_back(final);
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
    }
    store.push_back(final);
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
    if (label.find(line) == label.end())
    {
        final = "No such label was found\n";
    }
    ll imm = label[line] - pc;
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
        final += ret[i];
    store.push_back(final);
}

void getUformat(string txt)
{
}

void getUJformat(string txt)
{
}
