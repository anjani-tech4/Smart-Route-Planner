# Smart Route Planner

A C++ / Qt Widgets desktop application that finds the shortest route between
two stations using Dijkstra's algorithm, and visualizes the graph and the
computed path on an interactive canvas.

## Features

- Loads a station graph (stations, distances, costs) from `data.csv`
- Computes the shortest path between any two stations using a custom
  binary-heap priority queue (`pq.h`) driving Dijkstra's algorithm
- Displays total distance and total cost for the computed route
- Renders the full graph with `QGraphicsView`/`QGraphicsScene`, highlighting
  the shortest path in red

## Tech Stack

- C++17
- Qt 6 (Widgets module)
- qmake build system

## Project Structure

```
SmartRoutePlanner/
├── QTGUI.pro          # qmake project file
├── data.csv           # station graph data (station1, station2, distance, cost)
├── README.md
├── .gitignore
└── src/
    ├── main.cpp        # application entry point
    ├── gui.h / gui.cpp # RouteWindow: UI, graph loading, visualization
    ├── dijkstra.h / dijkstra.cpp  # Dijkstra's algorithm implementation
    └── pq.h            # custom generic priority queue (binary heap)
```

## Data Format

`data.csv` should have a header row followed by rows of:

```
station1,station2,distance,cost
```

Each row is treated as an undirected edge between `station1` and `station2`.

## Building

### Prerequisites
- Qt 6 (Widgets module) with a matching MinGW/GCC/MSVC toolchain
- qmake on your PATH

### Steps

```bash
qmake QTGUI.pro
mingw32-make      # or `make` on Linux/macOS
```

Then run the produced executable from the project root (so it can find
`data.csv` via its relative path).

> **Note:** if your Qt build uses a "shadow build" directory, copy
> `data.csv` next to the compiled executable, or run the binary from the
> project root.

## How It Works

1. `RouteWindow::loadGraph()` reads `data.csv` and builds an adjacency list
   (`unordered_map<string, vector<Edge>>`).
2. When the user picks a source and destination and clicks **Find Shortest
   Route**, `dijkstra()` runs Dijkstra's algorithm using the custom `PQ`
   priority queue to repeatedly extract the closest unvisited station.
3. The resulting path and cost are returned to the GUI, which displays the
   result and redraws the graph with the shortest path highlighted in red.

## Author

Anjani — [github.com/anjani-tech4](https://github.com/anjani-tech4)
