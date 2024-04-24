# Project Title

Webcam in Terminal

## Description

Terminal webcam is an webcam application for terminal. It resizes the input image from webcam and draws the pixelated image in the terminal.

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

### TODO
* Resize input image while keeping its aspect ratio
* Look for terminal window resize event. In such events update the drawer class to draw images in new terminal size.