#ifndef ASTAR_H_
#define ASTAR_H_

#include <list>
#include <cmath>
#include <limits>

#include "fixinclude.h"

using namespace std;

class AStar
{
    SearchNode **matrix;

    UINT width;
    UINT height;
    pair<UINT, UINT> start;
    pair<UINT, UINT> end;

    list<pair<UINT, UINT> > path;
    list<pair<UINT, UINT> > openedNodes;
    list<list<NodeState> > matrixsChanges;

    AStarHeuristics heuristic;

    bool ready;

    void calculateCost(pair<UINT, UINT> node);
    void openNode(UINT x, UINT y, UINT originX, UINT originY, list<NodeState> *changes = NULL);
    void freeNodes();

    void search(bool saveChanges = false);

    void setOriginNode(UINT x, UINT y, UINT originX, UINT originY);

    double pathCostToNode(UINT x, UINT y, UINT destX, UINT destY);

public:

    AStar();
    AStar(UINT width, UINT height);
    ~AStar();

    void setMatrix(UINT width, UINT height);
    void setStart(UINT x, UINT y);
    void setEnd(UINT x, UINT y);
    void setWall(UINT x, UINT y);
    void setWay(UINT x, UINT y);
    void setHeuristics(AStarHeuristics heuristic);
    void setExpandCost(UINT expandCost, UINT x, UINT y);

    NodeType getNodeType(UINT x, UINT y) const;
    pair<UINT, UINT> getStart() const;
    pair<UINT, UINT> getEnd() const;
    list<list<NodeState> > getChanges() const;

    double estimateCost(UINT x1, UINT y1, UINT x2, UINT y2);

    void destroyMatrix();

    list<pair<UINT, UINT> > getPath(bool saveChanges = false);
};

#endif
