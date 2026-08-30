# Project Title

Webcam in Terminal. The idea of webcam terminal was implemented based on the work done in the [Full C++17 course](https://www.youtube.com/playlist?list=PLwhKb0RIaIS1sJkejUmWj-0lk7v_xgCuT) by [Code for yourself](https://www.youtube.com/@CodeForYourself).

## Description

Webcam for terminal. It resizes the input image from the webcam to the terminal window.

- In **[Kitty](https://sw.kovidgoyal.net/kitty/)**, the feed is drawn as a **high-resolution image** using the [Kitty graphics protocol](https://sw.kovidgoyal.net/kitty/graphics-protocol/) (the same mechanism as [`kitten icat`](https://sw.kovidgoyal.net/kitty/kittens/icat/), without spawning icat per frame).
- In other terminals, it falls back to a pixelated view made of colored background cells.

![webcam_pixelated](https://github.com/ariyapour/terminal_webcam/assets/7849979/e4d9ec7b-9763-4f65-a0f6-0ad8c0eb385a)

## High-resolution webcam in Kitty

Kitty is a GPU-based terminal that can display real images, not only ASCII or colored cells. If you run this program **inside Kitty**, you get a sharp webcam view limited by the window size in pixels.

### Install Kitty

Official install instructions: [Install kitty](https://sw.kovidgoyal.net/kitty/binary/).

On Linux or macOS you can install a pre-built binary with:

```
curl -L https://sw.kovidgoyal.net/kitty/installer.sh | sh /dev/stdin
```

Many distributions also ship a `kitty` package (for example `apt install kitty` or `dnf install kitty`). The installer above is the supported way to get the latest release.

Project homepage and extra docs:

- [Kitty homepage](https://sw.kovidgoyal.net/kitty/)
- [Graphics protocol](https://sw.kovidgoyal.net/kitty/graphics-protocol/) (how images are sent to the terminal)
- [`icat` kitten](https://sw.kovidgoyal.net/kitty/kittens/icat/) (static images; useful to confirm graphics work: `kitten icat some.png`)

### Run this project in Kitty

Open Kitty, build as below, then from the project root:

```
./build/examples/terminal_webcam_example
```

The high-res path is used when Kitty is detected (`TERM=xterm-kitty` or `KITTY_WINDOW_ID` is set) and the window reports a non-zero pixel size. Resize the window and the feed follows. Press Ctrl-C to quit; the image is removed and the cursor is restored.

If you run the same binary in a normal terminal, you still get the pixelated cell renderer.

## Getting Started

### Dependencies
* OpenCV

### Installing

* Clone the repository
```
git clone https://github.com/ariyapour/terminal_webcam.git
```
* Navigate to the terminal_webcam directory and build the project
```
cd terminal_webcam
cmake -S . -B build
cmake --build build -j 8
```

### Executing program

You need a webcam. From the project root:

```
./build/examples/terminal_webcam_example
```

For a high-resolution feed, run that command **in Kitty** (see [High-resolution webcam in Kitty](#high-resolution-webcam-in-kitty)). Elsewhere you get the pixelated renderer. Press Ctrl-C to quit.

