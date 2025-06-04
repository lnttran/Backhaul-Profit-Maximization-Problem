#!/bin/bash

# instructions for running:
# $ cd project-directory/
# $ chmod +x run_script_deterministic.sh
# $ ./run_script_deterministic.sh

# Compile the C++ program
g++ main.cpp Algorithm.cpp path.cpp readData.cpp -o main -std=c++17

# Run the program with different arguments 
nohup ./main data_files/BPMP_Data_Files/10_nodes/ results/deterministic/10_nodes.csv data_files/BPMP_Data_Files/optimalProfit.csv false true true &
nohup ./main data_files/BPMP_Data_Files/20_nodes/ results/deterministic/20_nodes.csv data_files/BPMP_Data_Files/optimalProfit.csv false true true &
nohup ./main data_files/BPMP_Data_Files/30_nodes/ results/deterministic/30_nodes.csv data_files/BPMP_Data_Files/optimalProfit.csv false true true &
nohup ./main data_files/BPMP_Data_Files/40_nodes/ results/deterministic/40_nodes.csv data_files/BPMP_Data_Files/optimalProfit.csv false true true &
nohup ./main data_files/BPMP_Data_Files/50_nodes/ results/deterministic/50_nodes.csv data_files/BPMP_Data_Files/optimalProfit.csv false true true &
nohup ./main data_files/BPMP_Data_Files/60_nodes/ results/deterministic/60_nodes.csv data_files/BPMP_Data_Files/optimalProfit.csv false true true &
nohup ./main data_files/BPMP_Data_Files/70_nodes/ results/deterministic/70_nodes.csv data_files/BPMP_Data_Files/optimalProfit.csv false true true &
nohup ./main data_files/BPMP_Data_Files/80_nodes/ results/deterministic/80_nodes.csv data_files/BPMP_Data_Files/optimalProfit.csv false true true &


# Capture the process ID of the last background command
last_bg_pid=$!

# Wait for the background process to complete
wait $last_bg_pid

# Notify that the program has finished running
echo "Program execution is complete!"