#ifndef LATTICE_HPP
#define LATTICE_HPP

#include <vector>
#include <iostream>
#include <fstream>  // Adds file writing capabilities
#include <string>   // Adds string handling

// 1. The Physical States:
enum class Species {
    Vacancy,       // Empty space (crucial for diffusion)
    Li_Metal,      // Reduced Li^0 in the anode slab
    Li_Ion,        // Active Li^+ in the electrolyte/SEI
    F_Ion,         // F^- from LiFSI decomposition
    O_Ion,         // O^2- from extended decomposition
    FSDEE_Solv,    // Intact solvent molecule
    LiFSI_Salt     // Intact salt molecule
    // Note: We can add the intermediate amorphous species later
};

// 2. The Crystallographic State:
enum class SiteType {
    BCC_Basis,        // Sites for the Lithium metal slab
    FCC_Basis,        // Base sites for inorganic SEI (LiF, Li2O)
    FCC_Octahedral,   // Interstitial sites
    FCC_Tetrahedral   // Interstitial sites
};

// 3. The Fundamental Unit: A single point in space
class LatticeSite {
public:
    int i, j, k;             // Grid indices (e.g., 0, 1, 2...)
    double x, y, z;          // True physical coordinates in Angstroms
    Species currentSpecies;  // What is currently occupying this site
    SiteType type;           // The crystallographic nature of the site

    // Constructor to initialize a site
    LatticeSite(int _i, int _j, int _k, double _x, double _y, double _z, SiteType _type)
        : i(_i), j(_j), k(_k), x(_x), y(_y), z(_z), 
          currentSpecies(Species::Vacancy), type(_type) {}
};

// 4. The Simulation Space: The 3D Grid
class SimulationBox {
private:
    int dimX, dimY, dimZ;    // Grid dimensions (multipliers for a0)
    double a0;               // Lattice parameter (4.47 Angstroms)
    
    // A 1D vector acting as a 3D grid for maximum memory efficiency
    std::vector<LatticeSite> grid;

public:
    // Constructor
    SimulationBox(int _dimX, int _dimY, int _dimZ, double _a0 = 4.47)
        : dimX(_dimX), dimY(_dimY), dimZ(_dimZ), a0(_a0) {
        
        // Pre-allocate exact memory to prevent costly reallocations during setup
        grid.reserve(dimX * dimY * dimZ);
        buildEmptyLattice();
    }

    // Helper function to map 3D indices (i,j,k) to the 1D vector index
    int getIndex(int i, int j, int k) const {
        return i + dimX * (j + dimY * k);
    }

    // Core Initialization Functions (We will write the logic for these next)
    void buildEmptyLattice();
    void initializeLiSlab(int numLayers = 5);
    void initializeElectrolyte(double saltMolarity, double solvMolarity);
    
    // Debugging utility
    void printGridStats() const;
    // Physical dimension getters for the PBC calculations
    double getBoxLengthX() const { return dimX * a0; }
    double getBoxLengthY() const { return dimY * a0; }
    double getBoxLengthZ() const { return dimZ * a0; }
    
    // Getter to expose the grid for kinetics calculations
    const std::vector<LatticeSite>& getGrid() const { return grid; }

    // Safely swaps the chemical species of two sites (used for diffusion)
    void swapSpecies(int index1, int index2) {
        Species temp = grid[index1].currentSpecies;
        grid[index1].currentSpecies = grid[index2].currentSpecies;
        grid[index2].currentSpecies = temp;
    }

    // Exports the current atomic coordinates to a CSV file
    void exportToCSV(const std::string& filename, int step) const;
    // Exports the analytical counts of the grid to track reaction kinetics
    void exportMetrics(const std::string& filename, int step, double time) const;

    // Transforms electrolyte into inorganic SEI precursors upon reduction
    void reduceSpecies(int index) {
        if (grid[index].currentSpecies == Species::LiFSI_Salt) {
            grid[index].currentSpecies = Species::F_Ion; // Defluorination
        } else if (grid[index].currentSpecies == Species::FSDEE_Solv) {
            grid[index].currentSpecies = Species::O_Ion; // Organic breakdown
        }
    }
};

#endif