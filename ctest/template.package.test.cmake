#
# Include CTest Ext module
#

if(NOT DEFINED CTEST_EXT_MODULE_PATH)
    set(CTEST_EXT_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/ctest_ext.cmake")
endif()

include("${CTEST_EXT_MODULE_PATH}")

#
# Dashboard settings
#

if(NOT DEFINED CTEST_DASHBOARD_ROOT)
    create_tmp_dir(CTEST_DASHBOARD_ROOT)
endif()

set_ifndef(CTEST_MODEL              "Package")
set_ifndef(CTEST_SOURCE_DIRECTORY   "${CTEST_SCRIPT_DIRECTORY}")
set_ifndef(CTEST_WITH_UPDATE        FALSE)
set_ifndef(CTEST_WITH_SUBMIT        TRUE)

#
# Initialize testing
#

ctest_ext_init()

#
# Check supported targets and models
#

check_if_matches(CTEST_TARGET_SYSTEM    "^Linux" "^Windows" "^MacOS")
check_if_matches(CTEST_MODEL            "^Package$" "^Package-Performance$")

#
# Configure the testing model (set options, not specified by user, to default values)
#

set_ifndef(CTEST_TEST_TIMEOUT       7200)

if(CTEST_TARGET_SYSTEM MATCHES "Windows")
    if(CTEST_TARGET_SYSTEM MATCHES "64")
        set_ifndef(CTEST_CMAKE_GENERATOR "Visual Studio 12 Win64")
    else()
        set_ifndef(CTEST_CMAKE_GENERATOR "Visual Studio 12")
    endif()
else()
    set_ifndef(CTEST_CMAKE_GENERATOR "Unix Makefiles")
endif()

set_ifndef(CTEST_CONFIGURATION_TYPE "Release")

#
# Start testing
#

ctest_ext_start()

#
# Configure
#

ctest_ext_configure()

#
# Remove previous test reports
#

set(TEST_REPORTS_DIR "${CTEST_BINARY_DIRECTORY}/test-reports")
set(ACCURACY_REPORTS_DIR "${TEST_REPORTS_DIR}/accuracy")
set(PERF_REPORTS_DIR "${TEST_REPORTS_DIR}/performance")
set(SANITY_REPORTS_DIR "${TEST_REPORTS_DIR}/sanity")

if(CTEST_STAGE MATCHES "Configure")
    if(EXISTS "${ACCURACY_REPORTS_DIR}")
        file(REMOVE_RECURSE "${ACCURACY_REPORTS_DIR}")
    endif()
    if(EXISTS "${SANITY_REPORTS_DIR}")
        file(REMOVE_RECURSE "${SANITY_REPORTS_DIR}")
    endif()
    if(EXISTS "${PERF_REPORTS_DIR}")
        file(REMOVE_RECURSE "${PERF_REPORTS_DIR}")
    endif()
endif()

#
# Test
#

if(CTEST_MODEL MATCHES "Performance")
    ctest_ext_test(
        INCLUDE_LABEL "Performance"
        EXCLUDE "^(opencv_test_viz|opencv_test_highgui|opencv_test_shape|opencv_sanity_videoio)$")
else()
    ctest_ext_test(
        EXCLUDE_LABEL "Performance"
        EXCLUDE "^(opencv_test_viz|opencv_test_highgui|opencv_test_shape|opencv_sanity_videoio)$")
endif()

#
# Submit
#

if(CTEST_MODEL MATCHES "Performance")
    file(GLOB perf_xmls "${PERF_REPORTS_DIR}/*.xml")

    ctest_info("Upload performance reports : ${perf_xmls}")
    list(APPEND CTEST_UPLOAD_FILES ${perf_xmls})
endif()

ctest_ext_submit()
