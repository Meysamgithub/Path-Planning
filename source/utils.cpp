#include "utils.hpp"


namespace path_planning {

    namespace utils {

        ClipperLib::Paths import_paths_from_csv(const std::string& file_path){
            std::vector<std::vector<int>> content;
	        std::vector<int> row;
            std::string line, word;
            int number_of_obstacles = 0;  // placeholder to find the total number of obstacles

            std::fstream input_file(file_path);

            std::cout << file_path << "  " << input_file.is_open() << std::endl;
            
            if(input_file.is_open())
            {
                while(std::getline(input_file, line))
                {
                    row.clear();
        
                    std::stringstream str(line);
        
                    while(std::getline(str, word, ',')){
                        // multiplied by 1000 because we only care about 3 digit accuracy for now.
                        // also Clipper only works with integer numbers that's why we multiply our double numbers by 1000
                        // and then convert them to int. 
                        double double_word = std::stod(word) * 1000.0;  
                        row.push_back(int(double_word));
                    } 
                    // the third element of the row should not be multiplied by 1000 as it is the id of the field or obstacle
                    row[2] = row[2] / 1000; 

                    content.push_back(row);

                    // check the third column of the CSV file and if it's greater than 0 and if it changes it means there's a new obstacle in the csv file
                    if ( row[2] > number_of_obstacles ){
                        number_of_obstacles++;
                    }
                }
            }
            else
                std::cout << "Could not open the file\n";
            
            // create (number_of_obstacles + 1) Clipper paths to store the field and its obstacles
            ClipperLib::Paths boundaries(number_of_obstacles + 1);
            
            int boundary_id = 0;
            for(int i = 0; i < content.size(); i++)
            {
                if ( content[i][2] == boundary_id ) {
                    boundaries[boundary_id] << ClipperLib::IntPoint( content[i][0], content[i][1] );
                }
                else {
                    boundary_id++; 
                    boundaries[boundary_id] << ClipperLib::IntPoint( content[i][0], content[i][1] );
                }
            }

            return boundaries; 
        }


        void export_paths_to_csv(const std::string& file_name, ClipperLib::Paths paths){
            std::ofstream output_file;
            std::string output_file_name = file_name + ".csv";
            output_file.open (output_file_name);

            // loop over all paths
            for(int i = 0; i < paths.size(); i++){
                // loop over all points in a path and print them 
                for(int j = 0; j < paths[i].size(); j++){
                    // check if we're adding the last line of the csv and avoid adding an empty line in the end of the file
                    if ( i == paths.size()-1 && j == paths[i].size()-1 ){
                        output_file << paths[i][j].X / 1000.0 << "," << paths[i][j].Y / 1000.0 << "," << i;
                    }
                    else{
                        output_file << paths[i][j].X / 1000.0 << "," << paths[i][j].Y / 1000.0 << "," << i << "\n";
                    }
                }
            }
            output_file.close();
        }


        void print_clipper_paths(ClipperLib::Paths paths){
            // loop over all paths
            for(int i = 0; i < paths.size(); i++){
                std::cout << "________________\n" << "Path " << i << "\n";
                // loop over all points in a path and print them 
                for(int j = 0; j < paths[i].size(); j++){
                    std::cout << paths[i][j].X / 1000.0 << ", " << paths[i][j].Y / 1000.0 << "\n";
                }
            }
        }

        double distance_between_two_points(ClipperLib::IntPoint current_point, ClipperLib::IntPoint next_point){
            return std::hypot(( current_point.X- next_point.X), (current_point.Y - next_point.Y))/1000;
        }

        ClipperLib::IntPoint find_next_sanitized_point(double step_length, ClipperLib::IntPoint current_point, ClipperLib::IntPoint previous_reference_point, ClipperLib::IntPoint next_reference_point){
           ClipperLib::IntPoint sanitized_point(0,0);
            if (std::abs(previous_reference_point.X - next_reference_point.X) < distance_threshold) {
            // special case, when the line that connects the previous and next reference point is vertical
            sanitized_point.X = next_reference_point.X;
            double yub = fmax(current_point.Y, next_reference_point.Y);
            double ylb = fmin(current_point.Y, next_reference_point.Y);

        sanitized_point.Y
            = current_point.Y
                + sqrt(std::abs(pow(step_length, 2) - pow((next_reference_point.X - current_point.X), 2)));
                if ((sanitized_point.Y < ylb) || (sanitized_point.Y > yub)) {
                    sanitized_point.Y
                            = current_point.Y
                                    - sqrt(std::abs(pow(step_length, 2) - pow((next_reference_point.X - current_point.X), 2)));
                    }
                    } else {
             // The problem of finding the next sanitized point turns into a quadratic equation, that is solved here.
            // The variable names are picked based on the convention for a quadratic equation.
            double m = (next_reference_point.Y - previous_reference_point.Y)
                            / (next_reference_point.X - previous_reference_point.X);
            double c0 = previous_reference_point.Y - current_point.Y - (m * previous_reference_point.X);
            double a = 1 + pow(m, 2);
            double b = (2 * m * c0) - (2 * current_point.X);
            double c = pow(current_point.X, 2) + pow(c0, 2) - pow(step_length, 2);
            double delta = pow(b, 2) - (4 * a * c);
            if (delta < 0.0) {
                throw std::invalid_argument("No intersection point can be found.");
            } else if (delta < distance_threshold) {
            sanitized_point.X = -b / (2 * a);
            sanitized_point.Y
                    = m * (sanitized_point.X - previous_reference_point.X) + previous_reference_point.Y;
            } else {
            ClipperLib::IntPoint point_found_1(0,0);
            point_found_1.X = (-b + sqrt(delta)) / (2 * a);
            point_found_1.Y
                    = m * (point_found_1.X - previous_reference_point.X) + previous_reference_point.Y;
            ClipperLib::IntPoint point_found_2(0,0);
            point_found_2.X = (-b - sqrt(delta)) / (2 * a);
            point_found_2.Y
            = m * (point_found_2.X - previous_reference_point.X) + previous_reference_point.Y;
            if (distance_between_two_points(point_found_1, next_reference_point)
                        < distance_between_two_points(point_found_2, next_reference_point)) {
            sanitized_point = point_found_1;
            } else {
            sanitized_point = point_found_2;
                    }
                }
            }
        return sanitized_point;
        }

        ClipperLib::Path path_sanitizer(ClipperLib::Path user_defined_state_utm, double step_length){
        ClipperLib::Path sanitized_user_defined_state_utm;

        return sanitized_user_defined_state_utm;
        
        }

//         ClipperLib::Path sanitized_user_defined_state_utm;
//         ClipperLib::Path current_point = user_defined_state_utm.at(0);
//         sanitized_user_defined_state_utm.push_back(current_point);

//     // Sanitizing the position states (x, y) of the trajectory
//         int input_size = static_cast<int>(user_defined_state_utm.size());
//         for (int i = 0; i < (input_size - 1); i++) {
//         ClipperLib::Path next_reference_point = user_defined_state_utm.at(i + 1);
//         double dist = utils::distance_between_two_points(current_point.utm_coordinate, next_reference_point.utm_coordinate);
//         dist = dist - step_length;
//         ClipperLib::Path previous_reference_point = user_defined_state_utm.at(i);
//         current_point.heading_angle = previous_reference_point.heading_angle;
//         current_point.steering_angle = previous_reference_point.steering_angle;
//         current_point.type = previous_reference_point.type;
//         while (dist > utils::distance_threshold) {
//         current_point.utm_coordinate = utils::find_next_sanitized_point(
//         step_length,
//         current_point.utm_coordinate,
//         previous_reference_point.utm_coordinate,
//         next_reference_point.utm_coordinate
//       );
//       sanitized_user_defined_state_utm.push_back(current_point);
//       dist = utils::distance_between_two_points(current_point.utm_coordinate, next_reference_point.utm_coordinate);
//       dist = dist - step_length;
//     }
//   }
//   return sanitized_user_defined_state_utm;
// }

    } // namespace utils


} // namespace path_planning