Assignment name: argo
Expected files: argo.c
Allowed functions: getc, ungetc, printf, malloc, calloc, realloc, free, isdigit, fscanf, write
-----------------
Write a function argo that will parse a json file in the structure declared in argo.h:

int	argo(json *dst, FILE *stream);

	dst	- is the pointer to the AST that you will create
	stream	- is the file to parse (man FILE)

Your function will return 1 for success and -1 for failure.
If an unexpected token is found you will print the following message in stdout:
"Unexpected token '%c'\n"
or if the token is EOF:
"Unexpected end of input\n"

Only handle numbers, strings and maps.
Numbers will only be basic ints like in scanf("%d")
Handle escaping in the strings only for backslashes and quotation marks (no \n \u ...)
Don't handle spaces -> consider them as invalid tokens.

In case of doubt how to parse json, read rfc8259. But you won't need it as the format is simple. Tested with the main, the output should be exactly the same as the input (except for errors).
There are some functions in argo.c that might help you.

Examples that should work:

$> echo -n '1' | ./argo /dev/stdin | cat -e
1$
$> echo -n '"bonjour"' | ./argo /dev/stdin | cat -e
"bonjour"$
$> echo -n '"escape! \" "' | ./argo /dev/stdin | cat -e
"escape! \" "$
$> echo -n '{"tomatoes":42,"potatoes":234}' | ./argo /dev/stdin | cat -e
{"tomatoes":42,"potatoes":234}$
$> echo -n '{"recursion":{"recursion":{"recursion":{"recursion":"recursion"}}}}' | ./argo /dev/stdin | cat -e
{"recursion":{"recursion":{"recursion":{"recursion":"recursion"}}}}$
$> echo -n '"unfinished string' | ./argo /dev/stdin | cat -e
unexpected end of input$
$> echo -n '"unfinished string 2\"' | ./argo /dev/stdin | cat -e
unexpected end of input$
$> echo -n '{"no value?":}' | ./argo /dev/stdin | cat -e
unexpected token '}'$