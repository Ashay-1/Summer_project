import pandas as pd
import matplotlib.pyplot as plt

def plot_battery_efficiency(csv_file):
    print(f"Loading final metrics from {csv_file}...")
    df = pd.read_csv(csv_file)
    
    # 1. Calculate the Lithium Inventory
    # The initial pristine slab had exactly 600 Li atoms. 
    # Any missing Li_Metal atoms have been oxidized into Li+ within the SEI.
    INITIAL_LI = 600
    df['Trapped_Li_Plus'] = INITIAL_LI - df['Li_Metal']
    
    # Calculate the Surface Li+ / Li0 ratio
    df['Li_Ratio'] = df['Trapped_Li_Plus'] / df['Li_Metal']
    
    # Calculate the Inorganic Phase Ratio (Li2O / LiF)
    # Adding a tiny float to prevent division by zero in the very first step
    df['Phase_Ratio'] = df['O_Ion'] / (df['F_Ion'] + 1e-9)

    # 2. Set up the dual-axis plot
    fig, ax1 = plt.subplots(figsize=(10, 6))

    # Left Axis: Li+ / Li0 Ratio (Electrochemical Activity)
    color1 = '#FF8C00' # Dark Orange
    ax1.set_xlabel('Simulation Step (Monte Carlo Time)', fontweight='bold')
    ax1.set_ylabel('Surface $Li^+ / Li^0$ Ratio', color=color1, fontweight='bold')
    ax1.plot(df['Step'], df['Li_Ratio'], color=color1, linewidth=2, label='Lithium Depletion')
    ax1.tick_params(axis='y', labelcolor=color1)
    ax1.grid(True, linestyle=':', alpha=0.6)

    # Right Axis: Li2O / LiF Ratio (SEI Composition)
    ax2 = ax1.twinx()  
    color2 = '#4169E1' # Royal Blue
    ax2.set_ylabel('$Li_2O / LiF$ Ratio', color=color2, fontweight='bold')
    ax2.plot(df['Step'], df['Phase_Ratio'], color=color2, linewidth=2, label='Inorganic Phase Growth')
    ax2.tick_params(axis='y', labelcolor=color2)

    plt.title('Interfacial Electrodeposition Dynamics & SEI Evolution', fontsize=14, fontweight='bold')
    fig.tight_layout()
    plt.show()

if __name__ == "__main__":
    plot_battery_efficiency('metrics.csv')
