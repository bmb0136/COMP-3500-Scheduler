CC = gcc
CFLAGS = -g

.PHONY: scheduler
scheduler: main.o
	$(CC) $(CFLAGS) -o $@ $^ -o scheduler

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rvf *.o scheduler
