#ifndef FIXINCLUDE_H
#define FIXINCLUDE_H

#include <QGraphicsPixmapItem>
#include <QDir>
#include <list>

using namespace std;

#ifndef NULL
#define NULL 0
#endif

#ifndef UINT
#define UINT unsigned int
#endif

enum ToSetState
{
    TOSETWALL,
    TOSETWAY,
    TOSETDIRT,
    TOSETSTART,
    TOSETEND
};

enum NodeType
{
    NONE = 0,
    WAYNODE,
    WALLNODE,
    OPENEDNODE,
    VISITEDNODE
};

enum AStarHeuristics
{
    none,
    euclidean,
    manhattan,
    diagonal,
    newH
};

enum AncestorDirection
{
    SELF,
    UPLEFT,
    UP,
    UPRIGHT,
    LEFT,
    RIGHT,
    DOWNLEFT,
    DOWN,
    DOWNRIGHT
};

class SearchNode
{

public:

    NodeType type;

    UINT originX;
    UINT originY;
    double h;
    UINT g;
    UINT expandCost;
    double cost;
    double pathCost;

    SearchNode();
    ~SearchNode();
};

class PathNode
{

public:

    UINT x;
    UINT y;

    PathNode();
    ~PathNode();
};

class NodeState
{

public:

    UINT x;
    UINT y;

    NodeType state;

    AncestorDirection ancestor;

    NodeState();
    NodeState(UINT x, UINT y, NodeType state);
    ~NodeState();
};

class MapNode
{

public:
    NodeType type;

    UINT expandCost;

    QGraphicsPixmapItem pixmapItem;
};

struct RunResources
{
    class MapNode **map;
    list<pair<UINT, UINT> > *path;
    list<list<NodeState> > *steps;
    pair<UINT, UINT> start;
    pair<UINT, UINT> end;
};

#endif // FIXINCLUDE_H
