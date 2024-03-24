#include <bits/stdc++.h>
#include <fstream>
using namespace std;
#define ll long long
#define err 1e18

map<ll, string> bpnt_actual;
map<ll, string> bpnt_predicted;
map<ll, string> bpt_actual;
map<ll, string> bpt_predicted;
map<ll, string> bpob_actual;
map<ll, string> bpob_predicted;
map<ll, string> bptb_actual;
map<ll, string> bptb_predicted;
ll previous = 0;
ll current = 0;
string last = "";

ll hex_to_int(string line)
{
    ll res = 0;
    for (ll i = 2; i < 10; i++)
    {
        if (line[i] >= '0' && line[i] <= '9')
            res = (res * 16) + (ll)(line[i] - '0');
        else
            res = (res * 16) + (ll)(line[i] - 'a') + 10;
    }
    return res;
}
void branch_predictor_not_taken()
{

    if (last.size() == 0)
        return;
    else if (last[0] == 'b')
    {
        if (current - previous == 4)
            bpnt_actual[previous] += 'T';
        else
            bpnt_actual[previous] += 'N';
        bpnt_predicted[previous] += 'N'; // always not taken in predicted;
        return;
    }
    else
        return;
}

void branch_predictor_taken()
{

    if (last.size() == 0)
        return;
    else if (last[0] == 'b')
    {
        if (current - previous == 4)
            bpt_actual[previous] += 'T';
        else
            bpt_actual[previous] += 'N';
        bpt_predicted[previous] += 'T'; // always not taken in predicted;
        return;
    }
    else
        return;
}

void branch_predictor_one_bit(char prev_state)
{
    if (last.size() == 0)
        return;
    else if (last[0] == 'b')
    {
        if (current - previous == 4)
            bpob_actual[previous] += 'T';
        else
            bpob_actual[previous] += 'N';
        bpt_predicted[previous] += prev_state; // always not taken in predicted;
        return;
    }
    else
        return;
}

int main()
{
    // we start for not taken

    ifstream file("input.asm");
    string txt;
    previous = 0;
    current = 0;
    char prev_state='N'; // for one bit initialise as not taken
    last = ""; // initialise as zero size for the first instruction captured
    while (getline(file, txt))
    {
        istringstream iss(txt); // create string stream
        string line;
        iss >> line; // getting the core
        iss >> line; // 0:
        iss >> line; // hex;
        current = hex_to_int(line);
        iss >> line;
        iss >> line; // instruction
        branch_predictor_not_taken();
        branch_predictor_taken();
        previous = current;
        last = line; // store this instruction so we know whether to check for jump or not
    }
    file.close();
    cout << bpnt_actual.size() << " " << bpt_actual.size() << endl;

    return 0;
}
