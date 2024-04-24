add_executable(terminal_webcam_example terminal_webcam_example.cpp)
target_link_libraries(terminal_webcam_example PRIVATE cxx_setup ${OpenCV_LIBS} ftxui::screen drawer image)

# TODO: Add your binaries here
