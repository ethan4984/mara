CXX = gcc
CXXFLAGS = -Wall -Wextra -Isrc -Isrc/lib -g

CXX_SOURCE = $(shell find src -type f -name '*.c')

build:
	$(CXX) $(CXXFLAGS) $(CXX_SOURCE) -o mara
