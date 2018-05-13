link_flags :=
compile_flags := -O3 -march=native -DNDEBUG -std=c++11
cov_link_flags := $(link_flags) --coverage
cov_compile_flags := -g --coverage -std=c++11
CC := g++

main := main
tester := testing/tester
test_dir := testing/tests
bin_path := target/bin/
cov_path := target/cov/
doc_path := target/doc/

test_sources := $(tester).cpp testing/testgens/dinic_tests.cpp testing/testgens/linkcut_tests.cpp testing/testgens/splay_tests.cpp
sources := $(main).cpp dinic/dinic.cpp splaytree/splay_tree.cpp linkcut/link_cut_tree.cpp testing/test_utils.cpp $(test_sources)
objects := $(addprefix $(bin_path), $(sources:.cpp=.o))
depends := $(addprefix $(bin_path), $(sources:.cpp=.d))
cov_objects := $(addprefix $(cov_path), $(sources:.cpp=.o))
cov_depends := $(addprefix $(cov_path), $(sources:.cpp=.d))
cov_exclude_files := testing/* testing/testgens/*

$(bin_path)$(main) : $(filter-out $(bin_path)$(tester).o, $(objects))
	@mkdir -p $(dir $@)
	$(CC) $(compile_flags) $^ -o $@ $(link_flags)

$(bin_path)$(tester) : $(filter-out $(bin_path)$(main).o, $(objects))
	@mkdir -p $(dir $@)
	$(CC) $(compile_flags) $^ -o $@ $(link_flags)

$(cov_path)$(tester) : $(filter-out $(cov_path)$(main).o, $(cov_objects))
	@mkdir -p $(dir $@)
	$(CC) $(cov_compile_flags) $^ -o $@ $(cov_link_flags)

-include $(depends)
-include $(cov_depends)

$(bin_path)%.o : %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(compile_flags) -c -MMD $< -o $@

$(cov_path)%.o : %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(cov_compile_flags) -c -MMD $< -o $@

$(test_dir) : $(bin_path)$(tester) $(test_sources)
	@mkdir -p $(test_dir)
	@./$(bin_path)$(tester) generate

.PHONY : gentests
gentests : $(bin_path)$(tester) $(test_dir)
	@# Do nothing. Prerequisite $(test_dir) does all the work.

.PHONY : test 
test : $(bin_path)$(tester) gentests
	@./$(bin_path)$(tester)

.PHONY : performance
performance : $(bin_path)$(tester) gentests
	@./$(bin_path)$(tester) performance

.PHONY : run
run : $(bin_path)$(main)
	@./$(bin_path)$(main)

.PHONY : coverage
coverage : $(cov_path)$(tester) gentests
	@lcov -q -o $(cov_path)initcovdata.info -c -i -d $(cov_path) -b $(cov_path) 2>/dev/null
	@./$(cov_path)$(tester)
	@gcov $(cov_path)$(tester) -r >/dev/null
	@lcov -q -o $(cov_path)runcovdata.info -c -d $(cov_path) -b $(cov_path) 2>/dev/null
	@lcov -q -a $(cov_path)initcovdata.info -a $(cov_path)runcovdata.info -o $(cov_path)rescovdata.info 2>/dev/null
	@lcov -q -e $(cov_path)rescovdata.info "`pwd`/*" -o $(cov_path)rescovdata.info 2>/dev/null
	@lcov -q --remove $(cov_path)rescovdata.info -o $(cov_path)rescovdata.info $(cov_exclude_files) 2>/dev/null
	@genhtml -q -o $(cov_path)html/ $(cov_path)rescovdata.info 2>/dev/null
	@echo created coverage report index.html in $(cov_path)html/index.html

.PHONY : clean
clean :
	@rm -rf ./target    #Erase binaries
	@rm -rf $(test_dir) #Erase tests
