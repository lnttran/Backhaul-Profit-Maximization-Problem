import pandas as pd
import os
import matplotlib.pyplot as plt
import seaborn as sns

all_csv_files = []

# Function to process and create a pivot table for a CSV file


def average_pivotChart(df):
    # df = pd.read_csv(file_path)
    table_val = [' Profit after Local Search',
                 ' Error', ' CPU Time(ms)', ' Run Time(ms)']
    pivot_df = df.pivot_table(values=table_val, index=[
                              ' nn selection', ' iter selection'], aggfunc='mean')
    # Reorder the columns based on the desired order
    pivot_df = pivot_df[table_val]
    pivot_df[' Profit after Local Search'] = pivot_df[' Profit after Local Search'].round(
        4)
    pivot_df[' Error'] = pivot_df[' Error'].round(4)

    # # Calculate the overall averages and add a new row
    overall_avg = pivot_df.mean().to_frame().T
    overall_avg.index = ['Overall Average']
    pivot_df = pd.concat([pivot_df, overall_avg])

    # Round the values to 4 decimal places
    pivot_df = pivot_df.round(4)
    return pivot_df


def find_min_error(df):
    table_val = [' Error', ' CPU Time(ms)', ' Run Time(ms)']
    pivot_df = df.pivot_table(values=table_val, index='Filename', aggfunc={
                              ' Error': 'min', ' CPU Time(ms)': 'sum', ' Run Time(ms)': 'sum'})
    pivot_df = pivot_df[table_val]

    return pivot_df


# Directory containing CSV files
directory_path = '../results/seed42/complete-randomized/disturb-numOfNode-0.7-7-3000ms-3nodeLeast'

# Get a list of all CSV files in the directory
csv_files = [file for file in os.listdir(
    directory_path) if file.endswith('.csv')]

# Create an Excel writer
excel_writer = pd.ExcelWriter(os.path.join(
    directory_path, 'disturb-numOfNode-0.7-7-3000ms-3nodeLeast.xlsx'), engine='xlsxwriter')
excel_writer_minError = pd.ExcelWriter(os.path.join(
    directory_path, 'disturb-numOfNode-0.7-7-3000ms-3nodeLeast-minError.xlsx'), engine='xlsxwriter')

# Loop through each CSV file, process it, and save the pivot table to a separate sheet
for file in csv_files:
    file_path = os.path.join(directory_path, file)
    df = pd.read_csv(file_path)
    all_csv_files.append(df)
    # Use the file name as the sheet name
    sheet_name = os.path.splitext(file)[0]
    pivot_df = average_pivotChart(df)
    pivot_df.to_excel(excel_writer, sheet_name=sheet_name, index=True)
    pivot_df_minEror = find_min_error(df)
    pivot_df_minEror.to_excel(excel_writer_minError,
                              sheet_name=sheet_name, index=True)

# for file in csv_files:
#     file_path = os.path.join(directory_path, file)
#     df = pd.read_csv(file_path)
#     sheet_name = os.path.splitext(file)[0]  # Use the file name as the sheet name
#     pivot_df_minEror = find_min_error(df)
#     pivot_df_minEror.to_excel(excel_writer_minError, sheet_name=sheet_name, index=True)

# Concatenate all CSV files and create a pivot table for the combined data
all_csv_combined = pd.concat(all_csv_files, ignore_index=True)
all_data_pivot_df = average_pivotChart(all_csv_combined)
all_data_pivot_df.to_excel(excel_writer, sheet_name='All data', index=True)

with excel_writer:
    pass
with excel_writer_minError:
    pass
