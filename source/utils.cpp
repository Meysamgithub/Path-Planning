#include "utils.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

namespace path_planning {

    namespace utils {

        ClipperLib::Path read_path_from_file(const std::string& file_path){
            ClipperLib::Path output_path; 

            std::vector<std::vector<std::string>> content;
	        std::vector<std::string> row;
            std::string line, word;

            std::fstream input_file(file_path);

            std::cout << file_path << "  " << input_file.is_open() << std::endl;
            
            if(input_file.is_open())
            {
                while(std::getline(input_file, line))
                {
                    row.clear();
        
                    std::stringstream str(line);
        
                    while(std::getline(str, word, ',')){
                        row.push_back(word);
                    } 

                    content.push_back(row);
                }
            }
            else
                std::cout << "Could not open the file\n";

            for(int i = 0; i < content.size(); i++)
            {
                for(int j = 0; j < content[i].size(); j++)
                {
                    std::cout << content[i][j] << " ";
                }
                std::cout << "\n";
            }

            return output_path; 
        }

    } // namespace utils


} // namespace path_planning