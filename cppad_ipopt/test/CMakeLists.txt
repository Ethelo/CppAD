# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# Build the cppad_ipopt/test directory tests
# Inherit build tyope from ../CMakeList.txt

# Local include directories to search (not in package_prefix/include)
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/../src )

# Local link directories to search (not in external packages)
LINK_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/../src ${ipopt_LIBRARY_DIRS} )

# add_executable(<name> [WIN32] [MACOSX_BUNDLE] [EXCLUDE_FROM_ALL]
#                 source1 source2 ... sourceN
# )
SET(source_list test_more.cpp
	k_gt_one.cpp
	multiple_solution.cpp
	retape_k1_l1.cpp
	retape_k1_l2.cpp
)
set_compile_flags( cppad_ipopt_test "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE( cppad_ipopt_test EXCLUDE_FROM_ALL ${source_list} )

# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(cppad_ipopt_test
	cppad_ipopt
	${cppad_lib}
	${ipopt_LIBRARIES}
	${colpack_libs}
)

# Add the check_cppad_ipopt_test target
ADD_CUSTOM_TARGET(check_cppad_ipopt_test
	cppad_ipopt_test
	DEPENDS cppad_ipopt_test cppad_ipopt
)
MESSAGE(STATUS "make check_cppad_ipopt_test: available")

# Change check depends in parent environment
add_to_list(check_cppad_ipopt_depends check_cppad_ipopt_test)
SET(check_cppad_ipopt_depends "${check_cppad_ipopt_depends}" PARENT_SCOPE)
