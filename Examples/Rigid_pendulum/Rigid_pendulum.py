import numpy as np
import matplotlib.pyplot as plt

# Load the .mov file
data = np.loadtxt("Rigid_pendulum_out.mov", skiprows=1)  # Skip header if present

# Extract time (assuming constant time step of 1e-3 s)
time_step = 1e-3  # From the input file
time = data[:, 0] * time_step  # Assuming first column is step number or adjust if time is explicit

# Filter data for each mass node
nodes = [1002, 2002, 3002]  # Mass nodes from the three cases
z_positions = {}
for node in nodes:
    node_data = data[data[:, 0] == node]  # Filter rows by node label
    z_positions[node] = node_data[:, 3]   # Z-position is column 4 (index 3)

# Plot Z-position vs. time for each node
plt.figure(figsize=(10, 6))
for node in nodes:
    plt.plot(time[:len(z_positions[node])], z_positions[node], 
             label=f"Node {node} (Z-position)", linewidth=1.5)

# Customize the plot
plt.xlabel("Time (s)")
plt.ylabel("Z Position (m)")
plt.title("Rigid Pendulum: Z-Position of Mass Nodes vs. Time")
plt.grid(True)
plt.legend()
plt.tight_layout()

# Show the plot
plt.show()