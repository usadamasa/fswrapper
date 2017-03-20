//
// Created by UCHIDA Masaru on 2017/03/18.
//

#ifndef _FSWRAPPER_FILEFINDER_H_
#define _FSWRAPPER_FILEFINDER_H_

#include <string>
#include <vector>

#include <boost/filesystem.hpp>

namespace usadamasa {
namespace fswrapper {

class FileFinder {
 public:
  FileFinder(const std::string &basedir = ".");
  ~FileFinder() {}

  std::vector<std::string> FindRegularFiles(const std::string &basename, bool recursive = false) const;

 private:
  const std::string basedirname_;
  const boost::filesystem::path basedir_;
};
} // namespace fwrapper
} // namespace usadamasa
#endif //  _FSWRAPPER_FILEFINDER_H_
