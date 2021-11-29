#include "utils.hpp"


int main() { 

    const std::string example_csv_file = "square_field_example.csv";

    ClipperLib::Paths imported_paths = path_planning::utils::import_paths_from_csv(example_csv_file);

    // path_planning::utils::print_clipper_paths(imported_paths);

    path_planning::utils::export_paths_to_csv("exported_field", imported_paths);


    ClipperLib::Paths solution;

    ClipperLib::ClipperOffset co;
    co.AddPath(imported_paths[0], ClipperLib::jtRound, ClipperLib::etClosedPolygon);
    co.Execute(solution, -100);
    // ClipperLib::IntPoint first_point(0,0);
    // ClipperLib::IntPoint second_point(1,1);

    ClipperLib::IntPoint first_point = imported_paths[0][0];
    ClipperLib::IntPoint second_point = imported_paths[0][2];

    double d;
    d = path_planning::utils::distance_between_two_points(first_point, second_point);

    std::cout << "The distance between x and y is " << d << std::endl;
    double step_length = 2;
    ClipperLib::IntPoint current_point(1,0);
    ClipperLib::IntPoint previous_reference_point(0,0);
    ClipperLib::IntPoint next_reference_point(10,0);
    ClipperLib::IntPoint next_sanitized_point_1;
    next_sanitized_point_1 = path_planning::utils::find_next_sanitized_point(step_length, current_point, previous_reference_point, next_reference_point);
    std::cout << next_sanitized_point_1.X << std::endl;
    
    path_planning::utils::export_paths_to_csv("field_headland_1", solution);

    return 0; 

}
