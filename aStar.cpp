#include <iostream>
#include <set>
#include <vector>
#include <list>
#include <math.h>
#include <cstdlib>
#include <cmath>

using namespace std;

#define LINE_STEP 1000
#define BIAS_STEP 1400


const int LONGTH = 12;
const int HIGHTH = 8;
int ConfigMap[HIGHTH][LONGTH] =
{
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1},
    { 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1},
    { 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1},
    { 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1},
    { 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

struct Node
{
    int x;
    int y;
    int gScore;
    int hScore;
    Node* pFrom;

    bool isFree;
    int fScore() { return gScore + hScore;}
    void init()
    {
        x = 0;
        y = 0;
        gScore = 0;
        hScore = 0;
        pFrom = NULL;
    }
    Node(){}
};

typedef set<Node*> TSetNodeList;

TSetNodeList openlist;
TSetNodeList closelist;
TSetNodeList m_setAllNode;
Node* m_pEndNode;
bool init()
{
    for (int i = 0; i < HIGHTH; ++i)
    {
        for (int j = 0; j < LONGTH; ++j)
        {
            Node* pnode = new Node();
            if (pnode == NULL)
                return false;
            pnode->init();
            pnode->x = i;
            pnode->y = j;
            pnode->isFree = ConfigMap[i][j] == 0;

            m_setAllNode.insert(pnode);
        }
    }
    return true;
}

Node* getNodePtr(int x, int y)
{
    if (x < 0 || x > HIGHTH || y < 0 || y > LONGTH)
        return NULL;
    set<Node*>::iterator it;
    for (it = m_setAllNode.begin(); it != m_setAllNode.end(); ++it)
    {
        if ((*it)->x == x && (*it)->y == y)
            return (*it);
    }
    return NULL;
}

Node* getMinNode(const TSetNodeList& set)
{
    Node* temp = NULL;
    std::set<Node*>::iterator it;
    int minfScore = 1000000;
    for (it=set.begin(); it != set.end(); ++it)
    {
        if ((*it)->fScore() < minfScore)
        {
            temp = (*it);
            minfScore = (*it)->fScore();
        }

    }
    return temp;
}
bool canReach(int x, int y)
{
    if (x < 0 || x > HIGHTH)
        return false;
    if (y < 0 || y > LONGTH)
        return false;
    return ConfigMap[x][y] == 0;
}

bool canReach(Node* pStart, Node* pEnd, bool ignoreCorner)
{
    if (pStart == NULL || pEnd == NULL)
        return false;
    if(!pEnd->isFree || closelist.find(pEnd) != closelist.end())
        return false;
    if (abs(pStart->x - pEnd->x) + abs(pStart->y - pEnd->y) == 1)
        return true;
    if (canReach(pStart->x, pEnd->y) && canReach(pEnd->x, pStart->y))
        return true;
    return ignoreCorner;
}

void getAroundList(Node* pPoint, TSetNodeList& set, bool ignoreCorner)
{
    if (pPoint == NULL)
        return;
    for(int x = pPoint->x - 1; x <= pPoint->x + 1; ++x)
    {
        for (int y = pPoint->y - 1; y <= pPoint->y + 1; ++y)
        {
            Node* pTemp = getNodePtr(x,y);
            if (canReach(pPoint, pTemp, ignoreCorner))
                set.insert(pTemp);
        }
    }
}

int calcG(Node* pTempStart, Node* pNode)
{
    if (pTempStart == NULL || pNode == NULL)
        return -1;
    int dis = abs(pTempStart->x - pNode->x) + abs(pTempStart->y - pNode->y);
    int tempG = dis == 2 ? BIAS_STEP : LINE_STEP;
    return pTempStart->gScore + tempG;
}
int calcH(Node* pNode, Node* pEnd)
{
    if (pNode == NULL || pEnd == NULL)
        return -1;
    /*
    int dx1 = pNode->x - pEnd->x;
    int dy1 = pNode->y - pEnd->y;
    int dx = start->x - pEnd->x;
    int dy = start->y - pEnd->y;
    int cross = abs(dx1 * dy - dy1 * dx);
    return cross + LINE_STEP * (abs(pNode->x - pEnd->x) + abs(pNode->y - pEnd->y));
    */
    return LINE_STEP * (abs(pNode->x - pEnd->x) + abs(pNode->y - pEnd->y));
}

void exeFindPoint(Node* pTempStart, Node* pNode)
{
    if (pTempStart == NULL || pNode == NULL)
        return;
    int g = calcG(pTempStart, pNode);
    if (g < pNode->gScore)
    {
        pNode->gScore = g;
        pNode->pFrom = pTempStart;
    }
}
void exeNotFindPoint(Node* pTempStart, Node* pNode, Node* pEnd)
{
    if (pTempStart == NULL || pNode == NULL || pEnd == NULL)
        return;
    pNode->pFrom = pTempStart;
    pNode->gScore = calcG(pTempStart, pNode);
    pNode->hScore = calcH(pNode, pEnd);
    openlist.insert(pNode);
}
bool findpath(Node* start, Node* end, bool ignoreCorner)
{
    if (start == NULL || end == NULL)
      return false;
    start->hScore = calcH(start, end);

    openlist.insert(start);
    while(openlist.size() != 0)
    {
        Node* pTempStart = getMinNode(openlist);
        if(pTempStart == NULL)
            return false;

        set<Node*>::iterator it;

        cout<<"-------------"<<endl;
        cout<<pTempStart->x<<","<<pTempStart->y<<endl;
        for (it=openlist.begin(); it!=openlist.end();++it)
        {
            cout<<(*it)->x<<","<<(*it)->y<<
            "  g="<<(*it)->gScore<<"  h="<<(*it)->hScore<<
            "  f="<<(*it)->fScore()<<endl;
        }
        openlist.erase(pTempStart);
        closelist.insert(pTempStart);
        TSetNodeList aroundList;
        getAroundList(pTempStart, aroundList, ignoreCorner);

        for (it=aroundList.begin(); it != aroundList.end(); ++it)
        {
            if(openlist.find(*it) != openlist.end())
            {
                exeFindPoint(pTempStart, *it);
            }
            else
            {
                exeNotFindPoint(pTempStart, *it, end);
            }
        }
        if (openlist.find(end) != openlist.end())
            return true;
    }
    return false;
}




int main()
{
    if (init() == false)
        return 0;
    Node* pStart = getNodePtr(1,1);
    Node* pEnd = getNodePtr(6,10);
    if (pStart == NULL || pEnd == NULL)
        return 0;
    bool ignoreCorner = false;
    if (findpath(pStart, pEnd, ignoreCorner) == false)
    {
        cout<<"can't find a right path"<<endl;
        return 0;
    }
    Node* p = pEnd;
    cout<<endl<<"path is: "<<endl;
    while(p != NULL)
    {
        cout<<p->x<<","<<p->y<<endl;
        p = p->pFrom;
    }
    return 0;
}
