all:
	#g++  -pipe -O2 -std=c++17 -ggdb -Werror -pedantic -pedantic-errors -Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wduplicated-branches -Wduplicated-cond -Wextra-semi -Wfloat-equal -Wlogical-op -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wsign-conversion -Wsign-promo -Wsign-compare -Wfatal-errors main.cpp
	# g++  -pipe -O2 -std=c++17 -ggdb -Werror main.cpp
	# g++  -pipe -std=c++17 -ggdb -Werror main.cpp

	# для проверки выхода за границы массива, правильность работы итераторовЖй
	# g++ main.cpp -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_LIBCPP_DEBUG=2 -g
	
	# санитайзеры для проверки работы с памятью
	# g++ some.cpp -fsanitize=address -g -O0 -fno-omit-frame-pointer -fno-optimize-sibling-calls
	# g++ main.cpp -std=c++17 -fsanitize=address -g -O0 -fno-omit-frame-pointer -fno-optimize-sibling-calls
	#
	#----
	# g++ main.cpp -std=c++17 -fsanitize=address -g -O0 -fno-omit-frame-pointer -fno-optimize-sibling-calls -Werror -pedantic -pedantic-errors -Wall -Wextra
	#----

	# используеться последний компилятор и библиотеки от интел для многозадачности
	
	 # g++ main.cpp -std=c++17 -fsanitize=address -g -O0 -fno-omit-frame-pointer -fno-optimize-sibling-calls -Werror -pedantic -pedantic-errors -Wall -Wextra -ltbb -lpthread
	 
	# valgrind --leak-check=full ./a.out
	#
	# для работы с параллейными алгоритмами
	g++ -O2 -std=c++2a -Wall -Wextra -pedantic -DPARALLEL ./*.cpp -ltbb 

