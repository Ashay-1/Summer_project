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

    simBox.exportToCSV("trajectory.csv", 0);
    simBox.exportMetrics("metrics.csv", 0, engine.getCurrentTime());

    // Cranked up to 2 Million Steps for macroscopic SEI growth
    int totalSteps = 2000000; 
    std::cout << "\nStarting KMC FRM Loop (" << totalSteps << " steps)..." << std::endl;
    std::cout << "This may take a minute or two. Let the CPU work!" << std::endl;
    
    for (int step = 1; step <= totalSteps; ++step) {
        engine.executeFRMStep();
        
        // Export metrics continuously but less frequently to keep file size manageable
        if (step % 5000 == 0) {
            simBox.exportMetrics("metrics.csv", step, engine.getCurrentTime());
        }

        // Export the heavy 3D frame at 0%, 50%, and 100% completion
        if (step % 1000000 == 0) {
            std::cout << "Progress: " << (step / 20000) << "% | Sim Time: " 
                      << engine.getCurrentTime() << " seconds" << std::endl;
            simBox.exportToCSV("trajectory.csv", step);
        }
    }

    std::cout << "=== Simulation Complete. Data saved to CSVs ===" << std::endl;
    return 0;
}
