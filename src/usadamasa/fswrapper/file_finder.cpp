//
// Created by UCHIDA Masaru on 2017/03/18.
//

#include "file_finder.h"

#include <iostream>
#include <sstream>
#include <functional>

#include <boost/foreach.hpp>

namespace bfs = boost::filesystem;

namespace {

// base code from
// http://boost.2283326.n4.nabble.com/filesystem-C-11-range-based-for-statement-and-BOOST-FOREACH-support-tp4632941p4633360.html
template<typename Iter>
class DirectoryRange {
 public:
  DirectoryRange(const bfs::path &p) : iter_(p) {}
  virtual ~DirectoryRange() {}
  Iter begin() const { return iter_; }
  Iter end() const {
    return Iter();
  }

 protected:
  Iter iter_;
};

typedef DirectoryRange<bfs::directory_iterator> NormalDirectoryRange;
typedef DirectoryRange<bfs::recursive_directory_iterator> RecursiveDirectoryRange;

template<typename T, typename Range>
std::vector<T> Filter(const Range &range, const std::string &filename) {
  using fs_filter = std::function<bool(const T &)>;

  const auto is_regular_file = [](const T &e) -> bool {
    return bfs::is_regular_file(e);
  };

  const auto is_match_basename = [&filename](const T &e) -> bool {
    return e.path().filename().c_str() == filename;
  };

  const std::vector<fs_filter> filters = {
      is_regular_file, is_match_basename};

  std::vector<T> plist;
  std::copy_if(range.begin(), range.end(), std::back_inserter(plist), [&filters](const T &e) {
    return std::all_of(filters.begin(), filters.end(), [&e](const fs_filter &f) { return f(e); });
  });
  return plist;
}

}

namespace usadamasa {
namespace fswrapper {

FileFinder::FileFinder(const std::string &basedir) : basedirname_(basedir), basedir_(basedirname_) {
  if (!bfs::exists(this->basedir_)) {
    throw std::runtime_error(this->basedirname_ + " not exists!");
  }
  if (!bfs::is_directory(this->basedir_)) {
    throw std::runtime_error(this->basedirname_ + " is not directory!");
  }
}

std::vector<std::string> FileFinder::FindRegularFile(const std::string &basename, bool recursive) const {

  // If you create a base class for directory_iterator and recursive_directory_iterator,
  // you should be able to write clearly ....
  std::vector<bfs::directory_entry> plist;
  if (recursive) {
    const RecursiveDirectoryRange range(this->basedir_);
    plist = Filter<bfs::directory_entry, decltype(range)>(range, basename);
  } else {
    const NormalDirectoryRange range(this->basedir_);
    plist = Filter<bfs::directory_entry, decltype(range)>(range, basename);
  }

  std::vector<std::string> list;
  std::transform(plist.begin(), plist.end(), std::back_inserter(list),
                 [](const bfs::directory_entry &p) -> std::string {
                   return p.path().c_str();
                 });
  return list;
}

} // namespace fswrapper
} // namespace usadamasa
