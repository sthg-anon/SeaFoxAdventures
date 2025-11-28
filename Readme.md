# Sea Fox Adventures
This is a Sonic fangame starring my favorite character, Miles "Tails" Prower. It is a motherload-like game that involves drilling deep into the ocean with Tails' submarine, the Sea Fox.

# Building
This project uses git submodules to manage dependencies. If you are pulling
from git, don't forget to run:
```
git submodule update --init
```
## Dependencies
You will need the following installed:
* CMake
* Python 3
* C++ compiler
## Assets
You will need to package the assets before building. Run the following python command:
```
python headerize_files.py
```
If any of the assets are changed, re-run this python command before building.
## MSVC
```bash
cmake -S . -B build
cmake --build build --config Debug
build/Debug/SeaFoxAdventures
```
