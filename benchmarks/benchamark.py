import subprocess

compilers = ['g++', 'clang++-16']
CPPOPTFLAGS = ['0', '1', '2', '3', 'fast']
march_options = ['', '-march=native']
landscapes = ['2021_865', '1999_25j', '2013_12']
n_particles = 30
m_same_particles = 2


def compile_sh(compiler, flag, march):
    return f' && make CXX="{compiler}" CXXFLAGS="-DNDEBUG -std=c++2b" CPPOPTFLAGS="-O{flag} {march}"'


string = 'cd ../src'

for compiler in compilers:
    for flag in CPPOPTFLAGS:
        for march_option in march_options:
            string += compile_sh(compiler, flag, march_option)
            for landscape in landscapes:
                string += f' && ./a.out ../csv/{landscape} {n_particles} {m_same_particles} >> {compiler}_{flag}{march_option}_{landscape}.csv'

subprocess.run(string, shell=True)
