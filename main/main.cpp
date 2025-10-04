/**
 * @mainpage
 *
 * @section intro About
 * Locus No Pilotus is a project of four first grade MIPT DAFE/RSE students (for
 * engineering practical work in the second semester) in Qt C++.
 *
 * @section description Description
 * In this project, we are developing an application that calculates the
 * trajectory of a flying delivery robot that collects valuable cargo from
 * control points: **Targets**. On its way, it may encounter high mountains that
 * it cannot fly over: **Hills**; or elliptical territories that are impossible
 * to visit due to climate conditions: **TrappyCircles**. Also, through some
 * control points, the robot is simply unable to move, as the cargo may not be
 * ready for transportation at that time, these are **TrappyLines**.
 *
 * The robot's trajectory is calculated using several mathematical algorithms:
 * Little's Algorithm (solving the Traveling Salesman Problem (TSP) for multiple
 * robots), Dijkstra's Algorithm (finding the shortest path between two points
 * in a graph), Geometric Algorithms for Obstacle Handling (tangents to circles
 * and polygons and intersection checking), Visibility Graph Construction,
 * Composite Trajectory Planning Algorithm.
 *
 * This is a comprehensive system that combines geometric computations with
 * optimization algorithms to solve complex trajectory planning problems in the
 * presence of obstacles. The algorithms work together to find collision-free
 * paths that minimize total travel distance while visiting all required points.
 *
 * In the application, you can add objects using window forms, interact with the
 * trajectory calculation plot using the mouse cursor, create and open files in
 * `.json` format with a specific style for this application. Editing objects
 * can also be done with cursors or using a special dynamic input field with
 * tables, opened in a separate window mode or embedded in the main one.
 */

// Qt libs:
#include <QApplication>

// our code libs:
#include "mainwindow.h"  // lovely window :)

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;

  w.show();
  return a.exec();
}
