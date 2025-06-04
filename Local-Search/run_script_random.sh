#!/bin/bash

# instructions for running:
# $ cd project-directory/
# $ chmod +x run_script_random.sh
# $ ./run_script_random.sh

result_directory="results/seed42/complete-randomized/disturb-numOfNode-0.7-7-3000ms-3nodeLeast"

mkdir -p "$result_directory"

# Compile the C++ program
g++ randomize_main.cpp Algorithm.cpp path.cpp readData.cpp Randomize.cpp -o main -std=c++17

# Run the program with different arguments
nohup ./main data_files/BPMP_Data_Files/10_nodes $result_directory/random_10.csv data_files/BPMP_Data_Files/optimalProfit.csv random &
nohup ./main data_files/BPMP_Data_Files/20_nodes $result_directory/random_20.csv data_files/BPMP_Data_Files/optimalProfit.csv random &
nohup ./main data_files/BPMP_Data_Files/30_nodes $result_directory/random_30.csv data_files/BPMP_Data_Files/optimalProfit.csv random &
nohup ./main data_files/BPMP_Data_Files/40_nodes $result_directory/random_40.csv data_files/BPMP_Data_Files/optimalProfit.csv random &
nohup ./main data_files/BPMP_Data_Files/50_nodes $result_directory/random_50.csv data_files/BPMP_Data_Files/optimalProfit.csv random &
nohup ./main data_files/BPMP_Data_Files/60_nodes $result_directory/random_60.csv data_files/BPMP_Data_Files/optimalProfit.csv random &
nohup ./main data_files/BPMP_Data_Files/70_nodes $result_directory/random_70.csv data_files/BPMP_Data_Files/optimalProfit.csv random &
nohup ./main data_files/BPMP_Data_Files/80_nodes $result_directory/random_80.csv data_files/BPMP_Data_Files/optimalProfit.csv random &

# Capture the process ID of the last background command
last_bg_pid=$!

# Wait for the background process to complete
wait $last_bg_pid

# Notify that the program has finished running
echo "Program execution is complete!"