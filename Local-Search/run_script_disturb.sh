#!/bin/bash

# instructions for running:
# $ cd project-directory/
# $ chmod +x run_script_disturb.sh
# $ ./run_script_disturb

# Remove existing nohup.out file
# rm -f nohup.out

result_directory="results/sparse_data_60%/disturb-numOfNode-0.7-7-3000ms-3nodeLeast"

mkdir -p "$result_directory"

# Compile the C++ program
g++ main.cpp Algorithm.cpp path.cpp readData.cpp -o main -std=c++17

# Run the program with different arguments 
nohup ./main sparse_data_60%/10_nodes_sparse $result_directory/10_nodes.csv data_files/sparse_data_60%/optimalProfit.csv false true true &
nohup ./main sparse_data_60%/20_nodes_sparse $result_directory/20_nodes.csv data_files/sparse_data_60%/optimalProfit.csv false true true &
nohup ./main sparse_data_60%/30_nodes_sparse $result_directory/30_nodes.csv data_files/sparse_data_60%/optimalProfit.csv false true true &
nohup ./main sparse_data_60%/40_nodes_sparse $result_directory/40_nodes.csv data_files/sparse_data_60%/optimalProfit.csv false true true &
nohup ./main sparse_data_60%/50_nodes_sparse $result_directory/50_nodes.csv data_files/sparse_data_60%/optimalProfit.csv false true true &
nohup ./main sparse_data_60%/60_nodes_sparse $result_directory/60_nodes.csv data_files/sparse_data_60%/optimalProfit.csv false true true &
nohup ./main sparse_data_60%/70_nodes_sparse $result_directory/70_nodes.csv data_files/sparse_data_60%/optimalProfit.csv false true true &
nohup ./main sparse_data_60%/80_nodes_sparse $result_directory/80_nodes.csv data_files/sparse_data_60%/optimalProfit.csv false true true &
# nohup ./main big_files/100_nodes/ results/disturbLocal-Search/100_nodes.csv false true true &
# nohup ./main big_files/150_nodes/ results/disturbLocal-Search/150_nodes.csv false true true &

# Capture the process ID of the last background command
last_bg_pid=$!

# Wait for the background process to complete
wait $last_bg_pid

# Notify that the program has finished running
echo "Program execution is complete!"