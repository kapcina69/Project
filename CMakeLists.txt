cmake_minimum_required(VERSION 3.10)
project(PROJECT)

set(CMAKE_CXX_STANDARD 17)

# Podesi putanju za include direktorijum
include_directories(/usr/include)  # Možeš promeniti ako je `sndfile.h` na drugoj putanji

# Pronađi biblioteku
find_library(SNDFILE_LIBRARY NAMES sndfile PATHS /usr/lib /usr/local/lib)

# Proveri da li je biblioteka pronađena
if (NOT SNDFILE_LIBRARY)
    message(FATAL_ERROR "sndfile library not found!")
endif()

# Dodaj izvršni fajl
add_executable(PROJECT main11.cpp)

# Linkuj sa bibliotekom
target_link_libraries(PROJECT PRIVATE ${SNDFILE_LIBRARY})

# Dodaj KFR biblioteke i zaglavlja
include_directories("/usr/local/include/kfr")

# Dodaj putanju do direktorijuma sa bibliotekama
link_directories(/usr/local/lib)

# Linkuj sa KFR
target_link_libraries(PROJECT PUBLIC 
    kfr_dsp_avx2 
    kfr_dsp_avx512 
    kfr_dsp_avx 
    kfr_dsp_sse2 
    kfr_dsp_sse41 
    kfr_io
)

