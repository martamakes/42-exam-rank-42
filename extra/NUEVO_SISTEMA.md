# 🎯 Sistema de Práctica Realista - Como el Examen Real

## ✨ **¡Nuevo Sistema Mejorado!**

Ahora el sistema funciona **exactamente como el examen real de 42 School**:

- ✅ **Directorio `rendu/`** como en el examen
- ✅ **TÚ creas los archivos** desde cero 
- ✅ **Tests automáticos** integrados
- ✅ **Enunciado siempre visible** sin cambiar de menú
- ✅ **Flujo realista** de trabajo

---

## 🚀 **Flujo de Trabajo Mejorado**

### 1. **Selecciona Ejercicio** 
```
📜 ENUNCIADO:
print_bits

Assignment: print_bits
Files needed: print_bits.c
Allowed functions: write
--------------------------------------------------------------------------------

DESCRIPCIÓN:
Write a function that takes a byte, and prints it in binary WITHOUT A NEWLINE
AT THE END.

📋 EJEMPLOS:
Example, if you pass 2 to print_bits, it will print "00000010"

📁 ESTADO DEL EJERCICIO:
⚠️  Directorio no creado: rendu/print_bits/
💡 Necesitas crear: mkdir -p rendu/print_bits

¿Qué quieres hacer?
  c) Crear directorio y compilar/probar
  t) Solo compilar y probar (si ya tienes archivos)
  s) Marcar como completado
  e) Ver explicación del patrón
  r) Abrir directorio en terminal
  b) Volver al menú del patrón
```

### 2. **Crear Directorio (opción 'c')**
- ✅ Se crea `rendu/print_bits/`
- ✅ Se abre nueva terminal automáticamente 
- ✅ El enunciado sigue visible

### 3. **Programar la Solución**
En la nueva terminal:
```bash
cd rendu/print_bits/
vim print_bits.c    # O tu editor favorito
# Escribes tu solución desde cero
```

### 4. **Probar (opción 't')**
- ✅ Tests automáticos ejecutados
- ✅ Resultados detallados mostrados
- ✅ Como en el sistema real de 42

---

## 🎯 **Ventajas del Nuevo Sistema**

### **Realismo Total**
- **Directorio rendu/**: Exactamente como en examen
- **Crear archivos desde cero**: Como en la realidad  
- **Tests automáticos**: Mismos que usa 42 School
- **Presión realista**: Simula condiciones de examen

### **Sin Distracciones**
- **Enunciado siempre visible**: No cambias de menú
- **Estado del ejercicio**: Sabes qué archivos necesitas
- **Ubicación clara**: Path exacto mostrado siempre

### **Múltiples Terminales**
- **Terminal 1**: Sistema de práctica con enunciado
- **Terminal 2**: Tu editor de código
- **Flujo natural**: Como trabajarías realmente

---

## 📁 **Estructura Nueva**

```
extra/
├── rendu/                    ← TU DIRECTORIO DE TRABAJO
│   ├── print_bits/          ← Aquí creas print_bits.c
│   ├── ft_atoi/             ← Aquí creas ft_atoi.c  
│   └── [ejercicio]/         ← Etc.
├── scripts/
│   └── pattern_practice.sh  ← Sistema mejorado
└── progress/                ← Tracking como siempre
```

---

## 🎓 **Comandos del Sistema**

### **En el Menú de Ejercicio:**
- **`c`** - Crear directorio y preparar entorno
- **`t`** - Probar tu código (tests automáticos)
- **`s`** - Marcar como completado
- **`e`** - Ver explicación del patrón
- **`r`** - Abrir terminal en directorio ejercicio
- **`b`** - Volver al menú del patrón

### **Flujo Típico:**
1. `c` → Crear directorio
2. Codificar en nueva terminal
3. `t` → Probar con tests automáticos
4. Repetir 2-3 hasta que pasen todos los tests
5. `s` → Marcar completado

---

## 🧪 **Tests Automáticos Integrados**

Cuando presionas **'t'**:

```
🧪 Tests automáticos disponibles - ejecutando batería completa...

========== RESULTADOS DE TESTS ==========

=== Testing print_bits ===

Test Subject example: [OK]
Test Zero: [OK] 
Test All ones: [OK]
Test One: [OK]
Test Powers of 2: [OK]
Test Alternating: [OK]
Test Random byte: [OK]

=== Resumen de tests ===
✅ Todos los tests pasaron correctamente ✓

✅ ¡Todos los tests pasaron correctamente!
=========================================
```

---

## 💡 **Consejos para Usar el Sistema** 

### **Preparación:**
1. Abre **dos terminales**
2. Terminal 1: `make start` (sistema de práctica)  
3. Terminal 2: Para programar

### **Durante Práctica:**
1. **Lee enunciado completo** antes de programar
2. **Entiende qué archivos necesitas** crear
3. **Programa sin mirar** soluciones existentes
4. **Prueba frecuentemente** con 't'
5. **No marques completado** hasta que pasen todos los tests

### **Simulación de Examen:**
- **Tiempo límite**: 45 min por ejercicio
- **Sin ayuda externa**: Solo el enunciado
- **Tests automáticos**: Como criterio único
- **Presión realista**: Te preparas para lo real

---

## 🚀 **Para Empezar:**

```bash
cd extra/
make start
# Selecciona patrón → ejercicio → 'c' para empezar
```

¡Ahora tienes un sistema que te prepara **realmente** para el examen de 42! 🎯
