opt="$(echo $3 | sed -e "s/-O0/$(cat /etc/gcc.opt)/g") -fno-inline"
if ! gcc -S -finline-limit=2 -masm=intel -I./src/main -I./src/signals -I./src/option_parser -I./src/lexer -I./src/typer -I./src/rules -I./src/parser -I./src/expansion -I./src/execute -I./src/util -Wall -Wextra -std=c99 -pedantic -masm=intel "$1" -o "$2" $opt ; then
	echo "Error make asm 42sh"
	exit 1
fi

