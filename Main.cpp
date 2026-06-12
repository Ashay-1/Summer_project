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

    // Initialize fresh CSV files
    std::ofstream clearTraj("trajectory.csv", std::ios::trunc); clearTraj.close();
    std::ofstream clearMetrics("metrics.csv", std::ios::trunc); clearMetrics.close();

    KMCEngine engine(simBox);

    // Export Step 0 for both files
    simBox.exportToCSV("trajectory.csv", 0);
    simBox.exportMetrics("metrics.csv", 0, engine.getCurrentTime());

    std::cout << "\nStarting KMC FRM Loop (150 steps)..." << std::endl;
    
    for (int step = 1; step <= 150; ++step) {
        engine.executeFRMStep();
        
        // Export metrics continuously to get a smooth graph (every 100 steps)
        if (step % 10 == 0) {
            simBox.exportMetrics("metrics.csv", step, engine.getCurrentTime());
        }

        // Export the heavy 3D frame less frequently (every 1,500 steps)
        if (step % 150 == 0) {
            std::cout << "Step " << step << " | Sim Time: " 
                      << engine.getCurrentTime() << " seconds" << std::endl;
            simBox.exportToCSV("trajectory.csv", step);
        }
    }

    std::cout << "=== Simulation Complete. Data saved to CSVs ===" << std::endl;
    return 0;
}