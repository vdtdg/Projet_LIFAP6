CXX      = g++

SOURCES = pioche.cpp jeu.cpp jeu_io.cpp joueur_simple.cpp
HEADERS = $(SOURCES:.cpp=.hpp)
OBJECTS = $(SOURCES:.cpp=.o)

CXXFLAGS  += -g -Wall -std=c++11 -pedantic
LDFLAGS +=

all : test_pioche test_jeu

$(OBJECTS) : %.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $? -o $@

########## test_pioche ##########

PIOCHE_SOURCES = test_pioche.cpp
PIOCHE_OBJECTS = $(PIOCHE_SOURCES:.cpp=.o)

test_pioche : $(PIOCHE_OBJECTS) $(OBJECTS) $(HEADERS)
	$(CXX) $(PIOCHE_OBJECTS) $(OBJECTS) -o $@ $(LDFLAGS)
	
$(PIOCHE_OBJECTS): %.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $? -o $@

########## test_jeu ##########

JEU_SOURCES = test_jeu.cpp
JEU_OBJECTS = $(JEU_SOURCES:.cpp=.o)

test_jeu : $(JEU_OBJECTS) $(OBJECTS) $(HEADERS)
	$(CXX) $(JEU_OBJECTS) $(OBJECTS) -o $@ $(LDFLAGS)
	
$(JEU_OBJECTS): %.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $? -o $@

########## cleanup ##########

clean:
	@rm -f test_pioche test_jeu $(OBJECTS) $(PIOCHE_OBJECTS) $(JEU_OBJECTS)
