#include "utils.hpp"

int main() { 

    const std::string example_csv_file = "/home/meysamghomi/Desktop/Repositories/path_planning/build/standalone/meysam.csv";

    path_planning::utils::read_path_from_file(example_csv_file);

    return 0; 

}
