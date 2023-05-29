# OS_project2

# Project Readme

This repository contains four C++ source code files:

- Main.cpp: The main file that coordinates the execution of other files.
- country.cpp: Reads CSV files in a directory and spawns child processes to process the data.
- club.cpp: Reads a CSV file and writes data to child processes based on position.
- position.cpp: Receives data from club.cpp and calculates age statistics.

## Usage

To run the project, follow these steps:

1. Compile the source code files using a C++ compiler.
2. Execute the resulting executable file.

Make sure to place the appropriate CSV files in the designated directories before running the program.

## File Descriptions

- Main.cpp: This file initializes the necessary variables, lists the CSV files in the "clubs" directory, prints the positions from the specified CSV file, and prompts the user to enter positions. It then executes child processes to process the data based on the entered positions and waits for them to finish.
- country.cpp: This file lists the CSV files in the directory specified as the command-line argument, executes child processes to process the data from each CSV file based on the entered positions, and waits for them to finish.
- club.cpp: This file reads a CSV file specified as the command-line argument, stores player data in an array of structures, and writes the data to child processes based on the entered positions.
- position.cpp: This file receives data from club.cpp through named pipes, calculates age statistics, and prints the results.

## Dependencies

The project requires a C++ compiler and the following header files: `stdio.h`, `string.h`, `string`, `dirent.h`, `iostream`, `unistd.h`, `sys/wait.h`, `defs.h`, `fcntl.h`, and `sys/stat.h`. Make sure these dependencies are available in your environment.

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## License

This project is licensed under the [MIT License](LICENSE).
