# Git submodules
## Adding
git pull --recurse-submodules
git submodule update --init --recursive 
## Removing
git submodule deinit -f path/to/submodule
rm -rf .git/modules/path/to/submodule
git rm -f path/to/submodule
 
# Valgrind

valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all -s <binary>

# Checker

ARG="4 67 3 87 23"; ./push_swap $ARG | ./checker_OS $ARG

# Testers
## test 1
make -C push_swap_averager
## test 2
./tester.sh
## test 3
push_swap_test_linux.sh
## visualizer
./bin/visualizer

# install and make  cmake to work
sudo apt-get install pkg-config
sudo apt-get install mesa-utils libglu1-mesa-dev freeglut3-dev mesa-common-dev\nsudo apt-get install libglew-dev libglfw3-dev libglm-dev\nsudo apt-get install libao-dev libmpg123-dev
sudo apt-get install -y libudev-dev
sudo apt-get install -y  libfreetype-dev