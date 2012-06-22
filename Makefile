all:
	make -C src/
	mv src/arbiter .

clean:
	rm arbiter
	make -C src/ clean