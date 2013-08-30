#include "astar.h"

SearchNode::SearchNode()
{
    type = WAYNODE;
    originX = 0;
    originY = 0;
    h = numeric_limits<double>::max();
    g = numeric_limits<UINT>::max();
    expandCost = 1;
    cost = numeric_limits<double>::max();
    pathCost = numeric_limits<double>::max();
}

SearchNode::~SearchNode()
{

}

PathNode::PathNode()
{
    x = 0;
    y = 0;
}

PathNode::~PathNode()
{

}

NodeState::NodeState()
{
    x = 0;
    y = 0;

    state = NONE;
}

NodeState::NodeState(UINT x, UINT y, NodeType state)
{
    this->x = x;
    this->y = y;
    this->state = state;
    this->ancestor = SELF;
}

NodeState::~NodeState()
{

}

AStar::AStar()
{
    matrix = NULL;
    width = 0;
    height = 0;
    heuristic = euclidean;
    start.first = 0;
    start.second = 0;
    end.first = 0;
    end.second = 0;
    path.clear();
    openedNodes.clear();
    matrixsChanges.clear();
    ready = false;
}

AStar::AStar(UINT width, UINT height)
{
    matrix = new SearchNode*[height];

    for (UINT i = 0; i < height; i++)
        matrix[i] = new SearchNode[width];

    this->width = width;
    this->height = height;
    start.first = 0;
    start.second = 0;
    end.first = 0;
    end.second = 0;
    path.clear();
    openedNodes.clear();
    matrixsChanges.clear();
    ready = true;
}

AStar::~AStar()
{
    destroyMatrix();

    path.clear();
    openedNodes.clear();
}

void AStar::calculateCost(pair<UINT, UINT> node)
{
    double x, y, destX, destY, h, cost, euc, pathCost;
    UINT g, originX, originY;

    x = (double)node.first;
    y = (double)node.second;
    destX = (double)end.first;
    destY = (double)end.second;

    originX = matrix[node.first][node.second].originX;
    originY = matrix[node.first][node.second].originY;

    g = matrix[originX][originY].g + matrix[node.first][node.second].expandCost;

    euc = sqrt(pow(x - destX, 2.0) + pow(y - destY, 2.0));

    if (heuristic == none)					h = 0;
    else if (heuristic == euclidean)		h = euc;
    else if (heuristic == manhattan)		h = abs(x - destX) + abs(y - destY);
    else if (heuristic == diagonal)			h = max(abs(x - destX), abs(y - destY));
    else if (heuristic == newH)				h = ceil(euc) - euc;

    cost = h + (double)g;

    pathCost = matrix[originX][originY].pathCost + cost;

    matrix[node.first][node.second].h = h;
    matrix[node.first][node.second].g = g;
    matrix[node.first][node.second].cost = cost;
    matrix[node.first][node.second].pathCost = pathCost;
}

void AStar::openNode(UINT x, UINT y, UINT originX, UINT originY, list<NodeState> *changes)
{
    NodeState nodeState;
    double orignalPathCost, newPathCost;
    UINT predX, predY;

    if (x >= width       || y >= height)		return;
    if (originX >= width || originY >= height)	return;

    if (matrix[x][y].type == WALLNODE || matrix[x][y].type == VISITEDNODE) return;
    else if (matrix[x][y].type == WAYNODE)
    {
        matrix[x][y].type = OPENEDNODE;
        openedNodes.push_back(pair<UINT, UINT>(x, y));
    }
    else if (matrix[x][y].cost < estimateCost(originX, originY, x, y))
    {
        return;
    }
    else if (matrix[x][y].expandCost != 1)
    {
        orignalPathCost = pathCostToNode(matrix[x][y].originX, matrix[x][y].originY, x, y);
        newPathCost = pathCostToNode(originX, originY, x, y);

        predX = matrix[x][y].originX;
        predY = matrix[x][y].originY;

        //orignalPathCost = matrix[predX][predY].pathCost;
        //newPathCost = matrix[originX][originY].pathCost;

        if (orignalPathCost < newPathCost) return;
    }


    setOriginNode(x, y, originX, originY);
    calculateCost(pair<UINT, UINT>(x, y));

    if (changes != NULL)
    {
        nodeState.x = x;
        nodeState.y = y;
        nodeState.state = OPENEDNODE;

        originX -= x;
        originY -= y;

        if (originX == -1)
        {
            if (originY == -1) nodeState.ancestor = UPLEFT;
            else if (originY == 0) nodeState.ancestor = LEFT;
            else nodeState.ancestor = DOWNLEFT;
        }
        else if (originX == 1)
        {
            if (originY == -1) nodeState.ancestor = UPRIGHT;
            else if (originY == 0) nodeState.ancestor = RIGHT;
            else nodeState.ancestor = DOWNRIGHT;
        }
        else
        {
            if (originY == -1) nodeState.ancestor = UP;
            else nodeState.ancestor = DOWN;
        }

        changes->push_back(nodeState);
    }
}

void AStar::freeNodes()
{
    if (ready)			return;
    if (matrix == NULL)	return;

    for (UINT i = 0; i < width;  i++)
    for (UINT j = 0; j < height; j++)
        if (matrix[i][j].type != WALLNODE)
            matrix[i][j].type = WAYNODE;

    ready = true;
}

void AStar::setOriginNode(UINT x, UINT y, UINT originX, UINT originY)
{
    if (x >= width || y >= height)				return;
    if (originX >= width || originY >= height)	return;

    matrix[x][y].originX = originX;
    matrix[x][y].originY = originY;
}

double AStar::pathCostToNode(UINT x, UINT y, UINT destX, UINT destY)
{
    double euc, h;
    double cost = 0;
    UINT originX, originY;

    originX = matrix[x][y].originX;
    originY = matrix[x][y].originY;

    while (!(originX == x && originY == y))
    {
        euc = sqrt(pow((double)x - (double)destX, 2.0) + pow((double)y - (double)destY, 2.0));

        if (heuristic == none)					h = 0;
        else if (heuristic == euclidean)		h = euc;
        else if (heuristic == manhattan)		h = abs((double)x - (double)destX) + abs((double)y - (double)destY);
        else if (heuristic == diagonal)			h = max(abs((double)x - (double)destX), abs((double)y - (double)destY));
        else if (heuristic == newH)				h = ceil(euc) - euc;

        cost += h + matrix[x][y].expandCost;

        x = originX;
        y = originY;

        originX = matrix[x][y].originX;
        originY = matrix[x][y].originY;
    }

    return cost;
}

void AStar::search(bool saveChanges)
{
    UINT x, y;
    double minCost;
    pair<UINT, UINT> node;
    list<pair<UINT, UINT> >::iterator elem, it;
    list<NodeState> *newMatrixState;
    NodeState nodeState;

    if (matrix == NULL)									return;
    if (start.first > width || start.second > height)	return;
    if (end.first > width || end.second > height)		return;

    freeNodes();
    path.clear();
    matrixsChanges.clear();

    if (start == end)
    {
        path.push_front(start);
        return;
    }

    ready = false;

    if (saveChanges) newMatrixState = new list<NodeState>();
    else			 newMatrixState = NULL;

    openedNodes.clear();
    openedNodes.push_front(start);

    x = start.first;
    y = start.second;
    setOriginNode(x, y, x, y);
    matrix[x][y].cost = 0;
    matrix[x][y].pathCost = 0;
    matrix[x][y].type = OPENEDNODE;
    matrix[x][y].g = 0;
    //calculateCost(start);

    while (matrix[end.first][end.second].type != OPENEDNODE)
    {
        minCost = numeric_limits<double>::max();

        if (newMatrixState != NULL) newMatrixState->clear();

        if (openedNodes.size() == 0) return;

        for (it = openedNodes.begin(); it != openedNodes.end(); it++)
        {
            x = it->first;
            y = it->second;

            if (matrix[x][y].cost < minCost)
            {
                minCost = matrix[x][y].cost;
                elem = it;
            }
        }

        x = elem->first;
        y = elem->second;

        matrix[x][y].type = VISITEDNODE;

        if (newMatrixState != NULL)
        {
            nodeState.x = x;
            nodeState.y = y;
            nodeState.state = VISITEDNODE;

            if (matrix[x][y].originX == -1)
            {
                if (matrix[x][y].originY == -1) nodeState.ancestor = UPLEFT;
                else if (matrix[x][y].originY == 0) nodeState.ancestor = LEFT;
                else nodeState.ancestor = DOWNLEFT;
            }
            else if (matrix[x][y].originX == 1)
            {
                if (matrix[x][y].originY == -1) nodeState.ancestor = UPRIGHT;
                else if (matrix[x][y].originY == 0) nodeState.ancestor = RIGHT;
                else nodeState.ancestor = DOWNRIGHT;
            }
            else if (matrix[x][y].originX == 0)
            {
                if (matrix[x][y].originY == -1) nodeState.ancestor = UP;
                else if (matrix[x][y].originY == 1) nodeState.ancestor = DOWN;
                else nodeState.ancestor = SELF;
            }

            newMatrixState->push_back(nodeState);
        }

        openedNodes.remove(*elem);

        if (x > 0)
        {
            if (y > 0)			openNode(x - 1, y - 1, x, y, newMatrixState);
                                openNode(x - 1, y    , x, y, newMatrixState);
            if (y + 1 < height)	openNode(x - 1, y + 1, x, y, newMatrixState);
        }

        if (y > 0)				openNode(x    , y - 1, x, y, newMatrixState);
        if (y + 1 < height)		openNode(x    , y + 1, x, y, newMatrixState);

        if (x + 1 < width)
        {
            if (y > 0)			openNode(x + 1, y - 1, x, y, newMatrixState);
                                openNode(x + 1, y    , x, y, newMatrixState);
            if (y + 1 < height)	openNode(x + 1, y + 1, x, y, newMatrixState);
        }

        if (newMatrixState != NULL)
            matrixsChanges.push_back(*newMatrixState);
    }

    x = end.first;
    y = end.second;

    node.first = matrix[x][y].originX;
    node.second = matrix[x][y].originY;

    while (!(matrix[x][y].originX == x && matrix[x][y].originY == y))
    {
        node.first = x;
        node.second = y;

        path.push_front(node);

        x = matrix[node.first][node.second].originX;
        y = matrix[node.first][node.second].originY;
    }

    return;
}

void AStar::setMatrix(UINT width, UINT height)
{
    destroyMatrix();

    this->width = width;
    this->height = height;

    if (width == 0 || height == 0) return;

    matrix = new SearchNode*[width];

    for (UINT i = 0; i < width; i++)
        matrix[i] = new SearchNode[height];

    path.clear();
}

void AStar::setStart(UINT x, UINT y)
{
    if (x <= width && y <= height)
    {
        start.first = x;
        start.second = y;

        path.clear();
    }
}

void AStar::setEnd(UINT x, UINT y)
{
    if (x <= width && y <= height)
    {
        end.first = x;
        end.second = y;

        path.clear();
    }
}

void AStar::setWall(UINT x, UINT y)
{
    if		(matrix == NULL)			return;
    else if	(x > width || y > height)	return;

    matrix[x][y].type = WALLNODE;

    path.clear();
}

void AStar::setWay(UINT x, UINT y)
{
    if		(matrix == NULL)			return;
    else if	(x > width || y > height)	return;

    matrix[x][y].type = WAYNODE;

    path.clear();
}

void AStar::setHeuristics(AStarHeuristics heuristic)
{
    if (heuristic >= none && heuristic <= newH)
    {
        this->heuristic = heuristic;
    }
    else
    {
        this->heuristic = euclidean;
    }
}

void AStar::setExpandCost(UINT expandCost, UINT x, UINT y)
{
    if (x >= width || y >= height) return;

    matrix[x][y].expandCost = expandCost;
}

void AStar::destroyMatrix()
{
    if (matrix != NULL)
    {
        for (UINT i = 0; i < width; i++)
            delete [] matrix[i];
        delete [] matrix;

        matrix = NULL;

        path.clear();
        matrixsChanges.clear();
    }
}

NodeType AStar::getNodeType(UINT x, UINT y) const
{
    if		(matrix == NULL)			return NONE;
    else if	(x > width || y > height)	return NONE;

    return matrix[x][y].type;
}

pair<UINT, UINT> AStar::getStart() const
{
    return start;
}

pair<UINT, UINT> AStar::getEnd() const
{
    return end;
}

list<pair<UINT, UINT> > AStar::getPath(bool saveChanges)
{
    if (path.size() == 0)
        search(saveChanges);

    return path;
}

list<list<NodeState> > AStar::getChanges() const
{
    return matrixsChanges;
}

double AStar::estimateCost(UINT x1, UINT y1, UINT x2, UINT y2)
{
    double x, y, destX, destY, cost, h, euc;
    UINT g;

    x = (float)x2;
    y = (float)y2;
    destX = (float)end.first;
    destY = (float)end.second;

    euc = sqrt(pow(x - destX, 2.0) + pow(y - destY, 2.0));

    if (heuristic == none)					h = 0;
    else if (heuristic == euclidean)		h = euc;
    else if (heuristic == manhattan)		h = abs(x - destX) + abs(y - destY);
    else if (heuristic == diagonal)			h = max(abs(x - destX), abs(y - destY));
    else if (heuristic == newH)				h = ceil(euc) - euc;

    g = matrix[x1][y1].g + matrix[x2][y2].expandCost;

    cost = h + (double)g;

    return cost;
}

























