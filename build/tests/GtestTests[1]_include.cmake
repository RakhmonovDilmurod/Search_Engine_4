if(EXISTS "C:/Users/HP/OneDrive/Documents/GitHub/Search_Engine_4/build/tests/GtestTests[1]_tests.cmake")
  include("C:/Users/HP/OneDrive/Documents/GitHub/Search_Engine_4/build/tests/GtestTests[1]_tests.cmake")
else()
  add_test(GtestTests_NOT_BUILT GtestTests_NOT_BUILT)
endif()