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
