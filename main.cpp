#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <locale>
#include <ctype.h>

bool is_separator(char  input);
bool is_operator(char input);
bool is_keyword(std::string input);
std::string format_separators(std::string input);
std::string lexer(std::string token);
std::string strip_comments(std::string input, std::ifstream& stream);

int main(){

  std::string file_name;

  std::cout << "Enter the name of the file: ";
  std::cin >> file_name;

  std::ifstream input_file(file_name + ".txt");
  std::ofstream output_file("output.txt");

  if(!input_file.is_open()){
    std::cout << "Error! File did not open successfully!\n\n";
    return -1;
  }

  std::string token = "";
  std::string line = "";
  std::string temp = "";

  output_file << "\nTOKENS        Lexemes\n\n";

  while(std::getline(input_file, line)){
    std::string temp = line;
    int length = line.length();

    // strip out the comments
    for(unsigned i = 0; i < temp.length(); i++){
      if(temp[i] == '!'){
        // found the start of a comment
        temp = strip_comments(std::string(temp.begin() + i + 1, temp.end()), input_file);
        break;
      }
    }

    temp = format_separators(temp);

    std::stringstream ss(temp);
    while(ss >> token){ // parse the line into individual tokens to send to the lexer
      if(token.length() == 1){
        if(is_operator(token[0])){
          output_file << "OPERATOR      " << token << "\n";
        } else if(is_separator(token[0]) || token[0] == '.'){
          output_file << "SEPARATOR     " << token << "\n";
        }
      }
      else if(is_keyword(token)){
        output_file << "KEYWORD       " << token << "\n";
      } else {
        output_file << lexer(token);
      }
    }
    //std::cout << temp << std::endl;
  }

  input_file.close();

  return 0;
  // end main
}

bool is_separator(char input){
  const int NUM_SEPARATORS = 11;
  char separators[NUM_SEPARATORS] = {'(', ')', ',', ':', ';', '[', ']', '{', '}', '\''};

  for(int i = 0; i < NUM_SEPARATORS; i++){
    if(input == separators[i]) return true;
  }
  return false;
}

bool is_operator(char input){
  const int SIZE = 8;
  char operators[SIZE] = {'*', '+', '-', '=', '/', '>', '<', '%'};

  for(int i = 0; i < SIZE; i++){
    if(input == operators[i]) return true;
  }
  return false;
}
//int, float, bool, if, else, then, do, while, whileend, do, doend, for, and, or, function
bool is_keyword(std::string input){
  const int SIZE = 15;
  std::string keywords[SIZE] = {"int", "float", "bool", "if", "else", "then", "do", "while", "whileend", "do", "doend", "for", "and", "or", "function"};

  for(unsigned i = 0; i < SIZE; i++){
    if(input == keywords[i]) return true;
  }
  return false;
}

std::string lexer(std::string input){
  enum identifier_inputs { id_letter, id_digit, id_dollar, id_other};
  enum identifier_states { id_one, id_two, id_three };
  enum real_inputs { r_plus, r_minus, r_digit, r_other, r_period};
  enum real_states { r_one, r_two, r_three, r_four, r_five };
  enum integer_inputs { int_plus, int_minus, int_digit, int_other};
  enum integer_states {};
  
  identifier_states id_table[3][4] = {id_two, id_three, id_three, id_three, id_two, id_two, id_two, id_three, id_three, id_three, id_three, id_three};
  real_states rs_table[5][5] = { r_two, r_two, r_two, r_five, r_five, r_five, r_five, r_two, r_five, r_three, r_five, r_five, r_four, 
    r_five, r_five, r_five, r_five, r_four, r_five, r_five, r_five, r_five, r_five, r_five, r_five };
  integer_states int_table[0][0] = {};

  //std::cout << "checking token " << input << "\n\n";
  std::string result = "";
  identifier_states id_current_state = id_one;
  identifier_inputs id_current_input;
  
  for(unsigned i = 0; i < input.length(); i++){
    char temp = input[i];
    if(isalpha(temp)) id_current_input = id_letter;
    else if(isdigit(temp)) id_current_input = id_digit;
    else if(temp == '$') id_current_input = id_dollar;
    else id_current_input = id_other;
      
    id_current_state = id_table[id_current_state][id_current_input];
  }

  if(id_current_state == id_two){
    //found identifier
    result = "IDENTIFIER    " + input + "\n";
  } else { 
    //check if input is a real number
    real_states real_current_state = r_one;
    real_inputs real_current_input;
    
    for(unsigned i = 0; i < input.length(); i++){
      char temp = input[i];
      if(temp == '+') real_current_input = r_plus;
      else if(temp == '-') real_current_input = r_minus;
      else if(isdigit(temp)) real_current_input = r_digit;
      else if(temp == '.') real_current_input = r_period;
      else real_current_input = r_other;
      
      real_current_state = rs_table[real_current_state][real_current_input];
      
    }
    if(real_current_state == r_four) {
      result = "REAL NUMBER   " + input + "\n";
    } else {
      //check if input is an integer
    }
  }

  return result;
}

std::string format_separators(std::string input){
  for(unsigned i = 0; i < input.length(); i++){
    if(is_separator(input[i])){
      if(i == 0){
        input = std::string(" ") + input[0] + std::string(" ") + std::string(input.begin() + 1, input.end());
      } else if(i == input.length() - 1){
        input = std::string(input.begin(), input.begin() + i) + std::string(" ") + input[i] + " ";
      } else {
        std::string temp = std::string(input.begin() + i + 1, input.end());
        input = std::string(input.begin(), input.begin() + i) + " " + input[i] + " " + temp;
      }
      i += 2;
    }
  }
  return input;
}

std::string strip_comments(std::string input, std::ifstream& stream){
  // search for the next '!' that terminates the comments
  // then return the string that follows immediately after the '!'
  bool found = false;

  do{

    for(unsigned i = 0; i < input.length(); i++){
      if(input[i] == '!'){
        found = true;
        if(i == input.length() - 1) input = "";
        else {
          input = std::string(input.begin() + i + 1, input.end());
        }
        break;
      }
    }

    if(found) break;
  } while(std::getline(stream, input));
  
  if(!found) input = "";
  return input;

}
