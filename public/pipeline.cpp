#include<bits/stdc++.h>
using namespace std;


ofstream traceOut("output.txt");
ofstream statsOut("stats.txt");
ofstream web2("web2.txt");
ofstream web3("web3.txt");



unordered_map<string, int> dataTypeSize = {
    {".byte", 1},
    {".half", 2},
    {".word", 4},
    {".dword", 8},
    {".asciiz", 1},
    {".string" , 1}
};

// Op Codes Map
unordered_map<string, string> R_opcodeTable = {
    // R_Type
    {"add", "0110011"},
    {"and", "0110011"},
    {"or", "0110011"},
    {"sll", "0110011"},
    {"slt", "0110011"},
    {"sra", "0110011"},
    {"srl", "0110011"},
    {"sub", "0110011"},
    {"xor", "0110011"},
    {"mul", "0110011"},
    {"div", "0110011"},
    {"rem", "0110011"},
};

unordered_map<string, string> I_opcodeTable = {
    // I_Type
    {"addi", "0010011"},
    {"andi", "0010011"},
    {"ori", "0010011"},
    {"lb", "0000011"},
    {"ld", "0000011"},
    {"lh", "0000011"},
    {"lw", "0000011"},
    {"jalr", "1100111"},
    {"slti", "0010011"},
    {"sltiu", "0010011"},
    {"xori", "0010011"},
    {"lbu", "0000011"},
    {"lhu", "0000011"},
};
unordered_map<string, string> I_opcode_1{
  
    
    {"slli", "0010011"},
    {"srli", "0010011"},
    {"srai", "0010011"},
    
  

}
;
unordered_map<string, string> S_opcodeTable = {
    // S_Type
    {"sb", "0100011"},
    {"sw", "0100011"},
    {"sd", "0100011"},
    {"sh", "0100011"},
};

unordered_map<string, string> SB_opcodeTable = {
    // SB_Type
    {"beq", "1100011"},
    {"bne", "1100011"},
    {"bge", "1100011"},
    {"blt", "1100011"},
    
};

unordered_map<string, string> U_opcodeTable = {
    // U_Type
    {"auipc", "0010111"},
    {"lui", "0110111"},
};

unordered_map<string, string> UJ_opcodeTable = {
    // UJ_Type
    {"jal", "1101111"}};

// func3 map
unordered_map<string, string> func3Map = {
    // R-Type
    {"add", "000"},
    {"and", "111"},
    {"or", "110"},
    {"sll", "001"},
    {"slt", "010"},
    {"sra", "101"},
    {"srl", "101"},
    {"sub", "000"},
    {"xor", "100"},
    {"mul", "000"},
    {"div", "100"},
    {"rem", "110"},

    // I_Type
    {"addi", "000"},
    {"andi", "111"},
    {"ori", "110"},
    {"lb", "000"},
    {"ld", "011"},
    {"lh", "001"},
    {"lw", "010"},
    {"jalr", "000"},
    {"slti", "010"},
    {"sltiu", "011"},
    {"xori", "100"},
    {"lbu", "100"}, 
    {"lhu", "101"},
    
    // I new
    
  
    {"slli", "001"},
    {"srli", "101"},
    {"srai", "101"},
    



    // S_Type
    {"sb", "000"},
    {"sw", "010"},
    {"sd", "011"},
    {"sh", "001"},

    // SB_Type
    {"beq", "000"},
    {"bne", "001"},
    {"bge", "101"},
    {"blt", "100"},
    
};

// func7 map
unordered_map<string, string> func7Map = {
    // R-Type
    {"add", "0000000"},
    {"and", "0000000"},
    {"or", "0000000"},
    {"sll", "000000"},
    {"slt", "000000"},
    {"sra", "0100000"},
    {"srl", "0000000"},
    {"sub", "0100000"},
    {"xor", "0000000"},
    {"mul", "0000001"},
    {"div", "0000001"},
    {"rem", "0000001"},
    

    // I new Type
    {"slli", "0000000"},
    {"srli", "0000000"},
    {"srai", "0100000"}, 
    
   

};

// Risc-V Directives
class Directives
{
    int TEXT, DATA, BYTE, HALF,
        WORD, DWORD, ASCIZ;
};

// Inherting all instructions into a single class
class RISC_V_Instructions
{
    // To Make elements available for public
public:
    int rd = INT_MIN, rs1 = INT_MIN, rs2 = INT_MIN, imm = INT_MIN, type = 1;
    string OpCode = "NONE", Instruction = "NONE", func3 = "NONE", func7 = "NONE";

    void printInstruction()
    {
        traceOut << "OpCode: " << OpCode << endl;
        traceOut << "rd: " << rd << endl;
        traceOut << "rs1: " << rs1 << endl;
        traceOut << "rs2: " << rs2 << endl;
        traceOut << "func3: " << func3 << endl;
        traceOut << "func7: " << func7 << endl;
        traceOut << "Imm:" << imm <<endl;
    }
};

// Function Definations
const bool check_Derivative(const string &line)
{
    return line[0] == '.';
}


// Function to remove leading and trailing spaces from a string
const string trim(const string &str) {
    try {
        size_t first_non_space = 0;
        while (first_non_space < str.length() && isspace(static_cast<unsigned char>(str[first_non_space]))) {
            ++first_non_space;
        }

        if (first_non_space == str.length()) {
            return "";
        }

        size_t last_non_space = str.length() - 1;
        while (last_non_space > first_non_space && isspace(static_cast<unsigned char>(str[last_non_space]))) {
            --last_non_space;
        }

        return str.substr(first_non_space, last_non_space - first_non_space + 1);
    } catch (const exception& e) {
        traceOut << "error in trim: " << e.what() << endl;
        return "";
    }
}


// Check if string is integer (including negative numbers)
const bool isNumber(string s) {
    if (s.length() == 0) {
        return false;
    }

    size_t start = 0;
    bool result = true;

    if (s.at(0) == '-') {
        start = 1;
    }

    if (start == s.length()) {
        return false;
    }

    for (size_t i = start; i < s.length(); ++i) {
        char ch = s[i];
        if (!isdigit(static_cast<unsigned char>(ch))) {
            result = false;
            break;
        }
    }

    return result;
}


// function to convert decimal to hexadecimal
string decToHexa(long long n) {
    try {
        string ans;
        
        if (n == 0) {
            return "0x0";
        }

        bool negative = false;
        if (n < 0) {
            negative = true;
            n = -n;
        }

        while (n > 0) {
            int digit = static_cast<int>(n % 16);
            char hexChar;
            if (digit < 10) {
                hexChar = '0' + digit;
            } else {
                hexChar = 'A' + (digit - 10);
            }
            ans.insert(ans.begin(), hexChar);
            n = n / 16;
        }

        if (ans.empty()) {
            ans = "0";
        }

        if (negative) {
            ans.insert(0, "-0x");
        } else {
            ans.insert(0, "0x");
        }

        return ans;
    } catch (const exception& e) {
        traceOut << "error in decToHexa: " << e.what() << endl;
        return "0x0";
    }
}


// Function to create map between binary number and its equivalent hexadecimal 
void Map_Creation(unordered_map<string, char> *um) {
    string keys[] = {
        "0000", "0001", "0010", "0011",
        "0100", "0101", "0110", "0111",
        "1000", "1001", "1010", "1011",
        "1100", "1101", "1110", "1111"
    };

    char values[] = {
        '0', '1', '2', '3',
        '4', '5', '6', '7',
        '8', '9', 'A', 'B',
        'C', 'D', 'E', 'F'
    };

    for (int i = 0; i < 16; ++i) {
        um->insert(make_pair(keys[i], values[i]));
    }
}


// function to find hexadecimal equivalent of binary 
string Binary_to_HEx(string bin) {
    try {
        if (bin.length() == 0) {
            return "0x0";
        }

        // Ensure length is a multiple of 4 by prepending zeros
        int padding = 4 - (bin.length() % 4);
        if (padding != 4) {
            bin.insert(0, padding, '0');
        }

        unordered_map<string, char> bin_hex_map;
        Map_Creation(&bin_hex_map);

        string hex;
        for (size_t i = 0; i < bin.length(); i += 4) {
            string segment = bin.substr(i, 4);
            auto it = bin_hex_map.find(segment);
            if (it != bin_hex_map.end()) {
                hex.push_back(it->second);
            } else {
                throw runtime_error("Invalid binary digit sequence");
            }
        }

        return "0x" + (hex.empty() ? "0" : hex);
    } catch (const exception& e) {
        traceOut << "error in Binary_to_HEx: " << e.what() << endl;
        return "0x0";
    }
}

// function to convert decimal to binary with specified length
string decToBinary_Len(long long n, int len) {
    try {
        string binary(len, '0');
        bool isNegative = (n < 0);

        if (isNegative) {
            unsigned long long limit = (1ULL << len);
            n = limit + n; // two's complement
        }

        for (int pos = len - 1; pos >= 0 && n > 0; --pos) {
            if (n & 1) {
                binary[pos] = '1';
            }
            n >>= 1;
        }

        if (binary.length() > static_cast<size_t>(len)) {
            binary = binary.substr(binary.length() - len);
        }

        return binary;
    } catch (const exception& e) {
        traceOut << "error in decToBinary_Len: " << e.what() << endl;
        return string(len, '0');
    }
}

// Enum to have error code
enum ErrorType
{
    ERROR_NONE = 0x00,              // No error
    ERROR_SYNTAX = 0x01,            // Syntax is wrong
    INVALID_INSTRUCTION = 0x02,     // Instruction is not valid
    register_notvalid = 0x03,        // Register is invalid
    immediate_notvalid = 0x04, // Imm Value is invalid
    INVALID_DATA = 0x05,            // Invalid Data
    INVALID_LABEL = 0x06            // Branch Target is invalid
};

// Defining Structure of Return Errors
class error
{
private:
    // Getting Type of error
    ErrorType err;
    // String error message
    string msg;

public:
    // A Constructor
    error(ErrorType error, string message)
    {
        err = error;
        msg = message;
    }
    // To Alter error
    
    void print_error()
    {
        traceOut << "Code is exited with error code " << err << ". \"" << msg << "\"";
    }

    void check_error(ErrorType error, string message)
    {
        err = error;
        msg = message;
    }
};
int immediate_calculation(const string &hex, int width, bool is_signed, error *output_error)
{
    // Convert hex or decimal string to decimal value, checking against specified bit width
    string hexa_string = hex;
   
    int decim = 0;

    // Determine range based on bit width and signed/unsigned
    long long maxim = (1LL << (width - (is_signed ? 1 : 0))) - 1; // 2^(n-1) - 1 or 2^n - 1
    long long minim = is_signed ? -(1LL << (width - 1)) : 0;     // -2^(n-1) or 0

    if (hexa_string.substr(0, 2) != "0x")
    {
        // Assume decimal input
        try
        {
            decim = stoi(hexa_string);
            if (decim >= minim && decim <= maxim)
                return decim;
            else
            {
                string errorMsg = "Typed IMMEDIATE_VALUE is invalid. Out of limit (" + 
                                  to_string(minim) + ", " + to_string(maxim) + ")";
                (*output_error).check_error(immediate_notvalid, errorMsg);
                (*output_error).print_error();
                exit(immediate_notvalid);
                return INT_MIN;
            }
        }
        catch (const exception &e)
        {
            (*output_error).check_error(immediate_notvalid, "Typed IMMEDIATE_VALUE is invalid. Invalid decimal format");
            (*output_error).print_error();
            exit(immediate_notvalid);
            return INT_MIN;
        }
    }
    else if (hexa_string.substr(0, 2) == "0x")
    {
        hexa_string = hexa_string.substr(2);
        decim = 0;
        int power = hexa_string.length() - 1;
        for (char c : hexa_string)
        {
            int digit;
            if (c >= '0' && c <= '9')
                digit = c - '0';
            else if (c >= 'a' && c <= 'f')
                digit = c - 'a' + 10;
            else if (c >= 'A' && c <= 'F')
                digit = c - 'A' + 10;
            else
            {
                (*output_error).check_error(immediate_notvalid, "Typed IMMEDIATE_VALUE is invalid. Invalid hex letters");
                (*output_error).print_error();
                exit(immediate_notvalid);
                return INT_MIN;
            }
            decim += digit * pow(16, power);
            power--;
        }
        // Check bit width (number of hex digits <= ceil(width / 4))
        if (hexa_string.length() > (width + 3) / 4)
        {
            (*output_error).check_error(immediate_notvalid, "Typed IMMEDIATE_VALUE exceeds " + 
                                       to_string(width) + "-bit limit");
            (*output_error).print_error();
            exit(immediate_notvalid);
            return INT_MIN;
        }
        if (decim >= minim && decim <= maxim)
            return decim;
        else
        {
            string errorMsg = "Typed IMMEDIATE_VALUE is invalid. Out of limit (" + 
                              to_string(minim) + ", " + to_string(maxim) + ")";
            (*output_error).check_error(immediate_notvalid, errorMsg);
            (*output_error).print_error();
            exit(immediate_notvalid);
            return INT_MIN;
        }
    }
    else
    {
        (*output_error).check_error(immediate_notvalid, "Typed IMMEDIATE_VALUE is invalid. Neither Hex nor Integer");
        (*output_error).print_error();
        exit(immediate_notvalid);
        return INT_MIN;
    }
}
const int normal_parameter(const string &given_parameter, error *output_error)
{
    int register_number;
    if (given_parameter[0] == 'x')
    {
        // Try catch if string it not fully integer
        try
        {
            if (isNumber(given_parameter.substr(1, given_parameter.length() - 1)))
            {
                // Getting register number
                register_number = stoi(given_parameter.substr(1, given_parameter.length() - 1));
                // Throwing exception if register number is > 31
                if (register_number > 31)
                    throw invalid_argument("");
                // Reading if it is valid
                return register_number;
            }
            else
                throw invalid_argument("");
        }
        catch (exception e)
        {
            // Altering error to invalid register
            (*output_error).check_error(register_notvalid, "Typed Register is invalid");
            (*output_error).print_error();
            // Exiting with error code
            exit(register_notvalid);
        }
    }
    else
    {
        // Altering error to invalid register
        (*output_error).check_error(register_notvalid, "Typed Register is invalid");
        (*output_error).print_error();
        // Exiting with error code
        exit(register_notvalid);
    }
}

void Bracketed_Immediate_Parameter(RISC_V_Instructions *Current_Instruction,
    const string &given_parameter, error *output_error){
// Lambdas to report errors and exit
    auto immediate_error = [&](const string &val) {
        output_error->check_error(immediate_notvalid,
        "Immediate Value " + val + " is invalid");
        output_error->print_error();
        exit(immediate_notvalid);
    };
    auto register_error = [&]() {
        output_error->check_error(register_notvalid,
        "Typed Register is invalid");
        output_error->print_error();
        exit(register_notvalid);
    };

    string temp_word;
    int register_number;

    size_t position_open  = given_parameter.find('(');
    size_t position_closed = given_parameter.find(')');
    if (position_open == string::npos || position_closed == string::npos || position_closed <= position_open) {
        immediate_error("");
    }

    temp_word = trim(given_parameter.substr(0, position_open));
    bool hexFormat = (temp_word.length() >= 2 && temp_word.substr(0,2) == "0x");
    if (isNumber(temp_word) || hexFormat) {
        Current_Instruction->imm =
        immediate_calculation(temp_word, 12, true, output_error);
    } 
    else {
        immediate_error(temp_word);
    }

// --- Parse register part ---
    temp_word = trim(
    given_parameter.substr(position_open + 1, position_closed - position_open - 1)
    );
    if (temp_word.empty() || temp_word[0] != 'x') {
        register_error();
    }

    string regStr = temp_word.substr(1);
    if (!isNumber(regStr)) {
        register_error();
    }

    register_number = stoi(regStr);
    if (register_number < 0 || register_number > 31) {
        register_error();
    }

    Current_Instruction->rs1 = register_number;
}


int label_offset(unordered_map<string, long long> labels_PC, const string &label, long long program_counter, error *output_error)
{
    if (labels_PC.find(label) != labels_PC.end())
    {
        return (labels_PC[label] - program_counter );
    }
    else
    {
        (*output_error).check_error(INVALID_LABEL, "Typed Branch Target is invalid");
        (*output_error).print_error();
        // Exiting with error code
        exit(INVALID_LABEL);
        return -1;
    }
}


// Function to check if it a valid instruction
const RISC_V_Instructions InitializeInstruction(const unordered_map<string, long long> labels_PC,  string &inst, error *output_error, int program_counter )
{
    RISC_V_Instructions Current_Instruction;
    Current_Instruction.Instruction = inst;
    size_t comment_pos = inst.find('#');
    if (comment_pos != string::npos) {
        inst = inst.substr(0, comment_pos); // Keep only the part before '#'
    }
    inst = trim(inst); 
  
    // String stream to get word by word
    stringstream ss(inst);
    // Just some varibles for specific purpose
    string temp_word = "";
    int register_number = -1, values = 0;
    // Character by Character
    // Getting first word
    ss >> temp_word;
    // This is for R-type instructions
    if (R_opcodeTable.find(temp_word) != R_opcodeTable.end())
    {
        // func3 and func7 Initialization
        Current_Instruction.func3 = func3Map[temp_word];
        Current_Instruction.func7 = func7Map[temp_word];
        Current_Instruction.type = 1;
        // Reading Op Code for the instruction
        Current_Instruction.OpCode = R_opcodeTable[temp_word];
        // Getting rest of the values from the instruction
        while (!ss.eof()) // Run file end of instruction
        {
            values++;
            // Spliting and reading word by delimiter as comma
            getline(ss, temp_word, ',');
            
            // Removing trailing and leading spaces
            temp_word = trim(temp_word);

            // Reading if it is valid
            if (values == 1)
                Current_Instruction.rd = normal_parameter(temp_word, output_error);
            else if (values == 2)
                Current_Instruction.rs1 = normal_parameter(temp_word, output_error);
            else if (values == 3)
                Current_Instruction.rs2 = normal_parameter(temp_word, output_error);
            else
            {
                // Altering error to invalid Syntax
            
                (*output_error).check_error(ERROR_SYNTAX, "Typed Syntax is invalid");
                (*output_error).print_error();
                // Exiting with error code
                exit(ERROR_SYNTAX);
            }
        }
    }
    // I Type
    else if (I_opcodeTable.find(temp_word) != I_opcodeTable.end())
    {
        // Initializing func3
        Current_Instruction.func3 = func3Map[temp_word];
        Current_Instruction.type = 2;
        // Reading Op Code for the instruction
        Current_Instruction.OpCode = I_opcodeTable[temp_word];
        // Getting rest of the values from the instruction
        while (!ss.eof()) // Run file end of instruction
        {
            values++;
            // Spliting and reading word by delimiter as comma
            getline(ss, temp_word, ',');
            // Removing trailing and leading spaces
            temp_word = trim(temp_word);
            // Reading register value
            // If lw, ld, lh, lb
            if (Current_Instruction.OpCode == "0000011" && values == 2)
                Bracketed_Immediate_Parameter(&Current_Instruction, temp_word, output_error);
            // addi, andi, ori, jalr
            else if (temp_word[0] == 'x' && values != 3)
            {
                // Reading if it is valid
                if (values == 1)
                    Current_Instruction.rd = normal_parameter(temp_word, output_error);
                else if (values == 2)
                    Current_Instruction.rs1 = normal_parameter(temp_word, output_error);
            }
            else if (values == 3)
            {
                // Storing Immediate value
                if(temp_word[temp_word.length() - 1] == 'u'){
                    Current_Instruction.imm = immediate_calculation(temp_word,12 , false ,output_error);

                }
              else{
                Current_Instruction.imm = immediate_calculation(temp_word,12 , true ,output_error);
              }
            }
            else
            {
                // Altering error to invalid Syntax
               
                (*output_error).check_error(ERROR_SYNTAX, "Typed Syntax is invalid");
                (*output_error).print_error();
                // Exiting with error code
                exit(ERROR_SYNTAX);
            }
        }
    }
    // S Type
    else if (S_opcodeTable.find(temp_word) != S_opcodeTable.end())
    {
        Current_Instruction.func3 = func3Map[temp_word];
        Current_Instruction.type = 3;
        // Reading Op Code for the instruction
        Current_Instruction.OpCode = S_opcodeTable[temp_word];
        while (!ss.eof()) // Run file end of instruction
        {
            values++;
            // Spliting and reading word by delimiter as comma
            getline(ss, temp_word, ',');
            // Removing trailing and leading spaces
            temp_word = trim(temp_word);
            // Reading register value
            if (temp_word[0] == 'x' && values != 2)
            {
                if (values == 1)
                    Current_Instruction.rs2 = normal_parameter(temp_word, output_error);
            }
            else if (values == 2)
                Bracketed_Immediate_Parameter(&Current_Instruction, temp_word, output_error);
            else
            {
               
                (*output_error).check_error(ERROR_SYNTAX, "Typed Syntax is invalid");
                (*output_error).print_error();
                // Exiting with error code
                exit(ERROR_SYNTAX);
            }
        }
    }
    // SB Type
    else if (SB_opcodeTable.find(temp_word) != SB_opcodeTable.end())
    {
        // Initializing func3
        Current_Instruction.func3 = func3Map[temp_word];
        Current_Instruction.type = 4;
        // Reading Op Code for the instruction
        Current_Instruction.OpCode = SB_opcodeTable[temp_word];
        // Getting rest of the values from the instruction
        while (!ss.eof()) // Run file end of instruction
        {
            values++;
            // Spliting and reading word by delimiter as comma
            getline(ss, temp_word, ',');
            // Removing trailing and leading spaces
            temp_word = trim(temp_word);

            // Reading if it is valid
            if (values == 1)
                Current_Instruction.rs1 = normal_parameter(temp_word, output_error);
            else if (values == 2)
                Current_Instruction.rs2 = normal_parameter(temp_word, output_error);
            else if (values == 3){
                getline(ss, temp_word, '#');
                temp_word = trim(temp_word);
                Current_Instruction.imm = label_offset(labels_PC, temp_word, program_counter, output_error);
            }
            else
            {
                // Altering error to invalid Syntax
                (*output_error).check_error(ERROR_SYNTAX, "Typed Syntax is invalid");
                (*output_error).print_error();
                // Exiting with error code
                exit(ERROR_SYNTAX);
            }
        }
    }
    // U Type
    else if (U_opcodeTable.find(temp_word) != U_opcodeTable.end())
    {
        // Reading Op Code for the instruction
        Current_Instruction.OpCode = U_opcodeTable[temp_word];
        Current_Instruction.type = 5;

        while (!ss.eof()) 
        {
            values++;
            // Spliting and reading word by delimiter as comma
            getline(ss, temp_word, ',');
            // Removing trailing and leading spaces
            temp_word = trim(temp_word);
            if (values == 1)
                Current_Instruction.rd = normal_parameter(temp_word, output_error);
            else if (values == 2)
            {
                if (temp_word.substr(0, 2) == "0x")
                    Current_Instruction.imm = immediate_calculation(temp_word,20 , false, output_error);
                else if (stoi(temp_word) <= 1048576)
                    Current_Instruction.imm = stoi(temp_word);
                else
                {
                    // Altering error to invalid IMMEDIATE_VALUE
                   
                    (*output_error).check_error(immediate_notvalid, "Typed Immediate value is invalid");
                    (*output_error).print_error();
                    // Exiting with error code
                    exit(immediate_notvalid);
                }
            }

            else
            {
                // Altering error to invalid Syntax
            
                (*output_error).check_error(ERROR_SYNTAX, "Typed Syntax is invalid");
                (*output_error).print_error();
                // Exiting with error code
                exit(ERROR_SYNTAX);
            }
        }
    }
    else if (UJ_opcodeTable.find(temp_word) != UJ_opcodeTable.end())
    {
        // Reading Op Code for the instruction
        Current_Instruction.OpCode = UJ_opcodeTable[temp_word];
        Current_Instruction.type = 6;
        while (!ss.eof()) // Run file end of instruction
        {
            values++;
            // Spliting and reading word by delimiter as comma
            getline(ss, temp_word, ',');
            // Removing trailing and leading spaces
            temp_word = trim(temp_word);

            // Reading if it is valid
            if (values == 1)
                Current_Instruction.rd = normal_parameter(temp_word, output_error);
            else if (values == 2){
                Current_Instruction.imm = label_offset(labels_PC, temp_word, program_counter, output_error);
            }
            else
            {
                (*output_error).check_error(ERROR_SYNTAX, "Typed Syntax is invalid");
                (*output_error).print_error();
                exit(ERROR_SYNTAX);
            }
        }
    }
    else if (I_opcode_1.find(temp_word) != I_opcode_1.end()) { 
        Current_Instruction.func3 = func3Map[temp_word];
        Current_Instruction.type = 7;
        Current_Instruction.OpCode = I_opcode_1[temp_word];
        if (temp_word == "slli" || temp_word == "srli"){
            Current_Instruction.func7 = "0000000";
        } else if (temp_word == "srai") {
            Current_Instruction.func7 = "0100000";
        }

        while (!ss.eof()){
            values++;
            getline(ss, temp_word, ',');
            temp_word = trim(temp_word);
            if (values == 1)
                Current_Instruction.rd = normal_parameter(temp_word, output_error);
            else if (values == 2)
                Current_Instruction.rs1 = normal_parameter(temp_word, output_error);
            else if (values == 3) {
                Current_Instruction.imm = immediate_calculation(temp_word, 5, false, output_error);
                if (Current_Instruction.imm < 0 || Current_Instruction.imm > 31) {
                    (*output_error).check_error(immediate_notvalid, "Shift amount '" + temp_word + "' must be 0-31");
                    (*output_error).print_error();
                    exit(immediate_notvalid);
                }
            }
            }
    }
    else{
        (*output_error).check_error(INVALID_INSTRUCTION, "Typed Instruction " + temp_word + " is invalid");
        (*output_error).print_error();
        exit(INVALID_INSTRUCTION);
    }
    return Current_Instruction;
}

// Function to sign-extend a value
int32_t sign_extend(uint32_t value, int bits) {
    int32_t mask = 1 << (bits - 1);
    return (value ^ mask) - mask;
}

using namespace std;
string to_hex(uint32_t val) {
    stringstream ss;
    ss << "0x" << setfill('0') << setw(8) << hex << uppercase << val;
    return ss.str();
}

// Configuration knobs
struct Knobs {
    bool enable_pipelining = true;
    bool enable_data_forwarding = true;
    bool print_reg_file = false;
    bool print_pipeline_regs = false;
    int trace_instruction = -1;
    bool print_branch_predictor = false;
} knobs;

// Statistics
struct Stats {
    int total_cycles = 0;
    int total_instructions = 0;
    int data_transfer_instructions = 0;
    int alu_instructions = 0;
    int control_instructions = 0;
    int stall_count = 0;
    int data_hazards = 0;
    int control_hazards = 0;
    int branch_mispredictions = 0;
    int stalls_data_hazards = 0;
    int stalls_control_hazards = 0;
    double get_cpi() const {
        return total_instructions > 0 ? static_cast<double>(total_cycles) / total_instructions : 0;
    }
} stats;

// Control signals
struct Control {
    bool mem_read = false;
    bool mem_write = false;
    bool reg_write = false;
    bool branch = false;
    bool use_imm = false;
    int output_sel = 0; 
    string alu_op = "";
    string mem_size = "";
    bool is_nop = false;
};

// Pipeline Registers
struct IF_ID_Register {
    uint32_t pc = 0;
    uint32_t ir = 0;
    bool is_valid = false;
    int instruction_num = 0;
};

struct ID_EX_Register {
    uint32_t pc = 0;
    uint32_t ir = 0;
    int32_t reg_a_val = 0;
    int32_t reg_b_val = 0;
    int32_t imm = 0;
    uint32_t rs1 = 0;
    uint32_t rs2 = 0;
    uint32_t rd = 0;
    Control ctrl;
    bool is_valid = false;
    int instruction_num = 0;
};

struct EX_MEM_Register {
    uint32_t pc = 0;
    int32_t alu_result = 0;
    int32_t rs2_val = 0;
    uint32_t rd = 0;
    Control ctrl;
    bool is_valid = false;
    int instruction_num = 0;
};

struct MEM_WB_Register {
    uint32_t pc = 0;
    int32_t write_data = 0;
    uint32_t rd = 0;
    Control ctrl;
    bool is_valid = false;
    int instruction_num = 0;
};

// Global simulation state
uint32_t pc = 0;
array<int32_t, 32> reg_file = {0};
vector<pair<uint32_t, uint32_t>> text_memory;
vector<pair<uint32_t, int32_t>> data_memory;
const int MAX_CYCLES = 10000;
int instruction_count = 0;
bool program_done = false;

// Pipeline registers
IF_ID_Register if_id;
ID_EX_Register id_ex;
EX_MEM_Register ex_mem;
MEM_WB_Register mem_wb;

// Hazard/stall controls
bool stall_pipeline = false;

class BranchPredictor {
    private:
        struct BTBEntry {
            uint32_t pc;
            uint32_t target;
            bool valid;
            bool prediction; // 1-bit predictor: 0 (not taken), 1 (taken)
        };
        vector<BTBEntry> btb;
        uint32_t btb_size;
    
    public:
        BranchPredictor(uint32_t btb_s) : btb_size(btb_s) {
            btb.resize(btb_size, {0, 0, false, false}); // Initialize: invalid, predict not taken
        }
    
        bool predict(uint32_t pc) {
            uint32_t index = (pc >> 2) % btb_size;
            if (btb[index].valid && btb[index].pc == pc) {
                bool predicted_taken = btb[index].prediction;
                traceOut << "Predict: PC=" << to_hex(pc) << ", BTB hit, Prediction=" << predicted_taken << "\n";
                return predicted_taken;
            }
            traceOut << "Predict: PC=" << to_hex(pc) << ", No BTB entry, predict not taken\n";
            return false; // Default: predict not taken
        }
    
        uint32_t get_target(uint32_t pc) {
            uint32_t index = (pc >> 2) % btb_size;
            if (btb[index].valid && btb[index].pc == pc) {
                return btb[index].target;
            }
            return pc + 4; // Default: next instruction
        }
    
        void update(uint32_t pc, bool taken, uint32_t target) {
            uint32_t index = (pc >> 2) % btb_size;
            // Store all control instructions in BTB
            btb[index].pc = pc;
            btb[index].target = target;
            btb[index].valid = true;
            btb[index].prediction = taken; // 1-bit predictor: set to actual outcome
            traceOut << "BTB Update: PC=" << to_hex(pc) << ", Taken=" << taken 
                 << ", Target=" << to_hex(target) << ", Prediction=" << btb[index].prediction << "\n";
        }
    
        void print_state() {
            if (!knobs.print_branch_predictor) return;
            traceOut << "Branch Predictor State:\n";
            for (uint32_t i = 0; i < btb_size; i++) {
                if (btb[i].valid) {
                    traceOut << "BTB[" << i << "]: PC=" << to_hex(btb[i].pc) 
                         << ", Target=" << to_hex(btb[i].target) 
                         << ", Prediction=" << btb[i].prediction << "\n";
                }
            }
        }
    };
// Global branch predictor instance
BranchPredictor* branch_predictor = nullptr;
bool is_valid_hex(const string& str) {
    // Must start with "0x" and have at least one hex digit following
    if (str.length() < 3 || str.compare(0, 2, "0x") != 0) {
        return false;
    }
    return all_of(str.begin() + 2, str.end(), [](unsigned char c) {
        return isxdigit(c);
    });
}


bool load_memory(const string& text_file, const string& data_file) {
    ifstream text_in(text_file);
    if (!text_in) {
        traceOut << "error: Cannot open text file: " << text_file << endl;
        return false;
    }

    string line;
    int line_num = 0;
    int valid_lines = 0;
    while (getline(text_in, line)) {
        ++line_num;
        size_t comment_pos = line.find('#');
        if (comment_pos != string::npos) {
            line = line.substr(0, comment_pos);
        }
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);
        if (line.empty()) continue;

        stringstream ss(line);
        string addr_str, instr_str;
        if (!(ss >> addr_str >> instr_str)) {
            traceOut << "Warning: Skipping malformed line " << line_num << " in " << text_file << ": " << line << endl;
            continue;
        }

        if (!is_valid_hex(addr_str) || !is_valid_hex(instr_str)) {
            traceOut << "Warning: Invalid hex at line " << line_num << " in " << text_file
                 << ": " << addr_str << " " << instr_str << endl;
            continue;
        }

        try {
            uint32_t addr = stoul(addr_str, nullptr, 16);
            uint32_t instr = stoul(instr_str, nullptr, 16);
            text_memory.emplace_back(addr, instr);
            ++valid_lines;
            traceOut << "Loaded text: Addr=" << to_hex(addr) << ", Instr=" << to_hex(instr) << "\n";
        } catch (const exception& e) {
            traceOut << "Warning: Invalid number format at line " << line_num << " in " << text_file
                 << ": " << addr_str << " " << instr_str << endl;
            continue;
        }
    }
    text_in.close();

    if (valid_lines == 0) {
        traceOut << "error: No valid instructions loaded from " << text_file << endl;
        return false;
    }
    traceOut << "Total valid text instructions loaded: " << valid_lines << "\n";

    ifstream data_in(data_file);
    if (data_in) {
        line_num = 0;
        valid_lines = 0;
        while (getline(data_in, line)) {
            ++line_num;
            size_t comment_pos = line.find('#');
            if (comment_pos != string::npos) {
                line = line.substr(0, comment_pos);
            }
            line.erase(0, line.find_first_not_of(" \t"));
            line.erase(line.find_last_not_of(" \t") + 1);
            if (line.empty()) continue;

            stringstream ss(line);
            string addr_str, value_str;
            if (!(ss >> addr_str >> value_str)) {
                traceOut << "Warning: Skipping malformed line " << line_num << " in " << data_file
                     << ": " << line << endl;
                continue;
            }

            if (!is_valid_hex(addr_str) || !is_valid_hex(value_str)) {
                traceOut << "Warning: Invalid hex at line " << line_num << " in " << data_file
                     << ": " << addr_str << " " << value_str << endl;
                continue;
            }

            try {
                uint32_t addr = stoul(addr_str, nullptr, 16);
                int32_t value = static_cast<int32_t>(stoul(value_str, nullptr, 16));
                data_memory.emplace_back(addr, value);
                ++valid_lines;
                traceOut << "Loaded data: Addr=" << to_hex(addr) << ", Value=" << to_hex(value) << "\n";
            } catch (const exception& e) {
                traceOut << "Warning: Invalid number format at line " << line_num << " in " << data_file
                     << ": " << addr_str << " " << value_str << endl;
                continue;
            }
        }
        data_in.close();
        traceOut << "Total valid data entries loaded: " << valid_lines << "\n";
    } else {
        traceOut << "No data file found or could not open: " << data_file << endl;
    }

    return true;
}
void dump_stats_json() {
    web3 << "{\n"
         << "  \"total_cycles\": "           << stats.total_cycles           << ",\n"
         << "  \"total_instructions\": "     << stats.total_instructions     << ",\n"
         << "  \"cpi\": "                    << fixed << setprecision(3)
                                          << stats.get_cpi()             << ",\n"
         << "  \"data_transfer_instructions\": " << stats.data_transfer_instructions << ",\n"
         << "  \"alu_instructions\": "       << stats.alu_instructions      << ",\n"
         << "  \"control_instructions\": "   << stats.control_instructions  << ",\n"
         << "  \"stall_count\": "            << stats.stall_count           << ",\n"
         << "  \"data_hazards\": "           << stats.data_hazards          << ",\n"
         << "  \"control_hazards\": "        << stats.control_hazards       << ",\n"
         << "  \"branch_mispredictions\": "  << stats.branch_mispredictions << ",\n"
         << "  \"stalls_data_hazards\": "    << stats.stalls_data_hazards   << ",\n"
         << "  \"stalls_control_hazards\": " << stats.stalls_control_hazards << "\n"
         << "}\n";
}

void write_data_mc(const string& filename) {
    ofstream ofs(filename);
    if (!ofs.is_open()) {
        traceOut << "error: Cannot write to " << filename << endl;
        return;
    }

    for (const auto& pair : data_memory) {
        const auto& addr= pair.first;
        const auto& val=pair.second;
        ofs << to_hex(addr) << " " << to_hex(val) << "\n";
    }
}

bool detect_data_hazard() {
    if (!knobs.enable_pipelining || !if_id.is_valid) return false;
    uint32_t ir = if_id.ir;
    if (ir == 0) return false;
    uint32_t opcode = ir & 0x7F;
    uint32_t rs1 = (ir >> 15) & 0x1F;
    uint32_t rs2 = (ir >> 20) & 0x1F;

    bool uses_rs1 = (opcode == stoul(R_opcodeTable["add"], nullptr, 2) ||
                     opcode == stoul(I_opcodeTable["addi"], nullptr, 2) ||
                     opcode == stoul(I_opcodeTable["lw"], nullptr, 2) ||
                     opcode == stoul(I_opcodeTable["jalr"], nullptr, 2) ||
                     opcode == stoul(S_opcodeTable["sw"], nullptr, 2) ||
                     opcode == stoul(SB_opcodeTable["beq"], nullptr, 2));

    bool uses_rs2 = (opcode == stoul(R_opcodeTable["add"], nullptr, 2) ||
                     opcode == stoul(S_opcodeTable["sw"], nullptr, 2) ||
                     opcode == stoul(SB_opcodeTable["beq"], nullptr, 2));

    if (id_ex.is_valid && id_ex.ctrl.reg_write && id_ex.rd != 0) {
        if ((uses_rs1 && rs1 == id_ex.rd) || (uses_rs2 && rs2 == id_ex.rd)) {
            if (!knobs.enable_data_forwarding || id_ex.ctrl.mem_read) {
                stats.data_hazards++;
                stats.stalls_data_hazards++;
                return true;
            }
        }
    }

    // Check hazard with EX/MEM
    if (!knobs.enable_data_forwarding && ex_mem.is_valid && ex_mem.ctrl.reg_write && ex_mem.rd != 0) {
        if ((uses_rs1 && rs1 == ex_mem.rd) || (uses_rs2 && rs2 == ex_mem.rd)) {
            stats.data_hazards++;
            stats.stalls_data_hazards++;
            return true;
        }
    }

    // Check hazard with MEM/WB
    if (!knobs.enable_data_forwarding && mem_wb.is_valid && mem_wb.ctrl.reg_write && mem_wb.rd != 0) {
        if ((uses_rs1 && rs1 == mem_wb.rd) || (uses_rs2 && rs2 == mem_wb.rd)) {
            stats.data_hazards++;
            stats.stalls_data_hazards++;
            return true;
        }
    }

    return false;
}
void forwarding(uint32_t& reg_a_val, uint32_t& reg_b_val, uint32_t rs1, uint32_t rs2) {
    if (!knobs.enable_data_forwarding) return;

    // Helper to attempt a forward from a stage
    auto try_forward = [&](uint32_t src_rd, uint32_t src_val, const char* stage,
                           uint32_t& reg_val, uint32_t rs) {
        if (src_rd == rs) {
            reg_val = src_val;
            traceOut << "Forwarding " << stage << " to rs" << "(" << rs << "): " << reg_val << "\n";
            return true;
        }
        return false;
    };

    bool forwarded_rs1 = false, forwarded_rs2 = false;

    if (ex_mem.is_valid && ex_mem.ctrl.reg_write && ex_mem.rd != 0) {
        forwarded_rs1 = try_forward(ex_mem.rd, ex_mem.alu_result, "EX/MEM", reg_a_val, rs1);
        forwarded_rs2 = try_forward(ex_mem.rd, ex_mem.alu_result, "EX/MEM", reg_b_val, rs2) || forwarded_rs2;
    }

    if (mem_wb.is_valid && mem_wb.ctrl.reg_write && mem_wb.rd != 0) {
        if (!forwarded_rs1) {
            try_forward(mem_wb.rd, mem_wb.write_data, "MEM/WB", reg_a_val, rs1);
        }
        if (!forwarded_rs2) {
            try_forward(mem_wb.rd, mem_wb.write_data, "MEM/WB", reg_b_val, rs2);
        }
    }
}

void fetch() {
    if (program_done) {
        bool drained = !if_id.is_valid && !id_ex.is_valid && !ex_mem.is_valid && !mem_wb.is_valid;
        if (drained) {
            traceOut << "Fetch: Pipeline empty and program done\n";
            return;
        }
    }

    if (stall_pipeline && knobs.enable_pipelining) {
        ++stats.stall_count;
        traceOut << "Fetch: Stalled, keeping IF/ID unchanged\n";
        return;
    }

    uint32_t fetched_ir = 0;
    auto it = find_if(text_memory.begin(), text_memory.end(),
                      [&](const auto &entry){ return entry.first == pc; });
    if (it != text_memory.end()) {
        fetched_ir = it->second;
    }

    if (fetched_ir == 0) {
        traceOut << "Fetch: No instruction at PC=" << to_hex(pc)
             << ", marking IF/ID invalid\n";
        if_id = IF_ID_Register();
        pc += 4;

        bool drained2 = !if_id.is_valid && !id_ex.is_valid && !ex_mem.is_valid && !mem_wb.is_valid;
        if (drained2) {
            traceOut << "Fetch: Pipeline empty and no more instructions, setting program_done\n";
            program_done = true;
        }
        return;
    }

    // Valid fetch: populate IF/ID
    if_id.pc           = pc;
    if_id.ir           = fetched_ir;
    if_id.is_valid     = true;
    if_id.instruction_num = ++instruction_count;

    bool pred = branch_predictor->predict(pc);
    uint32_t next_pc = pred ? branch_predictor->get_target(pc) : pc + 4;

    traceOut << "Fetch: PC=" << to_hex(if_id.pc)
         << ", IR=" << to_hex(fetched_ir)
         << ", Instr#=" << instruction_count
         << ", NextPC=" << to_hex(next_pc)
         << ", PredictedTaken=" << pred << "\n";

    pc = next_pc;
}

int32_t extraction_imm(uint32_t ir, char type) {
    int32_t raw = 0;
    switch (type) {
        case 'I': {
            // imm[11:0]
            raw = (ir >> 20) & 0xFFF;
            break;
        }
        case 'S': {
            // imm[11:5] | imm[4:0]
            uint32_t hi = (ir >> 25) & 0x7F;
            uint32_t lo = (ir >> 7)  & 0x1F;
            raw = (hi << 5) | lo;
            break;
        }
        case 'B': {
            // imm[12] | imm[10:5] | imm[4:1] | imm[11]
            uint32_t b12 = (ir >> 31) & 0x1;
            uint32_t b11 = (ir >> 7)  & 0x1;
            uint32_t b10_5 = (ir >> 25) & 0x3F;
            uint32_t b4_1  = (ir >> 8)  & 0xF;
            raw = (b12 << 12) | (b11 << 11) | (b10_5 << 5) | (b4_1 << 1);
            break;
        }
        case 'U':
            // imm[31:12] << 12, high half only
            return static_cast<int32_t>(ir & 0xFFFFF000);
        case 'J': {
            // imm[20] | imm[10:1] | imm[11] | imm[19:12]
            uint32_t j20    = (ir >> 31)        & 0x1;
            uint32_t j10_1  = (ir >> 21)        & 0x3FF;
            uint32_t j11    = (ir >> 20)        & 0x1;
            uint32_t j19_12 = (ir >> 12)        & 0xFF;
            raw = (j20 << 20) | (j10_1 << 1) | (j11 << 11) | (j19_12 << 12);
            break;
        }
        default:
            return 0;
    }

    int width = 12;
    if (type == 'B') width = 13;
    else if (type == 'J') width = 21;

    return sign_extend(raw, width);
}
void decode() {
    if (!if_id.is_valid) {
        id_ex = ID_EX_Register();
        traceOut << "Decode: IF/ID invalid, inserting bubble\n";
        return;
    }

    if (stall_pipeline && knobs.enable_pipelining) {
        traceOut << "Decode: Pipeline stalled, keeping ID/EX unchanged\n";
        return; 
    }

    uint32_t ir = if_id.ir;
    id_ex.ir = ir;
    id_ex.pc = if_id.pc;
    id_ex.instruction_num = if_id.instruction_num;
    id_ex.is_valid = true;

    Control& ctrl = id_ex.ctrl;
    ctrl = Control();

    if (ir == 0) {
        ctrl.is_nop = true;
        traceOut << "Decode: Invalid instruction (IR=0)\n";
        return;
    }

    uint32_t opcode = ir & 0x7F;
    uint32_t rd = (ir >> 7) & 0x1F;
    uint32_t rs1 = (ir >> 15) & 0x1F;
    uint32_t rs2 = (ir >> 20) & 0x1F;
    uint32_t func3 = (ir >> 12) & 0x7;
    uint32_t func7 = (ir >> 25) & 0x7F;
    int32_t imm = 0;

    int32_t reg_a_val = reg_file[rs1];
    int32_t reg_b_val = reg_file[rs2];

    forwarding(reinterpret_cast<uint32_t&>(reg_a_val), reinterpret_cast<uint32_t&>(reg_b_val), rs1, rs2);

    id_ex.rs1 = rs1;
    id_ex.rs2 = rs2;
    id_ex.rd = rd;
    id_ex.reg_a_val = reg_a_val;
    id_ex.reg_b_val = reg_b_val;

    traceOut << "Decode: PC=" << to_hex(if_id.pc) << ", IR=" << to_hex(ir) << ", rs1=x" << rs1
         << "(" << reg_a_val << "), rs2=x" << rs2 << "(" << reg_b_val << "), rd=x" << rd << "\n";

    bool is_control = false;
    bool branch_taken = false;
    uint32_t branch_target = if_id.pc + 4;

    if (opcode == stoul(R_opcodeTable["add"], nullptr, 2)) {
        ctrl.reg_write = true;
        stats.alu_instructions++;
        if (func3 == stoul(func3Map["add"], nullptr, 2) && func7 == stoul(func7Map["add"], nullptr, 2)) {
            ctrl.alu_op = "ADD";
        } else if (func3 == stoul(func3Map["sub"], nullptr, 2) && func7 == stoul(func7Map["sub"], nullptr, 2)) {
            ctrl.alu_op = "SUB";
        } else if (func3 == stoul(func3Map["mul"], nullptr, 2) && func7 == stoul(func7Map["mul"], nullptr, 2)) {
            ctrl.alu_op = "MUL";
        } else if (func3 == stoul(func3Map["sll"], nullptr, 2) && func7 == stoul(func7Map["sll"], nullptr, 2)) {
            ctrl.alu_op = "SLL";
        } else if (func3 == stoul(func3Map["slt"], nullptr, 2) && func7 == stoul(func7Map["slt"], nullptr, 2)) {
            ctrl.alu_op = "SLT";
        } else if (func3 == stoul(func3Map["sltu"], nullptr, 2) && func7 == stoul(func7Map["sltu"], nullptr, 2)) {
            ctrl.alu_op = "SLTU";
        } else if (func3 == stoul(func3Map["xor"], nullptr, 2) && func7 == stoul(func7Map["xor"], nullptr, 2)) {
            ctrl.alu_op = "XOR";
        } else if (func3 == stoul(func3Map["srl"], nullptr, 2) && func7 == stoul(func7Map["srl"], nullptr, 2)) {
            ctrl.alu_op = "SRL";
        } else if (func3 == stoul(func3Map["sra"], nullptr, 2) && func7 == stoul(func7Map["sra"], nullptr, 2)) {
            ctrl.alu_op = "SRA";
        } else if (func3 == stoul(func3Map["or"], nullptr, 2) && func7 == stoul(func7Map["or"], nullptr, 2)) {
            ctrl.alu_op = "OR";
        } else if (func3 == stoul(func3Map["and"], nullptr, 2) && func7 == stoul(func7Map["and"], nullptr, 2)) {
            ctrl.alu_op = "AND";
        } else {
            ctrl.is_nop = true;
            traceOut << "Decode: Unknown R-type instruction, func3=0x" << hex << func3 << ", func7=0x" << func7 << dec << "\n";
        }
    } else if (opcode == stoul(I_opcodeTable["addi"], nullptr, 2)) {
        ctrl.reg_write = true;
        ctrl.use_imm = true;
        imm = sign_extend((ir >> 20) & 0xFFF, 12);
        stats.alu_instructions++;
        if (func3 == stoul(func3Map["addi"], nullptr, 2)) {
            ctrl.alu_op = "ADDI";
        } else if (func3 == stoul(func3Map["slti"], nullptr, 2)) {
            ctrl.alu_op = "SLTI";
        } else if (func3 == stoul(func3Map["sltiu"], nullptr, 2)) {
            ctrl.alu_op = "SLTIU";
        } else if (func3 == stoul(func3Map["xori"], nullptr, 2)) {
            ctrl.alu_op = "XORI";
        } else if (func3 == stoul(func3Map["ori"], nullptr, 2)) {
            ctrl.alu_op = "ORI";
        } else if (func3 == stoul(func3Map["andi"], nullptr, 2)) {
            ctrl.alu_op = "ANDI";
        } else if (func3 == stoul(func3Map["slli"], nullptr, 2) && func7 == stoul(func7Map["slli"], nullptr, 2)) {
            ctrl.alu_op = "SLLI";
            imm = (ir >> 20) & 0x1F;
        } else if (func3 == stoul(func3Map["srli"], nullptr, 2) && func7 == stoul(func7Map["srli"], nullptr, 2)) {
            ctrl.alu_op = "SRLI";
            imm = (ir >> 20) & 0x1F;
        } else if (func3 == stoul(func3Map["srai"], nullptr, 2) && func7 == stoul(func7Map["srai"], nullptr, 2)) {
            ctrl.alu_op = "SRAI";
            imm = (ir >> 20) & 0x1F;
        } else {
            ctrl.is_nop = true;
            traceOut << "Decode: Unknown I-type arithmetic instruction, func3=0x" << hex << func3 << ", func7=0x" << func7 << dec << "\n";
        }
    } else if (opcode == stoul(I_opcodeTable["lw"], nullptr, 2)) {
        ctrl.reg_write = true;
        ctrl.mem_read = true;
        ctrl.use_imm = true;
        ctrl.output_sel = 1;
        imm = sign_extend((ir >> 20) & 0xFFF, 12);
        stats.data_transfer_instructions++;
        if (func3 == stoul(func3Map["lb"], nullptr, 2)) {
            ctrl.alu_op = "LB";
            ctrl.mem_size = "BYTE";
        } else if (func3 == stoul(func3Map["lh"], nullptr, 2)) {
            ctrl.alu_op = "LH";
            ctrl.mem_size = "HALF";
        } else if (func3 == stoul(func3Map["lw"], nullptr, 2)) {
            ctrl.alu_op = "LW";
            ctrl.mem_size = "WORD";
        } else if (func3 == stoul(func3Map["lbu"], nullptr, 2)) {
            ctrl.alu_op = "LBU";
            ctrl.mem_size = "BYTE_U";
        } else if (func3 == stoul(func3Map["lhu"], nullptr, 2)) {
            ctrl.alu_op = "LHU";
            ctrl.mem_size = "HALF_U";
        } else {
            ctrl.is_nop = true;
            traceOut << "Decode: Unknown load instruction, func3=0x" << hex << func3 << dec << "\n";
        }
    } else if (opcode == stoul(S_opcodeTable["sw"], nullptr, 2)) {
        ctrl.mem_write = true;
        ctrl.reg_write = false;
        imm = extraction_imm(ir, 'S');
        stats.data_transfer_instructions++;
        if (func3 == stoul(func3Map["sb"], nullptr, 2)) {
            ctrl.alu_op = "SB";
            ctrl.mem_size = "BYTE";
        } else if (func3 == stoul(func3Map["sh"], nullptr, 2)) {
            ctrl.alu_op = "SH";
            ctrl.mem_size = "HALF";
        } else if (func3 == stoul(func3Map["sw"], nullptr, 2)) {
            ctrl.alu_op = "SW";
            ctrl.mem_size = "WORD";
        } else {
            ctrl.is_nop = true;
            traceOut << "Decode: Unknown store instruction, func3=0x" << hex << func3 << dec << "\n";
        }
    } else if (opcode == stoul(SB_opcodeTable["beq"], nullptr, 2)) {
        ctrl.branch = true;
        ctrl.reg_write = false;
        id_ex.rd = 0;
        is_control = true;
        stats.control_instructions++;
        imm = extraction_imm(ir, 'B');
        if (func3 == stoul(func3Map["beq"], nullptr, 2)) {
            ctrl.alu_op = "BEQ";
            branch_taken = (reg_a_val == reg_b_val);
        } else if (func3 == stoul(func3Map["bne"], nullptr, 2)) {
            ctrl.alu_op = "BNE";
            branch_taken = (reg_a_val != reg_b_val);
        } else if (func3 == stoul(func3Map["blt"], nullptr, 2)) {
            ctrl.alu_op = "BLT";
            branch_taken = (static_cast<int32_t>(reg_a_val) < static_cast<int32_t>(reg_b_val));
        } else if (func3 == stoul(func3Map["bge"], nullptr, 2)) {
            ctrl.alu_op = "BGE";
            branch_taken = (static_cast<int32_t>(reg_a_val) >= static_cast<int32_t>(reg_b_val));
        } else if (func3 == stoul(func3Map["bltu"], nullptr, 2)) {
            ctrl.alu_op = "BLTU";
            branch_taken = (static_cast<uint32_t>(reg_a_val) < static_cast<uint32_t>(reg_b_val));
        } else if (func3 == stoul(func3Map["bgeu"], nullptr, 2)) {
            ctrl.alu_op = "BGEU";
            branch_taken = (static_cast<uint32_t>(reg_a_val) >= static_cast<uint32_t>(reg_b_val));
        } else {
            ctrl.is_nop = true;
            traceOut << "Decode: Unknown SB-type instruction, func3=0x" << hex << func3 << dec << "\n";
            return;
        }
        branch_target = branch_taken ? if_id.pc + imm : if_id.pc + 4;
        traceOut << "Decode " << ctrl.alu_op << ": rs1=x" << rs1 << "(" << reg_a_val << "), rs2=x" << rs2
             << "(" << reg_b_val << "), Taken=" << branch_taken << ", Target=" << to_hex(branch_target) << "\n";
    } else if (opcode == stoul(UJ_opcodeTable["jal"], nullptr, 2)) {
        ctrl.reg_write = true;
        ctrl.alu_op = "JAL";
        ctrl.output_sel = 2;
        is_control = true;
        stats.control_instructions++;
        imm = extraction_imm(ir, 'J');
        branch_taken = true;
        branch_target = if_id.pc + imm;
        traceOut << "Decode JAL: rd=x" << rd << ", Target=" << to_hex(branch_target) << "\n";
    } else if (opcode == stoul(I_opcodeTable["jalr"], nullptr, 2)) {
        ctrl.reg_write = true;
        ctrl.alu_op = "JALR";
        ctrl.use_imm = true;
        ctrl.output_sel = 2;
        is_control = true;
        stats.control_instructions++;
        imm = sign_extend((ir >> 20) & 0xFFF, 12);
        branch_taken = true;
        branch_target = (reg_a_val + imm) & ~1;
        traceOut << "Decode JALR: rs1=x" << rs1 << "(" << reg_a_val << "), imm=" << imm
             << ", Target=" << to_hex(branch_target) << "\n";
    } else if (opcode == stoul(U_opcodeTable["lui"], nullptr, 2)) {
        ctrl.reg_write = true;
        ctrl.alu_op = "LUI";
        imm = extraction_imm(ir, 'U');
        stats.alu_instructions++;
        traceOut << "Decode LUI: rd=x" << rd << ", imm=" << to_hex(imm) << "\n";
    } else if (opcode == stoul(U_opcodeTable["auipc"], nullptr, 2)) {
        ctrl.reg_write = true;
        ctrl.alu_op = "AUIPC";
        imm = extraction_imm(ir, 'U');
        stats.alu_instructions++;
        traceOut << "Decode AUIPC: rd=x" << rd << ", imm=" << to_hex(imm) << "\n";
    } else {
        ctrl.is_nop = true;
        traceOut << "Decode: Unknown opcode: 0x" << hex << opcode << dec << "\n";
    }

    id_ex.imm = imm;

    if (is_control) {
        uint32_t current_pc = if_id.pc;
        bool predicted_taken = branch_predictor->predict(if_id.pc);
        uint32_t predicted_target = predicted_taken ? branch_predictor->get_target(if_id.pc) : if_id.pc + 4;

        if (branch_taken != predicted_taken || (branch_taken && branch_target != predicted_target)) {
            traceOut << "Misprediction: Flushing pipeline, ActualTarget=" << to_hex(branch_target)
                 << ", PredictedTarget=" << to_hex(predicted_target) << "\n";
            pc = branch_target;
            if_id = IF_ID_Register();
            stall_pipeline = true;
            stats.branch_mispredictions++;
            stats.control_hazards++;
            stats.stalls_control_hazards++;
        } else {
            traceOut << "Prediction correct: Taken=" << branch_taken << ", Target=" << to_hex(branch_target) << "\n";
        }

        branch_predictor->update(current_pc, branch_taken, branch_target);
    }
}void execute() {
    if (!id_ex.is_valid) {
        ex_mem = EX_MEM_Register();
        traceOut << "Execute: ID/EX invalid, skipping\n";
        return;
    }

    traceOut << "Execute: PC=" << to_hex(id_ex.pc)
         << ", IR=" << to_hex(id_ex.ir)
         << ", ALU=" << id_ex.ctrl.alu_op << "\n";

    int32_t reg_a_val = id_ex.reg_a_val;
    int32_t reg_b_val = id_ex.reg_b_val;
    forwarding(reinterpret_cast<uint32_t&>(reg_a_val),
                         reinterpret_cast<uint32_t&>(reg_b_val),
                         id_ex.rs1, id_ex.rs2);

    int32_t alu_result = 0;
    const string &op = id_ex.ctrl.alu_op;

    if (id_ex.ctrl.is_nop) {
        traceOut << "Execute: NOP\n";
    } else {
        if (op == "ADD")        alu_result = reg_a_val + reg_b_val;
        else if (op == "SUB")   alu_result = reg_a_val - reg_b_val;
        else if (op == "MUL")   alu_result = reg_a_val * reg_b_val;
        else if (op == "AND")   alu_result = reg_a_val & reg_b_val;
        else if (op == "OR")    alu_result = reg_a_val | reg_b_val;
        else if (op == "XOR")   alu_result = reg_a_val ^ reg_b_val;
        else if (op == "SLT")   alu_result = (reg_a_val < reg_b_val) ? 1 : 0;
        else if (op == "SLTU")  alu_result = (static_cast<uint32_t>(reg_a_val) < static_cast<uint32_t>(reg_b_val)) ? 1 : 0;
        else if (op == "SLL")   alu_result = reg_a_val << (reg_b_val & 0x1F);
        else if (op == "SRL")   alu_result = static_cast<uint32_t>(reg_a_val) >> (reg_b_val & 0x1F);
        else if (op == "SRA")   alu_result = reg_a_val >> (reg_b_val & 0x1F);

        else if (op == "ADDI" || op == "LW"  || op == "LH" || op == "LB" ||
                 op == "LHU"  || op == "LBU" || op == "SW" || op == "SH" ||
                 op == "SB"   || op == "JALR") {
            alu_result = reg_a_val + id_ex.imm;
        }

        else if (op == "SLTI")   alu_result = (reg_a_val < id_ex.imm) ? 1 : 0;
        else if (op == "SLTIU")  alu_result = (static_cast<uint32_t>(reg_a_val) < static_cast<uint32_t>(id_ex.imm)) ? 1 : 0;

        else if (op == "XORI")   alu_result = reg_a_val ^ id_ex.imm;
        else if (op == "ORI")    alu_result = reg_a_val | id_ex.imm;
        else if (op == "ANDI")   alu_result = reg_a_val & id_ex.imm;

        else if (op == "SLLI")   alu_result = reg_a_val << (id_ex.imm & 0x1F);
        else if (op == "SRLI")   alu_result = static_cast<uint32_t>(reg_a_val) >> (id_ex.imm & 0x1F);
        else if (op == "SRAI")   alu_result = reg_a_val >> (id_ex.imm & 0x1F);

        else if (op == "BEQ"  || op == "BNE"  || op == "BLT"  ||
                 op == "BGE"  || op == "BLTU" || op == "BGEU") {
            alu_result = id_ex.pc + id_ex.imm;
        }
        else if (op == "JAL" || op == "JALR") {
            alu_result = id_ex.pc + 4;
        }

        else if (op == "LUI")    alu_result = id_ex.imm;
        else if (op == "AUIPC")  alu_result = id_ex.pc + id_ex.imm;

        else {
            traceOut << "Execute: Unknown ALU op: " << op << "\n";
        }
    }

    ex_mem.pc           = id_ex.pc;
    ex_mem.alu_result   = alu_result;
    ex_mem.rs2_val      = reg_b_val;
    ex_mem.rd           = id_ex.rd;
    ex_mem.ctrl         = id_ex.ctrl;
    ex_mem.is_valid     = true;
    ex_mem.instruction_num = id_ex.instruction_num;

    id_ex = ID_EX_Register();
}

void memory() {
    if (!ex_mem.is_valid) {
        mem_wb = MEM_WB_Register();
        traceOut << "Memory: EX/MEM invalid, skipping\n";
        return;
    }

    int32_t mem_result = ex_mem.alu_result;

    if (ex_mem.ctrl.mem_read) {
        uint32_t addr = ex_mem.alu_result;
        if (ex_mem.ctrl.mem_size == "WORD" && (addr % 4 != 0)) {
            traceOut << "Warning: Misaligned WORD read at address " << to_hex(addr) << "\n";
        } else if (ex_mem.ctrl.mem_size == "HALF" || ex_mem.ctrl.mem_size == "HALF_U") {
            if (addr % 2 != 0) {
                traceOut << "Warning: Misaligned HALF read at address " << to_hex(addr) << "\n";
            }
        }

        bool found = false;
        for (const auto& mem_entry : data_memory) {
            if (mem_entry.first == addr) {
                mem_result = mem_entry.second;
                found = true;
                if (ex_mem.ctrl.mem_size == "BYTE") {
                    mem_result = sign_extend(mem_result & 0xFF, 8);
                } else if (ex_mem.ctrl.mem_size == "HALF") {
                    mem_result = sign_extend(mem_result & 0xFFFF, 16);
                } else if (ex_mem.ctrl.mem_size == "BYTE_U") {
                    mem_result = mem_result & 0xFF;
                } else if (ex_mem.ctrl.mem_size == "HALF_U") {
                    mem_result = mem_result & 0xFFFF;
                }
                break;
            }
        }
        if (!found) {
            traceOut << "Warning: Memory read at address " << to_hex(addr) << " found no data, returning 0\n";
            mem_result = 0;
        }

    } else if (ex_mem.ctrl.mem_write) {
        uint32_t addr = ex_mem.alu_result;
        int32_t value = ex_mem.rs2_val;
        if (ex_mem.ctrl.mem_size == "WORD" && (addr % 4 != 0)) {
            traceOut << "Warning: Misaligned WORD write at address " << to_hex(addr) << "\n";
        } else if (ex_mem.ctrl.mem_size == "HALF" && (addr % 2 != 0)) {
            traceOut << "Warning: Misaligned HALF write at address " << to_hex(addr) << "\n";
        }

        bool found = false;
        for (auto& mem_entry : data_memory) {
            if (mem_entry.first == addr) {
                if (ex_mem.ctrl.mem_size == "BYTE") {
                    mem_entry.second = (mem_entry.second & ~0xFF) | (value & 0xFF);
                } else if (ex_mem.ctrl.mem_size == "HALF") {
                    mem_entry.second = (mem_entry.second & ~0xFFFF) | (value & 0xFFFF);
                } else { // WORD
                    mem_entry.second = value;
                }
                found = true;
                break;
            }
        }
        if (!found) {
            if (ex_mem.ctrl.mem_size == "BYTE") {
                data_memory.emplace_back(addr, value & 0xFF);
            } else if (ex_mem.ctrl.mem_size == "HALF") {
                data_memory.emplace_back(addr, value & 0xFFFF);
            } else {
                data_memory.emplace_back(addr, value);
            }
        }
    }
    mem_wb.pc          = ex_mem.pc;
    mem_wb.write_data  = (ex_mem.ctrl.output_sel == 1) ? mem_result : ex_mem.alu_result;
    mem_wb.rd          = ex_mem.rd;
    mem_wb.ctrl        = ex_mem.ctrl;
    mem_wb.is_valid    = true;
    mem_wb.instruction_num= ex_mem.instruction_num;


    ex_mem = EX_MEM_Register();
    traceOut << "Memory: PC=" << to_hex(mem_wb.pc)
         << ", Instr#=" << mem_wb.instruction_num << "\n";
}

void writeback() {
    if (!mem_wb.is_valid) {
        traceOut << "Writeback: MEM/WB invalid, skipping\n";
        return;
    }

    auto &wb = mem_wb;

    if (!wb.ctrl.is_nop) {
        ++stats.total_instructions;
        traceOut << "Total Instructions are :  " << stats.total_instructions << endl;
    }

    if (wb.ctrl.reg_write && wb.rd != 0) {
        int32_t value_to_write = (wb.ctrl.output_sel == 2)
                                 ? (wb.pc + 4)
                                 : wb.write_data;
        reg_file[wb.rd] = value_to_write;
        traceOut << "Writeback: x" << wb.rd
             << " = " << to_hex(wb.write_data) << "\n";
    }

    // Final status log
    traceOut << "Writeback: PC=" << to_hex(wb.pc)
         << ", Instr#=" << wb.instruction_num << "\n";

    wb = MEM_WB_Register();
}


void print_register_file() {
    if (!knobs.print_reg_file) return;
    traceOut << "\nRegister File:\n";
    for (int i = 0; i < 32; i++) {
        if (i % 4 == 0 && i > 0) traceOut << "\n";
        traceOut << "x" << setw(2) << setfill('0') << i << ": " << setw(10) << setfill(' ')
             << reg_file[i] << " (0x" << hex << setw(8) << setfill('0') << reg_file[i] << dec << ")  ";
    }
    traceOut << "\n";
}

void print_pipeline_registers() {
    if (!knobs.print_pipeline_regs) return;
    traceOut << "\nPipeline Registers:\n";
    traceOut << "IF/ID: " << (if_id.is_valid ? "PC=" + to_hex(if_id.pc) + ", IR=" + to_hex(if_id.ir) +
                                  ", Instr#=" + to_string(if_id.instruction_num) : "INVALID") << "\n";
    traceOut << "ID/EX: ";
    if (id_ex.is_valid) {
        traceOut << "PC=" << to_hex(id_ex.pc) << ", rs1=x" << id_ex.rs1 << "(" << id_ex.reg_a_val
             << "), rs2=x" << id_ex.rs2 << "(" << id_ex.reg_b_val << "), rd=x" << id_ex.rd
             << ", imm=" << id_ex.imm << ", ALU=" << id_ex.ctrl.alu_op << ", Instr#=" << id_ex.instruction_num;
    } else {
        traceOut << "INVALID";
    }
    traceOut << "\n";
    traceOut << "EX/MEM: ";
    if (ex_mem.is_valid) {
        traceOut << "PC=" << to_hex(ex_mem.pc) << ", ALU=" << ex_mem.alu_result << ", rs2_val=" << ex_mem.rs2_val
             << ", rd=x" << ex_mem.rd << ", Ctrl=" << (ex_mem.ctrl.mem_read ? "MemRead " : "")
             << (ex_mem.ctrl.mem_write ? "MemWrite " : "") << (ex_mem.ctrl.reg_write ? "RegWrite " : "")
             << "Instr#=" << ex_mem.instruction_num;
    } else {
        traceOut << "INVALID";
    }
    traceOut << "\n";
    traceOut << "MEM/WB: ";
    if (mem_wb.is_valid) {
        traceOut << "PC=" << to_hex(mem_wb.pc) << ", WData=" << mem_wb.write_data << ", rd=x" << mem_wb.rd
             << ", Ctrl=" << (mem_wb.ctrl.reg_write ? "RegWrite " : "") << "Instr#=" << mem_wb.instruction_num;
    } else {
        traceOut << "INVALID";
    }
    traceOut << "\n";
}

void trace_instruction(int instruction_num) {
    if (instruction_num == -1) return;
    if (if_id.is_valid && if_id.instruction_num == instruction_num) {
        traceOut << "Trace: Instr#" << instruction_num << " in IF/ID, PC=" << to_hex(if_id.pc) << "\n";
    }
    if (id_ex.is_valid && id_ex.instruction_num == instruction_num) {
        traceOut << "Trace: Instr#" << instruction_num << " in ID/EX, PC=" << to_hex(id_ex.pc)
             << ", ALU=" << id_ex.ctrl.alu_op << "\n";
    }
    if (ex_mem.is_valid && ex_mem.instruction_num == instruction_num) {
        traceOut << "Trace: Instr#" << instruction_num << " in EX/MEM, PC=" << to_hex(ex_mem.pc)
             << ", ALU Result=" << ex_mem.alu_result << "\n";
    }
    if (mem_wb.is_valid && mem_wb.instruction_num == instruction_num) {
        traceOut << "Trace: Instr#" << instruction_num << " in MEM/WB, PC=" << to_hex(mem_wb.pc)
             << ", Data=" << mem_wb.write_data << "\n";
    }
}

void print_statistics() {
    statsOut << "\nSimulation Statistics:\n";
    statsOut << "Total Cycles: " << stats.total_cycles << "\n";
    statsOut << "Total Instructions: " << stats.total_instructions << "\n";
    statsOut << "CPI: " << fixed << setprecision(3) << stats.get_cpi() << "\n";
    statsOut << "Data Transfer Instructions: " << stats.data_transfer_instructions << "\n";
    statsOut << "ALU Instructions: " << stats.alu_instructions << "\n";
    statsOut << "Control Instructions: " << stats.control_instructions << "\n";
    statsOut << "Total Stalls/Bubbles: " << stats.stall_count << "\n";
    statsOut << "Data Hazards: " << stats.data_hazards << "\n";
    statsOut << "Control Hazards: " << stats.control_hazards << "\n";
    statsOut << "Branch Mispredictions: " << stats.branch_mispredictions << "\n";
    statsOut << "Stalls Due to Data Hazards: " << stats.stalls_data_hazards << "\n";
    statsOut << "Stalls Due to Control Hazards: " << stats.stalls_control_hazards << "\n";
}
void print_statistics_json() {
    web3 << "{\n"
         << "  \"total_cycles\": "           << stats.total_cycles          << ",\n"
         << "  \"total_instructions\": "     << stats.total_instructions    << ",\n"
         << "  \"cpi\": "                    << fixed << setprecision(3)
                                           << stats.get_cpi()            << ",\n"
         << "  \"data_transfer_instructions\": " << stats.data_transfer_instructions << ",\n"
         << "  \"alu_instructions\": "       << stats.alu_instructions     << ",\n"
         << "  \"control_instructions\": "   << stats.control_instructions << ",\n"
         << "  \"stall_count\": "            << stats.stall_count          << ",\n"
         << "  \"data_hazards\": "           << stats.data_hazards         << ",\n"
         << "  \"control_hazards\": "        << stats.control_hazards      << ",\n"
         << "  \"branch_mispredictions\": "  << stats.branch_mispredictions<< ",\n"
         << "  \"stalls_data_hazards\": "    << stats.stalls_data_hazards  << ",\n"
         << "  \"stalls_control_hazards\": " << stats.stalls_control_hazards << "\n"
         << "}\n";
}

void configure_knobs() {
    // Set knob values here (change these to control the simulator)
    knobs.enable_pipelining = true;          // Knob1: Enable pipelining
    knobs.enable_data_forwarding = false;     // Knob2: Enable data forwarding
    knobs.print_reg_file = true;             // Knob3: Print register file each cycle
    knobs.print_pipeline_regs = true;        // Knob4: Print pipeline registers each cycle
    knobs.trace_instruction = -1;            // Knob5: Trace specific instruction (-1 to disable, e.g., 10 for 10th instruction)
    knobs.print_branch_predictor = true;     // Knob6: Print branch predictor state each cycle

    // Print knob settings for clarity
    traceOut << "Knob settings:\n";
    traceOut << "  Pipelining: " << (knobs.enable_pipelining ? "Enabled" : "Disabled") << "\n";
    traceOut << "  Data Forwarding: " << (knobs.enable_data_forwarding ? "Enabled" : "Disabled") << "\n";
    traceOut << "  Print Register File: " << (knobs.print_reg_file ? "Enabled" : "Disabled") << "\n";
    traceOut << "  Print Pipeline Registers: " << (knobs.print_pipeline_regs ? "Enabled" : "Disabled") << "\n";
    traceOut << "  Trace Instruction: " << (knobs.trace_instruction >= 0 ? to_string(knobs.trace_instruction) : "Disabled") << "\n";
    traceOut << "  Print Branch Predictor: " << (knobs.print_branch_predictor ? "Enabled" : "Disabled") << "\n";
}
void initialize_simulator() {
    text_memory.clear();
    data_memory.clear();
    reg_file.fill(0);
    reg_file[2] = 0x7FFFFFE4;
    reg_file[3] = 0x10000000;
    reg_file[10] = 0x00000001;
    reg_file[11] = 0x07FFFFE4;

    pc = 0;
    if_id = IF_ID_Register();
    id_ex = ID_EX_Register();
    ex_mem = EX_MEM_Register();
    mem_wb = MEM_WB_Register();

    stall_pipeline = false;

    stats = Stats();
    instruction_count = 0;
    program_done = false;

    if (branch_predictor != nullptr) {
        delete branch_predictor;
    }
    branch_predictor = new BranchPredictor(16); // BTB length = 16
    traceOut << "Simulator initialized, BTB cleared\n";
}
void run_simulation() {
    stats.total_cycles       = 0;
    stats.total_instructions = 0;
    program_done            = false;
    stall_pipeline          = false;

    web2 << "[\n";
    web2 << "  { \"event\": \"simulation_start\", "
            "\"pipelining\": "
         << (knobs.enable_pipelining ? "\"enabled\"" : "\"disabled\"")
         << " },\n";

    traceOut << "Starting simulation.\n";
    traceOut << "Pipelining: "
             << (knobs.enable_pipelining ? "Enabled" : "Disabled")
             << "\n";

    if (knobs.enable_pipelining) {
        while (stats.total_cycles < MAX_CYCLES &&
               (!program_done || if_id.is_valid || id_ex.is_valid ||
                ex_mem.is_valid || mem_wb.is_valid))
        {
            int cycle = stats.total_cycles + 1;

            // Cycle header
            traceOut << "\n=== Cycle " << cycle << " ===\n";
            web2    << "  { \"cycle\": " << cycle << " },\n";

            // Hazard detection
            if (!stall_pipeline)
                stall_pipeline = detect_data_hazard();

            // Writeback stage
            int wbNum   = mem_wb.instruction_num;
            int wbRd    = mem_wb.rd;
            int wbValue = (mem_wb.ctrl.output_sel == 2) ? (mem_wb.pc + 4)
                                            : mem_wb.write_data;
            writeback();

            web2 << "    { "
                << "\"instruction\": " << wbNum << ", "
                << "\"stage\": \"WriteBack\", "
                << "\"message\": "
                    << "\"Reg x"  << wbRd
                    << " = "     << wbValue
                << "\" },\n";


                 memory();
                 // mem_wb has just been filled from ex_mem
                 web2 << "    { "
                     << "\"instruction\": " << mem_wb.instruction_num << ", "
                     << "\"stage\": \"Memory\", "
                     << "\"message\": ";
     
                 if (mem_wb.ctrl.mem_read) {
                     web2 << "\"Load from 0x" << to_hex(ex_mem.alu_result)
                         << " = " << mem_wb.write_data << "\"";
                 } else if (ex_mem.ctrl.mem_write) {
                     web2 << "\"Store to 0x" << to_hex(ex_mem.alu_result)
                         << " = " << ex_mem.rs2_val << "\"";
                 } else {
                     web2 << "\"No mem-op\"";
                 }
     
                 web2 << " },\n";

                 execute();
                 // ex_mem now holds the result of this instruction
                 web2 << "    { "
                     << "\"instruction\": " << ex_mem.instruction_num << ", "
                     << "\"stage\": \"Execute\", "
                     << "\"message\": "
                         << "\"ALU="    << ex_mem.ctrl.alu_op
                         << ", result=" << ex_mem.alu_result
                     << "\" },\n";

            // Decode stage
            decode();
            // right after decode(), id_ex has been populated
            web2 << "    { "
                << "\"instruction\": " << id_ex.instruction_num << ", "
                << "\"stage\": \"Decode\", "
                << "\"message\": "
                    << "\"rs1=x" << id_ex.rs1
                    << "("  << id_ex.reg_a_val << "), "
                    << "rs2=x" << id_ex.rs2
                    << "("  << id_ex.reg_b_val << "), "
                    << "rd=x"  << id_ex.rd
                << "\" },\n";
                 fetch();
                 // grab instr num from IF/ID register
                 web2 << "    { "
                     << "\"instruction\": " << if_id.instruction_num << ", "
                     << "\"stage\": \"Fetch\", "
                     << "\"message\": \"PC = " << to_hex(if_id.pc)
                     << ", IR = "   << to_hex(if_id.ir) << "\" },\n";
                // after the last web2 << "... },\n";
                

            // Advance cycle count
            stats.total_cycles++;

            if (stall_pipeline && !detect_data_hazard())
                stall_pipeline = false;
        }
    }
    else {
        // Non‑pipelined execution
        while (stats.total_cycles < MAX_CYCLES && !program_done) {
            int cycle = stats.total_cycles + 1;

            traceOut << "\n=== Cycle " << cycle << " ===\n";
            web2    << "  { \"cycle\": " << cycle << " },\n";

            // Fetch
            fetch();
            // grab instr num from IF/ID register
            web2 << "    { "
                << "\"instruction\": " << if_id.instruction_num << ", "
                << "\"stage\": \"Fetch\", "
                << "\"message\": \"PC = " << to_hex(if_id.pc)
                << ", IR = "   << to_hex(if_id.ir) << "\" },\n";


            // Decode
            // Decode stage
            decode();
            web2 << "    { "
                << "\"instruction\": " << id_ex.instruction_num << ", "
                << "\"stage\": \"Decode\", "
                << "\"message\": "
                    << "\"rs1=x" << id_ex.rs1
                    << "("  << id_ex.reg_a_val << "), "
                    << "rs2=x" << id_ex.rs2
                    << "("  << id_ex.reg_b_val << "), "
                    << "rd=x"  << id_ex.rd
                << "\" },\n";


            // Execute
            // Execute stage
            execute();
            // ex_mem now holds the result of this instruction
            web2 << "    { "
                << "\"instruction\": " << ex_mem.instruction_num << ", "
                << "\"stage\": \"Execute\", "
                << "\"message\": "
                    << "\"ALU="    << ex_mem.ctrl.alu_op
                    << ", result=" << ex_mem.alu_result
                << "\" },\n";


            // Memory
            // Memory stage
            memory();
            web2 << "    { "
                << "\"instruction\": " << mem_wb.instruction_num << ", "
                << "\"stage\": \"Memory\", "
                << "\"message\": ";

            if (mem_wb.ctrl.mem_read) {
                web2 << "\"Load from 0x" << to_hex(ex_mem.alu_result)
                    << " = " << mem_wb.write_data << "\"";
            } else if (ex_mem.ctrl.mem_write) {
                web2 << "\"Store to 0x" << to_hex(ex_mem.alu_result)
                    << " = " << ex_mem.rs2_val << "\"";
            } else {
                web2 << "\"No mem-op\"";
            }

            web2 << " },\n";


            // Writeback (last entry, no comma)
            // Writeback stage
            int wbNum    = mem_wb.instruction_num;
            int wbRd     = mem_wb.rd;
            int wbValue  = (mem_wb.ctrl.output_sel == 2)
                        ? (mem_wb.pc + 4)
                        : mem_wb.write_data;

            writeback();

            web2 << "    { "
                << "\"instruction\": " << wbNum << ", "
                << "\"stage\": \"WriteBack\", "
                << "\"message\": "
                    << "\"Reg x"  << wbRd
                    << " = "     << wbValue
                << "\" },\n";
            


            stats.total_cycles++;
        }
    }

    web2.seekp(-2, ios::cur);
    web2<< "  \n{ \"cycle\": \"0\" }\n";
    web2 << "\n]\n";

    print_statistics();
    dump_stats_json();
    write_data_mc("data.mc");
}

int main() {
    string text_file = "text.mc";
    string data_file = "data.mc";

    configure_knobs();
    std::ofstream traceOut("output.txt");
    std::ofstream statsOut("stats.txt");
    std::ofstream web2("web2.txt");
    std::ofstream web3("web3.txt");
    initialize_simulator();

    if (!load_memory(text_file, data_file)) {
        traceOut << "Failed to load memory\n";
        return 1;
    }

    run_simulation();

    delete branch_predictor;
    branch_predictor = nullptr;
    cout<<"Check Output"<<endl;
    return 0;
}