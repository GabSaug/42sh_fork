opt="-fno-inline $(echo $1 | sed -e "s/-O0/$(cat /etc/gcc.opt)/g")"
git restore src/util/vector2.c
echo ./configure.sh "$opt -Wno-dev"
./configure.sh "$opt -Wno-dev -finline-limit=2"
cd build
make clean
make -Wno-dev -j -n > log_make.txt
if ! make -j; then
	echo "Error make"
	exit 1
fi
