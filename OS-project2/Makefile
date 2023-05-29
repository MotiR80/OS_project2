CC=g++
STD=-std=c++14
CF=$(STD)

all: ClubsAgeStats.out position.out club.out country.out


ClubsAgeStats.out: main.cpp
	$(CC) $(CF) -o ClubsAgeStats.out main.cpp

country.out: country.cpp
	$(CC) $(CF) -o country.out country.cpp

club.out: club.cpp
	$(CC) $(CF) -o club.out club.cpp

position.out: position.cpp
	$(CC) $(CF) -o position.out position.cpp

.PHONY: clean
clean:
	rm *.out