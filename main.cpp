#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
using namespace std;


class Tag 
{
private:
    string mTagName;
    map<string, string> mAttributes;
    vector<Tag> mChildren;
    
public:
    Tag(string tagName);
    void addChild(Tag child);
    void addAttributes(string attributeName, string attributeValue);
    static Tag readHRML();
};

int main() 
{
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    int N, Q;
    cin >> N;
    cin >> Q;

    vector<string> HRML;
    vector<string> queries;
    string line;
    getline(cin, line);
    for (int i = 0; i < N; i++) 
    {
        getline(cin, line);
        HRML.push_back(line);
    }

    for (int i = 0; i < Q; i++) 
    {
        getline(cin, line);
        queries.push_back(line);
    }
    
    return 0;
}
