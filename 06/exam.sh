#!/bin/bash

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

clear

echo -e "${BLUE}==============================================${NC}"
echo -e "${BLUE}             EXAM RANK 06 TESTER             ${NC}"
echo -e "${BLUE}==============================================${NC}"
echo ""

while true; do
    echo "Menu:"
    echo "1. View Subject"
    echo "2. View main.c (template functions provided in the exam)"
    echo "3. View Template Code (to get started)"
    echo "4. Run Manual Test (interactive)"
    echo "5. Run Automatic Test"
    echo "6. Run Test Client"
    echo "7. Check rendu directory"
    echo "8. Check for memory leaks (manual check)"
    echo "0. Exit"
    echo ""
    
    read -p "Enter your choice: " choice
    echo ""
    
    case $choice in
        1)
            clear
            echo -e "${BLUE}============== SUBJECT ==============${NC}"
            cat subject/subject.en.txt
            echo ""
            read -p "Press Enter to continue..."
            clear
            ;;
        2)
            clear
            echo -e "${BLUE}============== MAIN.C ==============${NC}"
            cat subject/main.c
            echo ""
            read -p "Press Enter to continue..."
            clear
            ;;
        3)
            clear
            echo -e "${BLUE}============== TEMPLATE CODE ==============${NC}"
            cat examples/mini_serv_template.c
            echo ""
            read -p "Press Enter to continue..."
            clear
            ;;
        4)
            clear
            echo -e "${BLUE}============== MANUAL TEST ==============${NC}"
            cd tester
            bash test.sh
            cd ..
            echo ""
            read -p "Press Enter to continue..."
            clear
            ;;
        5)
            clear
            echo -e "${BLUE}============== AUTOMATIC TEST ==============${NC}"
            cd tester
            bash auto_test.sh
            cd ..
            echo ""
            read -p "Press Enter to continue..."
            clear
            ;;
        6)
            clear
            echo -e "${BLUE}============== TEST CLIENT ==============${NC}"
            echo -e "${YELLOW}Make sure your server is running on a specific port!${NC}"
            read -p "Enter the port number to connect to: " port
            cd tester
            bash run_client.sh $port
            cd ..
            echo ""
            read -p "Press Enter to continue..."
            clear
            ;;
        7)
            clear
            echo -e "${BLUE}============== RENDU DIRECTORY ==============${NC}"
            if [ -f "rendu/mini_serv.c" ]; then
                echo -e "${GREEN}mini_serv.c found in rendu directory.${NC}"
                ls -la rendu
            else
                echo -e "${RED}mini_serv.c not found in rendu directory!${NC}"
                echo -e "${YELLOW}Please copy your solution to the rendu directory before testing.${NC}"
            fi
            echo ""
            read -p "Press Enter to continue..."
            clear
            ;;
        8)
            clear
            echo -e "${BLUE}============== MEMORY LEAK CHECK ==============${NC}"
            echo -e "${YELLOW}This is a manual check. You can use tools like valgrind or leaks on macOS.${NC}"
            echo "On Linux: valgrind --leak-check=full ./mini_serv [port]"
            echo "On macOS: leaks -atExit -- ./mini_serv [port]"
            echo ""
            echo -e "${YELLOW}Note: For file descriptor leaks, use lsof command:${NC}"
            echo "lsof -p [pid]"
            echo ""
            read -p "Press Enter to continue..."
            clear
            ;;
        0)
            echo -e "${GREEN}Exiting tester. Good luck with your exam!${NC}"
            exit 0
            ;;
        *)
            echo -e "${RED}Invalid choice. Please try again.${NC}"
            read -p "Press Enter to continue..."
            clear
            ;;
    esac
done