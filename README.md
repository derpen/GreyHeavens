# Grey Heavens
That's the title of the game (soon).

`git clone --recurse-submodules https://github.com/derpen/GreyHeavens.git`

or, if already cloned without submodule

```sh
git submodule init
git submodule update
```

# Compile
Use `CMake`

```sh
cmake -S . -B build
cmake --build build
```

# Third Party Libraries
- SDL3
