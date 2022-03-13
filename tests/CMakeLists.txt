# Test is shell script
find_program(SHELL bash REQUIRED)
# Script uses diff from GNU diffutils
find_program(DIFF diff REQUIRED)

add_test(NAME tests COMMAND ${SHELL} ${CMAKE_CURRENT_SOURCE_DIR}/run_tests.sh $<TARGET_FILE:lca> WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )