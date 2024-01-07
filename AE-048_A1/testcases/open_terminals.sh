#!/bin/bash

# Get the current working directory
current_dir=$(pwd)

# Commands for Terminal 1
cmd1_1="echo 'Running command 1 in Terminal 1'; cd '$current_dir';"
cmd1_2="echo 'Running command 2 in Terminal 1'; cd '$current_dir';"

# Commands for Terminal 2
cmd2_1="echo 'Running command 1 in Terminal 2'; cd '$current_dir';"
cmd2_2="echo 'Running command 2 in Terminal 2'; cd '$current_dir';"

# Commands for Terminal 3
cmd3_1="echo 'Running command 1 in Terminal 3'; cd '$current_dir';"
cmd3_2="echo 'Running command 2 in Terminal 3'; cd '$current_dir';"

# Function to open a terminal and run the commands
open_terminal() {
    if [ "$(uname)" == "Darwin" ]; then
        # macOS (Darwin) uses the 'open' command to open a new terminal
        osascript -e "tell application \"Terminal\" to do script \"$1 $2 exec bash\""
    elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
        # Linux uses either 'gnome-terminal' or 'xterm' to open a new terminal
        if [ -n "$(command -v gnome-terminal)" ]; then
            gnome-terminal -- bash -c "$1 $2 exec bash"
        else
            xterm -e "bash -c \"$1 $2 exec bash\""
        fi
    else
        echo "Unsupported OS: $(uname)"
        exit 1
    fi
}

# Open three terminals and run the commands
open_terminal "$cmd1_1" "$cmd1_2"
open_terminal "$cmd2_1" "$cmd2_2"
open_terminal "$cmd3_1" "$cmd3_2"
