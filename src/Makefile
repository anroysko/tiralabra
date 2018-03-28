CPPFLAGS = -O2 -std=c++11

main : main.o splay_tree.o splay_node.o
	g++ -o main main.o splay_tree.o splay_node.o $(CPPFLAGS)

main.o : main.cpp splaytree/splay_tree.h
	g++ -c -o main.o main.cpp $(CPPFLAGS)

splay_tree.o : splaytree/splay_tree.cpp splaytree/splay_tree.h splaytree/splay_node.h
	g++ -c -o splay_tree.o splaytree/splay_tree.cpp $(CPPFLAGS)

splay_node.o : splaytree/splay_node.cpp splaytree/splay_node.h
	g++ -c -o splay_node.o splaytree/splay_node.cpp $(CPPFLAGS)

.PHONY : clean 
clean :
	rm main main.o splay_tree.o splay_node.o

.PHONY : test
test :
	cd testing && make -f TestMakefile
