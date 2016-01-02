#include "mapscene.h"

MapScene::MapScene(QObject *parent)
{
    map = NULL;
    mapWidth = 0;
    mapHeight = 0;
    itemSize = 8;
    itemScale = 1;
    stepsIt = steps.end();
    toSetState = TOSETWALL;

    this->setSceneRect(0, 0, 0, 0);

    freeNodeImgPath = ":/images/images/free.png";
    dirtNodeImgPath = ":/images/images/dirt.png";

    openedNodeImgPath[0] = ":/images/images/opened0.png";
    openedNodeImgPath[1] = ":/images/images/opened1.png";
    openedNodeImgPath[2] = ":/images/images/opened2.png";
    openedNodeImgPath[3] = ":/images/images/opened3.png";
    openedNodeImgPath[4] = ":/images/images/opened4.png";
    openedNodeImgPath[5] = ":/images/images/opened5.png";
    openedNodeImgPath[6] = ":/images/images/opened6.png";
    openedNodeImgPath[7] = ":/images/images/opened7.png";

    closedNodeImgPath = ":/images/images/closed.png";
    wallNodeImgPath = ":/images/images/wall.png";
    pathNodeImgPath = ":/images/images/path.png";
    startNodeImgPath = ":/images/images/start.png";
    endNodeImgPath = ":/images/images/end.png";

    pathOnScreen = false;

	parent = 0;
    //QGraphicsScene::QGraphicsScene(parent);
}

MapScene::~MapScene()
{
    destroyMap();

    this->items().clear();

    //QGraphicsScene::~QGraphicsScene();
}

void MapScene::freeNodes()
{
    for (UINT i = 0; i < mapWidth; i++)
    for (UINT j = 0; j < mapHeight; j++)
    {
        if (map[i][j].type != WALLNODE)
        {
            if (map[i][j].expandCost != 1)
            {
                map[i][j].pixmapItem.setPixmap(dirtNodeImgPath);
            }
            else
            {
                map[i][j].pixmapItem.setPixmap(freeNodeImgPath);
            }
        }
    }

    pathOnScreen = false;
}

void MapScene::destroyMap()
{
    if (map != NULL)
    {
        for (UINT i = 0; i < mapWidth; i++)
            delete [] map[i];
        delete [] map;

        pathOnScreen = false;
        astar.destroyMatrix();
        this->items().clear();
        path.clear();
        steps.clear();
        map = NULL;
    }
}

void MapScene::setMap(UINT mapWidth, UINT mapHeight)
{
    destroyMap();

    this->mapWidth = mapWidth;
    this->mapHeight = mapHeight;

    if (mapWidth == 0 || mapHeight == 0) return;

    map = new MapNode*[mapWidth];

    for (UINT i = 0; i < mapWidth; i++)
        map[i] = new MapNode[mapHeight];

    this->setSceneRect(0, 0, mapWidth, mapHeight);

    for (UINT i = 0; i < mapWidth; i++)
    for (UINT j = 0; j < mapHeight; j++)
    {
        map[i][j].type = WAYNODE;
        map[i][j].expandCost = 1;
        map[i][j].pixmapItem.setScale(itemScale);
        map[i][j].pixmapItem.setOffset(i*itemSize, j*itemSize);
        map[i][j].pixmapItem.setPixmap(QPixmap(freeNodeImgPath));
        this->addItem(&map[i][j].pixmapItem);
    }

    astar.setMatrix(mapWidth, mapHeight);
    astar.setEnd(mapWidth - 1, mapHeight - 1);
    reDrawStartEndNodes();
}

void MapScene::drawPath()
{
    QMessageBox messageBox;
    list<pair<UINT, UINT> >::iterator pathIt;

    if (path.size() == 0)
    {
        messageBox.setText("Erro!");
        messageBox.addButton(QMessageBox::Ok);
        messageBox.exec();
        return;
    }

    for (pathIt = path.begin(); pathIt != path.end(); pathIt++)
        map[pathIt->first][pathIt->second].pixmapItem.setPixmap(pathNodeImgPath);

    pathOnScreen = true;

    reDrawStartEndNodes();
}

void MapScene::reDrawStartEndNodes()
{
    pair<UINT, UINT> start, end;

    start = astar.getStart();
    end = astar.getEnd();

    map[start.first][start.second].pixmapItem.setPixmap(startNodeImgPath);
    map[end.first][end.second].pixmapItem.setPixmap(endNodeImgPath);
}

void MapScene::obtainPath(bool saveChanges)
{
    path = astar.getPath(saveChanges);

    if (saveChanges)
    {
        steps = astar.getChanges();
        stepsIt = steps.begin();
    }
}

void MapScene::showPath()
{
    freeNodes();
    obtainPath(true);
    drawPath();
}

void MapScene::showNextStep()
{
    list<NodeState>::iterator stepIt;

    if (steps.size() == 0)
    {
        obtainPath(true);

        if (steps.size() == 0) return;

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
            map[stepIt->x][stepIt->y].pixmapItem.setPixmap(closedNodeImgPath);
        }
        else if (stepIt->state == OPENEDNODE)
        {
            if (stepIt->ancestor == UPLEFT) map[stepIt->x][stepIt->y].pixmapItem.setPixmap(openedNodeImgPath[0]);
            else if (stepIt->ancestor == UP) map[stepIt->x][stepIt->y].pixmapItem.setPixmap(openedNodeImgPath[1]);
            else if (stepIt->ancestor == UPRIGHT) map[stepIt->x][stepIt->y].pixmapItem.setPixmap(openedNodeImgPath[2]);
            else if (stepIt->ancestor == LEFT) map[stepIt->x][stepIt->y].pixmapItem.setPixmap(openedNodeImgPath[3]);
            else if (stepIt->ancestor == RIGHT) map[stepIt->x][stepIt->y].pixmapItem.setPixmap(openedNodeImgPath[4]);
            else if (stepIt->ancestor == DOWNLEFT) map[stepIt->x][stepIt->y].pixmapItem.setPixmap(openedNodeImgPath[5]);
            else if (stepIt->ancestor == DOWN) map[stepIt->x][stepIt->y].pixmapItem.setPixmap(openedNodeImgPath[6]);
            else if (stepIt->ancestor == DOWNRIGHT) map[stepIt->x][stepIt->y].pixmapItem.setPixmap(openedNodeImgPath[7]);
        }
    }

    reDrawStartEndNodes();
    if (pathOnScreen) pathOnScreen = false;

    stepsIt++;
}

void MapScene::showSteps()
{
//    RunResources resources;

//    resources.start = astar.getStart();
//    resources.end = astar.getEnd();
//    resources.map = map;

//    path = astar.getPath(true);
//    steps = astar.getChanges();

//    resources.path = &path;
//    resources.steps = &steps;

//    pathStepsThread.setResources(resources);
//    pathStepsThread.start();
}

void MapScene::setHeuristics(AStarHeuristics heuristic)
{
    astar.setHeuristics(heuristic);
}

void MapScene::setScale(UINT scale)
{
    itemScale = scale;
}

void MapScene::setStart(UINT x, UINT y)
{
    astar.setStart(x, y);
}

void MapScene::setEnd(UINT x, UINT y)
{
    astar.setEnd(x, y);
}

void MapScene::setToSetState(ToSetState toSetState)
{
    this->toSetState = toSetState;
}

void MapScene::setStepsDelay(unsigned long delay)
{
    pathStepsThread.setDelay(delay);
}

void MapScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF initPos, finalPos;
    UINT initX, initY, finalX, finalY, swap;
    pair<UINT, UINT> point;

    initPos = event->buttonDownScenePos(Qt::LeftButton);
    finalPos = event->lastScenePos();

    initX = initPos.toPoint().x() / itemSize / itemScale;
    initY = initPos.toPoint().y() / itemSize / itemScale;
    finalX = finalPos.toPoint().x() / itemSize / itemScale;
    finalY = finalPos.toPoint().y() / itemSize / itemScale;

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

    if (finalX > mapWidth - 1) finalX = mapWidth - 1;
    if (finalY > mapHeight - 1) finalY = mapHeight - 1;

    if (toSetState == TOSETWALL)
    {
        for (UINT i = initX; i <= finalX; i++)
        for (UINT j = initY; j <= finalY; j++)
        {
            map[i][j].type = WALLNODE;
            map[i][j].pixmapItem.setPixmap(wallNodeImgPath);
            astar.setWall(i, j);
        }
    }
    else if (toSetState == TOSETWAY)
    {
        for (UINT i = initX; i <= finalX; i++)
        for (UINT j = initY; j <= finalY; j++)
        {
            map[i][j].type = WAYNODE;
            map[i][j].expandCost = 1;
            map[i][j].pixmapItem.setPixmap(freeNodeImgPath);
            astar.setWay(i, j);
            astar.setExpandCost(1, i, j);
        }
    }
    else if (toSetState == TOSETDIRT)
    {
        for (UINT i = initX; i <= finalX; i++)
        for (UINT j = initY; j <= finalY; j++)
        {
            map[i][j].type = WAYNODE;
            map[i][j].expandCost = 2;
            map[i][j].pixmapItem.setPixmap(dirtNodeImgPath);
            astar.setWay(i, j);
            astar.setExpandCost(2, i, j);
        }
    }
    else if (toSetState == TOSETSTART)
    {
        point = astar.getStart();
        map[point.first][point.second].pixmapItem.setPixmap(freeNodeImgPath);
        map[finalX][finalY].pixmapItem.setPixmap(startNodeImgPath);
        astar.setStart(finalX, finalY);
    }
    else
    {
        point = astar.getEnd();
        map[point.first][point.second].pixmapItem.setPixmap(freeNodeImgPath);
        map[finalX][finalY].pixmapItem.setPixmap(endNodeImgPath);
        astar.setEnd(finalX, finalY);
    }

    path.clear();
    steps.clear();
}

void MapScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    if (event) showNextStep();
}
