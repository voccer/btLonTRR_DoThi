#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <queue>
#include <stack>

using namespace std;

set<string> vertices;              // chua tap dinh cua do thi
map<string, set<string> > adjacent; // lien ket dinh va cac canh ke

map<string, string> parent;
map<string, char> color;

//Khoi tao tat ca la white
void init()
{
    for (set<string>::iterator p = vertices.begin(); p != vertices.end(); p++)
    {
        string v = *p;
        color[v] = 'W';
        parent[v] = "";
    }
}

bool checkWord(string a, string b)
{
    int check = 0;
    int l = a.length();

    for (int i = 0; i < l; i++)
    {
        if (a[i] != b[i])
            check++;
    }

    if (check == 1)
        return true;
    else
        return false;
}
void readFile(char *url)
{
    fstream f;
    f.open(url);

    while (true)
    {
        string s;
        getline(f, s);
        if (f.eof())
            break;

        vertices.insert(s);
    }

    f.close();
}

void getAdjacent()
{
    for (set<string>::iterator p = vertices.begin(); p != vertices.end(); p++)
    {
        for (set<string>::iterator q = vertices.begin(); q != vertices.end(); q++)
        {

            if (checkWord(*p, *q) == true)
            {
                adjacent[(*p)].insert(*q);
            }
        }
    }
}

void bfsVer(string v)
{
    queue<string> Q;
    Q.push(v);
    color[v] = 'G';
    while (Q.empty() == false)
    {
        string u = Q.front();
        Q.pop();
        for (set<string>::iterator p = adjacent[u].begin(); p != adjacent[u].end(); p++)
        {
            string x = *p;
            if (color[x] == 'W')
            {
                color[x] = 'G';
                Q.push(x);
            }
        }
    }
}

int result = 0;
void bfs()
{
    init();
    for (set<string>::iterator p = vertices.begin(); p != vertices.end(); p++)
    {
        string v = *p;
        if (color[v] == 'W')
        {
            bfsVer(v);
            result++;
        }
    }
}

void findPath(string u, string v)
{
    init();

    stack<string> path;

    queue<string> Q;

    Q.push(u);
    color[u] = 'G';

    while (Q.empty() == false)
    {
        string x = Q.front();
        Q.pop();

        for (set<string>::iterator p = adjacent[x].begin(); p != adjacent[x].end(); p++)
        {
            string t = *p;
            if (color[t] == 'W')
            {
                Q.push(t);
                color[t] = 'G';
                parent[t] = x;
                if (t == v)
                {
                    string i = t;
                    while (true)
                    {
                        path.push(i);
                        i = parent[i];
                        if (i == u)
                        {
                            path.push(i);

                            cout << "\nPath to " << u << "to " << v << " is: ";
                            while (!path.empty())
                            {
                                cout << "->" << path.top();
                                path.pop();
                            }
                            cout << endl;

                            return;
                        }
                    }
                }
            }
        }
    }
    cout << "No Path" << endl;
}

/*=============================Bai 2 =============================*/

map<string, set<string> > adjacentVector; // Lưu trữ tập cạnh kề thoả mãn điều kiện được giao

map<string, set<string> > adjacentVectorReverse; //Tập cạnh đảo ngược ban đầu

map<string, int> pre;     // lưu trữ thời gian pre, là thời điểm vừa được thăm
map<string, int> post;    // lưu trữ thời gian post, là thời điểm được duyệt xong
map<string, char> color2; // Lưu trữ trạng thái thăm của 1 đỉnh

map<int, string> reversePost; // Lưu trữ post sau khi chạy dfs trên reverse Graph, lưu int trước nhằm mục đích lợi dụng tính tự sort của map

int t;                        // Biến t đếm thời gian chạy

//Kiểm tra điều kiện tạo 1 cung từ a tới b
bool checkWordVector(string a, string b)
{
    int check = 0;

    int l = a.length();
    for (int i = 1; i < l; i++)
    {
        for (int j = 0; j < l; j++)
        {
            if (a[i] == b[j])
            {
                check++;
                b[j] = '@'; // tạo kí tự đặc biệt coi như loại đối tượng b[j] ra khoỉ cuộc chơi
                break;
            }
        }
    }

    if (check == 4)
        return true;
    else
        return false;
}

//Tạo adjacentVector
//int counter = 0; // đếm số cung

void getAdjacentVector()
{
    for (set<string>::iterator p = vertices.begin(); p != vertices.end(); p++)
    {
        for (set<string>::iterator q = vertices.begin(); q != vertices.end(); q++)
        {
            if (checkWordVector(*p, *q))
            {
                adjacentVector[(*p)].insert(*q);
                //counter ++;
            }
        }
    }
}

/*
vậy là đã có tập đỉnh và tập cạnh kề
giờ tiếp tục là đếm tất cả thành phân liên thông mạnh

*/

//Khởi tạo điều kiện cho bài 2

void getAdjacentVectorReverse()
{
    for (set<string>::iterator p = vertices.begin(); p != vertices.end(); p++)
    {
        for (set<string>::iterator q = vertices.begin(); q != vertices.end(); q++)
        {
            if (checkWordVector(*p, *q))
            {
                adjacentVectorReverse[*q].insert(*p);
            }
        }
    }
}

void init2()
{
    t = 0;
    for (set<string>::iterator q = vertices.begin(); q != vertices.end(); q++)
    {
        string v = *q;
        color2[v] = 'W';
        post[v] = 0;
        pre[v] = 0;
    }
}

//Quá trình thăm 1 đỉnh đang có trạng thái là 'W'
void dfsVisit(string v, map<string, set<string> > &adj)
{
    t++;
    pre[v] = t;
    color2[v] = 'G';

    //Duyệt tất cả các đỉnh kề với u
    for (set<string>::iterator q = adj[v].begin(); q != adj[v].end(); q++)
    {
        string u = *q;
        if (color2[u] == 'W')
        {
            color2[u] = 'G';
            dfsVisit(u, adj);
        }
    }
    t++;
    post[v] = t;
    color2[v] = 'B';
}

//int result2 = 0;
//dfs cho 1 adj nào đó
void dfs(map<string, set<string> > &adj)
{
    init2();

    for (set<string>::iterator q = vertices.begin(); q != vertices.end(); q++)
    {
        string v = *q;
        if (color2[v] == 'W')
        {
            dfsVisit(v, adj);
           // result2++; // Đếm số thành phần liên thông
        }
    }
}
/*
string findMaxPost()
{
    int max = 0;
    string rs = "";

    for (map<string, int>::iterator p = post.begin(); p != post.end(); p++)
    {
        if (max < p->second)
        {
            max = p->second;
            rs = p->first;
        }
    }
    return rs;
}
*/
//Lưu lại post khi chạy reverse Graph
void backupPost()
{
    for (map<string, int>::iterator p = post.begin(); p != post.end(); p++)
    {
        reversePost[p->second] = p->first;
    }
}


int countStrongInterconnect()
{
    int result = 0;
    // dfs lần 1 trên đồ thị ngược
    dfs(adjacentVectorReverse);

    //Trước khi trả về giá trị để chạy lần 2, ta cần back up lại chúng
    backupPost();

    // Trả về giá trị ban đầu của pre post color t cho các đỉnh;
    init2();

    //chaỵ dfs lần 2 với thứ tự đỉnh thay đổi
    // thứ tự vào lần này là thứ tự đỉnh đã được sắp xếp sau khi đã dfs lần 1
    for(map<int, string>::iterator p = reversePost.begin(); p != reversePost.end(); p++)
    {
        string v = p->second;
        if (color2[v] == 'W')
        {
            dfsVisit(v, adjacentVector);
            result ++;
        }
    }

    return result;
}

void dfsVisit2(string v, map<string, set<string> > adjacentVector)
{
    stack<string> branch;
    queue<string> Q;
    
    color2[v] = 'G';


    //Duyệt tất cả các đỉnh kề với u
    for (set<string>::iterator q = adjacentVector[v].begin(); q != adjacentVector[v].end(); q++)
    {
        string u = *q;
        if (color2[u] == 'W')
        {
            color2[u] = 'G';
            dfsVisit(u, adjacentVector);
        }
    }
    color2[v] = 'B';
}

void freeStack(stack<string> path)
{

}
void strongInterConnectBranch(string u)
{
    init2();
    stack<string> path;
    queue<string> Q;

    for(map<int, string>::iterator p = reversePost.begin(); p != reversePost.end(); p++)
    {
        string v = p->second;
        if (color2[v] == 'W')
        {
            if (v == u)
            {
                path.push(v);
            }
            Q.push(v);
            color2[v] = 'G';

            while (Q.empty() == false)
            {
                string t = Q.front();
                Q.pop();
                for (set<string>::iterator p = adjacentVector[t].begin(); p != adjacentVector[t].end(); p++)
                {
                    string x = *p;
                    if (color[x] == 'W')
                    {
                        color[x] = 'G';
                        Q.push(x);
                        path.push(x);
                    }
                }
            }
        }
    }

}

int main()
{
    char url[100];
    strcpy(url, "file.txt");
    readFile(url);
    /*    
    getAdjacent();
    bfs();
    cout << "Number of inter connect is : " <<result << endl;

    string u, v;


    cout << "input vertices u:  ";
    cin >> u;
    cout << "input vertices v:  ";
    cin >> v;

    findPath(u, v);
*/
    /*============================bai 2 ================================*/

    /*
    for (set<string>::iterator p = vertices.begin(); p != vertices.end(); p++)
    {
        cout << *p << ": ";
        for (set<string>::iterator q = adjacentVector[(*p)].begin(); q != adjacentVector[(*p)].end(); q++)
        {
            cout << *q << " ";
        }
        cout << endl;
    }
*/
    getAdjacentVector();
    getAdjacentVectorReverse();
    /*
    for (set<string>::iterator p = vertices.begin(); p != vertices.end(); p++)
    {
        cout << *p << ": ";
        for (set<string>::iterator q = adjacentVector[(*p)].begin(); q != adjacentVector[(*p)].end(); q++)
        {
            cout << *q << " ";
        }
        cout << endl;
    }
    for (int i = 0; i < 100; i++)
    {
        cout <<  endl << endl;
    }
        

    for (set<string>::iterator p = vertices.begin(); p != vertices.end(); p++)
    {
        cout << *p << ": ";
        for (set<string>::iterator q = adjacentVectorReverse[(*p)].begin(); q != adjacentVectorReverse[(*p)].end(); q++)
        {
            cout << *q << " ";
        }
        cout << endl;
    }
*/

    cout << "\nNumber of strong interconnect is:  " << countStrongInterconnect() << endl;
    
    string u;
    cout << "\ninput u: ";
    strongInterConnectBranch(u);

    return 0;
}
