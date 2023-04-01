import subprocess
import sys

if len(sys.argv) == 1:
    raise ValueError('Not landscapes where passed as arguments')

compilers = ['g++', 'clang++-16']
CPPOPTFLAGS = ['0', '1', '2', '3', 'fast']
march_options = ['', '-march=native']
landscapes_paths_prefix = sys.argv[1:0] 
n_particles = 30
m_same_particles = 2


def compile_sh(compiler, flag, march):
    return f' && make CXX="{compiler}" CXXFLAGS="-DNDEBUG -std=c++2b" CPPOPTFLAGS="-O{flag} {march}"'


string = 'cd ../src'

for compiler in compilers:
    for flag in CPPOPTFLAGS:
        for march_option in march_options:
            string += compile_sh(compiler, flag, march_option)
            for landscape_path in landscapes_paths_prefix:
                landscape_name = landscape_path.split('/')[-1]
                string += f' && ./a.out {landscape_path} {n_particles} {m_same_particles} >> {compiler}_{flag}{march_option}_{landscape_path}.csv'

subprocess.run(string, shell=True)
