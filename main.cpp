#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

bool is_separator(char input);

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
    // token represents a line fromthe source code 
    // we must now check for seperators and add whitespace before and after each separator
    
    unsigned length = token.length();
    
    for(unsigned i = 0; i < length; i++){
        if(is_separator(token[i])){
            if(i == length - 1){
                token.push_back(" ");
            } else {
                token.insert(i + 1, " ");
            }
            
            token.insert(i, " ");
            length += 2;
        }
    }
    
    std::stringstream ss(token);
    while(ss >> temp)
    std::cout << temp << std::endl;
  }
  
  
  input_file.close();
  
  return 0;
}

bool is_separator(char input){
    const unsigned NUM_SEPARATORS = 9;
    char separators[NUM_SEPARATORS] = {'(', ')', ',', ':', ';', '[', ']', '{', '}'};
    
    for(unsigned i = 0; i < NUM_SEPARATORS; i++){
        if(input == separators[i]) return true;
    }
    
    return false;
}

