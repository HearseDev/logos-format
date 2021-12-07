.PHONY = clean
logos-format: logos-format.cpp
	g++ -std=c++17 $^ -o $@
clean:
	rm -f logos-format
