# Me, Cart

## How to use this project?

### Setting up

#### Fedora

```
sudo dnf install glew-devel glfw-devel glm-devel stb-devel clang make
```

#### Debian/Ubuntu

```
sudo apt install libglew-dev libglfw3-dev libglm-dev libstb-dev clang make
```

### Running

You can run immediately by using `make run`, it builds and run the project with default params.

Or you can build first and the result file `me-cart` and pass the params by yourself
```
make build
./me-cart [terrain file path]
```

## Actions

Use `Mouse` to move around, it's implements a fly-like camera

| Key         | Description   |
| ----------- | ------------- |
| W           | Move forward  |
| S           | Move backward |
| A           | Move to left  |
| D           | Move to right |
| Escape      | Quit          |

