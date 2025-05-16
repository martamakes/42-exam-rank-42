#!/bin/bash

# Colors
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}Setting up Exam Rank 06 tester...${NC}"

# Create rendu directory if it doesn't exist
mkdir -p rendu

# Make scripts executable
chmod +x exam.sh
chmod +x tester/test.sh
chmod +x tester/auto_test.sh
chmod +x tester/run_client.sh

echo -e "${GREEN}Setup complete!${NC}"
echo -e "${GREEN}Run './exam.sh' to start the tester.${NC}"
