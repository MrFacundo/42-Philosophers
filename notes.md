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

