
# Read names of all cpp files except main.cpp and delete extension

files_cpp=$(ls ../*.cpp | grep -v main.cpp)

# Remove ../ and replace .cpp with .o in all
files_o=$(echo $files_cpp | sed 's/\.\.\///g' | sed 's/\.cpp/\.o/g')

# Define several methods of compiling a main file
# $1: compiler
# $2: main file
# $3: output file
# $4: Optimization level

with_object_files() {
  $1 -std=c++23 -O$4 -I../ -c $files_cpp
  $1 -std=c++23 -O$4 -I../ $files_o $2 -o $3
}

without_object_files() {
  $1 -std=c++23 -O$4 -I../ $files_cpp $2 -o $3
}



with_object_files g++-12 benchmark_simulate_fire.cpp benchmark_simulate_fire 1
echo "g++ with intermediate object files and -O1"
/usr/bin/time ./benchmark_simulate_fire 2>&1 | awk 'NR==1{print $1 " " $2 " " $3;}'

with_object_files g++-12 benchmark_simulate_fire.cpp benchmark_simulate_fire 2
echo "g++ with intermediate object files and -O2"
/usr/bin/time ./benchmark_simulate_fire 2>&1 | awk 'NR==1{print $1 " " $2 " " $3;}'

with_object_files g++-12 benchmark_simulate_fire.cpp benchmark_simulate_fire 3
echo "g++ with intermediate object files and -O3"
/usr/bin/time ./benchmark_simulate_fire 2>&1 | awk 'NR==1{print $1 " " $2 " " $3;}'

echo ""

without_object_files g++-12 benchmark_simulate_fire.cpp benchmark_simulate_fire 1
echo "g++ without intermediate object files and -O1"
/usr/bin/time ./benchmark_simulate_fire 2>&1 | awk 'NR==1{print $1 " " $2 " " $3;}'

without_object_files g++-12 benchmark_simulate_fire.cpp benchmark_simulate_fire 2
echo "g++ without intermediate object files and -O2"
/usr/bin/time ./benchmark_simulate_fire 2>&1 | awk 'NR==1{print $1 " " $2 " " $3;}'

without_object_files g++-12 benchmark_simulate_fire.cpp benchmark_simulate_fire 3
echo "g++ without intermediate object files and -O3"
/usr/bin/time ./benchmark_simulate_fire 2>&1 | awk 'NR==1{print $1 " " $2 " " $3;}'
