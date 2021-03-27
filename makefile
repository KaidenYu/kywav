

kywav:src/*.c
	gcc -Iinc/ $^ -o $@ 	

.PHONY:clean
clean:
	rm kywav