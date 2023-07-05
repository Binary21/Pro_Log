#!/bin/sh
# assumes root is defined as CMAKE_SOURCE_DIR
echo "Running Coverage assuming source root = " ${ROOT}

lcov -c -i -b ${ROOT} -d . --no-external -o Coverage.baseline
./unit_tests
lcov -c -b ${ROOT} -d . --no-external  -o Coverage.out
lcov -a Coverage.baseline -a Coverage.out -o Coverage.combined
echo "-----------------------------------------------------------------"
lcov -r Coverage.combined ${ROOT}/catch.hpp $PWD/test_config.hpp -o Coverage.combined.cleaned
echo "-----------------------------------------------------------------"
genhtml Coverage.combined.cleaned -o Coverage_Report
