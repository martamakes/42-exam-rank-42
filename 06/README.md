# Exam Rank 06 - mini_serv Tester

Este tester te ayudará a prepararte para el Exam Rank 06 de la Escuela 42, que consiste en implementar un servidor simple llamado `mini_serv`.

## Estructura del Tester

- `subject/`: Contiene el enunciado y archivos de referencia
- `examples/`: Contiene código de ejemplo y plantillas
- `rendu/`: Aquí debes colocar tu solución
- `tester/`: Scripts para probar tu implementación

## Cómo Usar el Tester

1. Clona este repositorio
2. Copia tu archivo `mini_serv.c` en el directorio `rendu/`
3. Ejecuta el script principal: `./exam.sh`
4. Sigue las opciones del menú para probar tu implementación

## Menú del Tester

- **Ver Enunciado**: Muestra el enunciado del ejercicio
- **Ver main.c**: Muestra las funciones auxiliares proporcionadas en el examen
- **Ver Código Template**: Muestra una plantilla para ayudarte a empezar
- **Ejecutar Prueba Manual**: Permite probar manualmente el servidor
- **Ejecutar Prueba Automática**: Ejecuta pruebas automáticas
- **Verificar directorio rendu**: Comprueba si tu solución está en el lugar correcto
- **Comprobar fugas de memoria**: Consejos para verificar fugas de memoria y descriptores de archivo

## Descripción del Ejercicio

El objetivo es crear un servidor que:
- Escuche conexiones en un puerto específico en la dirección 127.0.0.1
- Permita a los clientes conectarse y comunicarse entre sí
- Asigne IDs a los clientes y notifique a todos cuando un cliente se conecta o desconecta
- Reciba mensajes de los clientes y los reenvíe a todos los demás clientes

## Pruebas Manuales

Para probar manualmente tu servidor:

1. Ejecuta tu servidor: `./mini_serv <puerto>`
2. Conéctate con netcat: `nc 127.0.0.1 <puerto>`
3. Conéctate con más clientes desde terminales diferentes
4. Envía mensajes desde cada cliente y verifica que se reciban correctamente

## Consejos

- Asegúrate de manejar correctamente los errores
- No olvides liberar toda la memoria asignada
- Asegúrate de cerrar todos los descriptores de archivo al finalizar
- No uses #define en tu solución
- Tu servidor debe ser no bloqueante
- El programa solo debe escuchar en 127.0.0.1

## Notas Adicionales

- Las funciones `extract_message` y `str_join` se proporcionarán durante el examen
- El tester espera que envíes los mensajes lo más rápido posible, sin buffers innecesarios
