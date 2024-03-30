# Digital_Design_I
Project for DD1

# Digital Circuit Simulator

This program simulates a digital circuit based on input from three files: a library file, a circuit file, and a stimulus file. 

## Files

- `main.cpp`: This is the main file where the simulation is run.
- `Gates.h`: This file contains the `Gates` class which represents a type of gate used in the circuit.
- `Outputs.h`: This file contains the `Outputs` class which represents an output in the circuit.
- `component.h`: This file contains the `component` class which represents a component in the circuit.
- `Postfix_Functions.h`: This file contains functions for converting infix expressions to postfix and evaluating postfix expressions.
- `readInput_Functions.h`: This file contains functions for reading the library, circuit, and stimulus files.
- `Input.h`: This file contains the `Input` class which represents an input to the circuit.


# Logic Circuit Simulator

This is a C++ program for simulating logic circuits. It reads circuit information from a `.cir` file, library information from a `.lib` file, and stimulus information from a `.stim` file.

## How to Run

To run the simulator, follow these steps:

1. **Compile the Code**: Use a C++ compiler to compile the code into an executable. e.g. bash or windows terminal
2. **How to use terminal**: Open the terminal in the file directory the source code files are placed in.
3. **Commands**:   `g++ main.cpp -o logiscm`
                   `./logiscm <.lib> <.cir> <.stim>`
4. Replace `<.lib>` `<.cir>` `<.stim>` with the file names of the files being read by the program if they are located in the same directory as the source code or replace them with the files paths if they are not located in the same directory as the source code
