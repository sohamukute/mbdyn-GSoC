import numpy as np
import matplotlib.pyplot as plt
import subprocess
import os

# Step 1: Run MBDyn simulation
def run_mbdyn():
    if not os.path.exists("pid.mbd"):
        print("Error: pid.mbd not found!")
        return False
    try:
        subprocess.run(["mbdyn", "-f", "pid.mbd", "-o", "pid_out"], check=True)
        print("MBDyn simulation completed successfully.")
        return True
    except subprocess.CalledProcessError as e:
        print(f"Error running MBDyn: {e}")
        return False

# Step 2: Load and process data
def load_data():
    # Check if files exist
    if not os.path.exists("pid_out.mov") or not os.path.exists("pid_out.abs"):
        raise FileNotFoundError("Required output files (pid_out.mov or pid_out.abs) not found!")

    # Load structural node data (.mov file)
    mov_data = np.loadtxt("pid_out.mov", skiprows=1)
    # Filter for node 1 (structural node)
    node1_mask = mov_data[:, 0] == 1  # First column is node label
    node1_data = mov_data[node1_mask]
    time = np.arange(0, 10. + 1e-2, 1e-2)[:len(node1_data)]  # Time step = 0.01 s
    position = node1_data[:, 1]  # X[1] (column 2)
    velocity = node1_data[:, 7]  # XP[1] (column 8)

    # Load abstract node data (.abs file)
    abs_data = np.loadtxt("pid_out.abs", skiprows=1)
    # Filter for node 1 (abstract node)
    abs_node1_mask = abs_data[:, 0] == 1
    abs_node1_data = abs_data[abs_node1_mask]
    pid_state = abs_node1_data[:, 1]  # x (column 2)

    # Validate lengths
    if len(position) != len(pid_state):
        raise ValueError(f"Data length mismatch: position ({len(position)}) vs pid_state ({len(pid_state)})")

    return time, position, velocity, pid_state

# Step 3: Calculate PID terms
def calculate_pid_terms(position, velocity, pid_state, KP, KI, KD):
    p_term = KP * position  # Proportional term
    i_term = KI * pid_state  # Integral term
    d_term = KD * velocity  # Derivative term
    control_force = -(p_term + i_term + d_term)  # Total force (negative as per input)
    return p_term, i_term, d_term, control_force

# Step 4: Plotting
def plot_pid_response(time, position, p_term, i_term, d_term, control_force):
    # Create a figure with two subplots
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 10), sharex=True, height_ratios=[1, 1.5])

    # Subplot 1: Position
    ax1.plot(time, position, label="Position (X[1])", color="royalblue", lw=2)
    ax1.axhline(0, color="gray", ls="--", lw=1, alpha=0.5)
    ax1.set_ylabel("Position (m)", fontsize=12)
    ax1.set_title("PID-Controlled System Response", fontsize=14, pad=10)
    ax1.grid(True, ls=":", alpha=0.7)
    ax1.legend(loc="upper right", fontsize=10)

    # Subplot 2: PID Terms and Total Force
    ax2.plot(time, p_term, label=f"P Term (KP = {KP})", color="limegreen", lw=2)
    ax2.plot(time, i_term, label=f"I Term (KI = {KI})", color="coral", lw=2)
    ax2.plot(time, d_term, label=f"D Term (KD = {KD})", color="purple", lw=2)
    ax2.plot(time, control_force, label="Total Control Force", color="black", lw=2.5, ls="-")
    ax2.axhline(0, color="gray", ls="--", lw=1, alpha=0.5)
    ax2.set_xlabel("Time (s)", fontsize=12)
    ax2.set_ylabel("Force (N)", fontsize=12)
    ax2.set_title("PID Controller Contributions", fontsize=14, pad=10)
    ax2.grid(True, ls=":", alpha=0.7)
    ax2.legend(loc="upper right", fontsize=10, ncol=2)

    # Customize the plot
    for ax in [ax1, ax2]:
        ax.tick_params(axis="both", labelsize=10)
        ax.spines["top"].set_visible(False)
        ax.spines["right"].set_visible(False)

    plt.tight_layout()
    plt.savefig("pid_response.png", dpi=300, bbox_inches="tight")
    plt.show()

# Main execution
if __name__ == "__main__":
    # PID gains from the MBDyn file
    KP = 2e1  # 20
    KI = 1e-3  # 0.001
    KD = 1e1  # 10

    # Run simulation and plot
    if run_mbdyn():
        try:
            # Load data
            time, position, velocity, pid_state = load_data()

            # Calculate PID terms
            p_term, i_term, d_term, control_force = calculate_pid_terms(position, velocity, pid_state, KP, KI, KD)

            # Plot the results
            plot_pid_response(time, position, p_term, i_term, d_term, control_force)
        except Exception as e:
            print(f"Error processing data or plotting: {e}")
    else:
        print("Simulation failed. No plot generated.")