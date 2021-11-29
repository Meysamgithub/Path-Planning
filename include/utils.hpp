#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include "clipper/clipper.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>


namespace path_planning {

    namespace utils {

        ClipperLib::Paths import_paths_from_csv(const std::string& file_path);

        void export_paths_to_csv(const std::string& file_name, ClipperLib::Paths paths);

        void print_clipper_paths(ClipperLib::Paths);

        ClipperLib::Path path_sanitizer(ClipperLib::Path , double);

        double distance_between_two_points(ClipperLib::IntPoint , ClipperLib::IntPoint);

        ClipperLib::IntPoint find_next_sanitized_point(double, ClipperLib::IntPoint, ClipperLib::IntPoint, ClipperLib::IntPoint);

        double intpoint_to_double(int);

        constexpr double distance_threshold = 0.0001;  //!< To compare the distance between two points we need to define a
                                               //!< threshold to consider the points different or the same (unit: meter)

        constexpr double PRECISION = 1000.0;

    } // namespace utils


} // namespace path_planning

#endif  // __UTILS_H__