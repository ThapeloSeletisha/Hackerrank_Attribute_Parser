#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
using namespace std;


class Tag {
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

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    int N, Q;
    cin >> N;
    cin >> Q;
    
    return 0;
}
