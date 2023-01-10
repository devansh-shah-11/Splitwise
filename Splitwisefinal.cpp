#include<bits/stdc++.h>
using namespace std;
#include <iostream>
#include <fstream>

map<pair<string,string>,float> Transactions;
map<pair<string,string>,float> Simplified_Transactions;
map<string,float> Nodes;

void printmap1() //to print Transactions Map
{
    for (auto i: Transactions)
    {
        cout << i.first.first << "->" << i.first.second <<"\t" << i.second << endl;
    }
}

void printmap2() // to print Nodes
{
    for (auto i: Nodes)
    {
        cout << i.first << "->" << i.second << endl;
    }
}

void readfromfile() // error in this part
{
    string myText;
    ifstream f("Balance.csv");
    while (getline (f, myText))
    {
        cout << myText << endl;
        vector<string> v;
        stringstream ss(myText);
        while (ss.good()) {
            string substr;
            getline(ss, substr, ',');
            v.push_back(substr);
        }
        /*for (size_t i = 0; i < v.size()x; i++)
        {
            cout << v[i] << endl;
        }*/
        string a,b;
        float weight;
        a=v[0];
        b=v[1];
        //cout << a << " " << b << endl;
        weight=std::stof(v[2]);
        Transactions[{a,b}]=weight;
    }
    printmap1();
    f.close();
}

void writetofile(map<pair<string,string>,float> TEMP,string fname)
{
    remove("Balance.csv");
    remove("Masterbalance.csv");
    fstream fout;
    fout.open(fname, ios::out | ios::app);
    for (auto i: TEMP)
    {
        fout << i.first.first << "," << i.first.second <<"," << i.second << endl;
    }
}

bool compare(const pair<string, int>&a, const pair<string, int>&b)
{
    return a.second<b.second;
}

//ordered map - everything O(logn) (max_element mate O(nlogn))
//unordered map - everything O(1) except Deletion - O(n)
void settleup()
{
    //printmap2();
    while (Nodes.size()>=1)
    {
        //printmap2(Node);
        auto maxnode = max_element(Nodes.begin(), Nodes.end(), compare);
        auto minnode = min_element(Nodes.begin(), Nodes.end(), compare);
        //cout << maxnode->first << " " << maxnode->second << " " <<  minnode->first << " " << minnode->second << endl;
        if (maxnode->first == minnode->first)
        {
            cout << maxnode->first << " doesnt need to pay anything, already settled up!" << endl; // this line getting repeated twice!
            Nodes.erase(maxnode->first);
        }
        else
        {
            printmap2();
            if (maxnode->second == -(minnode->second))
            {
                cout << minnode->first << " pays Rs " << maxnode->second << " to " << maxnode->first << endl;
                Simplified_Transactions[{minnode->first,maxnode->first}]=maxnode->second;
                Nodes.erase(maxnode->first);
                Nodes.erase(minnode->first);
            }
            else
            {
                if (maxnode->second > -(minnode->second))
                {
                    cout << minnode->first << " pays Rs " << -(minnode->second) << " to " << maxnode->first << endl;
                    Simplified_Transactions[{minnode->first,maxnode->first}]=-(maxnode->second);
                    Nodes.erase(minnode->first);
                    maxnode->second+=minnode->second;
                }
                else
                {
                    cout << minnode->first << " pays Rs " << maxnode->second << " to " << maxnode->first << endl;
                    Simplified_Transactions[{minnode->first,maxnode->first}]=maxnode->second;
                    Nodes.erase(maxnode->first);
                    minnode->second+=maxnode->second;
                }
            }
        }
    }
}

void node() //fxn to calculate sum of indegree and outdegree
{
    for (auto i: Transactions)
    {
        if(Nodes.find(i.first.first)==Nodes.end())
        {
            Nodes[i.first.first]=0-i.second;
        }
        else
        {
            Nodes[i.first.first]-=i.second;
        }
        if(Nodes.find(i.first.second)==Nodes.end())
        {
            Nodes[i.first.second]=i.second;
        }
        else
        {
            Nodes[i.first.second]+=i.second;
        }
    }
    printmap2();
    settleup();
    writetofile(Simplified_Transactions,"Balance.csv");
}

void add_expense(string u, string v, float weight)
{
    if (Transactions.find({u,v})==Transactions.end())
    {
        Transactions[{u,v}]=weight;
    }
    else
    {
        Transactions[{u,v}]+=weight;
    }
    printmap1();
}

int main()
{
    map<pair<string,string>,float> MasterBalance;
    int choice;
    readfromfile();
    while (true)
    {
        cout << "\nEnter 1 to Add Expense \nEnter 2 to view simplified payments \nEnter 3 to view all expenses till now \nEnter 4 to settle up \nEnter 5 to exit \n";
        cin >> choice;
        if (choice==1)
        {
            string payer,reason;
            float amount;
            cout << "\nEnter Name, Category and Amount of Expense \n";
            cin >> payer >> reason >> amount;
            int tmp,total;
            MasterBalance[{payer,reason}]=amount;
            cout << "\nEnter the total members involved in expense: \n";
            cin >> total;
            cout << "Enter 1 to divide equally \nEnter 0 to divide unequally \n";
            cin >> tmp;
            for (int i=0;i<total-1;i++)
            {
                cout << "\nEnter name of payee: \n";
                string payee;
                cin >> payee;
                if (tmp==1)
                {
                    add_expense(payee,payer,(amount/total));
                    //file ma pan store karavu - bandh karu program pachi chalu karie tyare pachu aai javu joie badhu masterbalance ne store karavanu
                }
                else
                {
                    cout << "\nEnter % to be paid by: " << payee << endl;
                    float unequal_percent;
                    cin >> unequal_percent;
                    add_expense(payee,payer,((unequal_percent*amount)/100));
                }
            }
        }
        else if (choice==2)
        {
            //final settling up
            node();
        }
        else if (choice==3)
        {
            for (auto j: MasterBalance)
            {
                cout << j.first.first << " " << j.first.second << endl;
                writetofile(MasterBalance,"Masterbalance.csv");
            }
        }
        else if (choice==4)
        {
            remove("Masterbalance.csv");
            remove("Balance.csv");
            cout << "\nAll pending payment successfully settled - No dues remaining!!\n";
        }
        else
        {
            cout << "\nExiting\n";
            break;
        }
    }
}