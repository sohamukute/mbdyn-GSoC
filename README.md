# README for MBDyn Tasks (PID Control and Rigid Pendulum)

## Introduction
This repository contains two tasks completed for MBDyn:
- **Task 1**: Simulations of a PID controller and a rigid pendulum to study their dynamics and behavior.
- **Task 2**: Modification of the PID controller by adding a feedforward gain to enhance system performance.

Check Examples folder for the work done.

Each task includes MBDyn input files, Python scripts for analysis, and output files to demonstrate the results.

---

## Task 1: PID Control & Rigid Pendulum

### PID Control Simulation
#### Description
This task implements a **PID controller** (Proportional-Integral-Derivative) in MBDyn to regulate a system's response. It explores how tuning the parameters `Kp` (proportional), `Ki` (integral), and `Kd` (derivative) affects system stability and performance.

#### Files and Their Purpose
- `pid.mbd`: MBDyn input file defining the standard PID system.
- `pid.py`: Python script to process and analyze simulation data.
- `pid_mbplot.py`: Python script to generate plots from the simulation output.
- `pid.log`: Log file from the MBDyn simulation run.
- `pid.out`: Raw output data from the simulation.
- `pid_out.mov`: Visualization file showing the system's motion.
- `pid_response.png`: Graph of the PID controller's response.

#### Running the Simulation
1. Execute the MBDyn simulation:
   ```bash
   mbdyn -f pid.mbd -o pid_out
   ```
2. Analyze and visualize the results:
   ```bash
   python3 pid.py
   python3 pid_mbplot.py
   ```
3. View the motion in `pid_out.mov` and the response graph in `pid_response.png`.

---

### Rigid Pendulum Simulation
#### Description
This task simulates a **rigid pendulumGOOD** under gravity to analyze its oscillatory motion. It demonstrates MBDyn’s ability to model multibody dynamics and provides insights into pendulum behavior.

#### Files and Their Purpose
- `Rigid_pendulum.mbd`: MBDyn input file defining the pendulum system.
- `Rigid_pendulum.py`: Python script for post-processing and analyzing simulation data.
- `Rigid_pendulum_out.mov`: Visualization of the pendulum’s motion.
- `Rigid_pendulum_out.log`: Log file from the simulation.
- `Rigid_pendulum_out.out`: Raw output data from the simulation.

#### Running the Simulation
1. Run the MBDyn simulation:
   ```bash
   mbdyn -f Rigid_pendulum.mbd -o Rigid_pendulum_out
   ```
2. Process and visualize the results:
   ```bash
   python3 Rigid_pendulum.py
   ```
3. Open `Rigid_pendulum_out.mov` to view the pendulum’s motion.

---

## Task 2: PID with Feedforward Gain

### Description
This task extends the PID controller from Task 1 by adding a **feedforward gain (`Kff`)**. The feedforward component anticipates control inputs, improving the system’s response compared to the standard feedback-only PID.

### Changes from Standard PID
- Modified `pid.mbd` to include `Kff` alongside `Kp`, `Ki`, and `Kd`.
- Analyzed the improved system response and compared it to the standard PID.

### Files and Their Purpose
- `pid.mbd` (modified): Updated MBDyn input file with feedforward gain.
- `pid.log`, `pid.out`, `pid_out.mov`: Updated simulation outputs reflecting the modified PID.
- `pid_response_modified.png`: Graph showing the response with feedforward gain.

### Running the Simulation
1. Run the modified simulation:
   ```bash
   mbdyn -f pid.mbd -o pid_out
   ```
2. Analyze and compare the results:
   ```bash
   python3 pid.py
   python3 pid_mbplot.py
   ```
3. Compare `pid_response.png` (standard PID) with `pid_response_modified.png` (with feedforward).

---

## Prerequisites
- **MBDyn**: Installed and configured to run simulations.
- **Python 3**: Required for running analysis and plotting scripts (ensure dependencies like `matplotlib` are installed).

## Installation
1. Clone this repository:
   ```bash
   git clone <repository-url>
   ```
2. Navigate to the repository directory:
   ```bash
   cd <repository-name>
   ```
3. Ensure MBDyn and Python 3 are installed on your system.

## Conclusion
- **Task 1**: Successfully simulated a PID controller and a rigid pendulum, providing insights into their dynamics using MBDyn and Python analysis.
- **Task 2**: Enhanced the PID controller with a feedforward gain, demonstrating improved system performance.

For additional details, refer to the [MBDyn GitLab Wiki](https://www.mbdyn.org/?Documentation___Official_Documentation___Wiki).

---

