#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

bool is_separator(char input);
void lexer(std::string token);

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
  std::string line = "";

  while(std::getline(input_file, line)){
    // token represents a line fromthe source code 
    // we must now check for seperators and add whitespace before and after each separator
    std::string test = line;
    int length = line.length();

    
    // reformat the string with spaced separators!
    for(unsigned i = 0; i < test.length(); i++){
      
      if(is_separator(test[i])){
	if(i == 0){
	  test = std::string(" ") + test[0] + std::string(" ") + std::string(test.begin() + 1, test.end());
	} else if(i == length - 1){
	  test = std::string(test.begin(), test.begin() + i) + std::string(" ") + test[i] + " ";
	} else {
	  std::string temp = std::string(test.begin() + i + 1, test.end());
	  test = std::string(test.begin(), test.begin() + i) + " " + test[i] + " " + temp;
	}
	i += 2;
      }
    }
    
    // at this point the separators have been properly separated from their neighboring characters!
    // now we strip comments, and then we send the newly formatted string to the lexer!


    // strip out the comments!

    for(unsigned i = 0; i < test.length(); i++){
      if(test[i] == '!'){
	// found the start of a comment!
	test = std::string(test.begin(), test.begin() + i);
      }
    }

    // now separators have been reformatted and comments have been stripped!
    // we can send the tokens in the line to the lexer function!

    stringstream ss(test);
    while(ss >> token){ // parse the line into individual tokens to send to the lexer
      lexer(token); 
    }
    
    std::cout << test << std::endl;
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

void lexer(std::string input){

}
