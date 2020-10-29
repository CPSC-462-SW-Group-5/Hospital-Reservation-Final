# This make file was adopted to run in a MinGW environment.  Assumes your current working
# directory is the root of your project.

CXX       ::= g++
CXXFLAGS  ::= -g0 -O3 -DNDEBUG               \
              -pthread -std=c++17 -I.        \
              -DUSING_TOMS_SUGGESTIONS       \
              -D__func__=__PRETTY_FUNCTION__ \
              -Wall -Wextra -pedantic        \
              -Wdelete-non-virtual-dtor      \
              -Wduplicated-branches          \
              -Wduplicated-cond              \
              -Wextra-semi                   \
              -Wfloat-equal                  \
              -Winit-self                    \
              -Wlogical-op                   \
              -Wnoexcept                     \
              -Wnon-virtual-dtor             \
              -Wold-style-cast               \
              -Wstrict-null-sentinel         \
              -Wsuggest-override             \
              -Wswitch-default               \
              -Wswitch-enum                  \
              -Woverloaded-virtual           \
              -Wuseless-cast                 \
              -Wzero-as-null-pointer-constant

SOURCES   ::= $(filter %.cpp %.c, $(wildcard *  */*  */*/*  */*/*/*  */*/*/*/*))
args      ::=



.PHONY: project_($(CXX)).exe
project_($(CXX)).exe:
	@echo Compiling ...
	@$(foreach token, $(SOURCES), echo     $(token) &)
	@echo with:
	@echo $(CXX) $(CXXFLAGS) $(args)
	@echo ----
	@$(CXX) --version
	@$(CXX) $(CXXFLAGS) $(args) $(SOURCES) -o $@

# options to consider:
#       -Weffc++
