<p align="center">
    <strong>saap.vi</strong>
</p>

I developed this simple Snake Game using the SDL2 library in C.
I was inspired by a video on the [CS50](https://www.youtube.com/@cs50) YouTube channel, which demonstrated [2D Games in C using SDL](https://www.youtube.com/watch?v=yFLa3ln16w0).
The name **`saap.vi`** (with 'saap' meaning 'snake') playfully describes the snake-like behavior of my friends, while 'vi' refers to the Vim keybindings used for controls.

## Project setup

### Prerequisites

- [SDL2](https://www.libsdl.org/)
- C Compiler (gcc, g++ etc.)

#### Void linux

```bash
sudo xbps-install gcc SDL2 SDL2-devel -y
```

### Getting Started

Clone the repository

```bash
git clone git@github.com:iamsampang/saap.vi.git
```

Navitage to the project directory

```bash
cd saap.vi
```

Build the game using `make`

```bash
make
```

Run the game

```bash
./main.bin
```

## Controls

| KEY   | ACTION        |
| ----- | ------------- |
| `i`   | start game    |
| `h`   | move left     |
| `j`   | move down     |
| `k`   | move up       |
| `l`   | move right    |
| `esc` | quit the game |
