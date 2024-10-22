

set(command "C:/Program Files (x86)/CMake/bin/cmake.exe;-DCMAKE_BUILD_TYPE=;-Dgtest_force_shared_crt=ON;-Dgtest_disable_pthreads:BOOL=OFF;-DBUILD_GTEST=ON;-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs;-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=ReleaseLibs;-GVisual Studio 14 2015 ARM;C:/Users/juanmanuel/Documents/Visual Studio 2015/Projects/C++/Dependencies/assimp-master/test/gtest/src/gtest")
execute_process(
  COMMAND ${command}
  RESULT_VARIABLE result
  OUTPUT_FILE "C:/Users/juanmanuel/Documents/Visual Studio 2015/Projects/C++/Dependencies/assimp-master/test/gtest/src/gtest-stamp/gtest-configure-out.log"
  ERROR_FILE "C:/Users/juanmanuel/Documents/Visual Studio 2015/Projects/C++/Dependencies/assimp-master/test/gtest/src/gtest-stamp/gtest-configure-err.log"
  )
if(result)
  set(msg "Command failed: ${result}\n")
  foreach(arg IN LISTS command)
    set(msg "${msg} '${arg}'")
  endforeach()
  set(msg "${msg}\nSee also\n  C:/Users/juanmanuel/Documents/Visual Studio 2015/Projects/C++/Dependencies/assimp-master/test/gtest/src/gtest-stamp/gtest-configure-*.log")
  message(FATAL_ERROR "${msg}")
else()
  set(msg "gtest configure command succeeded.  See also C:/Users/juanmanuel/Documents/Visual Studio 2015/Projects/C++/Dependencies/assimp-master/test/gtest/src/gtest-stamp/gtest-configure-*.log")
  message(STATUS "${msg}")
endif()
