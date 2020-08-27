include outfiles.local

EXECUABLES = out/obli out/oblid
SHRINK_EXECUTABLES = out/oblid

all: start
	make next
prod: start
	make dnext GOLDFLAGS="-ldflags=\"-s -w\""

start:
	mkdir -p out
	python3 gen_outfiles.py

next: $(SRC_IGNFILES) $(EXECUABLES)
dnext: next shrink

out/obli: $(SRC_TARGETS)
	$(CC) $^ $(SRC_INCFLAGS) -o $@ -lgit2

out/oblid: $(DAEMON_TARGETS)
	cd daemon && \
	go build $(GOLDFLAGS) -o ../$@

shrink: $(SHRINK_EXECUTABLES)
	upx --brute $^

clean:
	rm -rf out