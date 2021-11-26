#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include "clipper/clipper.hpp"

namespace path_planning {

    namespace utils {

        ClipperLib::Path read_path_from_file(const std::string& file_path);

    } // namespace utils


} // namespace path_planning

#endif  // __UTILS_H__