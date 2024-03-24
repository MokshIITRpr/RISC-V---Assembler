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

map<ll, char> prev_state;
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

void branch_predictor_one_bit()
{
    if (prev_state.find(previous) == prev_state.end())
        prev_state[previous] = 'N';
    if (last.size() == 0)
        return;
    else if (last[0] == 'b')
    {
        bpob_predicted[previous] += prev_state[previous]; // always not taken in predicted;
        if (current - previous == 4)
        {
            bpob_actual[previous] += 'T';
            prev_state[previous] = 'T'; // new predicted for next set
        }
        else
        {
            bpob_actual[previous] += 'N';
            prev_state[previous] = 'N'; // new predicted for next set
        }
        return;
    }
    else
        return;
}

void branch_predictor_two_bit()
{
}

void display(map<ll, string> &mp1, map<ll, string> &mp2)
{
    for (auto i : mp1)
    {
        cout << "For Instruction with PC : 0x" << hex << i.first << endl;
        cout << "Predicted : ";
        cout << i.second[0] << " ";
        for (ll j = 1; j < (i.second).size(); j++)
            cout << "| " << i.second[j] << " ";
        cout << endl;
        cout << "   Actual : ";
        string temp = mp2[i.first];
        cout << temp[0] << " ";
        for (ll j = 1; j < temp.size(); j++)
            cout << "| " << temp[j] << " ";
        cout << endl;
        ll count = 0;
        for (ll j = 0; j < temp.size(); j++)
        {
            if ((i.second)[j] == temp[j])
                count++;
        }
        ll total = temp.size();
        cout << "The number of correctly predicted jumps are : " << count << endl;
        cout << "The total number of jumps are : " << total << endl;
        double per = ((double)count) / ((double)total);
        cout << "The correctly predicted percentage is : ";
        cout << setprecision(20) << (double)(per * 100.0) << endl;
        cout << "------------------------------------------------------------------------------------------" << endl;
    }
}

int main()
{
    // we start for not taken

    ifstream file("input.asm");
    string txt;
    previous = 0;
    current = 0;
    char prev_state = 'N'; // for one bit initialise as not taken
    last = "";             // initialise as zero size for the first instruction captured
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
        branch_predictor_one_bit();

        previous = current; // going to next instruction so we need to keep pc in case we had a jump
        last = line;        // store this instruction so we know whether to check for jump or not
    }
    file.close();
    display(bpnt_predicted, bpnt_actual);
    return 0;
}
