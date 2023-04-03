
# Read names of all cpp files except main.cpp and delete extension

files_cpp=$(ls ../src/*.cpp | grep -v main.cpp)

# Remove ../ and replace .cpp with .o in all
files_o=$(echo $files_cpp | sed 's/\.\.\/src\///g' | sed 's/\.cpp/\.o/g')

# Define several methods of compiling a main file
# $1: compiler
# $2: main file
# $3: output file
# $4: Optimization level

with_object_files() {
  $1 -std=c++23 -DNDEBUG -O$4 -I../src -c $files_cpp
  $1 -std=c++23 -DNDEBUG -O$4 -I../src $files_o $2 -o $3
}

without_object_files() {
  $1 -std=c++23 -O$4 -I../src $files_cpp $2 -o $3
}

benchmarks=$(ls ./*.cpp)

for benchmark_cpp in $benchmarks
do
  benchmark=$(echo $benchmark_cpp | sed 's/\.cpp//g')
  # Remove ./benchmark from start

  echo ""
  echo "####################"
  echo $(echo $benchmark | sed 's/\.\///g')
  echo ""

  for compilation in with_object_files without_object_files
  do

    for opt_level in 0 1 2 3 s z fast
    do
      $compilation g++-12 $benchmark_cpp $benchmark $opt_level
      echo "g++ $compilation and -O$opt_level"
      /usr/bin/time ./$benchmark 2>&1 | awk 'NR==1{print $1 " " $2 " " $3;}'

    done
    echo ""
  done
done

