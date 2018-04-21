#include <string>
#include <iostream>
#include <set>
#include <map>

using namespace std;

map<int, string> post;

int main()
{
    post[3] = "3";
    post[1] = "18";
    post[5] = "0";
    post[2] = "9";

    for (map<int, string>::iterator p = post.begin(); p != post.end(); p++)
    {
        cout << p->first << ":  " << p->second;
        cout << endl;
    }

    return 0;
}