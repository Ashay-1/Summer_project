import matplotlib.pyplot as plt
import pandas as pd
from mpl_toolkits.mplot3d import Axes3D

def plot_simulation_frame(csv_file, target_step):
    print(f"Loading data from {csv_file}...")
    
    # Load the exported C++ trajectory data
    df = pd.read_csv(csv_file)
    
    # Filter the data for the specific time step we want to view
    frame_data = df[df['Step'] == target_step]
    
    if frame_data.empty:
        print(f"No data found for Step {target_step}.")
        return

    # Define the color mapping based on the paper's Figure 4 aesthetics
    color_map = {
        'Li_Metal': '#DDA0DD',  # Light Purple for active reduced lithium
        'LiFSI': '#FFA500',     # Orange for salt particles
        'FSDEE': '#8B4513',     # Brown/Dark for solvent particles
        'Li_Ion': '#800080',    # Dark Purple for bound Li+
        'F_Ion': '#32CD32',     # Lime Green for Fluorine (LiF formation)
        'O_Ion': '#FF0000'      # Red for Oxygen (Li2O formation)
    }

    # Set up the 3D plot
    fig = plt.figure(figsize=(10, 8))
    ax = fig.add_subplot(111, projection='3d')
    
    # Plot each species group
    for species, color in color_map.items():
        subset = frame_data[frame_data['Species'] == species]
        if not subset.empty:
            ax.scatter(subset['X'], subset['Y'], subset['Z'], 
                       c=color, label=species, s=50, alpha=0.8, edgecolors='k', linewidth=0.5)

    # Format the axes
    ax.set_title(f"KMC SEI Simulation - Step {target_step}", fontsize=14, pad=20)
    ax.set_xlabel('X Coordinate (Å)')
    ax.set_ylabel('Y Coordinate (Å)')
    ax.set_zlabel('Z Coordinate (Å)')
    
    # Remove the background grid for a cleaner "computational chemistry" look
    ax.grid(False)
    

    # Add a legend outside the main plot area
    ax.legend(loc='center left', bbox_to_anchor=(1.05, 0.5), markerscale=1.5)
    
    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    # Point this to your generated file and select the final step (100)
    plot_simulation_frame('trajectory.csv', target_step=150)