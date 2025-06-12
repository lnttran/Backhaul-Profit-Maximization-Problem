# Greedy Algorithms for BPMP

This project aims at creating and comparing different greedy heuristic algorithms for the Backhaul Profit Maximization Problem (BPMP). There are 2 main algorithms that have been created, both of which will be outlined here. 

## Greedy Ensemple + Local search
This section provides instructions for running the Greedy Ensemble algorithm, which consists of 11 variations of a greedy heuristic for solving the BPMP problem. The algorithm operates on datasets organized in .txt files, categorized by the number of locations. Each dataset file includes information such as weights, distances between pickup and dropoff locations, vehicle specifications, and constraints.

```
├── 10_nodes/
│   ├── t10_01_data.txt
│   ├── t10_02_data.txt
│   └── t10_03_data.txt
```
Here, 10_nodes/ is a subdirectory containing .txt files that describe the problem instances. 

### Set up 

To ensure that you can compile and run the C++ program, you need to have C++17 support and the g++ compiler installed on your system.

Ensure that you have g++ installed. If not, you can install it using the package manager for your system:

```
sudo apt update
sudo apt install g++-17
```

Verify the installation:

```
g++ --version
```

### Running the Program

#### Parameters

The script requires six parameters to be passed via the command line. These parameters are used to specify the configuration of the performance profile generation.

1. **dataset_folder_directory_path:** The path to the directory containing the .txt dataset files. 

2. **filename:** The name of the output file where the results will be saved.

3. **optimal_solution_directory_path:** The path to the directory containing the optimal solution files for the dataset.

4. **fill-capacity:** Specify `true` to run the fill-capacity algorithm or `false` to skip it.

5. **insertion:** Specify `true` to run the insertion algorithm or `false` to skip it.

6. **replace:** Specify `true` to run the replace algorithm or `false` to skip it.



#### Command Format:
Compile the program:

```
g++ main.cpp Algorithm.cpp path.cpp readData.cpp -o main -std=c++17
```

Use the following command format to run the script:

```
./main <dataset_folder_directory_path> <filename> <optimal_solution_directory_path> <fill-capacity> <insertion> <replace> 
```

#### Example Command:
Below is an example command to execute the Greedy Ensemble algorithm with specific options:

```
./main data_files/BPMP_Data_Files/10_nodes/ results/deterministic/10_nodes.csv data_files/BPMP_Data_Files/optimalProfit.csv false true true
```

Ensure the output directory exists:

* The directory `results/deterministic/` should exist to successfully write to `10_nodes.csv`.


### Running the Program with Bash Script

Using a Bash script allows you to run multiple configurations efficiently.

**Example Script:** `run_script_deterministic.sh`
This script automates running the program with various datasets (e.g., 10 to 80 nodes). Here’s how to use it:

1. **Make the script executable:** Change the permission of the script to make it executable

```
chmod +x run_script_deterministic.sh
```

2. **Run the Script:** Execute the script using the following command:

```
./run_script_deterministic.sh
```

Feel free to create a new bash to execute your own test.

## Randomize + Local search

This section provides instructions for running the Randomize algorithm, which consists of 2 variations: complete random and roulette random. It takes advantange of the fast run times through numerous of iterations of random solutions. 

### Dataset and Setup
Dataset and setup are similar to Ensemble algorithms

### Running the Program

#### Parameters

The script requires six parameters to be passed via the command line. These parameters are used to specify the configuration of the performance profile generation.

1. **dataset_folder_directory_path:** The path to the directory containing the .txt dataset files. 

2. **filename:** The name of the output file where the results will be saved.

3. **optimal_solution_directory_path:** The path to the directory containing the optimal solution files for the dataset.

4. **random_option:** Select random option `random` or `roulette`.

#### Command Format:
Compile the program:

```
g++ randomize_main.cpp Algorithm.cpp path.cpp readData.cpp Randomize.cpp -o main -std=c++17
```

Use the following command format to run the script:

```
./main <dataset_folder_directory_path> <filename> <optimal_solution_directory_path> <random_option> 
```

### Running the Program with Bash Script

Using a Bash script allows you to run multiple configurations efficiently.

**Example Script:** `run_script_random.sh` and `run_script_roulette.sh`
This script automates running the program with various datasets (e.g., 10 to 80 nodes).


## Data Analysis: Performance Profile Generation

This section provides instructions for  running a Python script to generate performance profiles for comparing four different algorithms: Restricted Triple Heuristic, Greedy Ensemble, Complete Randomized, and Roulette Randomized. The performance profile will compare these algorithms based on runtime, profit, and gap to the optimal solution.


### Prerequisites
1. **Python Installation**:
    * While the script is compatible with all Python versions, the latest version is recommended for optimal performance and compatibility.
2. **Required Python Packages**:
    * Ensure the following packages are installed:
        * numpy
        * pandas
        * xlsxwriter
        * matplotlib
        * openpyxl
        * seaborn
        <br/>

    You can install these packages using pip:
    ```
    pip install numpy pandas xlsxwriter matplotlib openpyxl seaborn
    ```


### Directory Structure

Before running the script, ensure that your directory structure is correctly set up and that all necessary CSV data files are available. The script requires data for each of the four algorithms to be stored in separate directories as follows:

```
├── complete-randomized/
│   ├── random_10.csv
│   ├── random_20.csv
│   └── random_30.csv
├── roulette-randomized/
│   ├── roulette_10.csv
│   ├── roulette_20.csv
│   └── roulette_30.csv
├── greedy-ensemble/
│   ├── greedy_10.csv
│   ├── greedy_20.csv
│   └── greedy_30.csv
└── restricted-triple-heuristic/
    └── RTH Original.csv
```

Make sure that you have prepared and organized the CSV files in these directories before running the script to ensure accurate and comprehensive performance profiling.

### Running the Script

Navigate to the data-analysis directory where the script is located:

```
cd ./data-analysis 
```

### Script Parameters
The script requires eight parameters to be passed via the command line. These parameters are used to specify the configuration of the performance profile generation.

1. **nn_selection:** Determines the number of iterations for Complete Randomized and Roulette Randomized algorithms.
* `1` - All iterations
* `2` - 30% of file size
* `3` - Square root of size

2. **iter_selection:** Specifies the iteration size for the analysis.
* `1` - 1 × File Size
* `2` - 2 × File Size
* `3` - 5 × File Size

3. **filename:** The name of the output file where the performance profile will be saved.

4. **destination_directory_path:** Path to the directory where the output file will be saved.

5. **greedy_directory_path:** Path to the directory containing CSV data for the Greedy Ensemble algorithm.

6. **complete_randomized_directory_path:** Path to the directory containing CSV data for the Complete Randomized algorithm.

7. **roulette_randomized_directory_path:** Path to the directory containing CSV data for the Roulette Randomized algorithm.

8. **rth_directory_path:** Path to the directory containing CSV data for the Restricted Triple Heuristic algorithm.

### Command Format:
```
python3 performace-profile-all.py <nn_selection[1,2,3]> <iter_selection[1,2,3]> <filename> <destination_directory_path> <greedy_directory_path> <complete_randomized_directory_path> <roulette_randomized_directory_path> <rth_directory_path>
```

### Example Command:
In this example, `All` for nn-selected and `1 * File Size` for iter-selection are passed in the parameter.
```
python3 performace-profile-all.py 1 1 Performance-Profile-All ../results/4-solution-perfomance-profile/Original ../results/greedy-algo/disturb-numOfNode-0.7-7-3000ms ../results/complete-randomized/disturb-numOfNode-0.7-7-3000ms-3nodeLeast ../results/roulette-randomized/disturb-numOfNode-0.7-7-3000ms-3nodeLeast ../results/RTH/Original
```


## Dataset 
Link below consist of all dataset we use for our research 

https://dataverse.tdl.org/dataset.xhtml?persistentId=doi:10.18738/T8/VW0Z7D
