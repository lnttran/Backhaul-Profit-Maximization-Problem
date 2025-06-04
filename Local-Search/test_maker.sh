#!/bin/bash

# instructions for running:
# $ cd project-directory/
# $ chmod +x test_maker.sh
# $ ./test_maker.sh

# Compile the C++ program
g++ sparse_test_maker.cpp readData.h readData.cpp -o test_maker -std=c++17

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful."

    # Input and output directoriess
    # modify this 
    input_dir="big_files/150_nodes"
    output_dir="sparse_data_80%/150_nodes_sparse"

    # Create the output directory if it doesn't exist
    mkdir -p "$output_dir"

    # Iterate through input files
    for input_file in "$input_dir"/*; do
        if [ -f "$input_file" ]; then
            # Extract the filename without the directory path and extension
            filename=$(basename -- "$input_file")
            filename_noext="${filename%.*}"

            # Run the program with the input file and output to the corresponding output file
            ./test_maker "$input_file" "$output_dir/$filename_noext.txt"
            
            # Check if the program executed successfully
            if [ $? -eq 0 ]; then
                echo "Processed $input_file"
            else
                echo "Failed to process $input_file"
            fi
        fi
    done

    echo "Program executed successfully."
else
    echo "Compilation failed. Please fix any errors before running the program."
fi
