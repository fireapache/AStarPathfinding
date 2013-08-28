#ifndef PATHSTEPSTHREAD_H
#define PATHSTEPSTHREAD_H

#include <QThread>
#include <list>
#include "fixinclude.h"

using namespace std;

class PathStepsThread : public QThread
{
    class MapNode **map;

    list<pair<UINT, UINT> > *path;
    list<list<NodeState> > *steps;

    pair<UINT, UINT> startNode;
    pair<UINT, UINT> endNode;

    QString openedNodeImgPath[8];
    QString closedNodeImgPath;
    QString pathNodeImgPath;
    QString startNodeImgPath;
    QString endNodeImgPath;

    unsigned long delay;

    void reDrawStartEndNodes();

public:
    PathStepsThread();

    void run();

    void setDelay(unsigned long delay);
    void setResources(RunResources resources);
};

#endif // PATHSTEPSTHREAD_H
