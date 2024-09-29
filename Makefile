build:
	gcc quadtree.c -g -o quadtree
run: build
	./quadtree
clean:
	rm -f quadtree