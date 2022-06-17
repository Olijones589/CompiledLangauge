#include <fstream>
#include <iostream>
#include <list>
#include <string>

std::string keywords[10] = {"if",    "else", "while",  "bool",  "int",
                            "float", "char", "string", "print", "input"};
const short unsigned int steps = 7;
unsigned short int ssv = 0;

void State(short int stepval) {
  std::cout << "Step " << stepval << " of " << steps << "..." << std::endl;
  ssv = stepval;
}

char chartype(char tf) {
  if (isdigit(tf)) {
    return 'd';
  } else if (isalpha(tf)) {
    return 'a';
  } else if (tf == ' ') {
    return 's';
  } else {
    return 'o';
  }
}

void Error(const std::string error_reason) {
  std::string errortype;
  if (ssv + 1 == 1) {
    errortype = "FileReader";
  } else if (ssv + 1 == 2) {
    errortype = "LexicalAnalyzer";
  } else if (ssv + 1 == 3) {
    errortype = "SemanticAnalyzer";
  } else if (ssv + 1 == 4) {
    errortype = "IntermediateCodeGenerator";
  } else if (ssv + 1 == 5) {
    errortype = "CodeOptimizer";
  } else if (ssv + 1 == 6) {
    errortype = "TargetCodeGenerator";
  }
  std::cout << errortype << " error: " << error_reason << std::endl;
  exit(EXIT_FAILURE);
}
// non perm
void print(std::list<std::string> const &list) {
  for (auto const &i : list) {
    std::cout << i << std::endl;
  }
}
// non perm
std::string FileReader(std::string filename) {
  std::ifstream frcore;
  frcore.open(filename);
  if (frcore.fail()) {
    Error("Could not open file with name " + filename);
  }
  // maximum program length 4294967295 characters if size_t=32
  std::string lines;
  std::string line;
  unsigned int count = 0;
  while (count < 3000) {
    getline(frcore, line);
    if (frcore.fail()) {
      break;
    };
    lines = lines + line;
    count++;
  }
  State(1);
  return lines;
};

const std::list<std::string> LexicalAnalyzer(std::string filedata) {
  std::list<std::string> tokens;
  std::string working = "";
  bool determined = false;
  bool lst_q = false;
  bool lst_i = false;
  char lst_t = 'e';
  // The order is: letters, numbers, spaces, then other symbols
  for (long unsigned fdp = 0; fdp < filedata.length(); fdp++) {
    // all in file
    char fdpv = filedata[fdp];
    char cct = chartype(fdpv);
    if (cct == 'a' && lst_i == false) {
      if (lst_t != cct && lst_q == false) {
        if (working != "") {
          tokens.push_back(working);
          working = "";
        }
      }

      working += fdpv;
      lst_q = false;
    } else if (cct == 'd' && lst_i == false) {
      if (lst_t != cct) {
        tokens.push_back(working);
        working = "";
      }
      working += fdpv;
      lst_q = false;
    } else {
      if (lst_t != cct  && lst_i == false) {
        tokens.push_back(working);
        working = "";
      }
      if (fdpv == '"' && lst_i == false) {
        working += fdpv;
        lst_q = true;
      } else if (fdpv == '/') {
        if (lst_i == true) {
          lst_i = false;
        } else {
          lst_i = true;
        }
      } else if(lst_i == false){
        if (working != "") {
          tokens.push_back(working);
          working = "";
        }
        working = fdpv;
        tokens.push_back(working);
        working = "";
        lst_q = false;
      }
    }
    if (fdp == filedata.length() && working != "") {
      tokens.push_back(working);
      working = "";
    }
    lst_t = cct;
  }
  State(2);
  return tokens;
};

int SyntaxAnalyzer(const std::list<std::string> tokens) {
  print(tokens);
  Error("This part of the compiler is not complete.");
  State(3);
  return 0;
};

int SemanticAnalyzer(int f) {
  State(4);
  return 0;
};

int IntermediateCodeGenerator(int f) {
  State(5);
  return 0;
};

int CodeOptimizer(int f) {
  State(6);
  return 0;
};

int TargetCodeGenerator(int f) {
  State(7);
  return 0;
};

void Build(std::string filename) {
  std::cout << std::endl
            << "Building " << filename << "..." << std::endl
            << std::endl;
  TargetCodeGenerator(CodeOptimizer(IntermediateCodeGenerator(SemanticAnalyzer(
      SyntaxAnalyzer(LexicalAnalyzer(FileReader(filename)))))));
}

int main(int argc, char **argv) {
  // Source code: https://github.com/Olijones589/DiviLang/new/main
  // For info on compilers:
  // https://www.geeksforgeeks.org/phases-of-a-compiler/
  if (argc > 1) {
    if (argc == 2) {
      Build(argv[1]);
    } else {
      std::cout << "argc > 2. Too many arguments." << std::endl;
    }
  } else {
    std::cout << "argv = 1. Include filename." << std::endl;
  }
}
