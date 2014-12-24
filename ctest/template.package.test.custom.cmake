set(CTEST_TARGET_SYSTEM                     "@CTEST_TARGET_SYSTEM@")
set(CTEST_SITE                              "@CTEST_SITE@")
set(CTEST_DASHBOARD_ROOT                    "@PACKAGE_TEST_ROOT_DIR@")
set(CTEST_CMAKE_GENERATOR                   "@CTEST_CMAKE_GENERATOR@")
set(CTEST_CONFIGURATION_TYPE                "@CTEST_CONFIGURATION_TYPE@")

include("${CTEST_SCRIPT_DIRECTORY}/opencv_package_test.cmake")
