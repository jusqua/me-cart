# Me, Cart

## How to use this project?

This project in made in C++ with OpenGL, some tools and libraries are needed to this program run correctly:
  - Clang
  - Make
  - GLFW
  - GLEW
  - GLM
  - STB

### Setting up

#### Fedora

```
sudo dnf install glew-devel glfw-devel glm-devel stb-devel clang make
```

#### Debian/Ubuntu

```
sudo apt install libstdc++-12-dev libglew-dev libglfw3-dev libglm-dev libstb-dev clang make
```

### Running

There are available 4 types of terrain in PGM located at `resources/terrain/` that you can choose, or input your own PGM file.

You need to build first with `make` that returns a binary `me-cart`:
```
make
./me-cart [terrain file path]
```


## Actions

Use `Mouse` to move around, it's implements a fly-like camera:

| Key        | Description             |
| ---------- | ----------------------- |
| W          | Move forward            |
| S          | Move backward           |
| A          | Turn left               |
| D          | Turn right              |
| Escape     | Quit                    |

