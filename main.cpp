#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

int main(){

  std::string file_name;

  std::cout << "Enter the name of the file: ";
  std::cin >> file_name;

  std::ifstream input_file(file_name + ".txt");

  if(!input_file.is_open()){
    std::cout << "Error! File did not open successfully!\n\n";
    return -1;
  }

  std::string token = "";
  std::string temp = "";

  while(std::getline(input_file, token)){
    std::stringstream ss(token);
    while(ss >> temp)
    std::cout << temp << std::endl;
  }
  
  
  input_file.close();
  
  return 0;
}
