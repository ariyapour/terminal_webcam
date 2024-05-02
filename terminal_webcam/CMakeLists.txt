add_library(image image.cpp)
target_link_libraries(image PUBLIC cxx_setup ftxui::screen)

add_library(drawer drawer.cpp)
target_link_libraries(drawer PUBLIC cxx_setup ftxui::screen image)

add_library(webcam webcam.cpp)
target_link_libraries(webcam PUBLIC cxx_setup ftxui::screen drawer image utils)

add_library(utils INTERFACE)
target_link_libraries(utils INTERFACE ${OpenCV_LIBS})

if(BUILD_TESTING)
  # TODO: Add your tests executable

endif()
