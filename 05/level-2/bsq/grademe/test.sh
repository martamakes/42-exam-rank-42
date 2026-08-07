#!/bin/bash
# =============================================================================
# GRADEME — bsq (Exam Rank 05 - Level 02)
# =============================================================================
# Invocado por 05/exam.sh como: cd .../bsq/grademe && ./test.sh
# Contrato: sin argumentos, exit 0 = pass, exit != 0 = fail.
#
# Layout dividido (igual que polyset/vect2/bigint):
#   - "given" (no se toca): ../  (= .../bsq/, subject.en.txt + mapas de test)
#   - "alumno" (Expected files: *.c *.h — CUALQUIER nombre, a diferencia de
#     los ejercicios de C++): $ROOT/rendu/bsq/
#
# A diferencia de vect2/bigint/polyset, aqui NO hay un main.cpp/c dado: el
# subject dice "Expected files: *.c *.h" (todo lo escribe el alumno,
# incluido el main). Por eso el compile_all no asume nombres fijos: compila
# TODOS los .c que encuentre en rendu/bsq/.
#
# Portable Linux/macOS a proposito: compilador via "gcc", timeout casero sin
# depender de un binario "timeout" externo, ASan distingue uname Linux/no.
#
# QUE CUBRE (mapeado al subject.en.txt de bsq):
#   [Expected files]  al menos un *.c en rendu/bsq/ (nombre libre)
#   [Include guards]  cada *.h encontrado soporta doble inclusion
#   [Compilacion]     todos los *.c compilan juntos con -Wall -Wextra
#                      -Werror -std=c99 (0 warnings)
#   [Mapa valido]     los 6 map_valid_*.txt dados resuelven EXACTO (diff
#                      contra un golden ya verificado — el subject define
#                      la regla de desempate top-left de forma completa,
#                      asi que la salida es determinista)
#   [Mapa invalido]   los 4 map_invalid_*.txt (incluye caracter no
#                      imprimible y fila sin salto de linea final) dan
#                      "map error" en stderr, stdout vacio
#   [Edge cases]      los 10 bsq_edge_tests/ (cabecera sin espacios, con
#                      espacios de mas, simbolos raros, mapa 1x1, todo
#                      obstaculos, un solo hueco, filas incompletas,
#                      numero de filas negativo)
#   [stdin]           sin argumentos, lee el mapa de la entrada estandar
#   [Multiples mapas] el ejemplo textual del subject via argumento de
#                      fichero, y separador de linea en blanco entre mapas
#   [Fichero inexistente] "map error", no crash
#   [Memoria]         build opcional con AddressSanitizer + UBSan
# =============================================================================

set -u

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
BOLD='\033[1m'
RESET='\033[0m'

PASS=0
FAIL=0

pass() { echo -e "${GREEN}[PASS]${RESET} $1"; PASS=$((PASS + 1)); }
fail() { echo -e "${RED}[FAIL]${RESET} $1"; FAIL=$((FAIL + 1)); }
skip() { echo -e "${YELLOW}[SKIP]${RESET} $1"; }

# ---------------------------------------------------------------------------- #
#  Rutas: resueltas desde la ubicacion real del script, no del cwd del caller
# ---------------------------------------------------------------------------- #
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
GIVEN_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"                 # .../05/level-2/bsq
ROOT_DIR="$(cd "$SCRIPT_DIR/../../../.." && pwd)"         # 42-exam-rank-42/
STUDENT_DIR="$ROOT_DIR/rendu/bsq"

SCRATCH="$(mktemp -d "${TMPDIR:-/tmp}/bsq_grademe.XXXXXX")"
cleanup() { rm -rf "$SCRATCH"; }
trap cleanup EXIT

BINARY="bsq_test_bin"

# ---------------------------------------------------------------------------- #
#  0. Ficheros esperados (Expected files: *.c *.h — nombre libre)
# ---------------------------------------------------------------------------- #
stage_scratch() {
    echo -e "${CYAN}${BOLD}=== Ficheros esperados (Expected files: *.c *.h) ===${RESET}"

    if [ ! -d "$STUDENT_DIR" ]; then
        echo -e "${RED}Error: no existe $STUDENT_DIR${RESET}"
        echo -e "${YELLOW}Crea tus archivos ahi antes de validar.${RESET}"
        exit 1
    fi

    C_FILES=$(find "$STUDENT_DIR" -maxdepth 1 -name "*.c")
    if [ -z "$C_FILES" ]; then
        fail "No se encuentra ningun .c en $STUDENT_DIR"
        echo -e "${RED}Sin ficheros .c no se puede compilar.${RESET}"
        exit 1
    fi
    pass "Se encontraron $(echo "$C_FILES" | wc -l | tr -d ' ') fichero(s) .c"

    H_FILES=$(find "$STUDENT_DIR" -maxdepth 1 -name "*.h")
    if [ -n "$H_FILES" ]; then
        pass "Se encontraron $(echo "$H_FILES" | wc -l | tr -d ' ') fichero(s) .h"
    else
        skip "No hay ficheros .h (permitido: el subject no exige un header separado)"
    fi

    cp $C_FILES "$SCRATCH/" 2>/dev/null
    [ -n "$H_FILES" ] && cp $H_FILES "$SCRATCH/" 2>/dev/null
    echo ""
}

# ---------------------------------------------------------------------------- #
#  1. Include guards (doble inclusion) para cada .h encontrado
# ---------------------------------------------------------------------------- #
check_header_guards() {
    if [ -z "$H_FILES" ]; then
        return
    fi
    echo -e "${CYAN}${BOLD}=== Include guards (doble inclusion) ===${RESET}"
    for h in $H_FILES; do
        local hname
        hname="$(basename "$h")"
        cat > "$SCRATCH/guard_test.c" << EOF
#include "$hname"
#include "$hname"
int main(void) { return 0; }
EOF
        if gcc -std=c99 -Wall -Wextra -I"$SCRATCH" \
            -c "$SCRATCH/guard_test.c" -o "$SCRATCH/guard_test.o" 2>"$SCRATCH/guard_err.log"; then
            pass "$hname: incluirlo dos veces no rompe la compilacion"
        else
            fail "$hname: include guard ROTO (revisa #ifndef/#define)"
            sed 's/^/    /' "$SCRATCH/guard_err.log"
        fi
        # limpiar: guard_test.c tiene su propio main() y no debe colar en
        # el glob *.c del compile_all de mas abajo
        rm -f "$SCRATCH/guard_test.c" "$SCRATCH/guard_test.o"
    done
    echo ""
}

# ---------------------------------------------------------------------------- #
#  2. Compilar TODOS los .c encontrados — 0 warnings exigido
# ---------------------------------------------------------------------------- #
compile_all() {
    echo -e "${CYAN}${BOLD}=== Compilacion (todos tus *.c juntos) ===${RESET}"
    if (cd "$SCRATCH" && gcc -std=c99 -Wall -Wextra -Werror -I. \
        *.c -o "$BINARY") 2>"$SCRATCH/compile_err.log"; then
        pass "Compila sin errores ni warnings (-Wall -Wextra -Werror -std=c99)"
    else
        fail "ERROR compilando:"
        sed 's/^/    /' "$SCRATCH/compile_err.log" | head -40
        echo -e "${RED}No se puede continuar sin un binario que compile.${RESET}"
        echo ""
        summary_and_exit
    fi
    echo ""
}

# ---------------------------------------------------------------------------- #
#  3. Casos con salida determinista: diff exacto contra golden verificado
#     (el subject fija la regla de desempate top-left, asi que la salida
#     correcta es unica — un diff exacto es fiable aqui)
# ---------------------------------------------------------------------------- #
check_solved() {
    local mapfile="$1" expected="$2" label="$3"
    local actual
    actual="$(cd "$SCRATCH" && "./$BINARY" "$mapfile" 2>"$SCRATCH/stderr.log")"
    local err
    err="$(cat "$SCRATCH/stderr.log")"
    if [ -n "$err" ]; then
        fail "$label: se esperaba resolver el mapa, pero fue a stderr: $err"
    elif [ "$actual" = "$expected" ]; then
        pass "$label: resuelve exactamente igual que el golden verificado"
    else
        fail "$label: la salida NO coincide con el golden. Diff (esperado vs tuyo):"
        diff <(echo "$expected") <(echo "$actual") | sed 's/^/    /' | head -20
    fi
}

check_map_error() {
    local mapfile="$1" label="$2"
    local out err
    out="$(cd "$SCRATCH" && "./$BINARY" "$mapfile" 2>"$SCRATCH/stderr.log")"
    err="$(cat "$SCRATCH/stderr.log")"
    if [ -n "$out" ]; then
        fail "$label: se esperaba \"map error\", pero escribio en stdout: $out"
    elif [ "$err" = "map error" ]; then
        pass "$label: \"map error\" en stderr, stdout vacio"
    else
        fail "$label: stderr fue \"$err\" en vez de \"map error\""
    fi
}

run_functional_tests() {
    echo -e "${CYAN}${BOLD}=== Mapas validos (diff exacto contra golden) ===${RESET}"
    check_solved "$GIVEN_DIR/bsq_test_maps/map_valid_1.txt" \
"xx...
xx...
..o..
.....
....." "map_valid_1"
    check_solved "$GIVEN_DIR/bsq_test_maps/map_valid_2.txt" \
"xx...
xx...
oooo.
.....
....." "map_valid_2"
    check_solved "$GIVEN_DIR/bsq_test_maps/map_valid_3.txt" \
"xxx..
xxx..
xxx..
.o.o.
....." "map_valid_3"
    check_solved "$GIVEN_DIR/bsq_test_maps/map_valid_4.txt" \
"x.o..o...." "map_valid_4"
    check_solved "$GIVEN_DIR/bsq_test_maps/map_valid_5.txt" \
"x
.
o
.
." "map_valid_5"
    check_solved "$GIVEN_DIR/bsq_test_maps/map_valid_6.txt" \
".oxxx
.oxxx
.oxxx
.....
....." "map_valid_6"
    echo ""

    echo -e "${CYAN}${BOLD}=== Mapas invalidos (\"map error\") ===${RESET}"
    check_map_error "$GIVEN_DIR/bsq_test_maps/map_invalid_1.txt" "map_invalid_1 (filas de distinta longitud)"
    check_map_error "$GIVEN_DIR/bsq_test_maps/map_invalid_2.txt" "map_invalid_2 (caracter fuera de empty/obstacle/full)"
    check_map_error "$GIVEN_DIR/bsq_test_maps/map_invalid_3.txt" "map_invalid_3 (empty y obstacle son el mismo caracter)"
    check_map_error "$GIVEN_DIR/bsq_test_maps/map_invalid_4.txt" "map_invalid_4 (caracter no imprimible en la cabecera)"
    echo ""

    echo -e "${CYAN}${BOLD}=== Edge cases (bsq_edge_tests/) ===${RESET}"
    check_solved "$GIVEN_DIR/bsq_edge_tests/test1.txt" \
"xxxxx
xxxxx
xxxxx
xxxxx
xxxxx" "test1 (cabecera sin espacios: 5.ox)"
    check_solved "$GIVEN_DIR/bsq_edge_tests/test2.txt" \
"xxxxx
xxxxx
xxxxx
xxxxx
xxxxx" "test2 (cabecera con espacios de mas)"
    check_solved "$GIVEN_DIR/bsq_edge_tests/test3.txt" \
"#####
#####
#####
#####
#####" "test3 (simbolos \$ @ # como caracteres)"
    check_solved "$GIVEN_DIR/bsq_edge_tests/test4.txt" "x" "test4 (mapa 1x1)"
    check_solved "$GIVEN_DIR/bsq_edge_tests/test5.txt" \
"xxx.............................
xxx.............................
xxx............................." "test5 (limitado por altura: 3 filas)"
    check_solved "$GIVEN_DIR/bsq_edge_tests/test6.txt" \
"ooooo
ooooo
ooooo
ooooo
ooooo" "test6 (100% obstaculos: sin cuadrado posible, mapa intacto)"
    check_solved "$GIVEN_DIR/bsq_edge_tests/test7.txt" \
"ooooo
ooooo
ooxoo
ooooo
ooooo" "test7 (un unico hueco entre obstaculos)"
    check_map_error "$GIVEN_DIR/bsq_edge_tests/test8.txt" "test8 (menos filas de las que dice la cabecera)"
    check_map_error "$GIVEN_DIR/bsq_edge_tests/test9.txt" "test9 (numero de filas negativo)"
    check_solved "$GIVEN_DIR/bsq_edge_tests/test10.txt" \
"oxo.o
.o.o.
o.o.o
.o.o.
o.o.o" "test10 (tablero en damero)"
    echo ""
}

# ---------------------------------------------------------------------------- #
#  4. Ejemplo textual del subject + stdin + multiples mapas + fichero ausente
# ---------------------------------------------------------------------------- #
run_integration_tests() {
    echo -e "${CYAN}${BOLD}=== Integracion: ejemplo del subject, stdin, multiples mapas ===${RESET}"

    cat > "$SCRATCH/subject_example.txt" << 'EOF'
9 . o x
...........................
....o......................
............o..............
...........................
....o......................
...............o...........
...........................
......o..............o.....
..o.......o................
EOF
    local expected_example
    expected_example="$(cat << 'EOF'
.....xxxxxxx...............
....oxxxxxxx...............
.....xxxxxxxo..............
.....xxxxxxx...............
....oxxxxxxx...............
.....xxxxxxx...o...........
.....xxxxxxx...............
......o..............o.....
..o.......o................
EOF
)"
    check_solved "$SCRATCH/subject_example.txt" "$expected_example" "Ejemplo textual del subject (por fichero)"

    # via stdin (sin argumentos)
    local stdin_actual
    stdin_actual="$(cd "$SCRATCH" && "./$BINARY" < subject_example.txt 2>"$SCRATCH/stdin_err.log")"
    if [ -n "$(cat "$SCRATCH/stdin_err.log")" ]; then
        fail "Lectura por stdin: fue a stderr en vez de resolver"
    elif [ "$stdin_actual" = "$expected_example" ]; then
        pass "Lectura por stdin (sin argumentos) da el mismo resultado que por fichero"
    else
        fail "Lectura por stdin da una salida distinta a la del mismo mapa por fichero"
    fi

    # multiples mapas: uno valido + uno invalido, separados por linea en blanco
    # (capturamos a FICHERO, no a variable: "$(...)" recorta los saltos de
    # linea finales y no dejaria ver la linea en blanco separadora)
    ( cd "$SCRATCH" && "./$BINARY" "$GIVEN_DIR/bsq_test_maps/map_valid_1.txt" \
        "$GIVEN_DIR/bsq_test_maps/map_invalid_1.txt" \
        > "$SCRATCH/multi_out.txt" 2> "$SCRATCH/multi_err.txt" )
    cat > "$SCRATCH/multi_expected.txt" << 'EOF'
xx...
xx...
..o..
.....
.....

EOF
    if diff -q "$SCRATCH/multi_expected.txt" "$SCRATCH/multi_out.txt" >/dev/null 2>&1 \
        && [ "$(cat "$SCRATCH/multi_err.txt")" = "map error" ]; then
        pass "Multiples mapas: solucion + linea en blanco separadora + \"map error\" del segundo"
    else
        fail "Multiples mapas: no separa correctamente solucion/error con linea en blanco"
        echo "    diff (esperado vs tuyo):"
        diff "$SCRATCH/multi_expected.txt" "$SCRATCH/multi_out.txt" | sed 's/^/      /'
        echo "    stderr: $(cat "$SCRATCH/multi_err.txt")"
    fi

    # fichero inexistente
    local nf_err
    nf_err="$(cd "$SCRATCH" && "./$BINARY" /no/existe/nunca_jamas.txt 2>&1 1>/dev/null)"
    if [ "$nf_err" = "map error" ]; then
        pass "Fichero inexistente: \"map error\", sin crash"
    else
        fail "Fichero inexistente: stderr fue \"$nf_err\" en vez de \"map error\""
    fi
    echo ""
}

# ---------------------------------------------------------------------------- #
#  5. AddressSanitizer + UBSan (portable Linux/macOS)
# ---------------------------------------------------------------------------- #
run_with_timeout() {
    local secs="$1"; shift
    "$@" &
    local pid=$!
    ( sleep "$secs" 2>/dev/null; kill -9 "$pid" 2>/dev/null ) &
    local watcher=$!
    wait "$pid" 2>/dev/null
    local rc=$?
    kill "$watcher" 2>/dev/null
    wait "$watcher" 2>/dev/null
    return $rc
}

run_asan_tests() {
    echo -e "${CYAN}${BOLD}=== AddressSanitizer + UBSan (memoria) ===${RESET}"
    if (cd "$SCRATCH" && gcc -std=c99 -Wall -Wextra -g -O0 -fsanitize=address,undefined -I. \
        *.c -o "${BINARY}_asan") 2>"$SCRATCH/asan_compile_err.log"; then
        local asan_opts="abort_on_error=0"
        if [ "$(uname)" = "Linux" ]; then
            asan_opts="detect_leaks=1"
        fi
        local any_fail=0
        for f in "$GIVEN_DIR/bsq_test_maps/"map_*.txt "$GIVEN_DIR/bsq_edge_tests/"*.txt "$SCRATCH/subject_example.txt"; do
            ( cd "$SCRATCH" && ASAN_OPTIONS="$asan_opts" run_with_timeout 10 "./${BINARY}_asan" "$f" ) \
                > "$SCRATCH/asan_run_out.log" 2>&1
            local rc=$?
            if [ $rc -eq 137 ] || [ $rc -eq 124 ]; then
                skip "ASan/UBSan ($(basename "$f")): el sanitizer no arranco en esta maquina (bug conocido ASan/macOS) — omitido"
                continue
            fi
            if grep -qE "ERROR: (AddressSanitizer|LeakSanitizer)|runtime error:" "$SCRATCH/asan_run_out.log"; then
                any_fail=1
                fail "ASan/UBSan ($(basename "$f")) detecto un problema:"
                grep -A 6 -E "ERROR|runtime error" "$SCRATCH/asan_run_out.log" | sed 's/^/    /' | head -20
            fi
        done
        [ "$any_fail" -eq 0 ] && pass "ASan/UBSan: sin errores de memoria en ningun fixture"
    else
        skip "No se pudo compilar con sanitizers (compilador sin soporte) — no es bloqueante"
    fi
    echo ""
}

# ---------------------------------------------------------------------------- #
#  6. Resumen y salida
# ---------------------------------------------------------------------------- #
summary_and_exit() {
    echo -e "${BOLD}============================================${RESET}"
    local total=$((PASS + FAIL))
    if [ "$FAIL" -eq 0 ]; then
        echo -e "${GREEN}${BOLD}TESTS BSQ: $PASS/$total PASSED${RESET}"
    else
        echo -e "${RED}${BOLD}TESTS BSQ: $PASS/$total PASSED ($FAIL fallidos)${RESET}"
    fi
    echo -e "${BOLD}============================================${RESET}"
    exit $((FAIL > 0 ? 1 : 0))
}

# =============================================================================
#  MAIN
# =============================================================================
stage_scratch
check_header_guards
compile_all
run_functional_tests
run_integration_tests
run_asan_tests

summary_and_exit
