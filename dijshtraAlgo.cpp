#include <graphics.h>
#include <vector>
#include <set>
#include <limits>
#include <iostream>
#include <thread>
#include <chrono>
#include <stack>

using namespace std;

// Forward declaration of drawGraph function
void drawGraph(const vector<vector<pair<int, int>>>& graph, const vector<int>& distances, const vector<int>& previous, int nodeCount, int shortestPathEnd);

// Function to perform Dijkstra's algorithm
vector<int> dijkstra(int source, const vector<vector<pair<int, int>>>& graph, int nodeCount, vector<int>& previous) {
    vector<int> distance(nodeCount, numeric_limits<int>::max());
    set<pair<int, int>> minHeap; // {distance, vertex}

    distance[source] = 0;
    minHeap.insert({0, source});

    while (!minHeap.empty()) {
        int u = minHeap.begin()->second;
        minHeap.erase(minHeap.begin());

        // Relaxation step
        for (const auto& neighbor : graph[u]) {
            int v = neighbor.first;
            int weight = neighbor.second;

            if (distance[u] + weight < distance[v]) {
                minHeap.erase({distance[v], v});
                distance[v] = distance[u] + weight;
                previous[v] = u; // Keep track of the path
                minHeap.insert({distance[v], v});
            }
        }

        // Visualization step
        delay(2000); // Increased delay for better visualization
        drawGraph(graph, distance, previous, nodeCount, -1);
    }

    return distance;
}

// Function to draw nodes and edges
void drawGraph(const vector<vector<pair<int, int>>>& graph, const vector<int>& distances, const vector<int>& previous, int nodeCount, int shortestPathEnd) {
    cleardevice(); // Clear the screen

    // Set a background color or image
    setbkcolor(DARKGRAY);
    cleardevice();

    // Draw nodes
    for (int i = 0; i < nodeCount; ++i) {
        int x = 100 + i * 70; // X-position of the node
        int y = 300 + (i % 3) * 50; // Y-position to make it staggered

        // Draw a filled circle for the node
        if (distances[i] < numeric_limits<int>::max()) {
            setfillstyle(SOLID_FILL, GREEN);
        } else {
            setfillstyle(SOLID_FILL, RED);
        }
        floodfill(x, y, WHITE); // Filled node for better visibility

        // Display distance from the source
        char buffer[10];
        sprintf(buffer, "%d", distances[i]);
        setcolor(WHITE);
        outtextxy(x - 10, y - 10, buffer);
    }

    // Draw edges and their weights
    for (int i = 0; i < nodeCount; ++i) {
        for (const auto& neighbor : graph[i]) {
            int v = neighbor.first;
            int weight = neighbor.second;
            int x1 = 100 + i * 70; // X-position for node i
            int y1 = 300 + (i % 3) * 50; // Y-position for node i
            int x2 = 100 + v * 70; // X-position for neighbor
            int y2 = 300 + (v % 3) * 50; // Y-position for neighbor

            // Draw line for the edge
            setcolor(WHITE);
            setlinestyle(SOLID_LINE, 0, 2); // Thicker line for better visibility
            line(x1, y1, x2, y2);

            // Calculate the midpoint for displaying the weight
            int midX = (x1 + x2) / 2;
            int midY = (y1 + y2) / 2;

            // Display the weight
            char weightBuffer[10];
            sprintf(weightBuffer, "%d", weight);
            setcolor(YELLOW);
            outtextxy(midX - 10, midY - 10, weightBuffer); // Display weight above the edge
        }
    }

    // Highlight the path
    if (shortestPathEnd != -1) {
        setcolor(YELLOW);
        setlinestyle(SOLID_LINE, 0, 3); // Thicker line for the path

        for (int i = shortestPathEnd; i != -1; i = previous[i]) {
            if (previous[i] != -1) {
                int x1 = 100 + previous[i] * 70; // X-position for previous node
                int y1 = 300 + (previous[i] % 3) * 50; // Y-position for previous node
                int x2 = 100 + i * 70; // X-position for current node
                int y2 = 300 + (i % 3) * 50; // Y-position for current node
                line(x1, y1, x2, y2);
            }
        }

        // Print the shortest path
        stack<int> path;
        for (int at = shortestPathEnd; at != -1; at = previous[at]) {
            path.push(at);
        }

        // Display the path
        char pathBuffer[100] = "Shortest path: ";
        while (!path.empty()) {
            char nodeBuffer[5];
            sprintf(nodeBuffer, "%d ", path.top());
            strcat(pathBuffer, nodeBuffer);
            path.pop();
        }
        setcolor(WHITE);
        outtextxy(10, 10, pathBuffer); // Display the shortest path
    }

    // Show current state of distances
    const char* msg = "Current distances:";
    setcolor(WHITE);
    outtextxy(10, 40, (char*)msg);  // Cast to char*
    for (int i = 0; i < nodeCount; ++i) {
        char buf[30];
        sprintf(buf, "Node %d: %d", i, distances[i]);
        outtextxy(10, 60 + i * 20, buf);
    }
}

// Function to print the shortest path from source to destination
void printShortestPath(int destination, const vector<int>& previous) {
    stack<int> path;
    for (int at = destination; at != -1; at = previous[at]) {
        path.push(at);
    }

    cout << "Shortest path from 0 to " << destination << ": ";
    while (!path.empty()) {
        cout << path.top() << " ";
        path.pop();
    }
    cout << endl;
}

int main() {
    // Initialize graphics
    int gd = DETECT, gm;
    char arr[] = "";  // Declare a char array
    initgraph(&gd, &gm, arr);  // Use the char array for initialization

    // Create a more complex graph using an adjacency list
    vector<vector<pair<int, int>>> graph = {
        {{1, 4}, {2, 1}, {3, 2}, {4, 7}},    // Node 0
        {{0, 4}, {2, 2}, {3, 5}, {4, 1}}, // Node 1
        {{0, 1}, {1, 2}, {3, 8}, {4, 3}}, // Node 2
        {{0, 2}, {1, 5}, {2, 8}, {5, 6}}, // Node 3
        {{0, 7}, {1, 1}, {2, 3}, {5, 5}}, // Node 4
        {{1, 6}, {2, 5}, {3, 6}} // Node 5
    };

    int nodeCount = graph.size();

    // Prepare previous vector for path reconstruction
    vector<int> previous(nodeCount, -1);

    // Draw initial graph
    drawGraph(graph, vector<int>(nodeCount, numeric_limits<int>::max()), previous, nodeCount, -1);

    // Run Dijkstra's algorithm and get distances
    vector<int> distances = dijkstra(0, graph, nodeCount, previous);

    // Print shortest path to the last node (n-th node)
    printShortestPath(nodeCount - 1, previous);

    // Draw the final state of the graph with the shortest path
    drawGraph(graph, distances, previous, nodeCount, nodeCount - 1);

    cout << "Press Enter to exit...";
    cin.ignore(); // Wait for the user to press Enter

    closegraph(); // Close the graphics window
    return 0;
}

