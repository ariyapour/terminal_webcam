# Project Title

Webcam in Terminal. The idea of webcam terminal was implemented based on the work done in the [Full C++17 course](https://www.youtube.com/playlist?list=PLwhKb0RIaIS1sJkejUmWj-0lk7v_xgCuT) by [Code for yourself](https://www.youtube.com/@CodeForYourself).

## Description

Webcam for terminal. It resizes the input image from the webcam to the terminal window.

- In **Kitty**, the feed is drawn as a high-resolution image using the [Kitty graphics protocol](https://sw.kovidgoyal.net/kitty/graphics-protocol/) (the same mechanism as `kitten icat`, without spawning icat per frame).
- In other terminals, it falls back to a pixelated view made of colored background cells.

![webcam_pixelated](https://github.com/ariyapour/terminal_webcam/assets/7849979/e4d9ec7b-9763-4f65-a0f6-0ad8c0eb385a)

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

* To run the program you need a webcam. From the project root:

```
./build/examples/terminal_webcam_example
```

In Kitty (`TERM=xterm-kitty` or `KITTY_WINDOW_ID` set, with a non-zero pixel window size), you get a high-resolution image that follows the window size. Elsewhere you get the pixelated cell renderer.

Press Ctrl-C to quit. In Kitty this also removes the image and restores the cursor.

