# TODO: Add your libraries here
# You must have the following libraries:
# - stb_image_data_view
# - drawer
# - image
# - pixelate_image

add_library(image image.cpp)
target_link_libraries(image PUBLIC cxx_setup ftxui::screen)

add_library(drawer drawer.cpp)
target_link_libraries(drawer PUBLIC cxx_setup ftxui::screen image)


if(BUILD_TESTING)
  # TODO: Add your tests executable with all your tests here
  # Name it pixelator_tests.
  # Also use gtest_discover_tests(pixelator_tests)
  # to register them with CTest
endif()
