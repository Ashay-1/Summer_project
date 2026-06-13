#include "Lattice.hpp"
#include <iostream>
#include <random>
#include <fstream>

void SimulationBox::buildEmptyLattice() {
    std::cout << "Initializing 3D simulation grid..." << std::endl;

    for (int k = 0; k < dimZ; ++k) {
        for (int j = 0; j < dimY; ++j) {
            for (int i = 0; i < dimX; ++i) {
                
                double baseX = i * a0;
                double baseY = j * a0;
                double baseZ = k * a0;

                // --- 1. OVERLAY BASE SITES ---
                // Base Corner (Shared FCC/BCC)
                grid.push_back(LatticeSite(i, j, k, baseX, baseY, baseZ, SiteType::BCC_Basis));
                
                // BCC Center (Used for the Lithium Metal Slab)
                grid.push_back(LatticeSite(i, j, k, baseX + (0.5 * a0), baseY + (0.5 * a0), baseZ + (0.5 * a0), SiteType::BCC_Basis));

                // FCC Face-Centered Sites (Used for inorganic SEI like LiF/Li2O)
                grid.push_back(LatticeSite(i, j, k, baseX + (0.5 * a0), baseY + (0.5 * a0), baseZ, SiteType::FCC_Basis));
                grid.push_back(LatticeSite(i, j, k, baseX + (0.5 * a0), baseY, baseZ + (0.5 * a0), SiteType::FCC_Basis));
                grid.push_back(LatticeSite(i, j, k, baseX, baseY + (0.5 * a0), baseZ + (0.5 * a0), SiteType::FCC_Basis));

                // --- 2. FCC OCTAHEDRAL SITES (4 per unit cell) ---
                // The body center (0.5, 0.5, 0.5) is an octahedral site, but we already mapped it as a BCC basis.
                // The other 3 unique octahedral sites sit at the edge centers.
                grid.push_back(LatticeSite(i, j, k, baseX + (0.5 * a0), baseY, baseZ, SiteType::FCC_Octahedral));
                grid.push_back(LatticeSite(i, j, k, baseX, baseY + (0.5 * a0), baseZ, SiteType::FCC_Octahedral));
                grid.push_back(LatticeSite(i, j, k, baseX, baseY, baseZ + (0.5 * a0), SiteType::FCC_Octahedral));

                // --- 3. FCC TETRAHEDRAL SITES (8 per unit cell) ---
                // These sit at the 1/4 and 3/4 marks of the unit cell diagonals.
                double q = 0.25 * a0; // Quarter
                double t = 0.75 * a0; // Three-quarters

                grid.push_back(LatticeSite(i, j, k, baseX + q, baseY + q, baseZ + q, SiteType::FCC_Tetrahedral));
                grid.push_back(LatticeSite(i, j, k, baseX + t, baseY + q, baseZ + q, SiteType::FCC_Tetrahedral));
                grid.push_back(LatticeSite(i, j, k, baseX + q, baseY + t, baseZ + q, SiteType::FCC_Tetrahedral));
                grid.push_back(LatticeSite(i, j, k, baseX + q, baseY + q, baseZ + t, SiteType::FCC_Tetrahedral));
                
                grid.push_back(LatticeSite(i, j, k, baseX + t, baseY + t, baseZ + q, SiteType::FCC_Tetrahedral));
                grid.push_back(LatticeSite(i, j, k, baseX + t, baseY + q, baseZ + t, SiteType::FCC_Tetrahedral));
                grid.push_back(LatticeSite(i, j, k, baseX + q, baseY + t, baseZ + t, SiteType::FCC_Tetrahedral));
                grid.push_back(LatticeSite(i, j, k, baseX + t, baseY + t, baseZ + t, SiteType::FCC_Tetrahedral));
            }
        }
    }
    std::cout << "Lattice built with " << grid.size() << " total sites." << std::endl;
}

// Populate the bottom of the grid with Lithium metal
void SimulationBox::initializeLiSlab(int numLayers) {
    std::cout << "Depositing Lithium metal slab (" << numLayers << " layers)..." << std::endl;
    int atomsAdded = 0;
    
    for (auto& site : grid) {
        if (site.k < numLayers && site.type == SiteType::BCC_Basis) {
            site.currentSpecies = Species::Li_Metal;
            atomsAdded++;
        }
    }
    std::cout << "Slab initialized with " << atomsAdded << " Li atoms." << std::endl;
}


// 3. Fill the empty space with LiFSI salt and FSDEE solvent
void SimulationBox::initializeElectrolyte(double saltMolarity, double solvMolarity) {
    std::cout << "Injecting electrolyte (Salt: " << saltMolarity << "M, Solvent: " << solvMolarity << "M)..." << std::endl;
    
    // Calculate probabilities based on molarity ratios
    double totalMolarity = saltMolarity + solvMolarity;
    double saltProb = saltMolarity / totalMolarity;
    
    // Set up a random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    
    int saltAdded = 0;
    int solvAdded = 0;
    
    // Iterate through the grid
    for (auto& site : grid) {
        // Only populate empty space (vacancies)
        if (site.currentSpecies == Species::Vacancy) {
            
            // We leave roughly 20% of the space as true "vacancies" to allow 
            // for diffusion and movement during the Monte Carlo steps.
            double fillChance = dis(gen);
            
            if (fillChance <= 0.80) { // 80% chance to fill a site
                double speciesChance = dis(gen);
                
                if (speciesChance <= saltProb) {
                    site.currentSpecies = Species::LiFSI_Salt;
                    saltAdded++;
                } else {
                    site.currentSpecies = Species::FSDEE_Solv;
                    solvAdded++;
                }
            }
        }
    }
    
    std::cout << "Electrolyte initialized with " << saltAdded << " LiFSI and " 
              << solvAdded << " FSDEE molecules." << std::endl;
}

// 4. Updated printGridStats() to include the new molecules
void SimulationBox::printGridStats() const {
    int liCount = 0, vacCount = 0, saltCount = 0, solvCount = 0;
    
    for (const auto& site : grid) {
        if (site.currentSpecies == Species::Li_Metal) liCount++;
        else if (site.currentSpecies == Species::LiFSI_Salt) saltCount++;
        else if (site.currentSpecies == Species::FSDEE_Solv) solvCount++;
        else vacCount++;
    }
    
    std::cout << "\n--- Grid Statistics ---" << std::endl;
    std::cout << "Dimensions: " << dimX << "x" << dimY << "x" << dimZ << std::endl;
    std::cout << "Total Sites:   " << grid.size() << std::endl;
    std::cout << "Li Metal Atoms: " << liCount << std::endl;
    std::cout << "LiFSI Molecules: " << saltCount << std::endl;
    std::cout << "FSDEE Molecules: " << solvCount << std::endl;
    std::cout << "Empty Vacancies: " << vacCount << std::endl;
    std::cout << "-----------------------\n" << std::endl;
}

void SimulationBox::exportToCSV(const std::string& filename, int step) const {
    // Open the file in append mode
    std::ofstream file(filename, std::ios::app);
    
    // If it's the very first step, write the CSV headers
    if (step == 0) {
        file << "Step,X,Y,Z,Species\n";
    }
    
    // Iterate through the grid and export occupied sites
    for (const auto& site : grid) {
        if (site.currentSpecies != Species::Vacancy) {
            file << step << "," << site.x << "," << site.y << "," << site.z << ",";
            
            // Map the enum class to a readable string for Python
            switch(site.currentSpecies) {
                case Species::Li_Metal:   file << "Li_Metal\n"; break;
                case Species::Li_Ion:     file << "Li_Ion\n"; break;
                case Species::F_Ion:      file << "F_Ion\n"; break;
                case Species::O_Ion:      file << "O_Ion\n"; break;
                case Species::LiFSI_Salt: file << "LiFSI\n"; break;
                case Species::FSDEE_Solv: file << "FSDEE\n"; break;
                default:                  file << "Unknown\n"; break;
            }
        }
    }
    
    file.close();
}
void SimulationBox::exportMetrics(const std::string& filename, int step, double time) const {
    std::ofstream file(filename, std::ios::app);
    
    // Write headers on the first step
    if (step == 0) {
        file << "Step,Time_s,Li_Metal,LiFSI,FSDEE,F_Ion,O_Ion\n";
    }
    
    int liCount = 0, saltCount = 0, solvCount = 0, fCount = 0, oCount = 0;
    
    for (const auto& site : grid) {
        switch(site.currentSpecies) {
            case Species::Li_Metal:   liCount++; break;
            case Species::LiFSI_Salt: saltCount++; break;
            case Species::FSDEE_Solv: solvCount++; break;
            case Species::F_Ion:      fCount++; break;
            case Species::O_Ion:      oCount++; break;
            default: break;
        }
    }
    
    // Log the data row
    file << step << "," << time << "," << liCount << "," 
         << saltCount << "," << solvCount << "," << fCount << "," << oCount << "\n";
         
    file.close();
}
