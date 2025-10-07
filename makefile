EXECUTABLE = solitaire
$(EXECUTABLE): src/*.c
	gcc -c src/*.c
	gcc *.o -o $(EXECUTABLE) && rm *.o && ./$(EXECUTABLE)

.PHONY: clean
clean:
	rm *.o $(EXECUTABLE)