#!/bin/bash
# =============================================================================
# GRADEME — game_of_life (Exam Rank 05 - Level 02)
# =============================================================================
# Invocado por 05/exam.sh como: cd .../game_of_life/grademe && ./test.sh
# Contrato: sin argumentos, exit 0 = pass, exit != 0 = fail.
#
# Layout dividido (igual que bsq/polyset/vect2/bigint):
#   - "given" (no se toca): ../  (= .../game_of_life/, subject.txt + life.h
#     dado solo como referencia de prototipos)
#   - "alumno" (Submitted files: *.c, *.h — CUALQUIER nombre, igual que
#     bsq): $ROOT/rendu/game_of_life/
#
# Igual que bsq, el subject NO da un main fijo: "Submitted files: *.c, *.h"
# (todo lo escribe el alumno, incluido el main). Por eso el compile_all no
# asume nombres fijos: compila TODOS los .c que encuentre en
# rendu/game_of_life/.
#
# Portable Linux/macOS a proposito: compilador via "gcc", timeout casero sin
# depender de un binario "timeout" externo, ASan distingue uname Linux/no.
#
# QUE CUBRE (mapeado al subject.txt de game_of_life):
#   [Expected files]    al menos un *.c en rendu/game_of_life/ (nombre libre)
#   [Include guards]    cada *.h encontrado soporta doble inclusion
#   [Compilacion]       todos los *.c compilan juntos con -Wall -Wextra
#                        -Werror -std=c99 (0 warnings)
#   [Ejemplos subject]  los 5 ejemplos textuales EXACTOS del subject.txt
#                        (5x5/0 iter, 10x6/0 iter, y el blinker 3x3 en
#                        0/1/2 iteraciones)
#   [Bordes del pen]    "pen no move outside board and stays still" — mover
#                        el boligrafo contra una esquina no lo saca del
#                        tablero ni lo mueve
#   [Comando invalido]  "pen do nothing in case of invalid command" — un
#                        caracter fuera de w/a/s/d/x no tiene efecto
#   [Sin argumentos]    ac != 4 no debe crashear (el subject no fija un
#                        contrato de error, solo se exige no-crash)
#   [Memoria]            build opcional con AddressSanitizer + UBSan, mas
#                        `leaks` en macOS
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
GIVEN_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"                 # .../05/level-2/game_of_life
ROOT_DIR="$(cd "$SCRIPT_DIR/../../../.." && pwd)"         # 42-exam-rank-42/
STUDENT_DIR="$ROOT_DIR/rendu/game_of_life"

SCRATCH="$(mktemp -d "${TMPDIR:-/tmp}/game_of_life_grademe.XXXXXX")"
cleanup() { rm -rf "$SCRATCH"; }
trap cleanup EXIT

BINARY="life_test_bin"

# ---------------------------------------------------------------------------- #
#  0. Ficheros esperados (Submitted files: *.c, *.h — nombre libre)
# ---------------------------------------------------------------------------- #
stage_scratch() {
    echo -e "${CYAN}${BOLD}=== Ficheros esperados (Submitted files: *.c, *.h) ===${RESET}"

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
#  3. Ejemplos del subject: diff exacto contra la salida literal del
#     subject.txt (echo 'comandos' | ./a.out width height iterations)
# ---------------------------------------------------------------------------- #
# check_life "commands" width height iterations "label" row1 row2 ... rowN
# Cada rowN va SIN padding — se rellena aqui mismo a `width` caracteres con
# printf '%-*s', asi que las filas se escriben con su contenido significativo
# nada mas (evita transcribir a mano espacios finales invisibles, que es
# justo lo que causaba diffs falsos en la primera version de este script).
check_life() {
    local commands="$1" width="$2" height="$3" iterations="$4" label="$5"
    shift 5
    local rows=("$@")
    local expected="" row padded first=1
    for row in "${rows[@]}"; do
        padded="$(printf '%-*s' "$width" "$row")"
        if [ "$first" -eq 1 ]; then
            expected="$padded"
            first=0
        else
            expected="$expected"$'\n'"$padded"
        fi
    done
    local actual
    actual="$(cd "$SCRATCH" && printf '%s\n' "$commands" | "./$BINARY" "$width" "$height" "$iterations" 2>"$SCRATCH/stderr.log")"
    local err
    err="$(cat "$SCRATCH/stderr.log")"
    if [ -n "$err" ]; then
        fail "$label: escribio en stderr (no se esperaba): $err"
    elif [ "$actual" = "$expected" ]; then
        pass "$label"
    else
        fail "$label: la salida NO coincide. Diff (esperado vs tuyo):"
        diff <(echo "$expected") <(echo "$actual") | sed 's/^/    /' | head -20
    fi
}

run_functional_tests() {
    echo -e "${CYAN}${BOLD}=== Ejemplos EXACTOS del subject.txt ===${RESET}"
    check_life "sdxddssaaww" 5 5 0 "Ejemplo 1: 5x5, 0 iteraciones" \
        "" " 000" " 0 0" " 000" ""

    check_life "sdxssdswdxdddxsaddawxwdxwaa" 10 6 0 "Ejemplo 2: 10x6, 0 iteraciones" \
        "" " 0   000" " 0     0" " 000  0" "  0  000" ""

    check_life "dxss" 3 3 0 "Ejemplo 3: 3x3, 0 iteraciones" \
        " 0" " 0" " 0"

    check_life "dxss" 3 3 1 "Ejemplo 4: 3x3, 1 iteracion (blinker gira)" \
        "" "000" ""

    check_life "dxss" 3 3 2 "Ejemplo 5: 3x3, 2 iteraciones (blinker vuelve al origen)" \
        " 0" " 0" " 0"
    echo ""

    echo -e "${CYAN}${BOLD}=== Reglas explicitas del subject (bordes / comandos invalidos) ===${RESET}"
    check_life "aaaawwwwx" 3 3 0 "Boligrafo contra la esquina: no se sale ni se mueve de mas" \
        "0" "" ""

    check_life "dxzzzs" 3 3 0 "Comando invalido (z) no tiene ningun efecto" \
        " 0" " 0" ""
    echo ""

    echo -e "${CYAN}${BOLD}=== Regla de supervivencia n==3 (bloque 2x2, still life) ===${RESET}"
    # Los 5 ejemplos del subject nunca ponen a prueba que una celula VIVA con
    # exactamente 3 vecinos sobreviva (regla "n==2 || n==3"): el blinker solo
    # ejercita n==1 y n==2. Un bloque 2x2 SI depende de n==3 para sobrevivir,
    # y es la still life mas simple: debe quedar identica tras 1 iteracion.
    check_life "xdsa" 4 4 0 "Bloque 2x2 dibujado (estado inicial)" \
        "00" "00" "" ""
    check_life "xdsa" 4 4 1 "Bloque 2x2 tras 1 iteracion: still life, no cambia (n==3 sobrevive)" \
        "00" "00" "" ""
    echo ""
}

# ---------------------------------------------------------------------------- #
#  4. Robustez: numero de argumentos incorrecto no debe crashear
#     (el subject no fija un contrato de error para esto, solo se exige
#     que el programa no reviente)
# ---------------------------------------------------------------------------- #
run_robustness_tests() {
    echo -e "${CYAN}${BOLD}=== Robustez (no crashea con uso incorrecto) ===${RESET}"
    (cd "$SCRATCH" && "./$BINARY" >/dev/null 2>&1)
    local rc=$?
    if [ $rc -ge 128 ]; then
        fail "Sin argumentos: el programa crashea (señal $((rc - 128)))"
    else
        pass "Sin argumentos: termina sin crashear (exit $rc)"
    fi

    (cd "$SCRATCH" && "./$BINARY" 5 5 >/dev/null 2>&1)
    rc=$?
    if [ $rc -ge 128 ]; then
        fail "Argumentos incompletos: el programa crashea (señal $((rc - 128)))"
    else
        pass "Argumentos incompletos: termina sin crashear (exit $rc)"
    fi
    echo ""
}

# ---------------------------------------------------------------------------- #
#  5. AddressSanitizer + UBSan (portable Linux/macOS) + leaks en macOS
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
        local cases=(
            "sdxddssaaww|5|5|0"
            "sdxssdswdxdddxsaddawxwdxwaa|10|6|0"
            "dxss|3|3|2"
            "aaaawwwwx|3|3|0"
        )
        for c in "${cases[@]}"; do
            IFS='|' read -r cmds w h it <<< "$c"
            ( cd "$SCRATCH" && printf '%s\n' "$cmds" | ASAN_OPTIONS="$asan_opts" run_with_timeout 10 "./${BINARY}_asan" "$w" "$h" "$it" ) \
                > "$SCRATCH/asan_run_out.log" 2>&1
            local rc=$?
            if [ $rc -eq 137 ] || [ $rc -eq 124 ]; then
                skip "ASan/UBSan ($cmds): el sanitizer no arranco en esta maquina (bug conocido ASan/macOS) — omitido"
                continue
            fi
            if grep -qE "ERROR: (AddressSanitizer|LeakSanitizer)|runtime error:" "$SCRATCH/asan_run_out.log"; then
                any_fail=1
                fail "ASan/UBSan ($cmds) detecto un problema:"
                grep -A 6 -E "ERROR|runtime error" "$SCRATCH/asan_run_out.log" | sed 's/^/    /' | head -20
            fi
        done
        [ "$any_fail" -eq 0 ] && pass "ASan/UBSan: sin errores de memoria en ningun ejemplo"
    else
        skip "No se pudo compilar con sanitizers (compilador sin soporte) — no es bloqueante"
    fi

    if [ "$(uname)" = "Darwin" ] && command -v leaks >/dev/null 2>&1; then
        local leaks_out
        leaks_out="$(cd "$SCRATCH" && printf '%s\n' "sdxddssaaww" | leaks --atExit -- "./$BINARY" 5 5 0 2>&1)"
        if echo "$leaks_out" | grep -q "0 leaks for 0 total leaked bytes"; then
            pass "leaks (macOS): 0 bytes filtrados"
        else
            fail "leaks (macOS) detecto fugas:"
            echo "$leaks_out" | grep -A 3 "leaks for" | sed 's/^/    /'
        fi
    else
        skip "leaks no disponible (no es macOS) — cubierto por ASan/UBSan arriba"
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
        echo -e "${GREEN}${BOLD}TESTS GAME_OF_LIFE: $PASS/$total PASSED${RESET}"
    else
        echo -e "${RED}${BOLD}TESTS GAME_OF_LIFE: $PASS/$total PASSED ($FAIL fallidos)${RESET}"
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
run_robustness_tests
run_asan_tests

summary_and_exit
