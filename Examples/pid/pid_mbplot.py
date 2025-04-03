import numpy as np
import matplotlib.pyplot as plt

# Load data
mov_data = np.loadtxt("pid_out.mov", skiprows=1)
abs_data = np.loadtxt("pid_out.abs", skiprows=1)

# Filter node 1
mov_node1 = mov_data[mov_data[:, 0] == 1]
abs_node1 = abs_data[abs_data[:, 0] == 1]
time = np.arange(0, 10. + 1e-2, 1e-2)[:len(mov_node1)]

# Plot
plt.plot(time, mov_node1[:, 1], label="Position X[1]")
plt.plot(time, mov_node1[:, 7], label="Velocity XP[1]")
plt.plot(time, abs_node1[:, 1], label="PID State x")
plt.xlabel("Time (s)")
plt.ylabel("Value")
plt.title("PID Simulation Results")
plt.grid(True)
plt.legend()
plt.show()