#include <cstdio>
#include <iostream>
#include <string>
using namespace std;

static string buffer;

int main(int argc, char **argv)
{
    cout << "Very first beginning prompt for Lispy" << endl;

    while (1)
    {
        cout << "> ";
        cin >> buffer;
        cout << "You are not " << buffer << endl;
    }
}