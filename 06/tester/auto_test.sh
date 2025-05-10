#!/bin/bash

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

RENDU_PATH="../rendu"
SUBJECT_PATH="../subject"
PORT=9999

# Check if the source file exists
if [ ! -f "$RENDU_PATH/mini_serv.c" ]; then
    printf "${RED}Error: mini_serv.c not found in $RENDU_PATH${NC}\n"
    exit 1
fi

# Check for forbidden defines
printf "${BLUE}Checking for forbidden #define preprocessor directives...${NC}\n"
if grep -q "#define" "$RENDU_PATH/mini_serv.c"; then
    printf "${RED}Error: #define preprocessor directives are not allowed!${NC}\n"
    exit 1
fi
printf "${GREEN}No forbidden #define found.${NC}\n\n"

# Compile the program
printf "${BLUE}Compiling mini_serv.c...${NC}\n"
gcc -Wall -Wextra -Werror "$RENDU_PATH/mini_serv.c" -o mini_serv
if [ $? -ne 0 ]; then
    printf "${RED}Compilation failed!${NC}\n"
    exit 1
fi
printf "${GREEN}Compilation successful!${NC}\n\n"

# Run the program with no arguments
printf "${BLUE}Testing with no arguments...${NC}\n"
./mini_serv 2> error.txt
if grep -q "Wrong number of arguments" error.txt; then
    printf "${GREEN}Test passed: Program correctly outputs 'Wrong number of arguments' when no arguments are provided.${NC}\n\n"
else
    printf "${RED}Test failed: Program should output 'Wrong number of arguments' when no arguments are provided.${NC}\n\n"
    exit 1
fi

# Start the server in the background
printf "${BLUE}Starting the server on port $PORT...${NC}\n"
./mini_serv $PORT > server_output.txt 2> server_error.txt &
SERVER_PID=$!
sleep 1

# Function to simulate a client
simulate_client() {
    local id=$1
    local port=$2
    local delay=$3
    
    # Connect to the server
    (
        sleep $delay
        echo "Test message from client $id" | nc -w 1 127.0.0.1 $port
        sleep 2
    ) &
}

# Simulate multiple clients
printf "${BLUE}Simulating client connections...${NC}\n"
for i in {1..3}; do
    simulate_client $i $PORT $i
    sleep 2
done

# Wait for all clients to finish
sleep 10

# Check for memory leaks (using simple check if valgrind is not available)
printf "${BLUE}Checking for memory leaks and fd leaks...${NC}\n"
# On macOS
lsof -p $SERVER_PID | grep -E 'FIFO|REG|CHR|DIR' > fd_check.txt
cat fd_check.txt
printf "${YELLOW}Note: Manual verification of fd leaks may be required.${NC}\n\n"

# Kill the server
kill $SERVER_PID 2>/dev/null
wait $SERVER_PID 2>/dev/null

printf "${GREEN}Test completed!${NC}\n"
printf "${YELLOW}Please check server_output.txt and server_error.txt for further inspection.${NC}\n"
