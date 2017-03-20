#include <algorithm>

#include "gmock/gmock.h"
#include "usadamasa/fswrapper/file_finder.h"

using ::testing::Contains;
using ::testing::StrEq;

namespace ufs = usadamasa::fswrapper;

TEST(FileFinderTest, FindBasenamae) {
  const auto list = ufs::FileFinder("test/data/IAMDIRECTORY").FindRegularFiles("IAMREGULARFILE");
  ASSERT_EQ(1, list.size());
  ASSERT_THAT(list, Contains("test/data/IAMDIRECTORY/IAMREGULARFILE"));
}

TEST(FileFinderTest, FindBasenameRecursive){
  const auto list = ufs::FileFinder("test/data").FindRegularFiles("IAMREGULARFILE", true);
  ASSERT_EQ(2, list.size());
  ASSERT_THAT(list, Contains("test/data/IAMREGULARFILE/IAMREGULARFILE"));
  ASSERT_THAT(list, Contains("test/data/IAMDIRECTORY/IAMREGULARFILE"));
}

TEST(FileFinderTest, UseRegex) {
  const auto list = ufs::FileFinder("test/data").FindRegularFiles("F\\d{8}.txt", true);
  ASSERT_EQ(3, list.size());
}

TEST(FileFinderTest, NotDirectory) {
  ASSERT_THROW(ufs::FileFinder("42"), std::runtime_error);
  ASSERT_THROW(ufs::FileFinder("CMakeLists.txt"), std::runtime_error);
}
