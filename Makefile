# $@ name of the target
# $^ name of all prerequisites with duplicates removed
# $< name of the first prerequisite

PRJ = calc
TESTS = $(PRJ)_tests
CP = cp $< $@
CXXFLAGS = -Og -Wextra -Wall -Isrc -std=c++1z
LDLIBS = -lstdc++

all: $(PRJ) $(TESTS)

$(PRJ): src/main
	$(CP)

$(TESTS): src/main_tests
	$(CP)

src/main_tests: LDLIBS += -lgtest
src/main_tests: src/main_tests.o src/peg/grammar_test.o

src/peg/grammar_test.o: src/peg/grammar_test.cc src/peg/grammar.h \
src/peg/parser.h src/peg/ast.h

test: $(TESTS)
	@./$<

clean:
	find src -name *.o -delete
	rm -f src/main src/main_tests $(PRJ) $(TESTS)

.PHONY: all test clean
