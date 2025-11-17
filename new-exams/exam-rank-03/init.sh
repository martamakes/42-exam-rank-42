#!/bin/bash

# Initialization Script for Exam Rank 03
# Creates test environments for exercises

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Function to display colored text
print_color() {
    local color=$1
    local text=$2
    echo -e "${color}${text}${NC}"
}

# Function to check if exercise is a program or function
detect_exercise_type() {
    local level=$1
    local exercise=$2
    local exercise_dir="level-${level}/${exercise}"
    
    # Check if subject file exists
    if [ -f "${exercise_dir}/subject.txt" ]; then
        # Look for main function or program indicators in subject
        if grep -q "program" "${exercise_dir}/subject.txt" || grep -q "main" "${exercise_dir}/subject.txt"; then
            echo "program"
        else
            echo "function"
        fi
    else
        # Default to function if no subject found
        echo "function"
    fi
}

# Function to check if header file is needed
needs_header() {
    local level=$1
    local exercise=$2
    local exercise_dir="level-${level}/${exercise}"
    
    # Check if .h file already exists
    if ls "${exercise_dir}"/*.h >/dev/null 2>&1; then
        echo "yes"
    else
        echo "no"
    fi
}

# Function to create test_main.c for programs
create_program_test() {
    local level=$1
    local exercise=$2
    local exercise_dir="level-${level}/${exercise}"
    
    cat > "${exercise_dir}/test_main.c" << 'EOF'
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Test cases for the program
// Modify these test cases according to your exercise requirements

int main(void)
{
    printf("=== TESTING PROGRAM ===\n");
    
    // Test case 1
    printf("\n--- Test Case 1 ---\n");
    printf("Running: ./program arg1 arg2\n");
    system("echo 'test input' | ./program arg1 arg2");
    
    // Test case 2
    printf("\n--- Test Case 2 ---\n");
    printf("Running: ./program\n");
    system("./program");
    
    // Test case 3
    printf("\n--- Test Case 3 ---\n");
    printf("Running: ./program with_different_args\n");
    system("./program with_different_args");
    
    printf("\n=== END TESTING ===\n");
    return 0;
}
EOF
    
    print_color $GREEN "✅ Created test_main.c for program testing"
}

# Function to create test_main.c for functions
create_function_test() {
    local level=$1
    local exercise=$2
    local exercise_dir="level-${level}/${exercise}"
    local header_needed=$(needs_header $level $exercise)
    
    cat > "${exercise_dir}/test_main.c" << EOF
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
EOF

    if [ "$header_needed" = "yes" ]; then
        # Find header file name
        local header_file=$(ls "${exercise_dir}"/*.h 2>/dev/null | head -1 | xargs basename)
        if [ -n "$header_file" ]; then
            echo "#include \"$header_file\"" >> "${exercise_dir}/test_main.c"
        fi
    fi

    cat >> "${exercise_dir}/test_main.c" << 'EOF'

// Add your function prototype here if no header file
// Example: int your_function(char *str);

int main(void)
{
    printf("=== TESTING FUNCTION ===\n");
    
    // Test case 1
    printf("\n--- Test Case 1 ---\n");
    // Example function call - modify according to your function
    // printf("Result: %d\n", your_function("test"));
    
    // Test case 2
    printf("\n--- Test Case 2 ---\n");
    // Add more test cases here
    
    // Test case 3
    printf("\n--- Test Case 3 ---\n");
    // Add edge cases here
    
    printf("\n=== END TESTING ===\n");
    return 0;
}
EOF
    
    print_color $GREEN "✅ Created test_main.c for function testing"
}

# Function to create test.sh script
create_test_script() {
    local level=$1
    local exercise=$2
    local exercise_dir="level-${level}/${exercise}"
    local exercise_type=$(detect_exercise_type $level $exercise)
    
    cat > "${exercise_dir}/test.sh" << EOF
#!/bin/bash

# Test script for $exercise (Level $level)

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

print_color() {
    local color=\$1
    local text=\$2
    echo -e "\${color}\${text}\${NC}"
}

print_color \$BLUE "🧪 Testing $exercise..."

# Clean previous builds
rm -f $exercise test_program

EOF

    if [ "$exercise_type" = "program" ]; then
        cat >> "${exercise_dir}/test.sh" << 'EOF'
# Compile the main program
print_color $YELLOW "Compiling main program..."
if gcc -Wall -Wextra -Werror *.c -o program 2>/dev/null; then
    print_color $GREEN "✅ Main program compiled successfully"
    
    # Run basic tests
    print_color $YELLOW "Running basic tests..."
    echo "Test input" | ./program test_arg
    
else
    print_color $RED "❌ Main program compilation failed"
    gcc -Wall -Wextra -Werror *.c -o program
    exit 1
fi

# Compile and run test program if exists
if [ -f "test_main.c" ]; then
    print_color $YELLOW "Compiling test program..."
    if gcc -Wall -Wextra -Werror test_main.c *.c -o test_program 2>/dev/null; then
        print_color $GREEN "✅ Test program compiled successfully"
        print_color $YELLOW "Running comprehensive tests..."
        ./test_program
    else
        print_color $RED "❌ Test program compilation failed"
    fi
fi
EOF
    else
        cat >> "${exercise_dir}/test.sh" << 'EOF'
# Find the main C file (excluding test_main.c)
MAIN_FILE=$(ls *.c 2>/dev/null | grep -v test_main.c | head -1)

if [ -z "$MAIN_FILE" ]; then
    print_color $RED "❌ No main C file found"
    exit 1
fi

print_color $YELLOW "Found main file: $MAIN_FILE"

# Compile with test_main.c if it exists
if [ -f "test_main.c" ]; then
    print_color $YELLOW "Compiling with test file..."
    if gcc -Wall -Wextra -Werror test_main.c "$MAIN_FILE" -o test_program; then
        print_color $GREEN "✅ Compilation successful"
        print_color $YELLOW "Running tests..."
        ./test_program
    else
        print_color $RED "❌ Compilation failed"
        exit 1
    fi
else
    print_color $YELLOW "Compiling function only..."
    if gcc -Wall -Wextra -Werror -c "$MAIN_FILE"; then
        print_color $GREEN "✅ Function compiles without errors"
    else
        print_color $RED "❌ Compilation failed"
        exit 1
    fi
fi
EOF
    fi

    # Make test script executable
    chmod +x "${exercise_dir}/test.sh"
    
    print_color $GREEN "✅ Created test.sh script"
}

# Function to create Makefile
create_makefile() {
    local level=$1
    local exercise=$2
    local exercise_dir="level-${level}/${exercise}"
    local exercise_type=$(detect_exercise_type $level $exercise)
    
    cat > "${exercise_dir}/Makefile" << EOF
# Makefile for $exercise (Level $level)

CC = gcc
CFLAGS = -Wall -Wextra -Werror
NAME = $exercise

# Source files
SRCS = \$(wildcard *.c)
OBJS = \$(SRCS:.c=.o)

# Main target
\$(NAME): \$(OBJS)
	\$(CC) \$(CFLAGS) -o \$(NAME) \$(OBJS)

# Object files
%.o: %.c
	\$(CC) \$(CFLAGS) -c \$< -o \$@

# Test target
test: \$(NAME)
	./test.sh

# Clean
clean:
	rm -f \$(OBJS)

fclean: clean
	rm -f \$(NAME) test_program

re: fclean \$(NAME)

.PHONY: clean fclean re test
EOF
    
    print_color $GREEN "✅ Created Makefile"
}

# Main function
main() {
    if [ \$# -ne 2 ]; then
        print_color $RED "Usage: \$0 <level> <exercise>"
        print_color $YELLOW "Example: \$0 1 filter"
        print_color $YELLOW "Example: \$0 2 permutations"
        exit 1
    fi
    
    local level=\$1
    local exercise=\$2
    local exercise_dir="level-\${level}/\${exercise}"
    
    # Validate level
    if [[ ! "\$level" =~ ^[12]\$ ]]; then
        print_color \$RED "❌ Invalid level. Use 1 or 2"
        exit 1
    fi
    
    # Check if exercise directory exists
    if [ ! -d "\$exercise_dir" ]; then
        print_color \$RED "❌ Exercise directory not found: \$exercise_dir"
        exit 1
    fi
    
    print_color \$PURPLE "🚀 Initializing test environment for \$exercise (Level \$level)"
    print_color \$CYAN "Exercise directory: \$exercise_dir"
    
    # Detect exercise type
    local exercise_type=\$(detect_exercise_type \$level \$exercise)
    print_color \$BLUE "Detected type: \$exercise_type"
    
    # Create test files
    if [ "\$exercise_type" = "program" ]; then
        create_program_test \$level \$exercise
    else
        create_function_test \$level \$exercise
    fi
    
    create_test_script \$level \$exercise
    create_makefile \$level \$exercise
    
    print_color \$GREEN "🎉 Test environment initialized successfully!"
    print_color \$YELLOW "Available commands in \$exercise_dir:"
    echo "  - make: compile the exercise"
    echo "  - make test: run tests"
    echo "  - ./test.sh: run test script directly"
    echo "  - make clean/fclean: clean build files"
}

# Run main function
main "\$@"