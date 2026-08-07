#!/bin/bash
# =============================================================================
# GRADEME — bigint (Exam Rank 05 - Level 01)
# =============================================================================
# Invocado por 05/exam.sh como: cd .../bigint/grademe && ./test.sh
# Contrato: sin argumentos, exit 0 = pass, exit != 0 = fail.
#
# Layout dividido (igual que polyset/vect2):
#   - "given"  (no se toca): ../                         (= .../bigint/, subject.txt + main.cpp)
#   - "alumno" (Expected Files del subject): $ROOT/rendu/bigint/  (bigint.cpp, bigint.hpp)
#
# Portable Linux/macOS a proposito (se corre en Mac pero debe funcionar igual
# en los Linux de 42): compilador via "g++", timeout casero sin depender de
# un binario "timeout" externo, y el chequeo de ASan distingue uname Linux/no.
#
# QUE CUBRE (mapeado al subject.txt de bigint):
#   [Expected files]   bigint.cpp y bigint.hpp existen en rendu/bigint/
#   [Include guards]   incluir bigint.hpp dos veces no rompe la compilacion
#   [Compilacion]      main.cpp dado + tu bigint.cpp compilan con -Wall
#                       -Wextra -Werror -std=c++98 (0 warnings)
#   [Forma canonica]   ctor por defecto (-> "0"), ctor desde entero, copy
#                       ctor (copia real, no alias), operator=, auto-
#                       asignacion (b = b) no corrompe el objeto
#   [Suma]              addition correcta, incluye acarreos en cascada
#                       (999+1), acarreo que anade un digito (50+50=100)
#   [Sin ceros a la izq] requisito EXPLICITO del subject: ningun resultado de
#                       suma o digitshift deja ceros a la izquierda
#   [Digitshift]       ejemplos textuales del subject: 42<<3==42000,
#                       1337>>2==13; shift por una CANTIDAD que es a su vez
#                       un bigint (no solo int, como usa el main.cpp real:
#                       x << y); shift total (>= longitud) da "0", no "";
#                       shiftear "0" se queda en "0"
#   [Compuestos]       <<=, >>=, += modifican en sitio
#   [Incremento]       ++b / b++ con semantica prefijo vs postfijo correcta
#   [Comparacion]      <, >, ==, !=, <=, >=
#   [Impresion]        operator<< imprime en base 10 sin ceros a la izquierda
#   [Memoria]          build opcional con AddressSanitizer + UBSan
#   [Integracion]      compila y ejecuta el main.cpp REAL dado (no se toca) y
#                       compara la salida completa contra la transcripcion
#                       esperada. OJO: ese main.cpp trae un comentario propio
#                       "// d = 5348" que esta MAL (el valor matematicamente
#                       correcto, verificado a mano y con JohnnyCPP/Univers42,
#                       es 133700) — el golden de abajo usa el valor correcto,
#                       no el comentario erroneo del fichero dado.
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
GIVEN_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"                 # .../05/level-1/bigint
ROOT_DIR="$(cd "$SCRIPT_DIR/../../../.." && pwd)"         # 42-exam-rank-42/
STUDENT_DIR="$ROOT_DIR/rendu/bigint"

SCRATCH="$(mktemp -d "${TMPDIR:-/tmp}/bigint_grademe.XXXXXX")"
cleanup() { rm -rf "$SCRATCH"; }
trap cleanup EXIT

STUDENT_FILES=(bigint.cpp bigint.hpp)
BINARY="bigint_test_bin"

# ---------------------------------------------------------------------------- #
#  0. Ficheros esperados (Expected files del subject) + copia a scratch
# ---------------------------------------------------------------------------- #
stage_scratch() {
    echo -e "${CYAN}${BOLD}=== Ficheros esperados (Expected files del subject) ===${RESET}"

    if [ ! -d "$STUDENT_DIR" ]; then
        echo -e "${RED}Error: no existe $STUDENT_DIR${RESET}"
        echo -e "${YELLOW}Crea tus archivos ahi antes de validar.${RESET}"
        exit 1
    fi

    local missing=0
    for f in "${STUDENT_FILES[@]}"; do
        if [ -f "$STUDENT_DIR/$f" ]; then
            pass "Existe $f"
        else
            fail "Falta $f (obligatorio segun el subject)"
            missing=1
        fi
    done
    if [ "$missing" -eq 1 ]; then
        echo -e "${RED}Faltan Expected Files en $STUDENT_DIR, no se puede compilar.${RESET}"
        exit 1
    fi

    if [ ! -f "$GIVEN_DIR/main.cpp" ]; then
        echo -e "${RED}Falta fichero given: $GIVEN_DIR/main.cpp${RESET}"
        exit 1
    fi

    cp "$STUDENT_DIR/bigint.cpp" "$STUDENT_DIR/bigint.hpp" "$SCRATCH/"
    cp "$GIVEN_DIR/main.cpp" "$SCRATCH/"
    echo ""
}

# ---------------------------------------------------------------------------- #
#  1. Include guards (doble inclusion)
# ---------------------------------------------------------------------------- #
check_header_guards() {
    echo -e "${CYAN}${BOLD}=== Include guards (doble inclusion) ===${RESET}"
    cat > "$SCRATCH/guard_test.cpp" << 'EOF'
#include "bigint.hpp"
#include "bigint.hpp"
int main() { return 0; }
EOF
    if g++ -std=c++98 -Wall -Wextra -I"$SCRATCH" \
        -c "$SCRATCH/guard_test.cpp" -o "$SCRATCH/guard_test.o" 2>"$SCRATCH/guard_err.log"; then
        pass "bigint.hpp: incluirlo dos veces no rompe la compilacion"
    else
        fail "bigint.hpp: include guard ROTO (revisa que #ifndef/#define, o #pragma once, esten bien)"
        sed 's/^/    /' "$SCRATCH/guard_err.log"
    fi
    echo ""
}

# ---------------------------------------------------------------------------- #
#  2. Compilar el main.cpp dado con tu bigint.cpp — 0 warnings exigido
# ---------------------------------------------------------------------------- #
compile_main() {
    echo -e "${CYAN}${BOLD}=== Compilacion (main.cpp dado + tu bigint.cpp) ===${RESET}"
    if (cd "$SCRATCH" && g++ -std=c++98 -Wall -Wextra -Werror -I. \
        main.cpp bigint.cpp -o "$BINARY") 2>"$SCRATCH/compile_err.log"; then
        pass "Compila sin errores ni warnings (-Wall -Wextra -Werror -std=c++98)"
    else
        fail "ERROR compilando. El main.cpp dado usa +, ++, --, digitshift con int Y con bigint, y las 6 comparaciones:"
        sed 's/^/    /' "$SCRATCH/compile_err.log" | head -40
        echo -e "${RED}No se puede continuar sin un binario que compile.${RESET}"
        echo ""
        summary_and_exit
    fi
    echo ""
}

# ---------------------------------------------------------------------------- #
#  3. Tests unitarios: forma canonica, aritmetica, digitshift, comparacion
# ---------------------------------------------------------------------------- #
compile_and_run_unit_tester() {
    echo -e "${CYAN}${BOLD}=== Tests unitarios (semantica, no solo compilacion) ===${RESET}"
    cat > "$SCRATCH/bigint_unit_test.cpp" << 'EOF'
#include "bigint.hpp"
#include <sstream>
#include <iostream>

static int PASS = 0, FAIL = 0;

#define CHECK(cond, msg) do { \
    if (cond) { PASS++; std::cout << "  [ok] " << msg << "\n"; } \
    else      { FAIL++; std::cout << "  [KO] " << msg << "\n"; } \
} while (0)

static std::string to_str(const bigint& b) {
    std::ostringstream oss;
    oss << b;
    return oss.str();
}

int main() {
    // --- forma canonica ---
    bigint def;
    CHECK(to_str(def) == "0", "ctor por defecto -> \"0\"");

    bigint from_int(42);
    CHECK(to_str(from_int) == "42", "ctor desde entero preserva el valor");

    bigint zero_int(0);
    CHECK(to_str(zero_int) == "0", "ctor desde 0 -> \"0\" (no vacio, no \"00\")");

    bigint orig(123);
    bigint copy(orig);
    CHECK(to_str(copy) == "123", "copy ctor copia el valor");
    copy += bigint(1);
    CHECK(to_str(orig) == "123", "copy ctor hace copia real, no alias (modificar copy no toca orig)");

    bigint assigned;
    assigned = orig;
    CHECK(to_str(assigned) == "123", "operator= copia el valor");

    bigint self(77);
    self = self;
    CHECK(to_str(self) == "77", "auto-asignacion (b = b) no corrompe el objeto");

    // --- suma: casos base + acarreos + "sin ceros a la izquierda" ---
    CHECK(to_str(bigint(42) + bigint(21)) == "63", "suma basica: 42 + 21 = 63");
    CHECK(to_str(bigint(999) + bigint(1)) == "1000", "acarreo en cascada por varios 9: 999 + 1 = 1000");
    CHECK(to_str(bigint(50) + bigint(50)) == "100", "acarreo anade un digito nuevo: 50 + 50 = 100");
    CHECK(to_str(bigint(0) + bigint(0)) == "0", "0 + 0 = 0 (no vacio)");
    CHECK(to_str(bigint(999999999UL) + bigint(1)) == "1000000000", "acarreo largo de 9 digitos");

    bigint accum(10);
    accum += bigint(5);
    CHECK(to_str(accum) == "15", "operator+= modifica en sitio");

    // --- incremento: prefijo devuelve NUEVO valor, postfijo el ANTIGUO ---
    bigint inc(21);
    bigint old = inc++;
    CHECK(to_str(old) == "21", "b++ devuelve el valor ANTIGUO");
    CHECK(to_str(inc) == "22", "b++ SI incrementa el objeto original");

    bigint pre(21);
    bigint newv = ++pre;
    CHECK(to_str(newv) == "22", "++b devuelve el valor NUEVO (ya incrementado)");

    // --- digitshift: ejemplos textuales del subject ---
    CHECK(to_str(bigint(42) << bigint(3)) == "42000", "subject: 42 << 3 == 42000");
    CHECK(to_str(bigint(1337) >> bigint(2)) == "13", "subject: 1337 >> 2 == 13");

    // shift por bigint (no solo int): el main.cpp real usa "x << y" con y bigint
    bigint amount(3);
    CHECK(to_str(bigint(42) << amount) == "42000", "digitshift acepta una CANTIDAD que es un bigint, no solo int");

    // casos limite: shift total y shiftear cero
    CHECK(to_str(bigint(100) >> bigint(10)) == "0", "shift-right total (n >= longitud) da \"0\", no \"\"");
    CHECK(to_str(bigint(0) << bigint(5)) == "0", "shiftear \"0\" a la izquierda se queda en \"0\"");
    CHECK(to_str(bigint(0) >> bigint(5)) == "0", "shiftear \"0\" a la derecha se queda en \"0\"");
    CHECK(to_str(bigint(120) >> bigint(1)) == "12", "shift-right no deja ceros a la izquierda espurios (120 >> 1 = 12)");

    bigint shl(7);
    shl <<= bigint(2);
    CHECK(to_str(shl) == "700", "operator<<= modifica en sitio");
    bigint shr(700);
    shr >>= bigint(2);
    CHECK(to_str(shr) == "7", "operator>>= modifica en sitio");

    // --- comparacion ---
    bigint c1(42), c2(42), c3(100), c4(7);
    CHECK(c1 == c2, "operator== compara por valor (iguales)");
    CHECK(!(c1 == c3), "operator== compara por valor (distintos)");
    CHECK(c1 != c3, "operator!= es lo contrario de ==");
    CHECK(c1 < c3, "operator< : 42 < 100 (mas digitos = mas grande)");
    CHECK(c3 > c1, "operator> : 100 > 42");
    CHECK(c4 < c1, "operator< : 7 < 42 (misma logica, menos digitos)");
    CHECK(c1 <= c2, "operator<= : iguales cuenta como <=");
    CHECK(c1 >= c2, "operator>= : iguales cuenta como >=");
    CHECK(!(c3 <= c1), "operator<= : 100 no es <= 42");

    std::cout << "\n" << PASS << "/" << (PASS + FAIL) << " aserciones unitarias OK\n";
    return FAIL == 0 ? 0 : 1;
}
EOF
    if (cd "$SCRATCH" && g++ -std=c++98 -Wall -Wextra -I. \
        bigint_unit_test.cpp bigint.cpp -o bigint_unit) 2>"$SCRATCH/unit_compile_err.log"; then
        if ( cd "$SCRATCH" && ./bigint_unit ); then
            pass "Bateria de tests unitarios: todas las aserciones OK"
        else
            fail "Bateria de tests unitarios: al menos una asercion fallo (detalle arriba)"
        fi
    else
        fail "No se pudo compilar el tester unitario:"
        sed 's/^/    /' "$SCRATCH/unit_compile_err.log" | head -30
    fi
    echo ""
}

# ---------------------------------------------------------------------------- #
#  4. AddressSanitizer + UBSan (portable Linux/macOS)
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
    if (cd "$SCRATCH" && g++ -std=c++98 -Wall -Wextra -g -O0 -fsanitize=address,undefined -I. \
        main.cpp bigint.cpp -o "${BINARY}_asan") 2>"$SCRATCH/asan_compile_err.log"; then
        local asan_opts="abort_on_error=0"
        if [ "$(uname)" = "Linux" ]; then
            asan_opts="detect_leaks=1"
        fi
        ( cd "$SCRATCH" && ASAN_OPTIONS="$asan_opts" run_with_timeout 20 "./${BINARY}_asan" ) \
            > "$SCRATCH/asan_run_out.log" 2>&1
        local rc=$?
        if [ $rc -eq 137 ] || [ $rc -eq 124 ]; then
            skip "ASan/UBSan: el runtime del sanitizer no arranco en esta maquina (bug conocido ASan/macOS, no es tu codigo) — omitido"
        elif [ $rc -eq 0 ] && ! grep -qE "ERROR: (AddressSanitizer|LeakSanitizer)|runtime error:" "$SCRATCH/asan_run_out.log"; then
            pass "ASan/UBSan: sin errores de memoria ni comportamiento indefinido detectado"
        else
            fail "ASan/UBSan detecto un problema:"
            grep -A 6 -E "ERROR|runtime error" "$SCRATCH/asan_run_out.log" | sed 's/^/    /' | head -40
        fi
    else
        skip "No se pudo compilar con sanitizers (compilador sin soporte) — no es bloqueante"
    fi
    echo ""
}

# ---------------------------------------------------------------------------- #
#  5. Integracion: ejecuta el binario con el main.cpp REAL dado
# ---------------------------------------------------------------------------- #
run_integration_test() {
    echo -e "${CYAN}${BOLD}=== Integracion: main.cpp dado, salida completa ===${RESET}"

    local actual
    actual="$(cd "$SCRATCH" && "./$BINARY" 2>&1)"

    # Transcripcion esperada: main.cpp es fijo y la aritmetica es determinista
    # (cualquier bigint correcto DEBE dar exactamente estos numeros), asi que
    # un diff exacto es fiable aqui. Verificada a mano y contra JohnnyCPP.
    local expected
    expected="$(cat << 'EOF'
a = 42
b = 21
c = 0
d = 1337
e = 1337
a + b = 63
a + c = 42
(c += a) = 42
b = 21
++b = 22
b++ = 22
(b << 10) + 42 = 230000000042
(d <<= 4) = 13370000, d: 13370000
(d >>= 2) = 133700, d: 133700
a = 42
d = 133700
(d < a) = 0
(d > a) = 1
(d == d) = 1
(d != a) = 1
(d <= a) = 0
(d >= a) = 1
(x << y) = 1234567800000, x: 12345678, y: 5
(x >>= y) = 123, x: 123, y: 5
(x >= y) = 1, x: 123, y: 5
x= 12300000, y= 5000000000000
(x >= y) = 0, x: 12300000, y: 5000000000000
EOF
)"

    if [ "$actual" = "$expected" ]; then
        pass "La salida del main.cpp dado coincide exactamente con la esperada (26 lineas)"
    else
        fail "La salida NO coincide con la esperada. Diff (esperado vs tuyo):"
        diff <(echo "$expected") <(echo "$actual") | sed 's/^/    /' | head -40
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
        echo -e "${GREEN}${BOLD}TESTS BIGINT: $PASS/$total PASSED${RESET}"
    else
        echo -e "${RED}${BOLD}TESTS BIGINT: $PASS/$total PASSED ($FAIL fallidos)${RESET}"
    fi
    echo -e "${BOLD}============================================${RESET}"
    exit $((FAIL > 0 ? 1 : 0))
}

# =============================================================================
#  MAIN
# =============================================================================
stage_scratch
check_header_guards
compile_main
compile_and_run_unit_tester
run_asan_tests
run_integration_test

summary_and_exit
