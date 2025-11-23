#include "application.h"
#include <iostream>

Application::Application(Renderer* renderer)
    : renderer(renderer), map(NULL), mapWidth(0), mapHeight(0),
      itemSize(8), itemScale(1), pathOnScreen(false), mapBuilt(false),
      toSetState(TOSETWALL), currentHeuristic(euclidean)
{
    stepsIt = steps.end();
}

Application::~Application()
{
    destroyMap();
}

void Application::freeNodes()
{
    for (UINT i = 0; i < mapWidth; i++)
    {
        for (UINT j = 0; j < mapHeight; j++)
        {
            if (map[i][j].type != WALLNODE)
            {
                if (map[i][j].expandCost != 1)
                {
                    map[i][j].textureIndex = TEX_DIRT;
                }
                else
                {
                    map[i][j].textureIndex = TEX_FREE;
                }
            }
        }
    }

    pathOnScreen = false;
}

void Application::destroyMap()
{
    if (map != NULL)
    {
        for (UINT i = 0; i < mapWidth; i++)
            delete[] map[i];
        delete[] map;

        pathOnScreen = false;
        astar.destroyMatrix();
        path.clear();
        steps.clear();
        map = NULL;
        mapBuilt = false;
    }
}

void Application::buildMap(UINT width, UINT height, UINT scale)
{
    destroyMap();

    mapWidth = width;
    mapHeight = height;
    itemScale = scale;

    if (mapWidth == 0 || mapHeight == 0)
        return;

    map = new MapNode*[mapWidth];

    for (UINT i = 0; i < mapWidth; i++)
        map[i] = new MapNode[mapHeight];

    for (UINT i = 0; i < mapWidth; i++)
    {
        for (UINT j = 0; j < mapHeight; j++)
        {
            map[i][j].type = WAYNODE;
            map[i][j].expandCost = 1;
            map[i][j].textureIndex = TEX_FREE;
        }
    }

    astar.setMatrix(mapWidth, mapHeight);
    astar.setStart(0, 0);
    astar.setEnd(mapWidth - 1, mapHeight - 1);
    astar.setHeuristics(currentHeuristic);

    renderer->setGridSize(mapWidth, mapHeight);
    renderer->setScale(scale);

    reDrawStartEndNodes();

    mapBuilt = true;
}

void Application::updateNodeTexture(UINT x, UINT y, TextureID texID)
{
    if (x >= mapWidth || y >= mapHeight)
        return;

    map[x][y].textureIndex = texID;
}

void Application::drawPath()
{
    if (path.size() == 0)
    {
        std::cout << "No path found!" << std::endl;
        return;
    }

    std::list<std::pair<UINT, UINT> >::iterator pathIt;
    for (pathIt = path.begin(); pathIt != path.end(); pathIt++)
    {
        map[pathIt->first][pathIt->second].textureIndex = TEX_PATH;
    }

    pathOnScreen = true;
    reDrawStartEndNodes();
}

void Application::reDrawStartEndNodes()
{
    std::pair<UINT, UINT> start, end;

    start = astar.getStart();
    end = astar.getEnd();

    map[start.first][start.second].textureIndex = TEX_START;
    map[end.first][end.second].textureIndex = TEX_END;
}

void Application::obtainPath(bool saveChanges)
{
    path = astar.getPath(saveChanges);

    if (saveChanges)
    {
        steps = astar.getChanges();
        stepsIt = steps.begin();
    }
}

void Application::showPath()
{
    if (!mapBuilt)
        return;

    freeNodes();
    obtainPath(true);
    drawPath();
}

void Application::showNextStep()
{
    if (!mapBuilt)
        return;

    std::list<NodeState>::iterator stepIt;

    if (steps.size() == 0)
    {
        obtainPath(true);

        if (steps.size() == 0)
            return;

        pathOnScreen = false;
    }

    if (stepsIt == steps.end())
    {
        if (pathOnScreen)
        {
            stepsIt = steps.begin();
            freeNodes();
        }
        else
        {
            drawPath();
            return;
        }
    }

    for (stepIt = stepsIt->begin(); stepIt != stepsIt->end(); stepIt++)
    {
        if (stepIt->state == VISITEDNODE)
        {
            map[stepIt->x][stepIt->y].textureIndex = TEX_CLOSED;
        }
        else if (stepIt->state == OPENEDNODE)
        {
            TextureID texID = TEX_OPENED0;
            switch (stepIt->ancestor)
            {
                case UPLEFT: texID = TEX_OPENED0; break;
                case UP: texID = TEX_OPENED1; break;
                case UPRIGHT: texID = TEX_OPENED2; break;
                case LEFT: texID = TEX_OPENED3; break;
                case RIGHT: texID = TEX_OPENED4; break;
                case DOWNLEFT: texID = TEX_OPENED5; break;
                case DOWN: texID = TEX_OPENED6; break;
                case DOWNRIGHT: texID = TEX_OPENED7; break;
                default: break;
            }
            map[stepIt->x][stepIt->y].textureIndex = texID;
        }
    }

    reDrawStartEndNodes();
    if (pathOnScreen)
        pathOnScreen = false;

    stepsIt++;
}

void Application::setHeuristics(AStarHeuristics heuristic)
{
    currentHeuristic = heuristic;
    astar.setHeuristics(heuristic);
}

void Application::setToSetState(ToSetState state)
{
    toSetState = state;
}

void Application::handleMouseClick(int screenX, int screenY, bool isDragging, int dragStartX, int dragStartY)
{
    if (!mapBuilt)
        return;

    UINT initX, initY, finalX, finalY, swap;

    if (isDragging)
    {
        initX = dragStartX / (itemSize * itemScale);
        initY = dragStartY / (itemSize * itemScale);
        finalX = screenX / (itemSize * itemScale);
        finalY = screenY / (itemSize * itemScale);
    }
    else
    {
        initX = finalX = screenX / (itemSize * itemScale);
        initY = finalY = screenY / (itemSize * itemScale);
    }

    if (initX > finalX)
    {
        swap = finalX;
        finalX = initX;
        initX = swap;
    }

    if (initY > finalY)
    {
        swap = finalY;
        finalY = initY;
        initY = swap;
    }

    if (finalX >= mapWidth) finalX = mapWidth - 1;
    if (finalY >= mapHeight) finalY = mapHeight - 1;

    std::pair<UINT, UINT> point;

    if (toSetState == TOSETWALL)
    {
        for (UINT i = initX; i <= finalX; i++)
        {
            for (UINT j = initY; j <= finalY; j++)
            {
                map[i][j].type = WALLNODE;
                map[i][j].textureIndex = TEX_WALL;
                astar.setWall(i, j);
            }
        }
    }
    else if (toSetState == TOSETWAY)
    {
        for (UINT i = initX; i <= finalX; i++)
        {
            for (UINT j = initY; j <= finalY; j++)
            {
                map[i][j].type = WAYNODE;
                map[i][j].expandCost = 1;
                map[i][j].textureIndex = TEX_FREE;
                astar.setWay(i, j);
                astar.setExpandCost(1, i, j);
            }
        }
    }
    else if (toSetState == TOSETDIRT)
    {
        for (UINT i = initX; i <= finalX; i++)
        {
            for (UINT j = initY; j <= finalY; j++)
            {
                map[i][j].type = WAYNODE;
                map[i][j].expandCost = 2;
                map[i][j].textureIndex = TEX_DIRT;
                astar.setWay(i, j);
                astar.setExpandCost(2, i, j);
            }
        }
    }
    else if (toSetState == TOSETSTART)
    {
        point = astar.getStart();
        map[point.first][point.second].textureIndex = TEX_FREE;
        map[finalX][finalY].textureIndex = TEX_START;
        astar.setStart(finalX, finalY);
    }
    else if (toSetState == TOSETEND)
    {
        point = astar.getEnd();
        map[point.first][point.second].textureIndex = TEX_FREE;
        map[finalX][finalY].textureIndex = TEX_END;
        astar.setEnd(finalX, finalY);
    }

    path.clear();
    steps.clear();
}
