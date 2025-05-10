#!/bin/bash

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Check if port is provided
if [ $# -ne 1 ]; then
    echo -e "${RED}Usage: $0 <port>${NC}"
    exit 1
fi

PORT=$1

# Compile the client
printf "${BLUE}Compiling test client...${NC}\n"
gcc -Wall -Wextra -Werror client.c -o client
if [ $? -ne 0 ]; then
    printf "${RED}Compilation failed!${NC}\n"
    exit 1
fi
printf "${GREEN}Compilation successful!${NC}\n\n"

# Run the client
printf "${BLUE}Running client on port $PORT...${NC}\n"
./client $PORT

# Clean up
rm -f client