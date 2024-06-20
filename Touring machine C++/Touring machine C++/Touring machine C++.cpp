#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>
#include <csignal>

using namespace std;

struct juostos
{
    int poz;
    vector <char> simbolis;
};

struct komandos
{
    string Pr_bus;
    char* symbol;
    char* new_symbol;
    char* direction;
    string Pb_bus;
};

bool keep_going(true);
void Order_stop(int signum);
void Spausdinimas(int ind, juostos T[]);
string Bus_gav(int& i, string& rules);
void Komandos_gav(int n, int& i, string& rules, char X[]);

int main()
{
    signal(SIGINT, Order_stop); // Ctrl + C
    int Gal_kiek;
    int komandu_kiek = 0;
    int found = 0;
    string Dab_bus;
    set <string> State;
    char bad_simbol;
    juostos* juosta;
    komandos* cmd;
    bool Ar_pabaiga = true;

    string file_name;
    cout << "INPUT FILE NAME (.txt) - ";
    cin >> file_name;

    ifstream fd(file_name);
    if (!fd.is_open()) {
        cerr << "Could not open the file!" << endl;
        return 1;
    }

    fd >> Gal_kiek;
    juosta = new juostos[Gal_kiek];

    string line;

    for (int i = 0; i < Gal_kiek; i++)
    {
        fd >> line;
        for (int j = 0; j < line.size(); j++)
        {
            juosta[i].simbolis.push_back(line[j]);
        }
    }
    for (int i = 0; i < Gal_kiek; i++)
    {
        fd >> juosta[i].poz;
        juosta[i].poz--;
    }

    string temp;
    string rules;

    while (fd >> temp)
    {
        rules += temp + " ";
        komandu_kiek++;
        for (int i = 1; i < 3 * Gal_kiek + 2; i++)
        {
            fd >> temp;
            rules += temp + " ";
        }
    }
    fd.close();

    komandu_kiek /= (3 * Gal_kiek + 2);

    cmd = new komandos[komandu_kiek];

    for (int i = 0; i < komandu_kiek; i++)
    {
        cmd[i].symbol = new char[Gal_kiek];
        cmd[i].new_symbol = new char[Gal_kiek];
        cmd[i].direction = new char[Gal_kiek];
    }

    int t = 0;
    for (int i = 0; i < rules.size() && t < komandu_kiek;)
    {
        cmd[t].Pr_bus = Bus_gav(i, rules);
        State.insert(cmd[t].Pr_bus);
        Komandos_gav(Gal_kiek, i, rules, cmd[t].symbol);
        Komandos_gav(Gal_kiek, i, rules, cmd[t].new_symbol);
        Komandos_gav(Gal_kiek, i, rules, cmd[t].direction);
        cmd[t].Pb_bus = Bus_gav(i, rules);
        t++;
    }

    Dab_bus = cmd[0].Pr_bus;

    do {
        found = 0;
        for (int i = 0; i < komandu_kiek; i++)
        {
            if (Dab_bus == cmd[i].Pr_bus)
            {
                bool match = true;
                for (int j = 0; j < Gal_kiek; j++)
                {
                    if (cmd[i].symbol[j] != juosta[j].simbolis[juosta[j].poz])
                    {
                        match = false;
                        bad_simbol = juosta[j].simbolis[juosta[j].poz];
                        break;
                    }
                }

                if (match)
                {
                    Dab_bus = cmd[i].Pb_bus;
                    for (int j = 0; j < Gal_kiek; j++)
                    {
                        juosta[j].simbolis[juosta[j].poz] = cmd[i].new_symbol[j];

                        if (cmd[i].direction[j] == 'R')
                        {
                            juosta[j].poz++;
                            if (juosta[j].poz == juosta[j].simbolis.size())
                            {
                                juosta[j].simbolis.push_back(' ');
                            }
                        }
                        else if (cmd[i].direction[j] == 'L')
                        {
                            if (juosta[j].poz == 0)
                            {
                                juosta[j].simbolis.insert(juosta[j].simbolis.begin(), ' ');
                            }
                            else
                            {
                                juosta[j].poz--;
                            }
                        }

                        cout << "J" << j + 1 << ":";
                        Spausdinimas(j, juosta);
                    }
                    cout << endl;
                    found = 1;
                    break;
                }
            }
        }
    } while (found);

    if (State.find(Dab_bus) != State.end())
    {
        Ar_pabaiga = false;
    }

    if (!Ar_pabaiga)
        cout << "Nera busenos [" << Dab_bus << "] siam simboliui [" << bad_simbol << "]" << endl;
    else
        cout << "Pasekta paskutine busena [" << Dab_bus << "] done!" << endl << endl;

    for (int i = 0; i < Gal_kiek; i++)
    {
        cout << "Juosta " << i + 1 << ": ";
        Spausdinimas(i, juosta);
    }

    for (int i = 0; i < komandu_kiek; i++)
    {
        delete[] cmd[i].symbol;
        delete[] cmd[i].new_symbol;
        delete[] cmd[i].direction;
    }
    delete[] cmd;
    delete[] juosta;

    return 0;
}

void Spausdinimas(int i, juostos juost[])
{
    for (const auto& symbol : juost[i].simbolis)
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

void Komandos_gav(int Gal_kiek, int& i, string& rules, char cmd[])
{
    for (int l = 0; l < Gal_kiek; l++)
    {
        cmd[l] = rules[i];
        i += 2;
    }
}

void Order_stop(int signum)
{
    keep_going = false;
}
