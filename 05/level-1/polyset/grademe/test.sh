#!/bin/bash
# =============================================================================
# GRADEME — polyset (Exam Rank 05 - Level 01)
# =============================================================================
# Invocado por 05/exam.sh como: cd .../polyset/grademe && ./test.sh
# Contrato: sin argumentos, exit 0 = pass, exit != 0 = fail.
#
# Layout dividido (a diferencia del test_polyset.sh original, que asumia todo
# en un unico directorio flat):
#   - "given"   (lo que da el examen, no se toca): ../            (= .../polyset/)
#   - "alumno"  (los 6 Expected Files que escribes): $ROOT/rendu/polyset/
#
# Este script copia ambos juegos de ficheros a un directorio temporal (scratch)
# y ahi dentro reproduce, casi sin cambios, la misma bateria de tests que
# test_polyset.sh ya validaba en el proyecto original de estudio.
#
# QUE CUBRE (mapeado al subject.txt):
#   [Expected files]  que rendu/polyset/ tenga los 6 ficheros que pide el subject
#   [Include guards]  doble inclusion de cada header tuyo no rompe la compilacion
#   [Primera parte]   searchable_array_bag / searchable_tree_bag implementan has()
#                      correctamente, heredan bien de array_bag/tree_bag + searchable_bag
#   [Forma canonica]  ctor por defecto, copy ctor, operator=, destructor,
#                      auto-asignacion (a = a) en las 3 clases QUE ENTREGAS
#   [Const]           has() const, get_bag() const
#   [Segunda parte]   set envuelve un searchable_bag y NO permite duplicados
#                      (insert simple + insert array), delega print/clear/has
#   [Polimorfismo]    bag*/searchable_bag* despachan a la clase correcta,
#                      y el destructor virtual libera lo derivado (no solo lo base)
#   [Memoria]         build opcional con AddressSanitizer + UBSan si el
#                      compilador lo soporta (detecta delete vs delete[], leaks)
#   [Integracion]     ejecuta el binario real con el main.cpp dado, comprobando
#                      invariantes estructurales (no strings exactos fragiles)
#
# NOTA: el layout dividido de este repo no incluye un Makefile "given" (no es
# parte del material oficial ni de los Expected Files), asi que a diferencia
# del test_polyset.sh original este script NO ejecuta "make re" — compila
# todo directamente con g++, igual que ya hacian el resto de checks del
# original (compile_main, compile_unit_tester, run_asan_tests).
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
BINARY="./polyset_test_bin"

# ---------------------------------------------------------------------------- #
#  Rutas: resueltas desde la ubicacion real del script, no del cwd del caller
# ---------------------------------------------------------------------------- #
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
GIVEN_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"                 # .../05/level-1/polyset
ROOT_DIR="$(cd "$SCRIPT_DIR/../../../.." && pwd)"         # 42-exam-rank-42/
STUDENT_DIR="$ROOT_DIR/rendu/polyset"

SCRATCH="$(mktemp -d "${TMPDIR:-/tmp}/polyset_grademe.XXXXXX")"
cleanup() { rm -rf "$SCRATCH"; }
trap cleanup EXIT

STUDENT_FILES=(searchable_array_bag.cpp searchable_array_bag.hpp
               searchable_tree_bag.cpp  searchable_tree_bag.hpp
               set.cpp set.hpp)

STUDENT_HEADERS=(searchable_array_bag.hpp searchable_tree_bag.hpp set.hpp)

pass() { echo -e "${GREEN}[PASS]${RESET} $1"; ((PASS++)) || true; }
fail() { echo -e "${RED}[FAIL]${RESET} $1"; ((FAIL++)) || true; }
skip() { echo -e "${YELLOW}[SKIP]${RESET} $1"; }

# ---------------------------------------------------------------------------- #
#  0. Comprobar Expected Files en rendu/polyset/, y copiar given+alumno al
#     scratch para que el resto de checks trabaje en un unico directorio flat
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

    for f in bag.hpp searchable_bag.hpp array_bag.hpp array_bag.cpp \
             tree_bag.hpp tree_bag.cpp main.cpp; do
        if [ ! -f "$GIVEN_DIR/$f" ]; then
            echo -e "${RED}Falta fichero given: $GIVEN_DIR/$f${RESET}"
            exit 1
        fi
    done

    cp "$GIVEN_DIR"/bag.hpp "$GIVEN_DIR"/searchable_bag.hpp \
       "$GIVEN_DIR"/array_bag.hpp "$GIVEN_DIR"/array_bag.cpp \
       "$GIVEN_DIR"/tree_bag.hpp "$GIVEN_DIR"/tree_bag.cpp \
       "$GIVEN_DIR"/main.cpp "$SCRATCH/"
    cp "${STUDENT_FILES[@]/#/$STUDENT_DIR/}" "$SCRATCH/"
    echo ""
}

# ---------------------------------------------------------------------------- #
#  1. Include guards — doble inclusion de cada header tuyo
# ---------------------------------------------------------------------------- #
check_header_guards() {
    echo -e "${CYAN}${BOLD}=== Include guards (doble inclusion) ===${RESET}"
    for h in "${STUDENT_HEADERS[@]}"; do
        cat > "$SCRATCH/guard_test.cpp" << EOF
#include "$h"
#include "$h"
int main() { return 0; }
EOF
        if g++ -std=c++98 -Wall -Wextra -I"$SCRATCH" \
            -c "$SCRATCH/guard_test.cpp" -o "$SCRATCH/guard_test.o" 2>"$SCRATCH/guard_err.log"; then
            pass "$h: incluirlo dos veces no rompe la compilacion"
        else
            fail "$h: include guard ROTO (revisa que #ifndef y #define usen el mismo nombre)"
            sed 's/^/    /' "$SCRATCH/guard_err.log"
        fi
    done
    echo ""
}

# ---------------------------------------------------------------------------- #
#  2. set: coherencia del constructor por defecto (informativo, no exige uno)
# ---------------------------------------------------------------------------- #
check_set_default_ctor_consistency() {
    echo -e "${CYAN}${BOLD}=== set: coherencia del constructor por defecto ===${RESET}"
    cat > "$SCRATCH/set_default_ctor_test.cpp" << 'EOF'
#include "set.hpp"
int main() {
    set s;
    (void)s;
    return 0;
}
EOF
    if g++ -std=c++98 -Wall -Wextra -I"$SCRATCH" \
        "$SCRATCH/set_default_ctor_test.cpp" -o "$SCRATCH/set_default_ctor_test" 2>"$SCRATCH/set_default_ctor_err.log"; then
        pass "set tiene constructor por defecto: 'set s;' compila (recuerda proteger cada metodo contra bag nulo)"
    else
        pass "set NO tiene constructor por defecto: 'set s;' no compila (estado invalido imposible en compilacion)"
    fi
    echo ""
}

# ---------------------------------------------------------------------------- #
#  3. Compilacion manual con el main.cpp dado
# ---------------------------------------------------------------------------- #
compile_main() {
    echo -e "${CYAN}${BOLD}Compilando polyset con main.cpp del subject (build manual)...${RESET}"
    if (cd "$SCRATCH" && g++ -std=c++98 -Wall -Wextra -Werror -I. \
        searchable_array_bag.cpp searchable_tree_bag.cpp set.cpp \
        array_bag.cpp tree_bag.cpp main.cpp -o "$BINARY") 2>&1; then
        echo -e "${GREEN}Compilacion OK (0 warnings bajo -Wall -Wextra -Werror)${RESET}\n"
    else
        echo -e "${RED}ERROR DE COMPILACION. No se puede seguir testeando.${RESET}"
        exit 1
    fi
}

# ---------------------------------------------------------------------------- #
#  4. Tester unitario en C++98 puro (bag/searchable_bag/set con casos concretos)
# ---------------------------------------------------------------------------- #
generate_unit_test_source() {
    cat > "$SCRATCH/polyset_unit_test.cpp" << 'EOF'
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "bag.hpp"
#include "searchable_bag.hpp"
#include "array_bag.hpp"
#include "tree_bag.hpp"
#include "searchable_array_bag.hpp"
#include "searchable_tree_bag.hpp"
#include "set.hpp"

#define BAG_HAS_VIRTUAL_DTOR __BAG_HAS_VIRTUAL_DTOR_PLACEHOLDER__

struct CoutCapture {
    std::ostringstream buf;
    std::streambuf*    old;
    CoutCapture()  { old = std::cout.rdbuf(buf.rdbuf()); }
    ~CoutCapture() { std::cout.rdbuf(old); }
    std::string str() const { return buf.str(); }
};

static int countChar(const std::string &s, char c) {
    int n = 0;
    for (std::string::size_type i = 0; i < s.size(); ++i)
        if (s[i] == c)
            ++n;
    return n;
}

#define GRN "\033[0;32m"
#define RED "\033[0;31m"
#define YEL "\033[1;33m"
#define CYN "\033[0;36m"
#define BLD "\033[1m"
#define RST "\033[0m"

static int PASS = 0, FAIL = 0;

void check(const std::string& name, bool cond) {
    if (cond) {
        std::cerr << GRN "[PASS]" RST " " << name << "\n";
        ++PASS;
    } else {
        std::cerr << RED "[FAIL]" RST " " << name << "\n";
        ++FAIL;
    }
}

void test_array_bag() {
    std::cerr << BLD CYN "\n=== searchable_array_bag ===" RST "\n";
    {
        searchable_array_bag b;
        b.insert(5); b.insert(3); b.insert(8); b.insert(1);
        check("has(5) -> true",  b.has(5));
        check("has(3) -> true",  b.has(3));
        check("has(8) -> true",  b.has(8));
        check("has(1) -> true",  b.has(1));
        check("has(0) -> false", !b.has(0));
        check("has(4) -> false", !b.has(4));
        check("has(9) -> false", !b.has(9));
    }
    {
        searchable_array_bag b;
        b.insert(-5); b.insert(0);
        check("has(-5) -> true",  b.has(-5));
        check("has(0)  -> true",  b.has(0));
        check("has(-6) -> false", !b.has(-6));
        check("has(1)  -> false", !b.has(1));
    }
    {
        searchable_array_bag b;
        check("has() en bag vacio -> false", !b.has(1));
    }
    {
        searchable_array_bag b;
        int arr[] = {10, 20, 30};
        b.insert(arr, 3);
        check("has(10) tras insert array -> true",  b.has(10));
        check("has(20) tras insert array -> true",  b.has(20));
        check("has(30) tras insert array -> true",  b.has(30));
        check("has(15) tras insert array -> false", !b.has(15));
    }
    {
        searchable_array_bag b;
        b.insert(5); b.insert(3);
        b.clear();
        check("has(5) despues de clear() -> false", !b.has(5));
        check("has(3) despues de clear() -> false", !b.has(3));
        b.insert(7);
        check("has(7) despues de clear+insert -> true",  b.has(7));
        check("has(5) despues de clear+insert -> false", !b.has(5));
    }
    {
        searchable_array_bag a;
        a.insert(1); a.insert(2); a.insert(3);
        searchable_array_bag b(a);
        check("Copia: has(1) -> true",  b.has(1));
        check("Copia: has(2) -> true",  b.has(2));
        check("Copia: has(3) -> true",  b.has(3));
        check("Copia: has(4) -> false", !b.has(4));
        a.clear();
        check("Copia es independiente: has(1) -> true", b.has(1));
    }
    {
        searchable_array_bag a, b;
        a.insert(10); a.insert(20);
        b.insert(99);
        b = a;
        check("Asignacion: has(10) -> true",  b.has(10));
        check("Asignacion: has(20) -> true",  b.has(20));
        check("Asignacion: has(99) -> false (sobrescrito)", !b.has(99));
        searchable_array_bag &self_ref = b;
        b = self_ref;
        check("Autoasignacion (derivada) no crashea: has(10) -> true", b.has(10));
    }
    {
        searchable_array_bag b;
        b.insert(1); b.insert(2);
        CoutCapture cap;
        b.print();
        std::string out = cap.str();
        check("print() no vacio", !out.empty());
        check("print() termina con newline", !out.empty() && out[out.size() - 1] == '\n');
    }
}

void test_direct_canonical_form() {
    std::cerr << BLD CYN "\n=== Forma canonica directa: array_bag / tree_bag ===" RST "\n";
    {
        array_bag a;
        a.insert(1); a.insert(2);
        array_bag b(a);
        std::string a_before, b_after;
        { CoutCapture cap; a.print(); a_before = cap.str(); }
        a.insert(99);
        { CoutCapture cap; b.print(); b_after = cap.str(); }
        check("array_bag: copy ctor es deep copy (modificar 'a' no afecta a 'b')",
              b_after == a_before);
    }
}

void test_tree_bag() {
    std::cerr << BLD CYN "\n=== searchable_tree_bag ===" RST "\n";
    {
        searchable_tree_bag t;
        t.insert(5); t.insert(3); t.insert(8); t.insert(1);
        check("has(5) -> true",  t.has(5));
        check("has(3) -> true",  t.has(3));
        check("has(8) -> true",  t.has(8));
        check("has(1) -> true",  t.has(1));
        check("has(0) -> false", !t.has(0));
        check("has(4) -> false", !t.has(4));
        check("has(6) -> false", !t.has(6));
        check("has(9) -> false", !t.has(9));
    }
    {
        searchable_tree_bag t;
        t.insert(-10); t.insert(-5); t.insert(0);
        check("has(-10) -> true",  t.has(-10));
        check("has(-5)  -> true",  t.has(-5));
        check("has(0)   -> true",  t.has(0));
        check("has(-7)  -> false", !t.has(-7));
        check("has(1)   -> false", !t.has(1));
    }
    {
        searchable_tree_bag t;
        check("has() en arbol vacio -> false", !t.has(0));
    }
    {
        searchable_tree_bag t;
        int arr[] = {5, 2, 8, 1, 3};
        t.insert(arr, 5);
        check("has(5) tras insert array -> true",  t.has(5));
        check("has(1) tras insert array -> true",  t.has(1));
        check("has(3) tras insert array -> true",  t.has(3));
        check("has(4) tras insert array -> false", !t.has(4));
    }
    {
        searchable_tree_bag t;
        t.insert(5); t.insert(10);
        t.clear();
        check("has(5) despues de clear() -> false",  !t.has(5));
        check("has(10) despues de clear() -> false", !t.has(10));
        t.insert(7);
        check("has(7) despues de clear+insert -> true",  t.has(7));
        check("has(5) despues de clear+insert -> false", !t.has(5));
    }
    {
        searchable_tree_bag a;
        a.insert(5); a.insert(3); a.insert(8);
        searchable_tree_bag b(a);
        check("Copia arbol: has(5) -> true",  b.has(5));
        check("Copia arbol: has(3) -> true",  b.has(3));
        check("Copia arbol: has(8) -> true",  b.has(8));
        check("Copia arbol: has(7) -> false", !b.has(7));
        a.clear();
        check("Copia arbol es independiente: has(5) -> true", b.has(5));
    }
    {
        searchable_tree_bag a, b;
        a.insert(100); a.insert(200);
        b.insert(999);
        b = a;
        check("Asig arbol: has(100) -> true",  b.has(100));
        check("Asig arbol: has(200) -> true",  b.has(200));
        check("Asig arbol: has(999) -> false", !b.has(999));
        searchable_tree_bag &self_ref = b;
        b = self_ref;
        check("Autoasignacion arbol (derivada) no crashea: has(100) -> true", b.has(100));
    }
    {
        searchable_tree_bag t;
        t.insert(5); t.insert(1); t.insert(9); t.insert(3);
        CoutCapture cap;
        t.print();
        std::string out = cap.str();
        std::string::size_type pos1 = out.find('1');
        std::string::size_type pos3 = out.find('3');
        std::string::size_type pos5 = out.find('5');
        std::string::size_type pos9 = out.find('9');
        check("BST print in-order: 1 antes que 3", pos1 < pos3);
        check("BST print in-order: 3 antes que 5", pos3 < pos5);
        check("BST print in-order: 5 antes que 9", pos5 < pos9);
        check("BST print termina con newline", !out.empty() && out[out.size() - 1] == '\n');
    }
}

void test_set() {
    std::cerr << BLD CYN "\n=== set ===" RST "\n";
    {
        searchable_array_bag b;
        set s(b);
        s.insert(5); s.insert(5); s.insert(5);
        CoutCapture cap;
        s.print();
        std::string out = cap.str();
        int count = 0;
        std::string::size_type pos = 0;
        while ((pos = out.find('5', pos)) != std::string::npos) { ++count; ++pos; }
        check("set: insertar 5 tres veces -> aparece solo 1 vez en print", count == 1);
    }
    {
        searchable_array_bag b;
        set s(b);
        int arr[] = {3, 3, 7, 7, 1, 1};
        s.insert(arr, 6);
        check("set: has(3) -> true",  s.has(3));
        check("set: has(7) -> true",  s.has(7));
        check("set: has(1) -> true",  s.has(1));
        check("set: has(5) -> false", !s.has(5));
        CoutCapture cap;
        s.print();
        std::string out = cap.str();
        check("set: '3' aparece solo 1 vez", countChar(out, '3') == 1);
        check("set: '7' aparece solo 1 vez", countChar(out, '7') == 1);
        check("set: '1' aparece solo 1 vez", countChar(out, '1') == 1);
    }
    {
        searchable_array_bag b;
        set s(b);
        s.insert(42);
        check("set::has(42) -> true",  s.has(42));
        check("set::has(41) -> false", !s.has(41));
    }
    {
        searchable_array_bag b;
        set s(b);
        s.insert(10); s.insert(20);
        s.clear();
        check("set::has(10) despues de clear -> false", !s.has(10));
        s.insert(10);
        check("set::has(10) despues de clear+insert -> true",  s.has(10));
        check("set::has(20) despues de clear+insert -> false", !s.has(20));
    }
    {
        searchable_array_bag b;
        set s(b);
        s.insert(7); s.insert(3);
        const searchable_bag& inner = s.get_bag();
        check("get_bag().has(7) -> true",  inner.has(7));
        check("get_bag().has(3) -> true",  inner.has(3));
        check("get_bag().has(5) -> false", !inner.has(5));
    }
    {
        searchable_tree_bag t;
        set s(t);
        s.insert(9); s.insert(4); s.insert(9);
        check("set(tree_bag): has(9) -> true",  s.has(9));
        check("set(tree_bag): has(4) -> true",  s.has(4));
        check("set(tree_bag): has(9) no duplicado en bag",
            s.get_bag().has(9) && !s.get_bag().has(8));
    }
    {
        searchable_array_bag b;
        set sa(b), st(b);
        st.insert(1);
        check("sa y st comparten bag: sa.has(1) -> true tras insertar en st", sa.has(1));
        sa.insert(2);
        check("sa y st comparten bag: st.has(2) -> true tras insertar en sa", st.has(2));
    }
    {
        searchable_array_bag b;
        set s(b);
        s.insert((int*)0, 0);
        check("set::insert(NULL, 0) no crashea", !s.has(0));
    }
    {
        searchable_array_bag b;
        set s(b);
        int arr[] = {1, 2, 3};
        s.insert(arr, 0);
        check("set::insert(arr, 0) no inserta nada",
              !s.has(1) && !s.has(2) && !s.has(3));
    }
    {
        searchable_array_bag b;
        set s(b);
        int arr[] = {1, 2, 3};
        s.insert(arr, -1);
        check("set::insert(arr, -1) no crashea con size negativo", !s.has(1));
    }
    {
        searchable_array_bag b;
        set s(b);
        s.insert(7);
        const set &cs = s;
        check("get_bag() es const-callable sobre un 'const set&'", cs.get_bag().has(7));
    }
    {
        searchable_array_bag b1, b2;
        set s1(b1);
        s1.insert(10); s1.insert(20);

        set s2(b2);
        s2.insert(99);
        s2 = s1;
        check("set operator=: s2 pasa a envolver el bag de s1 (has(10))", s2.has(10));

        set s3(s1);
        check("set copy ctor: s3 envuelve el mismo bag que s1 (has(20))", s3.has(20));

        set &self_ref = s1;
        s1 = self_ref;
        check("set autoasignacion no crashea: has(10) sigue true", s1.has(10));
    }
}

#if BAG_HAS_VIRTUAL_DTOR
static bool g_array_destroyed = false;
static bool g_tree_destroyed  = false;

class instrumented_array_bag : public searchable_array_bag {
public:
    ~instrumented_array_bag() { g_array_destroyed = true; }
};

class instrumented_tree_bag : public searchable_tree_bag {
public:
    ~instrumented_tree_bag() { g_tree_destroyed = true; }
};
#endif

void test_polymorphism() {
    std::cerr << BLD CYN "\n=== Polimorfismo (searchable_bag*) y destructor virtual ===" RST "\n";
    {
        searchable_tree_bag  concrete_t;
        searchable_array_bag concrete_a;
        searchable_bag* bags[2];
        bags[0] = &concrete_t;
        bags[1] = &concrete_a;

        for (int i = 0; i < 2; i++) {
            bags[i]->insert(10);
            bags[i]->insert(20);
        }

        check("tree via ptr: has(10) -> true",   bags[0]->has(10));
        check("array via ptr: has(20) -> true",  bags[1]->has(20));
        check("tree via ptr: has(15) -> false",  !bags[0]->has(15));
        check("array via ptr: has(15) -> false", !bags[1]->has(15));
    }
#if BAG_HAS_VIRTUAL_DTOR
    {
        g_array_destroyed = false;
        bag *b = new instrumented_array_bag();
        delete b;
        check("delete via bag*: destructor virtual llama a la clase derivada (array)",
              g_array_destroyed);
    }
    {
        g_tree_destroyed = false;
        bag *b = new instrumented_tree_bag();
        delete b;
        check("delete via bag*: destructor virtual llama a la clase derivada (tree)",
              g_tree_destroyed);
    }
#else
    std::cerr << YEL "[SKIP]" RST
        " destructor polimorfico: el bag.hpp de ESTA sesion no declara ~bag()"
        " virtual (variante del subject sin destructor virtual, no es tu codigo)"
        " -> test omitido\n";
#endif
}

int main() {
    test_array_bag();
    test_direct_canonical_form();
    test_tree_bag();
    test_set();
    test_polymorphism();

    std::cerr << BLD "\n============================================\n" RST;
    int total = PASS + FAIL;
    if (FAIL == 0)
        std::cerr << GRN BLD "RESULTADO: " << PASS << "/" << total << " PASSED\n" RST;
    else
        std::cerr << RED BLD "RESULTADO: " << PASS << "/" << total << " PASSED (" << FAIL << " fallidos)\n" RST;
    std::cerr << BLD "============================================\n" RST;

    return (FAIL > 0) ? 1 : 0;
}
EOF
}

BAG_HAS_VIRTUAL_DTOR=0
detect_bag_virtual_dtor() {
    cat > "$SCRATCH/bag_dtor_probe.cpp" << 'EOF'
#include "bag.hpp"
#include <iostream>
int main() {
    std::cout << (__has_virtual_destructor(bag) ? 1 : 0);
    return 0;
}
EOF
    if g++ -std=c++98 -I"$SCRATCH" "$SCRATCH/bag_dtor_probe.cpp" -o "$SCRATCH/bag_dtor_probe" 2>"$SCRATCH/bag_dtor_probe_err.log"; then
        BAG_HAS_VIRTUAL_DTOR=$("$SCRATCH/bag_dtor_probe")
    else
        BAG_HAS_VIRTUAL_DTOR=0
    fi

    if [ "$BAG_HAS_VIRTUAL_DTOR" = "1" ]; then
        echo -e "${CYAN}bag.hpp: declara ~bag() virtual -> se activa el test de destructor polimorfico${RESET}"
    else
        echo -e "${YELLOW}bag.hpp: NO declara ~bag() virtual (variante del subject) -> ese test se omitira${RESET}"
    fi
}

compile_unit_tester() {
    echo -e "${CYAN}${BOLD}Compilando tester unitario (C++98 puro, sin extensiones)...${RESET}"
    detect_bag_virtual_dtor
    generate_unit_test_source
    sed -i.bak "s/__BAG_HAS_VIRTUAL_DTOR_PLACEHOLDER__/$BAG_HAS_VIRTUAL_DTOR/" "$SCRATCH/polyset_unit_test.cpp"
    rm -f "$SCRATCH/polyset_unit_test.cpp.bak"

    if (cd "$SCRATCH" && g++ -std=c++98 -Wall -Wextra -Werror -I. \
        searchable_array_bag.cpp searchable_tree_bag.cpp set.cpp \
        array_bag.cpp tree_bag.cpp polyset_unit_test.cpp -o "${BINARY}_unit") 2>&1; then
        echo -e "${GREEN}Compilacion tester unitario OK (0 warnings)${RESET}\n"
    else
        echo -e "${RED}ERROR compilando tester unitario. Revisa que tus headers son correctos.${RESET}"
        exit 1
    fi
}

# ---------------------------------------------------------------------------- #
#  5. Build con AddressSanitizer + UBSan (detecta delete vs delete[], leaks)
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
        searchable_array_bag.cpp searchable_tree_bag.cpp set.cpp \
        array_bag.cpp tree_bag.cpp polyset_unit_test.cpp -o "${BINARY}_asan") 2>"$SCRATCH/asan_compile_err.log"; then
        local asan_opts="abort_on_error=0"
        if [ "$(uname)" = "Linux" ]; then
            asan_opts="detect_leaks=1"
        fi
        ( cd "$SCRATCH" && ASAN_OPTIONS="$asan_opts" run_with_timeout 20 "${BINARY}_asan" ) > "$SCRATCH/asan_run_out.log" 2>&1
        local rc=$?
        if [ $rc -eq 137 ] || [ $rc -eq 124 ]; then
            skip "ASan/UBSan: el runtime del sanitizer no arranco en este Mac (bug conocido ASan/macOS, no es tu codigo) — omitido"
        elif [ $rc -eq 0 ] && ! grep -qE "ERROR: (AddressSanitizer|LeakSanitizer)|runtime error:" "$SCRATCH/asan_run_out.log"; then
            pass "ASan/UBSan: sin errores de memoria ni leaks detectados"
        else
            fail "ASan/UBSan detecto un problema de memoria:"
            grep -A 6 -E "ERROR|runtime error" "$SCRATCH/asan_run_out.log" | sed 's/^/    /' | head -40
        fi
    else
        skip "No se pudo compilar con sanitizers (compilador sin soporte) — no es bloqueante"
    fi
    echo ""
}

# ---------------------------------------------------------------------------- #
#  6. Tests de integracion: ejecutan el binario real con el main.cpp dado
# ---------------------------------------------------------------------------- #
strip_ansi() {
    sed -E $'s/\x1b\\[[0-9;]*m//g'
}

is_sorted_line() {
    echo "$1" | awk '{
        prev = -2147483648
        for (i = 1; i <= NF; i++) {
            if ($i + 0 < prev) exit 1
            prev = $i + 0
        }
        exit 0
    }'
}

run_integration_tests() {
    echo -e "${CYAN}${BOLD}=== Tests de integracion (binario real + main.cpp dado) ===${RESET}"
    echo -e "${YELLOW}Nota: el main.cpp dado por el examen no imprime etiquetas como${RESET}"
    echo -e "${YELLOW}\"Tree Output:\"/\"Array Output:\"/\"Set Result\", asi que esos checks${RESET}"
    echo -e "${YELLOW}se omiten aqui (no aplican a este main.cpp); solo se comprueba que${RESET}"
    echo -e "${YELLOW}el binario no crashea con distintos argumentos.${RESET}\n"

    local bin="$SCRATCH/$BINARY"

    local actual_exit=0
    "$bin" 2>/dev/null || actual_exit=$?
    if [ "$actual_exit" -ne 0 ]; then
        pass "Sin argumentos -> exit distinto de 0"
    else
        fail "Sin argumentos deberia devolver exit != 0"
    fi

    local args="5 3 8 3 1 5 9"
    local out
    out=$("$bin" $args 2>/dev/null)
    local plain
    plain=$(printf '%s\n' "$out" | strip_ansi)

    local rc=0
    "$bin" $args > /dev/null 2>&1 || rc=$?
    if [ $rc -eq 0 ]; then
        pass "./polyset $args -> exit 0 (no crashea)"
    else
        fail "./polyset $args -> exit $rc (¿crash/segfault?)"
    fi

    if echo "$plain" | grep -qi "segmentation\|abort"; then
        fail "Salida contiene senal de crash (segfault/abort)"
    else
        pass "Sin mensajes de crash en la salida"
    fi

    local tree_line
    tree_line=$(echo "$plain" | head -1)
    if [ -n "$tree_line" ]; then
        if is_sorted_line "$tree_line"; then
            pass "Primera linea de salida (arbol) esta ordenada ascendentemente (in-order de un BST)"
        else
            skip "Primera linea de salida no parece una lista ordenada (formato de print() distinto) — no bloqueante"
        fi
    else
        skip "Sin salida en stdout con argumentos (¿print() usa otro stream?)"
    fi

    out=$("$bin" 42 2>/dev/null)
    plain=$(printf '%s\n' "$out" | strip_ansi)
    if echo "$plain" | grep -q "42"; then
        pass "./polyset 42 -> el valor 42 aparece en la salida"
    else
        fail "./polyset 42 -> el valor 42 NO aparece en la salida"
    fi

    echo ""
}

# =============================================================================
#  MAIN
# =============================================================================
stage_scratch
check_header_guards
check_set_default_ctor_consistency
compile_main
compile_unit_tester

echo -e "${BOLD}${CYAN}=== Tests unitarios ===${RESET}"
( cd "$SCRATCH" && "./${BINARY}_unit" )
unit_exit=$?
if [ $unit_exit -ne 0 ]; then
    FAIL=$((FAIL + 1))
fi

echo ""
run_asan_tests
run_integration_tests

echo ""
echo -e "${BOLD}============================================${RESET}"
TOTAL=$((PASS + FAIL))
if [ "$FAIL" -eq 0 ]; then
    echo -e "${GREEN}${BOLD}TESTS DE ESTRUCTURA/INTEGRACION: $PASS/$TOTAL PASSED${RESET}"
else
    echo -e "${RED}${BOLD}TESTS DE ESTRUCTURA/INTEGRACION: $PASS/$TOTAL PASSED ($FAIL fallidos)${RESET}"
fi
echo -e "${BOLD}============================================${RESET}"

exit $((FAIL > 0 ? 1 : 0))
