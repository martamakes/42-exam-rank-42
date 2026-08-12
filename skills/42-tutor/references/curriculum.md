# Curriculum cubierto — 42-tutor

Se lee para ubicar en qué módulo/bloque está el usuario, o para ofrecer opciones cuando no lo
especifica.

> **Nota de fuente**: 42 no publica una tabla oficial única de círculos/ranks — varía por campus
> y cohorte. Círculos 1–4 abajo son los que el propio usuario reportó de su cursus. Círculo 5–6
> se cruzó con [MarkosComK/42-Common-Core-Guide](https://github.com/MarkosComK/42-Common-Core-Guide)
> (repo comunitario, no documentación oficial). Si tu campus difiere, dímelo y lo ajusto.

> **Nota — dos tracks paralelos**: el campus del usuario tiene Common Core en C (CCC) y Common
> Core en Python (CCP) como itinerarios paralelos (reportado por el usuario, 2026-08-12). Varios
> módulos son compartidos entre ambos tracks (Libft, push_swap, Born2beroot, ft_printf
> confirmados; Get Next Line sin confirmar todavía) pero **el número de círculo puede diferir
> entre tracks para el mismo módulo compartido** (ej. push_swap: círculo 2 en CCC, círculo 1 en
> CCP; Born2beroot: círculo 1 en CCC, círculo 2 en CCP) — la tabla de abajo sigue la numeración
> de CCC por defecto; cuando difiera se anota en la fila del módulo.

### Círculo 1 — fundamentos de C

| Módulo | Qué es | Conceptos clave |
|---|---|---|
| Libft *(compartido CCC/CCP)* | Biblioteca estática propia (`libft.a`), reimplementa funciones de la libc y se reutiliza en el resto del cursus | memoria dinámica, punteros, strings, listas enlazadas, Makefile |
| ft_printf *(compartido CCC/CCP — círculo 1 en ambos)* | Reimplementación de `printf` | funciones variádicas (`va_list`/`va_start`/`va_arg`/`va_end`), parsing de especificadores, conversión de bases |
| Born2beroot *(compartido CCC/CCP — círculo 1 en CCC, círculo 2 en CCP)* | Instalación y securización de una VM Linux (Debian o Rocky) | particionado LVM, usuarios/sudo, políticas de contraseña, SSH, firewall, scripts de monitorización, AppArmor/SELinux |

### Círculo 2 — algoritmia, procesos y primer gráfico

| Módulo | Qué es | Conceptos clave |
|---|---|---|
| get_next_line | Lee un fd línea a línea, preservando estado entre llamadas | variables estáticas, buffers, fds, múltiples fds simultáneos (bonus) |
| pipex | Recrea pipes/redirecciones de Unix entre dos comandos | `fork`, `pipe`, `dup2`, `execve`, variables de entorno |
| minitalk | Cliente-servidor por señales Unix, un bit por señal | IPC, señales, PID, operaciones a nivel de bit |
| so_long *(elección 1 de 3: so_long / FdF / fract'ol)* | Videojuego 2D con MiniLibX sobre mapas propios | MiniLibX, parsing/validación de mapas, eventos, sprites |
| FdF *(elección 1 de 3: so_long / FdF / fract'ol)* | Wireframe 3D isométrico a partir de un mapa de alturas | proyección isométrica, algoritmos de trazado de líneas, MiniLibX |
| fract'ol *(elección 1 de 3: so_long / FdF / fract'ol)* | Explorador interactivo de fractales (Mandelbrot/Julia) | números complejos, render por píxel, zoom/pan, MiniLibX |
| push_swap *(compartido CCC/CCP, proyecto en grupo de 2 — círculo 2 en CCC, círculo 1 en CCP; la numeración de círculo difiere por track, ver nota abajo)* | Ordena enteros con 2 pilas y un set restringido de operaciones, minimizando movimientos | estructuras de datos, complejidad algorítmica, optimización |
| Exam Rank 02 | Examen individual cronometrado: lógica en C, strings, memoria, conversión de bases | — |

### Círculo 3 — concurrencia y shell

| Módulo | Qué es | Conceptos clave |
|---|---|---|
| philosophers | Simulación del problema de los filósofos comensales | `pthread`, mutexes, sincronización, prevención de deadlocks/data races |
| minishell | Shell Unix simplificada (estilo Bash), proyecto en grupo | parsing, `fork`/`execve`, pipes, redirecciones, heredoc, señales, builtins |
| Exam Rank 03 | Examen individual: `ft_printf` simplificado o `get_next_line` bajo tiempo | — |

### Círculo 4 — redes, gráficos avanzados y arranque de C++

| Módulo | Qué es | Conceptos clave |
|---|---|---|
| NetPractice | Ejercicios de configuración de red (IPs, subredes, rutas) | IPv4, CIDR, subnetting, gateways, routing |
| cub3d *(o miniRT — alternativas de grupo)* | Motor pseudo-3D estilo Wolfenstein por raycasting | MiniLibX, raycasting, parsing de mapas, render en tiempo real |
| miniRT *(o cub3d)* | Ray tracer básico desde una escena `.rt` | álgebra vectorial, geometría analítica, intersecciones, iluminación |
| CPP00–CPP04 | Mini-piscina C++98: bases de POO | clases, forma canónica, memoria dinámica, sobrecarga de operadores, herencia, polimorfismo |
| Exam Rank 04 | Examen individual: microshell (`cd`, `;`, `\|`) | `fork`, `execve`, `pipe`, `dup2`, `waitpid` |

### Círculo 5 — C++ avanzado, red/Docker, IRC o WebServ

C++ Modules (CPP05–CPP09), mini-piscina avanzada — `/Volumes/BIWIN/CPP/`:

| Módulo | Conceptos clave |
|--------|----------------|
| CPP05 | Excepciones: try/catch/throw, clases de excepción, stack unwinding |
| CPP06 | Casts: static_cast, dynamic_cast, reinterpret_cast, const_cast, RTTI |
| CPP07 | Templates: function templates, class templates, specialization |
| CPP08 | Contenedores STL: vector, list, deque, stack, queue, set, map; iteradores; algoritmos |
| CPP09 | STL avanzado: algoritmos de búsqueda/ordenación, Ford-Johnson, RPN, Bitcoin exchange |

Inception — `/Volumes/BIWIN/42 - subjects/5inception/`:

| Bloque | Conceptos |
|--------|-----------|
| Docker base | Contenedor vs VM, imagen vs contenedor, Dockerfile, capas |
| Redes Docker | bridge network, DNS interno, puertos |
| Servicios | NGINX + TLS, WordPress + php-fpm, MariaDB |
| Compose | docker-compose.yml, volumes, depends_on, env_file |
| Seguridad | no root, variables de entorno, no secrets en imagen |

WebServ *(alternativa de grupo a ft_irc)* — `/Volumes/BIWIN/42 - subjects/5awebserv/`:

| Bloque | Conceptos |
|--------|-----------|
| HTTP base | request/response, métodos, headers, status codes |
| Sockets | socket(), bind(), listen(), accept(), I/O multiplexing (select/poll/epoll) |
| Config | parsing del fichero de config, server blocks, location blocks |
| CGI | fork/exec, variables de entorno CGI, stdin/stdout |
| C++ server | non-blocking I/O, event loop |

ft_irc *(alternativa de grupo a WebServ)* — `/Volumes/BIWIN/42 - subjects/5ft_irc/`:

| Bloque | Conceptos |
|--------|-----------|
| Protocolo IRC | mensajes RFC 1459, comandos (NICK, JOIN, PRIVMSG...) |
| Servidor TCP | sockets, multiplexing, parsing de mensajes |
| Arquitectura | clients, channels, operadores |

Exam Rank 05 | Piscine-level C (bigint, game of life, bsq, polyset, etc.) — es el examen que cubre
este mismo repo (`42-exam-rank-42`).

### Círculo 6 — proyecto final

Transcendence — `/Volumes/BIWIN/42 - subjects/6Transcendence/`:

| Bloque | Conceptos |
|--------|-----------|
| Backend | API REST, autenticación (JWT/OAuth), base de datos |
| Frontend | SPA, routing, WebSockets para el juego |
| Pong | lógica de juego, sincronización cliente-servidor |
| DevOps | Docker, despliegue, variables de entorno |

Exam Rank 06 | mini_serv — servidor TCP en C, I/O multiplexing.
