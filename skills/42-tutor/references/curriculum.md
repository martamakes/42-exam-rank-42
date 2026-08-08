# Curriculum cubierto — 42-tutor

Se lee para ubicar en qué módulo/bloque está el usuario, o para ofrecer opciones cuando no lo
especifica.

### C++ Modules (CPP05–CPP09) — `/Volumes/BIWIN/CPP/`

| Módulo | Conceptos clave |
|--------|----------------|
| CPP05 | Excepciones: try/catch/throw, clases de excepción, stack unwinding |
| CPP06 | Casts: static_cast, dynamic_cast, reinterpret_cast, const_cast, RTTI |
| CPP07 | Templates: function templates, class templates, specialization |
| CPP08 | Contenedores STL: vector, list, deque, stack, queue, set, map; iteradores; algoritmos |
| CPP09 | STL avanzado: algoritmos de búsqueda/ordenación, Ford-Johnson, RPN, Bitcoin exchange |

### Inception — `/Volumes/BIWIN/42 - subjects/5inception/`

| Bloque | Conceptos |
|--------|-----------|
| Docker base | Contenedor vs VM, imagen vs contenedor, Dockerfile, capas |
| Redes Docker | bridge network, DNS interno, puertos |
| Servicios | NGINX + TLS, WordPress + php-fpm, MariaDB |
| Compose | docker-compose.yml, volumes, depends_on, env_file |
| Seguridad | no root, variables de entorno, no secrets en imagen |

### WebServ — `/Volumes/BIWIN/42 - subjects/5awebserv/`

| Bloque | Conceptos |
|--------|-----------|
| HTTP base | request/response, métodos, headers, status codes |
| Sockets | socket(), bind(), listen(), accept(), I/O multiplexing (select/poll/epoll) |
| Config | parsing del fichero de config, server blocks, location blocks |
| CGI | fork/exec, variables de entorno CGI, stdin/stdout |
| C++ server | non-blocking I/O, event loop |

### ft_irc — `/Volumes/BIWIN/42 - subjects/5ft_irc/`

| Bloque | Conceptos |
|--------|-----------|
| Protocolo IRC | mensajes RFC 1459, comandos (NICK, JOIN, PRIVMSG...) |
| Servidor TCP | sockets, multiplexing, parsing de mensajes |
| Arquitectura | clients, channels, operadores |

### Transcendence — `/Volumes/BIWIN/42 - subjects/6Transcendence/`

| Bloque | Conceptos |
|--------|-----------|
| Backend | API REST, autenticación (JWT/OAuth), base de datos |
| Frontend | SPA, routing, WebSockets para el juego |
| Pong | lógica de juego, sincronización cliente-servidor |
| DevOps | Docker, despliegue, variables de entorno |

### Exams

| Exam | Qué practica |
|------|-------------|
| Rank 05 | Piscine-level C (bigint, game of life, polyset, etc.) |
| Rank 06 | mini_serv — servidor TCP en C, I/O multiplexing |
