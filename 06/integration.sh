#!/bin/bash

# Integración con el sistema principal de exámenes
# Este script puede ser llamado desde el menú principal

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

CURRENT_DIR=$(pwd)
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

cd "$SCRIPT_DIR"

# Check if setup has been run
if [ ! -x "exam.sh" ] || [ ! -x "tester/test.sh" ]; then
    echo -e "${YELLOW}Setting up Exam Rank 06 tester for the first time...${NC}"
    bash setup.sh
fi

# Run the exam script
bash exam.sh

# Return to the original directory
cd "$CURRENT_DIR"
