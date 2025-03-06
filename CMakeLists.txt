cmake_minimum_required(VERSION 3.10)
project(PROJECT)

set(CMAKE_CXX_STANDARD 17)

# Podesi putanju za include direktorijum
include_directories(/usr/include)  # Možeš promeniti ako je `sndfile.h` na drugoj putanji

# Pronađi biblioteku
find_library(SNDFILE_LIBRARY sndfile)

# Dodaj izvršni fajl
add_executable(PROJECT main11.cpp)

# Linkuj sa bibliotekom
target_link_libraries(PROJECT PRIVATE ${SNDFILE_LIBRARY})



