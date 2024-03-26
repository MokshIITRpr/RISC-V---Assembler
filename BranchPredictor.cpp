#include <bits/stdc++.h>
#include <fstream>
using namespace std;
#define ll long long
#define err 1e18
ofstream op("bp_output.mc");

map<ll, string> bpnt_actual;
map<ll, string> bpnt_predicted;
map<ll, string> bpt_actual;
map<ll, string> bpt_predicted;
map<ll, string> bpob_actual;
map<ll, string> bpob_predicted;
map<ll, string> bptb_actual;
map<ll, string> bptb_predicted;
map<ll, vector<ll>> btb;
vector<double> final;

map<ll, char> prev_state;
map<ll, string> dyn_state;
double keep = 0;
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
            bpnt_actual[previous] += 'N';
        else
            bpnt_actual[previous] += 'T';
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
            bpt_actual[previous] += 'N';
        else
            bpt_actual[previous] += 'T';
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
            bpob_actual[previous] += 'N';
            prev_state[previous] = 'N'; // new predicted for next set
        }
        else
        {
            bpob_actual[previous] += 'T';
            prev_state[previous] = 'T'; // new predicted for next set
        }
        return;
    }
    else
        return;
}

void branch_predictor_two_bit()
{
    if (dyn_state.find(previous) == dyn_state.end())
        dyn_state[previous] = "NN";

    // we initialise as strongly not taken

    if (last.size() == 0)
        return;
    else if (last[0] == 'b')
    {
        bptb_predicted[previous] += dyn_state[previous][0]; // always not taken in predicted;
        if (current - previous == 4)
        {
            bptb_actual[previous] += 'N';
            if (dyn_state[previous] == "NN")
                dyn_state[previous] = "NN";
            else if (dyn_state[previous] == "NT")
                dyn_state[previous] = "NN";
            else if (dyn_state[previous] == "TN")
                dyn_state[previous] = "NT";
            else if (dyn_state[previous] == "TT")
                dyn_state[previous] = "TN";
        }
        else
        {
            bptb_actual[previous] += 'T';
            if (dyn_state[previous] == "NN")
                dyn_state[previous] = "NT";
            else if (dyn_state[previous] == "NT")
                dyn_state[previous] = "TN";
            else if (dyn_state[previous] == "TN")
                dyn_state[previous] = "TT";
            else if (dyn_state[previous] == "TT")
                dyn_state[previous] = "TT";
        }
        return;
    }
    else
        return;
}

void display(map<ll, string> &mp1, map<ll, string> &mp2)
{
    double count = 0;
    double total = 0;
    for (auto i : mp1)
    {
        op << "For Instruction with PC : 0x" << hex << i.first << endl
           << endl;
        op << "Predicted : ";
        op << i.second[0] << " ";
        for (ll j = 1; j < (i.second).size(); j++)
            op << "| " << i.second[j] << " ";
        op << endl;
        op << "   Actual : ";
        string temp = mp2[i.first];
        op << temp[0] << " ";
        for (ll j = 1; j < temp.size(); j++)
            op << "| " << temp[j] << " ";
        op << endl;
        for (ll j = 0; j < temp.size(); j++)
        {
            if ((i.second)[j] == temp[j])
                count++;
        }
        op << "Hit / Miss : ";
        (i.second)[0] == temp[0] ? op << 'H' : op << 'M';
        op << " ";
        for (ll j = 1; j < temp.size(); j++)
        {
            op << "| ";
            (i.second)[j] == temp[j] ? op << 'H' : op << 'M';
            op << " ";
        }
        total += temp.size();
        op << "------------------------------------------------------------------------------------------" << endl
           << endl;
    }

    op << "The number of hits are : " << setprecision(20) << (count) << endl;
    op << "The number of miss are : " << setprecision(20) << (total - count) << endl;
    op << "The total number of jumps are : " << setprecision(20) << (total) << endl;
    double per = ((double)count) / ((double)total);
    op << "The accuracy is : ";
    op << setprecision(20) << (double)(per * 100.0) << endl
       << endl;
    op << endl;
    op << endl;
    op << endl;
    final.push_back(per * 100.0);
}

void branch_target_buffer(string txt)
{
    istringstream iss(txt); // create string stream
    string line;
    iss >> line; // getting the core
    iss >> line; // 0:
    iss >> line; // hex;
    string store = line;
    iss >> line;
    iss >> line; //
    if (line[0] != 'b')
        return;
    op << endl;
    op << setprecision(20) << (keep) << "     "
       << "Instruction : " << store << endl;
    op << "       "
       << "PC : "
       << "0x" << hex << current << endl;
    while (line[0] != 'p' && !(line[0] <= '9' && line[0] >= '0') && line[0] != '-')
        iss >> line;
    string temp;
    if (line[0] == 'p')
    {
        iss >> line;
        temp = line; // store the sign
        iss >> line;
    }
    else if (line[0] == '-')
    {
        temp = '-';
        string t;
        for (ll i = 1; i < line.size(); i++)
            t += line[i];
        line = t;
    }
    // else we already have the integer
    ll get = 0;
    op << "       "
       << "Target : ";
    if (line.size() == 1 && line[0] == '0')
        get = 0;
    else if (line[0] != '0')
    {
        for (ll i = 0; i < line.size(); i++)
            get = (get * 10) + (ll)(line[i] - '0');
    }
    else
    {
        for (ll i = 2; i < line.size(); i++)
        {
            if (line[i] <= '9' && line[i] >= '0')
                get = (get * 16) + (ll)(line[i] - '0');
            else
                get = (get * 16) + (ll)(line[i] - 'A') + 10ll;
        }
    }
    if (temp == "-")
        op << "0x" << hex << current - get << endl;
    else
        op << "0x" << hex << current + get << endl;
    keep += 1;
    op << "================================================================" << endl;
}

int main()
{
    // we start for not taken

    ifstream file("bp_input.asm");

    string txt;
    previous = 0;
    current = 0;
    char prev_state = 'N'; // for one bit initialise as not taken
    last = "";             // initialise as zero size for the first instruction captured
    op << "Branch target buffer : " << endl
       << endl
       << endl;
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
        branch_predictor_two_bit();
        branch_target_buffer(txt);
        previous = current; // going to next instruction so we need to keep pc in case we had a jump
        last = line;        // store this instruction so we know whether to check for jump or not
    }
    op << endl
       << endl
       << endl;
    op << "Not Taken Branch Predictor" << endl
       << endl;
    display(bpnt_predicted, bpnt_actual);
    op << "Taken Branch Predictor" << endl
       << endl;
    display(bpt_predicted, bpt_actual);
    op << "One Bit Dynamic Branch Predictor" << endl
       << endl;
    display(bpob_predicted, bpob_actual);
    op << "Two Bit Dyanmic Branch Predictor" << endl
       << endl;
    display(bptb_predicted, bptb_actual);
    op << "The final outcome : " << endl;
    op << "Not Taken Branch Predictor Accuracy : " << final[0] << endl;
    op << "Taken Branch Predictor Accuracy : " << final[1] << endl;
    op << "One Bit Branch Predictor Accuracy : " << final[2] << endl;
    op << "Two Bit Branch Predictor Accuracy : " << final[3] << endl;
    file.close();
    op.close();
    return 0;
}
