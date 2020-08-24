include outfiles.local

all:
	mkdir -p out
	python3 gen_outfiles.py
	make next

next: $(IGNFILES) out/obli

out/obli: $(TARGETS)
	$(CC) $^ $(INCFLAGS) -o $@ -lgit2

clean:
	rm -rf out