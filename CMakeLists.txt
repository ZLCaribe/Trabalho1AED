cmake_minimum_required(VERSION 3.23)
project(.)

set(CMAKE_CXX_STANDARD 14)

add_executable(.
        main.cpp
        src/Alteracao.cpp
        src/Estudante.cpp
        src/Gestor.cpp
        src/Pedido.cpp
        src/Slot.cpp
        src/TurmaH.cpp
        src/UCTurma.cpp
)
