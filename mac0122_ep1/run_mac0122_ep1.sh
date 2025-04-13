#!/bin/bash
echo test
# Compile the C file
gcc -o mac0122_ep1 mac0122_ep1.c -lm

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. Running the program..."
    # Run the compiled program
    ./mac0122_ep1
    
    # Check if the program created mac0122_ep1.ps
    if [ -f "mac0122_ep1.ps" ]; then
        echo "mac0122_ep1.ps found. Opening with gv..."
        # Open the .ps file using gv
        gv mac0122_ep1.ps
    else
        echo "mac0122_ep1.ps not found."
    fi
else
    echo "Compilation failed."
fi
