CC = gcc
CCFLAGS = -Wall -Wextra -Werror -g2  # -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment
OUTPUT_DIR = .
WORKING_FILES := $(filter-out tests.c, *.c)
LIBNAME = s21_matrix
UNAME := $(shell uname)

SRCDIR = ..
TESTDIR = .
GCOVDIR = gcov_report
LIBRARY = $(SRCDIR)/$(LIBNAME).a
TESTS = $(TESTDIR)/$(LIBNAME).a

all: scratch_compile

s21_matrix.a: all
s21_matrix.a: all
s21_matrix: all
s21_matrix: all

scratch_compile: clean format_project compile

compile:
	find .. -type f -name '*.c' ! -name "tests.c" -exec $(CC) -c -g2 -Wall -Wextra -Werror {} \;
	ar rcs $(LIBNAME).a *.o

clean: clear_test
	find ./../ -type f -name '*.gch'  			-delete
	find ./../ -type f -name '*.o'    			-delete
	find ./../ -type f -name '*.a'      			-delete
	find ./../ -type f -name '*.gcda'      			-delete
	find ./../ -type f -name '*.gcov'      			-delete
	find ./../ -type f -name '*.gcno'      			-delete
	find ./../ -name '*.dSYM'      				-exec rm -rv {} +
	find ./../ -type f -name '*.out'  			-delete
	find ./../ -type f -name 'vgcore*'  			-delete
	rm -rf $(GCOVDIR)

format_project:
	find ../ -iname '*.c' | xargs clang-format -i
	find ../ -iname '*.h' | xargs clang-format -i
	cppcheck --check-config --enable=all --suppress=missingIncludeSystem ../

test: clean c_test
	./tt

gcov_report: test
#	gcov -o . $(LIBRARY)
#	gcov -o . $(TESTS)
	mkdir -p $(GCOVDIR)
	mv *.gcda $(GCOVDIR)
	mv *.gcno $(GCOVDIR)
	lcov --capture --directory $(GCOVDIR) --output-file $(GCOVDIR)/coverage.info
	genhtml $(GCOVDIR)/coverage.info --output-directory $(GCOVDIR)/html

c_test:
ifeq ($(UNAME),  Darwin)
	$(CC) --coverage -fprofile-arcs -ftest-coverage -g2 -Wall -Wextra -Werror *.c -o ./tt -lcheck -lm -lpthread
else
	find .. -type f -name '*.c' ! -name "tests.c" -exec $(CC) --coverage -fprofile-arcs -ftest-coverage -c -g2 -Wall -Wextra -Werror {} \;
	ar rcs $(LIBNAME).a *.o
	$(CC) $(CCFLAGS) -o $(OUTPUT_DIR)/tt tests.c -l:$(LIBNAME).a -lcheck -lm -lpthread -lsubunit -lgcov -L.
endif

clear_test:
	find ./../ -type f -name 'tt*'  		-delete
	find ./../ -type f -name 'tt'  		-delete
