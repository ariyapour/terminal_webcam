# Project Title

Webcam in Terminal. The idea of webcam terminal was implemented based on the work done in the [Full C++17 course](https://www.youtube.com/playlist?list=PLwhKb0RIaIS1sJkejUmWj-0lk7v_xgCuT) by [Code for yourself](https://www.youtube.com/@CodeForYourself).

## Description

Webcam for terminal. It resizes the input image from webcam and draws the pixelated image in the terminal.

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

* To run the program you need a webcam. In the root directory of the project run the following command to display webcam feed in terminal.
```
./build/examples/terminal_webcam_example
```

