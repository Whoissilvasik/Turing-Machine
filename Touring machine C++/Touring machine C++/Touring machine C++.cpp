#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>
#include <csignal>

using namespace std;

struct juosta
{
    int poz;
    vector <char> simbolis;
};

struct komanda
{
    string Pr_bus;
    char symbol;
    char new_symbol;
    char direction;
    string Pb_bus;
};

bool keep_going(true);
void Order_stop(int signum);
void Spausdinimas(juosta T);
string Bus_gav(int& i, string& rules);
void Komandos_gav(int& i, string& rules, komanda& cmd);

int main()
{
    signal(SIGINT, Order_stop); // Ctrl + C
    int komandu_kiek = 0;
    string Dab_bus;
    set <string> State;
    char bad_simbol;
    juosta t;
    komanda* cmd;
    bool Ar_pabaiga = true;

    string file_name;
    cout << "INPUT FILE NAME (.txt) - ";
    cin >> file_name;

    ifstream fd(file_name);
    if (!fd.is_open()) {
        cerr << "Could not open the file!" << endl;
        return 1;
    }

    string line;
    fd >> line;
    for (char c : line)
    {
        t.simbolis.push_back(c);
    }
    fd >> t.poz;
    t.poz--;

    string temp;
    string rules;

    while (fd >> temp)
    {
        rules += temp + " ";
        komandu_kiek++;
        for (int i = 1; i < 4 + 2; i++)
        {
            fd >> temp;
            rules += temp + " ";
        }
    }
    fd.close();

    komandu_kiek /= 6;

    cmd = new komanda[komandu_kiek];

    int t_index = 0;
    for (int i = 0; i < rules.size() && t_index < komandu_kiek;)
    {
        cmd[t_index].Pr_bus = Bus_gav(i, rules);
        State.insert(cmd[t_index].Pr_bus);
        Komandos_gav(i, rules, cmd[t_index]);
        t_index++;
    }

    Dab_bus = cmd[0].Pr_bus;

    do {
        bool found = false;
        for (int i = 0; i < komandu_kiek; i++)
        {
            if (Dab_bus == cmd[i].Pr_bus && cmd[i].symbol == t.simbolis[t.poz])
            {
                Dab_bus = cmd[i].Pb_bus;
                t.simbolis[t.poz] = cmd[i].new_symbol;

                if (cmd[i].direction == 'R')
                {
                    t.poz++;
                    if (t.poz == t.simbolis.size())
                    {
                        t.simbolis.push_back(' ');
                    }
                }
                else if (cmd[i].direction == 'L')
                {
                    if (t.poz == 0)
                    {
                        t.simbolis.insert(t.simbolis.begin(), ' ');
                    }
                    else
                    {
                        t.poz--;
                    }
                }

                cout << "J: ";
                Spausdinimas(t);
                cout << endl;

                found = true;
                break;
            }
        }
        if (!found)
        {
            bad_simbol = t.simbolis[t.poz];
            break;
        }
    } while (keep_going && State.find(Dab_bus) != State.end());

    if (State.find(Dab_bus) == State.end())
    {
        Ar_pabaiga = false;
    }

    if (!Ar_pabaiga)
        cout << "No state [" << Dab_bus << "] for this simbol [" << bad_simbol << "]" << endl;
    else
        cout << "Final state [" << Dab_bus << "] done!" << endl << endl;

    cout << "Tape: ";
    Spausdinimas(t);

    delete[] cmd;

    return 0;
}

void Spausdinimas(juosta T)
{
    for (const auto& symbol : T.simbolis)
    {
        cout << symbol;
    }
    cout << endl;
}

string Bus_gav(int& i, string& rules)
{
    string BUSENA;
    while (i < rules.size() && rules[i] != ' ')
    {
        BUSENA += rules[i];
        i++;
    }
    i++;
    return BUSENA;
}

void Komandos_gav(int& i, string& rules, komanda& cmd)
{
    cmd.symbol = rules[i];
    i += 2;
    cmd.new_symbol = rules[i];
    i += 2;
    cmd.direction = rules[i];
    i += 2;
    cmd.Pb_bus = Bus_gav(i, rules);
}

void Order_stop(int signum)
{
    keep_going = false;
}
