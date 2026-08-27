#include "gui.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QPen>
#include <QBrush>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>

RouteWindow::RouteWindow()
{
    setWindowTitle("Smart Route Planner");
    resize(900, 750);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *title = new QLabel("SMART ROUTE PLANNER");

    QLabel *sourceLabel = new QLabel("Source Station:");
    sourceBox = new QComboBox();

    QLabel *destinationLabel = new QLabel("Destination Station:");
    destinationBox = new QComboBox();

    findButton = new QPushButton("Find Shortest Route");

    resultLabel = new QLabel("Route will appear here");

    graphScene = new QGraphicsScene(this);
    graphView = new QGraphicsView(graphScene);
    graphView->setMinimumHeight(400);
    graphView->setRenderHint(QPainter::Antialiasing);

    loadGraph();

    vector<string> stations;
    for (auto &entry : graph)
    {
        stations.push_back(entry.first);
    }
    sort(stations.begin(), stations.end());

    for (auto &stationName : stations)
    {
        QString qName = QString::fromStdString(stationName);
        sourceBox->addItem(qName);
        destinationBox->addItem(qName);
    }

    calculateNodePositions();
    drawGraph();

    layout->addWidget(title);

    layout->addWidget(sourceLabel);
    layout->addWidget(sourceBox);

    layout->addWidget(destinationLabel);
    layout->addWidget(destinationBox);

    layout->addWidget(findButton);

    layout->addWidget(resultLabel);

    layout->addWidget(graphView);

    connect(findButton, &QPushButton::clicked, this, [this]()
   {
    findRoute();
   }); 
}
   void RouteWindow::loadGraph()
{
    ifstream file("data.csv");

    if(!file.is_open())
    {
        resultLabel->setText("Could not open data.csv");
        return;
    }

    string line;

    getline(file, line);

    while(getline(file, line))
    {
        stringstream ss(line);

        string station1;
        string station2;
        string distanceString;
        string costString;

        getline(ss, station1, ',');
        getline(ss, station2, ',');
        getline(ss, distanceString, ',');
        getline(ss, costString, ',');

        double distance = stod(distanceString);
        double cost = stod(costString);

        graph[station1].push_back({station2, distance, cost});
        graph[station2].push_back({station1, distance, cost});
    }

    file.close();
}


void RouteWindow::findRoute()
{
    string source = sourceBox->currentText().toStdString();
    string destination = destinationBox->currentText().toStdString();

    if(graph.find(source) == graph.end())
    {
        resultLabel->setText("Source station not found!");
        return;
    }

    if(graph.find(destination) == graph.end())
    {
        resultLabel->setText("Destination station not found!");
        return;
    }

    if(source == destination)
    {
        resultLabel->setText("Source and destination are the same station.");
        return;
    }

    // Pass a vector to receive the actual path dijkstra() computed.
    vector<string> path;
    double cost = 0;
    double distance = dijkstra(graph, source, destination, &path, &cost);

    if(distance == -1)
    {
        resultLabel->setText("Route does not exist.");
        drawGraph(); // plain graph, nothing to highlight
    }
    else
    {
        resultLabel->setText(
        "Shortest Distance: " +
        QString::number(distance) +
        " km\nTotal Cost: ₹" +
         QString::number(cost)
     );
        drawGraph(path);
    }
}

// ---------------- visualization helpers ----------------

void RouteWindow::calculateNodePositions()
{
    vector<string> stations;
    for (auto &entry : graph)
    {
        stations.push_back(entry.first);
    }
    sort(stations.begin(), stations.end());

    int n = stations.size();
    if (n == 0) return;

    double radius = 250.0;
    double centerX = 300.0;
    double centerY = 300.0;

    for (int i = 0; i < n; i++)
    {
        double angle = (2 * M_PI * i) / n;
        double x = centerX + radius * cos(angle);
        double y = centerY + radius * sin(angle);
        nodePosition[stations[i]] = QPointF(x, y);
    }
}

void RouteWindow::drawGraph(const vector<string> &highlightPath)
{
    graphScene->clear();

    vector<pair<string,string>> pathEdges;
    for (size_t i = 0; i + 1 < highlightPath.size(); i++)
    {
        pathEdges.push_back({highlightPath[i], highlightPath[i + 1]});
    }

    auto isPathEdge = [&](const string &a, const string &b)
    {
        for (auto &e : pathEdges)
        {
            if ((e.first == a && e.second == b) || (e.first == b && e.second == a))
                return true;
        }
        return false;
    };

    auto isPathNode = [&](const string &s)
    {
        for (auto &n : highlightPath)
            if (n == s) return true;
        return false;
    };

    for (auto &entry : graph)
    {
        const string &from = entry.first;
        if (nodePosition.find(from) == nodePosition.end()) continue;

        for (auto &edge : entry.second)
        {
            const string &to = edge.station;
            if (nodePosition.find(to) == nodePosition.end()) continue;

            if (from > to) continue; // draw each undirected edge once

            QPointF p1 = nodePosition[from];
            QPointF p2 = nodePosition[to];

            bool onPath = isPathEdge(from, to);

            QPen pen(onPath ? Qt::red : Qt::gray);
            pen.setWidth(onPath ? 3 : 1);
            graphScene->addLine(p1.x(), p1.y(), p2.x(), p2.y(), pen);

            QPointF mid((p1.x() + p2.x()) / 2, (p1.y() + p2.y()) / 2);
            QGraphicsTextItem *distText = graphScene->addText(
                QString::number(edge.distance) + " km"
            );
            distText->setDefaultTextColor(onPath ? Qt::red : Qt::darkGray);
            distText->setPos(mid);
        }
    }

    double nodeRadius = 18.0;
    for (auto &pos : nodePosition)
    {
        const string &station = pos.first;
        QPointF p = pos.second;

        bool onPath = isPathNode(station);

        QBrush brush(onPath ? Qt::red : Qt::blue);
        graphScene->addEllipse(
            p.x() - nodeRadius, p.y() - nodeRadius,
            nodeRadius * 2, nodeRadius * 2,
            QPen(Qt::black), brush
        );

        QGraphicsTextItem *label = graphScene->addText(QString::fromStdString(station));
        label->setPos(p.x() - nodeRadius, p.y() + nodeRadius);
    }

    graphScene->setSceneRect(0, 0, 650, 650);
}