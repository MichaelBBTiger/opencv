cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
enable_testing()

project(OpenCV NONE)

# Locations

get_filename_component(OPENCV_INSTALL_BASE_DIR "${CMAKE_SOURCE_DIR}/@CTEST_OPENCV_INSTALL_BASE_DIR@" ABSOLUTE)

get_filename_component(OPENCV_BIN_INSTALL_PATH "${OPENCV_INSTALL_BASE_DIR}/@OPENCV_BIN_INSTALL_PATH@" ABSOLUTE)
get_filename_component(OPENCV_LIB_INSTALL_PATH "${OPENCV_INSTALL_BASE_DIR}/@OPENCV_LIB_INSTALL_PATH@" ABSOLUTE)
get_filename_component(OPENCV_TEST_INSTALL_PATH "${OPENCV_INSTALL_BASE_DIR}/@OPENCV_TEST_INSTALL_PATH@" ABSOLUTE)
get_filename_component(OPENCV_TEST_DATA_INSTALL_PATH "${OPENCV_INSTALL_BASE_DIR}/@OPENCV_TEST_DATA_INSTALL_PATH@" ABSOLUTE)

# Environment

set(ENVIRONMENT "OPENCV_TEST_DATA_PATH=${OPENCV_TEST_DATA_INSTALL_PATH}")
if(UNIX)
    list(APPEND ENVIRONMENT "LD_LIBRARY_PATH=$ENV{LD_LIBRARY_PATH}:${OPENCV_LIB_INSTALL_PATH}")
endif()

# Tests

set(test_report_dir "${CMAKE_BINARY_DIR}/test-reports")
set(accuracy_report_dir "${test_report_dir}/accuracy")
set(sanity_report_dir "${test_report_dir}/sanity")
set(performance_report_dir "${test_report_dir}/performance")
file(MAKE_DIRECTORY "${test_report_dir}" "${accuracy_report_dir}" "${sanity_report_dir}" "${performance_report_dir}")

file(GLOB accuracy_tests "${OPENCV_TEST_INSTALL_PATH}/opencv_test_*")
file(GLOB performace_tests "${OPENCV_TEST_INSTALL_PATH}/opencv_perf_*")

foreach(test_path ${accuracy_tests})
    get_filename_component(test_name "${test_path}" NAME_WE)

    add_executable("${test_name}" IMPORTED)
    set_target_properties("${test_name}" PROPERTIES IMPORTED_LOCATION "${test_path}")

    add_test(NAME "${test_name}" COMMAND "${test_name}" "--gtest_output=xml:${test_name}.xml")
    set_tests_properties("${test_name}" PROPERTIES
        LABELS "Accuracy"
        WORKING_DIRECTORY "${accuracy_report_dir}"
        ENVIRONMENT "${ENVIRONMENT}")
endforeach()

foreach(test_path ${performace_tests})
    get_filename_component(test_name "${test_path}" NAME_WE)

    add_executable("${test_name}" IMPORTED)
    set_target_properties("${test_name}" PROPERTIES IMPORTED_LOCATION "${test_path}")

    string(REPLACE "opencv_perf_" "" module_name "${test_name}")

    add_test(NAME "${test_name}" COMMAND "${test_name}" "--gtest_output=xml:${test_name}.xml")
    set_tests_properties("${test_name}" PROPERTIES
        LABELS "Performance"
        WORKING_DIRECTORY "${perf_report_dir}"
        ENVIRONMENT "${ENVIRONMENT}")

    add_test(NAME "opencv_sanity_${module_name}"
        COMMAND "${test_name}"
                "--gtest_output=xml:${test_name}.xml"
                "--perf_min_samples=1" "--perf_force_samples=1" "--perf_verify_sanity")
    set_tests_properties("opencv_sanity_${module_name}" PROPERTIES
        LABELS "Sanity"
        WORKING_DIRECTORY "${sanity_report_dir}"
        ENVIRONMENT "${ENVIRONMENT}")
endforeach()
