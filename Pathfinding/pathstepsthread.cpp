#include "pathstepsthread.h"

PathStepsThread::PathStepsThread()
{
    delay = 200;

    map = NULL;
    path = NULL;
    steps = NULL;

    openedNodeImgPath[0] = QDir::currentPath() + "/images/opened0.png";
    openedNodeImgPath[1] = QDir::currentPath() + "/images/opened1.png";
    openedNodeImgPath[2] = QDir::currentPath() + "/images/opened2.png";
    openedNodeImgPath[3] = QDir::currentPath() + "/images/opened3.png";
    openedNodeImgPath[4] = QDir::currentPath() + "/images/opened4.png";
    openedNodeImgPath[5] = QDir::currentPath() + "/images/opened5.png";
    openedNodeImgPath[6] = QDir::currentPath() + "/images/opened6.png";
    openedNodeImgPath[7] = QDir::currentPath() + "/images/opened7.png";

    closedNodeImgPath = QDir::currentPath() + "/images/closed.png";
    pathNodeImgPath = QDir::currentPath() + "/images/path.png";
    startNodeImgPath = QDir::currentPath() + "/images/start.png";
    endNodeImgPath = QDir::currentPath() + "/images/end.png";
}

void PathStepsThread::reDrawStartEndNodes()
{
    map[startNode.first][startNode.second].pixmapItem.setPixmap(startNodeImgPath);
    map[endNode.first][endNode.second].pixmapItem.setPixmap(endNodeImgPath);
}

void PathStepsThread::run()
{
    list<list<NodeState> >::iterator stepsIt;
    list<NodeState>::iterator stepIt;

    if (map == NULL) return;
    if (path == NULL) return;
    if (steps == NULL) return;

    for (stepsIt = steps->begin(); stepsIt != steps->end(); stepsIt++)
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

        reDrawStartEndNodes();

        wait(delay);
    }
}

void PathStepsThread::setDelay(unsigned long delay)
{
    this->delay = delay;
}

void PathStepsThread::setResources(RunResources resources)
{
    map = resources.map;
    path = resources.path;
    steps = resources.steps;
    startNode = resources.start;
    endNode = resources.end;
}
