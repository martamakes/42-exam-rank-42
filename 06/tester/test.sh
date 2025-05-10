#!/bin/bash

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

RENDU_PATH="../rendu"
SUBJECT_PATH="../subject"
PORT=8081

# Check if the source file exists
if [ ! -f "$RENDU_PATH/mini_serv.c" ]; then
    printf "${RED}Error: mini_serv.c not found in $RENDU_PATH${NC}\n"
    exit 1
fi

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
fi

# Run the program with invalid port
printf "${BLUE}Testing with invalid port (already in use or restricted)...${NC}\n"
./mini_serv 1 2> error.txt
if grep -q "Fatal error" error.txt; then
    printf "${GREEN}Test passed: Program correctly outputs 'Fatal error' when an invalid port is provided.${NC}\n\n"
else
    printf "${RED}Test failed: Program should output 'Fatal error' when an invalid port is provided.${NC}\n\n"
fi

# Start the server
printf "${BLUE}Starting the server on port $PORT...${NC}\n"
./mini_serv $PORT &
SERVER_PID=$!
sleep 1

# Testing client connections and messaging
printf "${BLUE}Testing client connections and messaging...${NC}\n"
echo "Setting up test client. Connect using: nc 127.0.0.1 $PORT"
echo "Press Ctrl+C to end the test when you're done."
echo ""
echo "Expected behavior:"
echo "1. When a client connects, all connected clients should see: 'server: client X just arrived'"
echo "2. When a client sends a message, all other clients should see: 'client X: message'"
echo "3. When a client disconnects, all connected clients should see: 'server: client X just left'"
echo ""

# Wait for user to test
read -p "Press Enter to continue..."

# Kill the server
kill $SERVER_PID 2>/dev/null
wait $SERVER_PID 2>/dev/null

printf "${GREEN}Test completed!${NC}\n"
