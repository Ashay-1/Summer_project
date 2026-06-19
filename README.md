# Summer_project

This repository contains C++ source code for a lattice-based kinetic Monte Carlo (KMC) simulation plus Python scripts for plotting and visualizing results.

Files of note
- C++ sources and headers
  - Main.cpp — program entry point
  - Lattice.cpp, Lattice.hpp — lattice implementation
  - KMCEngine.hpp, Kinetics.hpp, Physics.hpp — KMC engine and physics/kinetics headers
  - sei_simulation — prebuilt executable (if present)

- Data and analysis
  - metrics.csv, trajectory.csv — example output data (if present)
  - plot_efficiency.py, plot_metrics.py, visualize_sei.py — Python scripts to plot/visualize results
  - tempCodeRunnerFile.py — temporary/script file

Quickstart — build (Linux / macOS)
1. Install a C++ compiler that supports C++17 (g++ or clang++).
2. From the repository root, compile the project with:

```sh
# Compile Main.cpp and Lattice.cpp into an executable named sei_simulation
g++ -std=c++17 -O2 Main.cpp Lattice.cpp -o sei_simulation
```

3. If the repository already contains a compiled `sei_simulation` file you can run that directly (remove execute bit if necessary):

```sh
# Make sure the executable is runnable, then run
chmod +x sei_simulation
./sei_simulation
```

Notes on running
- The exact command-line options for the C++ program are not documented here. If Main.cpp parses command-line arguments, open `Main.cpp` to see expected flags/inputs.
- Running the simulation typically produces CSV output files (for example `metrics.csv` and `trajectory.csv`) in the repository root — the included Python scripts use CSV output to create plots.

Python visualization
1. Install Python 3 and required packages. At minimum install numpy, pandas and matplotlib:

```sh
pip install numpy pandas matplotlib
```

2. Run the plotting/visualization scripts (they will likely read `metrics.csv` or `trajectory.csv`):

```sh
python plot_metrics.py
python plot_efficiency.py
python visualize_sei.py
```

If a script requires command-line arguments, open the script to see the usage and pass appropriate filenames.

Repository structure
- Main.cpp — main program
- Lattice.cpp, Lattice.hpp — lattice implementation
- KMCEngine.hpp, Kinetics.hpp, Physics.hpp — KMC engine and supporting headers
- sei_simulation — optional prebuilt executable
- *.py — plotting/visualization scripts
- *.csv — example output data

Contributing
- Please open issues or pull requests if you want to add build scripts, CI, more documentation, or a requirements file for Python dependencies.

License
- No license specified. If you want this repository to be open-source, add a LICENSE file (for example MIT or Apache-2.0).

Contact
- Repository owner: @Ashay-1

