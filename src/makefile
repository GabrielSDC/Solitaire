EXECUTABLE = solitaire
$(EXECUTABLE): *.c
	gcc -c *.c
	gcc *.o -o $(EXECUTABLE)

.PHONY: clean
clean:
	rm *.o $(EXECUTABLE)