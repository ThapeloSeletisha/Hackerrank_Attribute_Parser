#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
#include <stack>
#include <queue>
#include <string>
using namespace std;

std::string trim(const std::string& str,
                 const std::string& whitespace = " \t")
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

class Tag 
{
private:
    string mTagName;
    map<string, string> mAttributes;
    vector<Tag> mChildren;

    static Tag readTag(string tagLine);
    static bool isOpening(string tagLine);
    static string readTagName(string tagLine);
    static map<string, string> readAttributes(string tagLine);
    
public:
    Tag(string tagName);
    operator string() const;
    void addChild(Tag child);
    void addAttributes(string attributeName, string attributeValue);

    vector<Tag> getChildren();
    string getName();
    string getAttrValue(string attributeName);
    
    static vector<Tag> readHRML(queue<string> hrml);
    static string queryHRML(vector<Tag> hrml, string query);
};

Tag::Tag(string tagName) : mTagName(tagName) {}

Tag::operator string() const
{
    string hrml = "<";
    hrml.append(mTagName);

    for (pair<string, string> attr : mAttributes)
    {
        hrml.push_back(' ');
        hrml.append(attr.first);
        hrml.append(" = ");
        hrml.append(attr.second);
    }

    hrml.append(">\n");
    hrml.append("</");
    hrml.append(mTagName);
    hrml.push_back('>');

    return hrml;
}

string Tag::readTagName(string tagLine)
{
    string tagName = "";

    for (int i = 1; i < tagLine.size(); i++)
    {
        if (tagLine[i] == ' ' || 
            tagLine[i] == '>')
        {
            if (tagName[0] == '/')
            {
                tagName = trim(tagName, "/");
            }
            return tagName;
        }

        tagName.push_back(tagLine[i]);
    }
    return "readTagNameERROR: tag name format issues";
}

vector<Tag> Tag::readHRML(queue<string> hrml)
{
    vector<Tag> tags;
    stack<Tag> openTags;
    while(!hrml.empty())
    {
        string tagLine = hrml.front();
        if (isOpening(tagLine))
        {
            Tag tag = readTag(tagLine);
            openTags.push(tag);
        }
        else 
        {
            Tag closedTag = openTags.top();
            openTags.pop();
            if (openTags.empty())
            {
                tags.push_back(closedTag);
            }
            else
            {
                openTags.top().addChild(closedTag);
            }
        }
        hrml.pop();
    }
    return tags;
}

Tag Tag::readTag(string tagLine)
{
    string tagName = readTagName(tagLine);
    Tag tag = Tag(tagName);

    map<string, string> attributes = readAttributes(tagLine);
    for (pair<string, string> name_value : attributes)
    {
        tag.addAttributes(name_value.first, name_value.second);
    }
    return tag;
}

map<string, string> Tag::readAttributes(string tagLine)
{
    enum State {TAG_NAME, ATTR_NAME, ATTR_VALUE, ATTR_VALUE_START};
    State state = TAG_NAME;
    map<string, string> attributes;
    string attrName = "", attrValue = "";
    for (char c : tagLine)
    {
        
        switch(state)
        {
        case TAG_NAME:
            if (c == ' ')
            {
                state = ATTR_NAME;
            }
            break;

        case ATTR_NAME:
            if (c == '=')
            {
                attrName = trim(attrName);
                state = ATTR_VALUE;
            }
            else if (c == '>')
            {
                break;
            } 
            else 
            {
                attrName.push_back(c);
            }
            break;

        case ATTR_VALUE:
            if (c == '"')
            {
                state = ATTR_VALUE_START;
            }
            break;

        case ATTR_VALUE_START:
            if (c == '"')
            {
                attributes[attrName] = attrValue;
                attrName = "";
                attrValue = "";
                state = ATTR_NAME;
            }
            else 
            {
                attrValue.push_back(c);
            }
            break;
        }
        
    }
    return attributes;
}

string Tag::queryHRML(vector<Tag> hrml, string query)
{
    if (query.find('.') == -1)
    {
        size_t tildePos = query.find('~');
        string tagName = query.substr(0, tildePos);

        for (Tag tag : hrml) 
        {
            if (tag.getName() == tagName)
            {
                size_t n = query.size();
                string attrName = query.substr(tildePos + 1, n - tildePos + 1);   
                return tag.getAttrValue(attrName);
            }
        }
        return "Not Found!";

    }
    else 
    {
        size_t dotPos = query.find('.');
        string tagName = query.substr(0, dotPos);
        for (Tag tag : hrml)
        {
            if (tag.getName() == tagName)
            {
                vector<Tag> children = tag.getChildren();
                size_t n = query.size();
                string subQuery = query.substr(dotPos + 1, n - dotPos + 1);
                return queryHRML(children, subQuery);
            }
            
        }
        return "Not Found!";
    }
    
}

void Tag::addAttributes(string name, string value)
{
    mAttributes[name] = value;
}

bool Tag::isOpening(string tagLine)
{
    return !(tagLine[1] == '/');
}

void Tag::addChild(Tag child)
{
    mChildren.push_back(child);
}

vector<Tag> Tag::getChildren()
{
    return mChildren;
}

string Tag::getName()
{
    return mTagName;
}

string Tag::getAttrValue(string attributeName)
{
    if (mAttributes.find(attributeName) != mAttributes.end())
    {
        return mAttributes[attributeName];
    }
    return "Not Found!";
}

int main() 
{
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    int N, Q;
    cin >> N;
    cin >> Q;

    queue<string> HRML;
    queue<string> queries;
    string line;
    getline(cin, line);
    for (int i = 0; i < N; i++) 
    {
        getline(cin, line);
        HRML.push(line);
    }

    for (int i = 0; i < Q; i++) 
    {
        getline(cin, line);
        queries.push(line);
    }


    vector<Tag> hrml = Tag::readHRML(HRML);
    while (!queries.empty())
    {
        string query = queries.front();
        cout << Tag::queryHRML(hrml, query) << endl;
        queries.pop();
    }

    return 0;
}
