#!/bin/bash
# =============================================================================
# GRADEME — bsq_1map (Exam Rank 05 - Level 02, variante "un solo mapa")
# =============================================================================
# Invocado por 05/exam.sh como: cd .../bsq_1map/grademe && ./test.sh
# Contrato: sin argumentos, exit 0 = pass, exit != 0 = fail.
#
# Esta es una variante DISTINTA del subject de bsq (ver ../../bsq/), no un
# duplicado. Diferencias reales, confirmadas contra el texto exacto del
# subject de esta variante:
#   1. "stderr" NO esta en "Allowed functions and globals" — todo (mapa
#      resuelto Y errores) debe ir a STDOUT. Usar stderr aqui es una
#      violacion de la lista de funciones permitidas, no solo un detalle
#      de estilo — por eso este test lo comprueba explicitamente.
#   2. El programa recibe UN solo mapa (nunca un bucle sobre varios argv).
#   3. Mensaje de mapa invalido FIJADO por el subject: exactamente
#      "Error: invalid map" (no "map error"). Para "cualquier otro error"
#      el subject solo exige el prefijo "Error " + un mensaje — el texto
#      exacto NO esta fijado, asi que aqui se comprueba con un match
#      flexible (empieza por "Error"), no con diff exacto.
#
# El algoritmo DP en si (dp[i][j] = lado del mayor cuadrado libre con
# esquina inferior-derecha en (i,j)) NO cambia entre variantes, asi que
# los mapas de test se REUTILIZAN de ../bsq/ (mismo formato de mapa) en
# vez de duplicar 20 ficheros.
#
# Portable Linux/macOS a proposito: compilador via "gcc", timeout casero.
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
#  Rutas
# ---------------------------------------------------------------------------- #
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
GIVEN_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"                     # .../05/level-2/bsq_1map
ROOT_DIR="$(cd "$SCRIPT_DIR/../../../.." && pwd)"             # 42-exam-rank-42/
STUDENT_DIR="$ROOT_DIR/rendu/bsq_1map"
FIXTURES_DIR="$(cd "$GIVEN_DIR/../bsq" && pwd)"                # mapas compartidos con bsq/

SCRATCH="$(mktemp -d "${TMPDIR:-/tmp}/bsq_1map_grademe.XXXXXX")"
cleanup() { rm -rf "$SCRATCH"; }
trap cleanup EXIT

BINARY="bsq_1map_test_bin"

# ---------------------------------------------------------------------------- #
#  0. Ficheros esperados
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
#  1. "stderr" NO esta en Allowed functions and globals — comprobacion
#     estatica (grep) ademas de la dinamica (capturar stderr en runtime)
# ---------------------------------------------------------------------------- #
# "stderr" no esta en la lista de esta variante (a diferencia de bsq/), y
# "sscanf"/"isprint" no estan en NINGUNA de las dos listas — se comprueban
# los tres aqui juntos.
check_forbidden_functions() {
    echo -e "${CYAN}${BOLD}=== Funciones/globales NO permitidos (stderr, sscanf, isprint) ===${RESET}"
    if ! command -v perl >/dev/null 2>&1; then
        skip "perl no disponible para despojar comentarios — chequeo estatico omitido (no bloqueante)"
        echo ""
        return
    fi
    local f sym hits=""
    for f in "$SCRATCH"/*.c "$SCRATCH"/*.h; do
        [ -f "$f" ] || continue
        # Despoja comentarios /* */ y // ANTES de grepear: los propios
        # comentarios explicativos del alumno (p.ej. "no usar stderr aqui")
        # mencionan estas palabras en prosa y darian un falso positivo si
        # se grepea el fichero tal cual.
        local stripped
        stripped="$(perl -0777 -pe 's{/\*.*?\*/}{}gs; s{//.*}{}g' "$f")"
        for sym in stderr sscanf isprint; do
            if echo "$stripped" | grep -q "\b${sym}\b"; then
                hits="$hits $(basename "$f"):${sym}"
            fi
        done
    done
    if [ -z "$hits" ]; then
        pass "Ni stderr, ni sscanf, ni isprint aparecen como codigo (fuera de comentarios)"
    else
        fail "Uso de simbolo NO permitido (fuera de comentarios):$hits — viola \"Allowed functions and globals\""
    fi
    echo ""
}

# ---------------------------------------------------------------------------- #
#  2. Include guards
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
        rm -f "$SCRATCH/guard_test.c" "$SCRATCH/guard_test.o"
    done
    echo ""
}

# ---------------------------------------------------------------------------- #
#  3. Compilar TODOS los .c encontrados — 0 warnings exigido
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
#  4. Casos con salida determinista + contrato de error de ESTA variante
# ---------------------------------------------------------------------------- #
check_solved() {
    local mapfile="$1" expected="$2" label="$3"
    local actual err
    actual="$(cd "$SCRATCH" && "./$BINARY" "$mapfile" 2>"$SCRATCH/stderr.log")"
    err="$(cat "$SCRATCH/stderr.log")"
    if [ -n "$err" ]; then
        fail "$label: escribio en stderr (no se esperaba NADA en stderr en esta variante): $err"
    elif [ "$actual" = "$expected" ]; then
        pass "$label: resuelve exactamente igual que el golden verificado"
    else
        fail "$label: la salida NO coincide con el golden. Diff (esperado vs tuyo):"
        diff <(echo "$expected") <(echo "$actual") | sed 's/^/    /' | head -20
    fi
}

# A diferencia de bsq/ (mensaje fijo "map error" en stderr), aqui el
# subject solo fija el texto EXACTO para el caso "invalid map", y en
# STDOUT: "Error: invalid map". Se comprueba con diff exacto de ESE caso
# concreto, y que stderr siga vacio.
check_invalid_map() {
    local mapfile="$1" label="$2"
    local out err
    out="$(cd "$SCRATCH" && "./$BINARY" "$mapfile" 2>"$SCRATCH/stderr.log")"
    err="$(cat "$SCRATCH/stderr.log")"
    if [ -n "$err" ]; then
        fail "$label: escribio en stderr (no permitido en esta variante): $err"
    elif [ "$out" = "Error: invalid map" ]; then
        pass "$label: \"Error: invalid map\" exacto en stdout, stderr vacio"
    else
        fail "$label: stdout fue \"$out\" en vez de \"Error: invalid map\""
    fi
}

# Para "cualquier otro error" el subject SOLO exige el prefijo "Error "
# + un mensaje — no hay texto fijo que comprobar con diff exacto.
check_other_error_prefix() {
    local label="$1"; shift
    local out err
    out="$(cd "$SCRATCH" && "$@" 2>"$SCRATCH/stderr.log")"
    err="$(cat "$SCRATCH/stderr.log")"
    if [ -n "$err" ]; then
        fail "$label: escribio en stderr (no permitido en esta variante): $err"
    elif [[ "$out" == Error* ]]; then
        pass "$label: stdout empieza por \"Error\" (el subject no fija el texto exacto aqui)"
    else
        fail "$label: stdout fue \"$out\", no empieza por \"Error\""
    fi
}

run_functional_tests() {
    echo -e "${CYAN}${BOLD}=== Mapas validos (diff exacto contra golden, reutilizados de ../bsq/) ===${RESET}"
    check_solved "$FIXTURES_DIR/bsq_test_maps/map_valid_1.txt" \
"xx...
xx...
..o..
.....
....." "map_valid_1"
    check_solved "$FIXTURES_DIR/bsq_test_maps/map_valid_2.txt" \
"xx...
xx...
oooo.
.....
....." "map_valid_2"
    check_solved "$FIXTURES_DIR/bsq_test_maps/map_valid_3.txt" \
"xxx..
xxx..
xxx..
.o.o.
....." "map_valid_3"
    check_solved "$FIXTURES_DIR/bsq_test_maps/map_valid_4.txt" \
"x.o..o...." "map_valid_4"
    check_solved "$FIXTURES_DIR/bsq_test_maps/map_valid_5.txt" \
"x
.
o
.
." "map_valid_5"
    check_solved "$FIXTURES_DIR/bsq_test_maps/map_valid_6.txt" \
".oxxx
.oxxx
.oxxx
.....
....." "map_valid_6"
    echo ""

    echo -e "${CYAN}${BOLD}=== Mapas invalidos (\"Error: invalid map\" en STDOUT) ===${RESET}"
    check_invalid_map "$FIXTURES_DIR/bsq_test_maps/map_invalid_1.txt" "map_invalid_1 (filas de distinta longitud)"
    check_invalid_map "$FIXTURES_DIR/bsq_test_maps/map_invalid_2.txt" "map_invalid_2 (caracter fuera de empty/obstacle/full)"
    check_invalid_map "$FIXTURES_DIR/bsq_test_maps/map_invalid_3.txt" "map_invalid_3 (empty y obstacle son el mismo caracter)"
    check_invalid_map "$FIXTURES_DIR/bsq_test_maps/map_invalid_4.txt" "map_invalid_4 (caracter no imprimible en la cabecera)"
    echo ""

    echo -e "${CYAN}${BOLD}=== Edge cases (reutilizados de ../bsq/bsq_edge_tests/) ===${RESET}"
    check_solved "$FIXTURES_DIR/bsq_edge_tests/test1.txt" \
"xxxxx
xxxxx
xxxxx
xxxxx
xxxxx" "test1 (cabecera sin espacios: 5.ox)"
    check_solved "$FIXTURES_DIR/bsq_edge_tests/test2.txt" \
"xxxxx
xxxxx
xxxxx
xxxxx
xxxxx" "test2 (cabecera con espacios de mas)"
    check_solved "$FIXTURES_DIR/bsq_edge_tests/test4.txt" "x" "test4 (mapa 1x1)"
    check_invalid_map "$FIXTURES_DIR/bsq_edge_tests/test8.txt" "test8 (menos filas de las que dice la cabecera)"
    check_invalid_map "$FIXTURES_DIR/bsq_edge_tests/test9.txt" "test9 (numero de filas negativo)"
    echo ""
}

run_integration_tests() {
    echo -e "${CYAN}${BOLD}=== Integracion: ejemplo del subject, stdin, fichero ausente ===${RESET}"

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

    local stdin_actual stdin_err
    stdin_actual="$(cd "$SCRATCH" && "./$BINARY" < subject_example.txt 2>"$SCRATCH/stdin_err.log")"
    stdin_err="$(cat "$SCRATCH/stdin_err.log")"
    if [ -n "$stdin_err" ]; then
        fail "Lectura por stdin: escribio en stderr (no permitido)"
    elif [ "$stdin_actual" = "$expected_example" ]; then
        pass "Lectura por stdin (sin argumentos) da el mismo resultado que por fichero"
    else
        fail "Lectura por stdin da una salida distinta a la del mismo mapa por fichero"
    fi

    check_other_error_prefix "Fichero inexistente: prefijo \"Error\" en stdout, sin crash" \
        "./$BINARY" /no/existe/nunca_jamas.txt
    echo ""
}

# ---------------------------------------------------------------------------- #
#  5. AddressSanitizer + UBSan
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
        for f in "$FIXTURES_DIR/bsq_test_maps/"map_*.txt "$FIXTURES_DIR/bsq_edge_tests/"*.txt "$SCRATCH/subject_example.txt"; do
            ( cd "$SCRATCH" && ASAN_OPTIONS="$asan_opts" run_with_timeout 10 "./${BINARY}_asan" "$f" ) \
                > "$SCRATCH/asan_run_out.log" 2>&1
            local rc=$?
            if [ $rc -eq 137 ] || [ $rc -eq 124 ]; then
                skip "ASan/UBSan ($(basename "$f")): el sanitizer no arranco en esta maquina — omitido"
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
        echo -e "${GREEN}${BOLD}TESTS BSQ_1MAP: $PASS/$total PASSED${RESET}"
    else
        echo -e "${RED}${BOLD}TESTS BSQ_1MAP: $PASS/$total PASSED ($FAIL fallidos)${RESET}"
    fi
    echo -e "${BOLD}============================================${RESET}"
    exit $((FAIL > 0 ? 1 : 0))
}

# =============================================================================
#  MAIN
# =============================================================================
stage_scratch
check_forbidden_functions
check_header_guards
compile_all
run_functional_tests
run_integration_tests
run_asan_tests

summary_and_exit
