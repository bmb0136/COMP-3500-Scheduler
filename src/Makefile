CC = gcc
CFLAGS = -g

.PHONY: scheduler
scheduler: main.o config.o taskqueue.o simulation.o policy_fcfs.o policy_srtf.o policy_rr.o
	$(CC) $(CFLAGS) -o $@ $^ -o scheduler

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rvf *.o scheduler
