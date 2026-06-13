import pandas as pd
import matplotlib.pyplot as plt

def plot_kinetics(csv_file):
    print(f"Loading analytical data from {csv_file}...")
    
    # Load the metrics data
    try:
        df = pd.read_csv(csv_file)
    except FileNotFoundError:
        print(f"Error: {csv_file} not found. Make sure your C++ simulation has run completely.")
        return

    # Set up the figure with two subplots (one for reactants, one for products)
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8), sharex=True)
    
    # --- Top Plot: The Reactants (Electrolyte & Active Lithium) ---
    ax1.plot(df['Step'], df['LiFSI'], label='LiFSI Salt', color='#FFA500', linewidth=2)
    ax1.plot(df['Step'], df['FSDEE'], label='FSDEE Solvent', color='#8B4513', linewidth=2)
    
    # Use a secondary Y-axis for Lithium Metal since its count is much lower
    ax1_twin = ax1.twinx()
    ax1_twin.plot(df['Step'], df['Li_Metal'], label='Active Li Metal', color='#DDA0DD', linewidth=2, linestyle='--')
    ax1_twin.set_ylabel('Li Metal Count', color='#DDA0DD', fontweight='bold')
    ax1_twin.tick_params(axis='y', labelcolor='#DDA0DD')

    ax1.set_title('Electrolyte Reduction Kinetics', fontsize=14, fontweight='bold')
    ax1.set_ylabel('Molecule Count')
    ax1.grid(True, linestyle=':', alpha=0.6)
    
    # Combine legends for the top plot
    lines_1, labels_1 = ax1.get_legend_handles_labels()
    lines_twin, labels_twin = ax1_twin.get_legend_handles_labels()
    ax1.legend(lines_1 + lines_twin, labels_1 + labels_twin, loc='center right')

    # --- Bottom Plot: The Products (Inorganic SEI Growth) ---
    ax2.plot(df['Step'], df['F_Ion'], label='Fluorine Ions (LiF formation)', color='#32CD32', linewidth=2)
    ax2.plot(df['Step'], df['O_Ion'], label='Oxygen Ions (Li2O formation)', color='#FF0000', linewidth=2)
    
    ax2.set_title('Inorganic SEI Accumulation', fontsize=14, fontweight='bold')
    ax2.set_xlabel('Simulation Step (Monte Carlo Time)')
    ax2.set_ylabel('Ion Count')
    ax2.grid(True, linestyle=':', alpha=0.6)
    ax2.legend(loc='upper left')

    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    plot_kinetics('metrics.csv')