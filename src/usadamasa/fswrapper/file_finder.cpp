//
// Created by UCHIDA Masaru on 2017/03/18.
//

#include "file_finder.h"

#include <functional>
#include <iostream>
#include <sstream>

#include <boost/foreach.hpp>
#include <boost/regex.hpp>

namespace bfs = boost::filesystem;

namespace {

// base code from
// http://boost.2283326.n4.nabble.com/filesystem-C-11-range-based-for-statement-and-BOOST-FOREACH-support-tp4632941p4633360.html
template<typename Iter>
class DirectoryRange {
 public:
  DirectoryRange(const bfs::path &p) : path_(p), iter_(this->path_) {}
  DirectoryRange(const DirectoryRange &obj)
      : path_(obj.path_), iter_(this->path_) {}
  virtual ~DirectoryRange() {}
  Iter begin() const { return iter_; }
  Iter end() const { return Iter(); }

 protected:
  const bfs::path path_;
  const Iter iter_;
};

typedef DirectoryRange<bfs::directory_iterator> NormalDirectoryRange;
typedef DirectoryRange<bfs::recursive_directory_iterator>
    RecursiveDirectoryRange;

template<typename T, typename Range>
std::vector<T> Filter(const Range &range, const std::string &filename) {

  const auto is_regular_file = [](const T &e) -> bool {
    return bfs::is_regular_file(e);
  };

  const boost::regex reg(filename);
  const auto is_match_basename = [&reg](const T &e) -> bool {
    boost::smatch what;
    return boost::regex_match(e.path().filename().string(), what, reg);
  };

  using fs_filter = std::function<bool(const T &)>;
  const std::vector<fs_filter> filters = {is_regular_file, is_match_basename};

  // to get list size, copying range.
  const auto temp(range);
  std::vector<T> plist;
  plist.reserve(std::distance(temp.begin(), temp.end()));

  std::copy_if(range.begin(), range.end(), std::back_inserter(plist),
               [&filters](const T &e) {
                 return std::all_of(
                     filters.begin(), filters.end(),
                     [&e](const fs_filter &f) { return f(e); });
               });

  plist.shrink_to_fit();
  return plist;
}
}

namespace usadamasa {
namespace fswrapper {

FileFinder::FileFinder(const std::string &basedir)
    : basedirname_(basedir), basedir_(basedirname_) {
  if (!bfs::exists(this->basedir_)) {
    throw std::runtime_error(this->basedirname_ + " not exists!");
  }
  if (!bfs::is_directory(this->basedir_)) {
    throw std::runtime_error(this->basedirname_ + " is not directory!");
  }
}

std::vector<std::string>
FileFinder::FindRegularFile(const std::string &basename, bool recursive) const {

  std::vector<bfs::directory_entry> plist;
  if (recursive) {
    const RecursiveDirectoryRange range(this->basedir_);
    plist = Filter<bfs::directory_entry, decltype(range)>(range, basename);
  } else {
    const NormalDirectoryRange range(this->basedir_);
    plist = Filter<bfs::directory_entry, decltype(range)>(range, basename);
  }

  std::vector<std::string> list;
  list.reserve(plist.size());
  std::transform(plist.begin(), plist.end(), std::back_inserter(list),
                 [](const bfs::directory_entry &p) -> std::string {
                   return p.path().string();
                 });
  list.shrink_to_fit();
  return list;
}

} // namespace fswrapper
} // namespace usadamasa
