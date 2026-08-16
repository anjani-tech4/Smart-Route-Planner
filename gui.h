#ifndef GUI_H
#define GUI_H

#include <QWidget>
#include <QPointF>
#include <unordered_map>
#include <vector>
#include <string>
#include "dijkstra.h"

class QComboBox;
class QPushButton;
class QLabel;
class QGraphicsView;
class QGraphicsScene;

class RouteWindow : public QWidget
{
public:
    RouteWindow();

private:
    QComboBox *sourceBox;
    QComboBox *destinationBox;
    QPushButton *findButton;
    QLabel *resultLabel;

    QGraphicsView *graphView;
    QGraphicsScene *graphScene;

    unordered_map<string, vector<Edge>> graph;
    unordered_map<string, QPointF> nodePosition;

    void loadGraph();
    void findRoute();

    void calculateNodePositions();
    void drawGraph(const vector<string> &highlightPath = {});
};

#endif