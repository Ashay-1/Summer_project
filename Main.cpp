#include "Lattice.hpp"
#include "Kinetics.hpp"
#include "KMCEngine.hpp"
#include <iostream>
#include <fstream>

int main() {
    std::cout << "=== KMC SEI Simulation Engine Initialized ===" << std::endl;

    SimulationBox simBox(10, 10, 10);
    simBox.initializeLiSlab(3);
    simBox.initializeElectrolyte(2.0, 6.5);

    // Initialize fresh CSV files in the current folder
    std::ofstream clearTraj("trajectory.csv", std::ios::trunc); clearTraj.close();
    std::ofstream clearMetrics("metrics.csv", std::ios::trunc); clearMetrics.close();

    KMCEngine engine(simBox);

    // Export Step 0 in the current folder (CRITICAL FOR PANDAS HEADERS)
    simBox.exportToCSV("trajectory.csv", 0);
    simBox.exportMetrics("metrics.csv", 0, engine.getCurrentTime());

    // Scaled down for a sub-60-second test run
    int totalSteps = 1000; 
    std::cout << "\nStarting KMC FRM Loop (" << totalSteps << " steps)..." << std::endl;
    
    for (int step = 1; step <= totalSteps; ++step) {
        engine.executeFRMStep();
        
        // Export metrics every 100 steps
        if (step % 100 == 0) {
            simBox.exportMetrics("metrics.csv", step, engine.getCurrentTime());
        }

        // Export the 3D frame at the end
        if (step % 1000 == 0) {
            std::cout << "Progress: 100% | Sim Time: " 
                      << engine.getCurrentTime() << " seconds" << std::endl;
            simBox.exportToCSV("trajectory.csv", step);
        }
    }

    std::cout << "=== Simulation Complete. Data saved to CSVs ===" << std::endl;
    return 0;
}