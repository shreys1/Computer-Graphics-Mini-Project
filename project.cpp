#include <graphics.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>
#include <iomanip> // For std::setw

using namespace std;

struct Process {
    int id;        // Process ID
    int arrival;   // Arrival Time
    int burst;     // Burst Time
    int completion;// Completion Time
    int turnaround; // Turnaround Time
    int waiting;   // Waiting Time
};

// Function to convert string to char*
char* stringToChar(const string& str) {
    char* cstr = new char[str.length() + 1]; // Allocate memory for C-style string
    strcpy(cstr, str.c_str());                // Copy string content
    return cstr;                              // Return C-style string
}

void drawProcess(int x, int y, int width, const string& name) {
    setcolor(WHITE);
    rectangle(x, y, x + width, y + 40); // Draw the rectangle for the process
    setcolor(BLUE);
    line(x, y + 40, x + width, y + 40); // Draw bottom line
    setcolor(WHITE);

    char* nameCstr = stringToChar(name);  // Convert to C-style string
    int textWidth = textwidth(nameCstr);   // Get the width of the text
    int textX = x + (width - textWidth) / 2; // Center the text
    outtextxy(textX, y + 10, nameCstr);  // Display process name
    delete[] nameCstr;                     // Free allocated memory
}

void displayResults(const vector<Process>& processes) {
    cout << left << setw(10) << "Process" << setw(10) << "Arrival" << setw(10) 
         << "Burst" << setw(15) << "Completion" << setw(15) << "Turnaround" << setw(10) << "Waiting" << endl;

    for (const auto& p : processes) {
        cout << left << setw(10) << p.id << setw(10) << p.arrival << setw(10) 
             << p.burst << setw(15) << p.completion << setw(15) << p.turnaround << setw(10) << p.waiting << endl;
    }
}

int main() {
    int gd = DETECT, gm;
    char arr[] = "";  // This can be left empty
    initgraph(&gd, &gm, arr); // Initialize graphics

    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> processes(n);
    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        cout << "Enter arrival time and burst time for process " << i + 1 << ": ";
        cin >> processes[i].arrival >> processes[i].burst;
    }

    // Sort processes by arrival time
    sort(processes.begin(), processes.end(), [](Process a, Process b) {
        return a.arrival < b.arrival;
    });

    int currentTime = 0;
    for (int i = 0; i < n; i++) {
        if (currentTime < processes[i].arrival) {
            currentTime = processes[i].arrival;
        }
        processes[i].completion = currentTime + processes[i].burst;
        processes[i].turnaround = processes[i].completion - processes[i].arrival;
        processes[i].waiting = processes[i].turnaround - processes[i].burst;
        currentTime += processes[i].burst;
    }

    // Display the processes in the console
    displayResults(processes);

    // Draw Gantt Chart with animation
    int x = 50, y = 100, width = 80; // Reduced width for better spacing
    for (const auto& p : processes) {
        drawProcess(x, y, width, "P" + to_string(p.id));
        delay(1000); // Delay for 1 second to animate process execution
        x += width + 10; // Add space between processes
    }

    // Draw x-axis for the Gantt chart
    setcolor(WHITE);
    line(50, y + 40, x - 10, y + 40); // Draw the x-axis

    // Draw completion times on the Gantt chart
    x = 50; // Reset x for drawing times
    for (const auto& p : processes) {
        char time[10];
        sprintf(time, "%d", p.completion);
        outtextxy(x + width / 2 - textwidth(time) / 2, y + 45, time); // Center completion time
        x += width + 10; // Move to the next position
    }

    // Wait for user input to close the graphics window
    cout << "Press Enter to exit...";
    cin.ignore();  // Ignore the newline character from the previous input
    cin.get();     // Wait for user to press Enter

    closegraph();
    return 0;
}

