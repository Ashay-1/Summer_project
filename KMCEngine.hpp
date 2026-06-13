#ifndef KMCENGINE_HPP
#define KMCENGINE_HPP

#include "Lattice.hpp"
#include "Kinetics.hpp"
#include <random>
#include <limits>
#include <iostream>
#include <cmath>

// Structure to track the TYPE of event
struct Event {
    int sourceIndex;
    int targetIndex;
    double deltaTime;
    int eventType; // 0 = Diffusion, 1 = Electrolyte Reduction
};

class KMCEngine {
private:
    SimulationBox& simBox;
    double currentTime;
    std::mt19937 gen;
    std::uniform_real_distribution<> dis;

    const double attemptFrequency = 1e13; 

public:
    KMCEngine(SimulationBox& box) : simBox(box), currentTime(0.0), 
                                    gen(std::random_device{}()), dis(0.0, 1.0) {}

    double getCurrentTime() const { return currentTime; }

    void executeFRMStep() {
        const auto& grid = simBox.getGrid();
        Event bestEvent = {-1, -1, std::numeric_limits<double>::max(), -1};
        
        // 1. OPTIMIZATION: Square the cutoff distance to avoid calculating square roots later
        const double hopCutoffSq = 3.9 * 3.9; 

        // 2. SCAN THE GRID FOR ALL POSSIBLE EVENTS
        for (size_t i = 0; i < grid.size(); ++i) {
            const LatticeSite& sourceSite = grid[i];
            Species s = sourceSite.currentSpecies;
            
            // Allow Salt, Solvent, and the freshly reduced Ions to participate
            if (s == Species::LiFSI_Salt || s == Species::FSDEE_Solv || 
                s == Species::F_Ion || s == Species::O_Ion) {
                
                double bindingEnergy = Kinetics::calculateBindingEnergy(
                    i, grid, simBox.getBoxLengthX(), simBox.getBoxLengthY(), simBox.getBoxLengthZ()
                );

                for (size_t j = 0; j < grid.size(); ++j) {
                    if (i == j) continue; 
                    
                    const LatticeSite& targetSite = grid[j];
                    
                    // --- THIS IS WHERE SNIPPET GOES ---
                    // We only care if the target site is a Vacancy (for diffusion) 
                    // OR Lithium Metal (for reduction).
                    if (targetSite.currentSpecies == Species::Vacancy || targetSite.currentSpecies == Species::Li_Metal) {
                        
                        // Calculate squared distance
                        double distSq = Kinetics::calculatePBCDistanceSquared(
                            sourceSite, targetSite, 
                            simBox.getBoxLengthX(), simBox.getBoxLengthY(), simBox.getBoxLengthZ()
                        );
                            
                        // If it is within the nearest-neighbor cutoff
                        if (distSq <= hopCutoffSq) {
                            
                            // EVENT A: REDUCTION (Only if it's Salt/Solvent touching Lithium)
                            if (targetSite.currentSpecies == Species::Li_Metal && 
                               (s == Species::LiFSI_Salt || s == Species::FSDEE_Solv)) {
                                
                                // Near-zero energy barrier (0.01) for spontaneous interfacial reduction
                                double rate = Kinetics::calculateReactionRate(attemptFrequency, 0.01, 0.0, 0.0, 0.0);
                                double xi = 1.0 - dis(gen); 
                                double dt = -std::log(xi) / rate;
                                
                                if (dt < bestEvent.deltaTime) {
                                    bestEvent = { (int)i, (int)j, dt, 1 };
                                }
                            }
                            
                            // EVENT B: DIFFUSION (Moving into an empty vacancy)
                            else if (targetSite.currentSpecies == Species::Vacancy) {
                                
                                // Uses the actual calculated binding energy for the diffusion barrier
                                double rate = Kinetics::calculateReactionRate(attemptFrequency, bindingEnergy, 0.0, 0.0, 0.0);
                                double xi = 1.0 - dis(gen); 
                                double dt = -std::log(xi) / rate;
                                
                                if (dt < bestEvent.deltaTime) {
                                    bestEvent = { (int)i, (int)j, dt, 0 };
                                }
                            }
                        }
                    }
                }
            }
        }

        // 3. EXECUTE THE WINNING EVENT
        if (bestEvent.sourceIndex != -1) {
            currentTime += bestEvent.deltaTime;
            
            if (bestEvent.eventType == 0) {
                simBox.swapSpecies(bestEvent.sourceIndex, bestEvent.targetIndex); // Diffuse
            } else if (bestEvent.eventType == 1) {
                simBox.reduceSpecies(bestEvent.sourceIndex); // Reduce
            }
        }
    }
};

#endif
