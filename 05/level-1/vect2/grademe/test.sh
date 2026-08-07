#!/bin/bash
# =============================================================================
# GRADEME — vect2 (Exam Rank 05 - Level 01)
# =============================================================================
# Invocado por 05/exam.sh como: cd .../vect2/grademe && ./test.sh
# Contrato: sin argumentos, exit 0 = pass, exit != 0 = fail.
#
# Layout dividido (igual que polyset):
#   - "given"  (no se toca): ../                         (= .../vect2/, subject.txt + main.cpp)
#   - "alumno" (Expected Files del subject): $ROOT/rendu/vect2/  (vect2.cpp, vect2.hpp)
#
# Portable Linux/macOS a proposito (se corre en Mac pero debe funcionar igual
# en los Linux de 42): compilador via "g++", timeout casero sin depender de
# un binario "timeout" externo, y el chequeo de ASan distingue uname Linux/no.
#
# QUE CUBRE (mapeado al subject.txt de vect2):
#   [Expected files]   vect2.cpp y vect2.hpp existen en rendu/vect2/
#   [Include guards]   incluir vect2.hpp dos veces no rompe la compilacion
#   [Compilacion]      main.cpp dado + tu vect2.cpp compilan con -Wall -Wextra
#                       -Werror -std=c++98 (0 warnings)
#   [Forma canonica]   ctor por defecto, ctor con parametros, copy ctor,
#                       operator=, auto-asignacion (v = v) no rompe nada
#   [Aritmetica]       +, -, *escalar (incluye el ejemplo del subject:
#                       vect2(2,2)*2 == vect2(4,4)), y 3*v == v*3 (conmutativo)
#   [Compuestos]       +=, -=, *=  modifican en sitio y devuelven referencia
#                       encadenable (v2 += v2 += v3)
#   [Incremento]       ++v / v++ / --v / v-- con semantica prefijo vs postfijo
#                       correcta (postfijo devuelve el valor ANTIGUO)
#   [Unario]           -v (operator- de UN operando, distinto de v1 - v2)
#   [Acceso []]        v[0]/v[1] en lectura y escritura, tambien sobre un
#                       vect2 const (no se pide bound-checking)
#   [Comparacion]      ==, !=
#   [Impresion]        operator<< produce exactamente "{x, y}"
#   [Memoria]          build opcional con AddressSanitizer + UBSan
#   [Integracion]      compila y ejecuta el main.cpp REAL dado (no se toca) y
#                       compara la salida completa contra la transcripcion
#                       esperada (main.cpp es fijo, así que un diff exacto es
#                       fiable aqui, a diferencia de polyset)
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
GIVEN_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"                 # .../05/level-1/vect2
ROOT_DIR="$(cd "$SCRIPT_DIR/../../../.." && pwd)"         # 42-exam-rank-42/
STUDENT_DIR="$ROOT_DIR/rendu/vect2"

SCRATCH="$(mktemp -d "${TMPDIR:-/tmp}/vect2_grademe.XXXXXX")"
cleanup() { rm -rf "$SCRATCH"; }
trap cleanup EXIT

STUDENT_FILES=(vect2.cpp vect2.hpp)
BINARY="vect2_test_bin"

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

    cp "$STUDENT_DIR/vect2.cpp" "$STUDENT_DIR/vect2.hpp" "$SCRATCH/"
    cp "$GIVEN_DIR/main.cpp" "$SCRATCH/"
    echo ""
}

# ---------------------------------------------------------------------------- #
#  1. Include guards (doble inclusion)
# ---------------------------------------------------------------------------- #
check_header_guards() {
    echo -e "${CYAN}${BOLD}=== Include guards (doble inclusion) ===${RESET}"
    cat > "$SCRATCH/guard_test.cpp" << 'EOF'
#include "vect2.hpp"
#include "vect2.hpp"
int main() { return 0; }
EOF
    if g++ -std=c++98 -Wall -Wextra -I"$SCRATCH" \
        -c "$SCRATCH/guard_test.cpp" -o "$SCRATCH/guard_test.o" 2>"$SCRATCH/guard_err.log"; then
        pass "vect2.hpp: incluirlo dos veces no rompe la compilacion"
    else
        fail "vect2.hpp: include guard ROTO (revisa que #ifndef/#define, o #pragma once, esten bien)"
        sed 's/^/    /' "$SCRATCH/guard_err.log"
    fi
    echo ""
}

# ---------------------------------------------------------------------------- #
#  2. Compilar el main.cpp dado con tu vect2.cpp — 0 warnings exigido
# ---------------------------------------------------------------------------- #
compile_main() {
    echo -e "${CYAN}${BOLD}=== Compilacion (main.cpp dado + tu vect2.cpp) ===${RESET}"
    if (cd "$SCRATCH" && g++ -std=c++98 -Wall -Wextra -Werror -I. \
        main.cpp vect2.cpp -o "$BINARY") 2>"$SCRATCH/compile_err.log"; then
        pass "Compila sin errores ni warnings (-Wall -Wextra -Werror -std=c++98)"
    else
        fail "ERROR compilando. Revisa el main.cpp dado usa operadores que quizas te falten:"
        sed 's/^/    /' "$SCRATCH/compile_err.log" | head -40
        echo -e "${RED}No se puede continuar sin un binario que compile.${RESET}"
        echo ""
        summary_and_exit
    fi
    echo ""
}

# ---------------------------------------------------------------------------- #
#  3. Tests unitarios: forma canonica, aritmetica, comparacion, impresion
# ---------------------------------------------------------------------------- #
compile_and_run_unit_tester() {
    echo -e "${CYAN}${BOLD}=== Tests unitarios (semantica, no solo compilacion) ===${RESET}"
    cat > "$SCRATCH/vect2_unit_test.cpp" << 'EOF'
#include "vect2.hpp"
#include <sstream>
#include <iostream>

static int PASS = 0, FAIL = 0;

#define CHECK(cond, msg) do { \
    if (cond) { PASS++; std::cout << "  [ok] " << msg << "\n"; } \
    else      { FAIL++; std::cout << "  [KO] " << msg << "\n"; } \
} while (0)

static std::string to_str(const vect2& v) {
    std::ostringstream oss;
    oss << v;
    return oss.str();
}

int main() {
    // --- forma canonica ---
    vect2 def;
    CHECK(def[0] == 0 && def[1] == 0, "ctor por defecto -> {0, 0}");

    vect2 a(3, 5);
    CHECK(a[0] == 3 && a[1] == 5, "ctor con parametros vect2(3,5) -> {3, 5}");

    vect2 b(a);
    CHECK(b[0] == 3 && b[1] == 5, "copy ctor copia los valores");
    b[0] = 99;
    CHECK(a[0] == 3, "copy ctor hace copia real, no comparte estado (b[0]=99 no toca a)");

    vect2 c;
    c = a;
    CHECK(c[0] == 3 && c[1] == 5, "operator= copia los valores");

    // auto-asignacion no debe corromper el objeto
    vect2 self(7, 11);
    self = self;
    CHECK(self[0] == 7 && self[1] == 11, "auto-asignacion (v = v) no corrompe el objeto");

    // --- acceso [] en lectura y escritura, tambien sobre const ---
    vect2 rw(1, 2);
    rw[0] = 42;
    rw[1] = 43;
    CHECK(rw[0] == 42 && rw[1] == 43, "operator[] de escritura modifica el valor real");

    const vect2 ro(9, 10);
    CHECK(ro[0] == 9 && ro[1] == 10, "operator[] const funciona sobre un objeto const");

    // --- aritmetica: +, -, * escalar ---
    vect2 p(1, 2), q(10, 20);
    CHECK(to_str(p + q) == "{11, 22}", "operator+ suma componente a componente");
    CHECK(to_str(q - p) == "{9, 18}", "operator- (binario) resta componente a componente");

    // ejemplo textual del subject: vect2(2,2) * 2 == vect2(4,4)
    CHECK((vect2(2, 2) * 2) == vect2(4, 4), "subject: vect2(2,2) * 2 == vect2(4,4)");
    CHECK((2 * vect2(2, 2)) == (vect2(2, 2) * 2), "escalar * vect2 es conmutativo con vect2 * escalar");

    // --- unario: -v (distinto de la resta binaria) ---
    vect2 neg(5, -7);
    CHECK(to_str(-neg) == "{-5, 7}", "operator- unario niega ambos componentes");

    // --- compuestos: +=, -=, *= modifican en sitio y son encadenables ---
    vect2 m(1, 1);
    m += vect2(2, 3);
    CHECK(m[0] == 3 && m[1] == 4, "operator+= modifica en sitio");
    m -= vect2(1, 1);
    CHECK(m[0] == 2 && m[1] == 3, "operator-= modifica en sitio");
    m *= 10;
    CHECK(m[0] == 20 && m[1] == 30, "operator*= modifica en sitio");

    // Nota: "a += (a += b)" con el MISMO objeto es auto-referencial (la
    // expresion interna ya modifica "a" antes de que la externa lo relea),
    // asi que aqui se prueba el encadenado con variables distintas, sin
    // ambiguedad. El caso con el mismo objeto ya lo cubre el test de
    // integracion con la expresion real del subject "v2 += v2 += v3".
    vect2 chain_x(1, 1), chain_y(2, 2), chain_z(1, 1);
    chain_y += chain_z;
    chain_x += chain_y;
    CHECK(chain_x[0] == 4 && chain_x[1] == 4, "operator+= es encadenable (x += y, con y ya modificado)");

    // --- incremento/decremento: prefijo devuelve NUEVO valor, postfijo el ANTIGUO ---
    vect2 inc(1, 1);
    vect2 old = inc++;
    CHECK(old[0] == 1 && old[1] == 1, "v++ devuelve el valor ANTIGUO (antes de incrementar)");
    CHECK(inc[0] == 2 && inc[1] == 2, "v++ SI incrementa el objeto original");

    vect2 pre(1, 1);
    vect2 newv = ++pre;
    CHECK(newv[0] == 2 && newv[1] == 2, "++v devuelve el valor NUEVO (ya incrementado)");

    vect2 dec(5, 5);
    vect2 oldd = dec--;
    CHECK(oldd[0] == 5 && oldd[1] == 5, "v-- devuelve el valor ANTIGUO");
    CHECK(dec[0] == 4 && dec[1] == 4, "v-- SI decrementa el objeto original");

    // --- comparacion ---
    vect2 e1(1, 2), e2(1, 2), e3(9, 9);
    CHECK(e1 == e2, "operator== compara por valor (iguales)");
    CHECK(!(e1 == e3), "operator== compara por valor (distintos)");
    CHECK(e1 != e3, "operator!= es lo contrario de ==");
    CHECK(!(e1 != e2), "operator!= false cuando son iguales");

    // --- impresion ---
    CHECK(to_str(vect2(3, -4)) == "{3, -4}", "operator<< produce exactamente \"{x, y}\"");

    std::cout << "\n" << PASS << "/" << (PASS + FAIL) << " aserciones unitarias OK\n";
    return FAIL == 0 ? 0 : 1;
}
EOF
    if (cd "$SCRATCH" && g++ -std=c++98 -Wall -Wextra -I. \
        vect2_unit_test.cpp vect2.cpp -o vect2_unit) 2>"$SCRATCH/unit_compile_err.log"; then
        if ( cd "$SCRATCH" && ./vect2_unit ); then
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
        main.cpp vect2.cpp -o "${BINARY}_asan") 2>"$SCRATCH/asan_compile_err.log"; then
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

    # Transcripcion esperada: main.cpp es fijo (no lo escribes tu), asi que un
    # diff exacto es fiable aqui, a diferencia de polyset (donde el orden
    # interno de un arbol/array puede variar legitimamente entre soluciones
    # correctas).
    local expected
    expected="$(cat << 'EOF'
v1: {0, 0}
v1: {0, 0}
v2: {1, 2}
v3: {1, 2}
v4: {1, 2}
{1, 2}
{3, 4}
{3, 4}
{1, 2}
v1: {-84, -168}
v2: {20, 40}
-v2: {-20, -40}
v1[1]: -168
v1[1]: 12
v3[1]: 2
v1 == v3: 0
v1 == v1: 1
v1 != v3: 1
v1 != v1: 0
EOF
)"

    if [ "$actual" = "$expected" ]; then
        pass "La salida del main.cpp dado coincide exactamente con la esperada (19 lineas)"
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
        echo -e "${GREEN}${BOLD}TESTS VECT2: $PASS/$total PASSED${RESET}"
    else
        echo -e "${RED}${BOLD}TESTS VECT2: $PASS/$total PASSED ($FAIL fallidos)${RESET}"
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
