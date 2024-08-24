#ifndef __PROGTEST__
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <climits>
#include <cfloat>
#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <variant>
#include <optional>
#include <compare>
#include <charconv>
#include <span>
#include <utility>
#include "expression.h"
using namespace std::literals;
using CValue = std::variant<std::monostate, double, std::string>;

constexpr unsigned                     SPREADSHEET_CYCLIC_DEPS                 = 0x01;
constexpr unsigned                     SPREADSHEET_FUNCTIONS                   = 0x02;
constexpr unsigned                     SPREADSHEET_FILE_IO                     = 0x04;
constexpr unsigned                     SPREADSHEET_SPEED                       = 0x08;
constexpr unsigned                     SPREADSHEET_PARSER                      = 0x10;
#endif /* __PROGTEST__ */

using namespace std;

class CPos
{
public:
    CPos() : columnIndex(0), row(0) {}
    // Existing constructor from string_view
    CPos(std::string_view str) {
        posString = str;
        parsePosition(str);
    }
    // New constructor that takes a position string and a shift
    CPos(std::string_view str, const CPos& shift) {
        parsePosition(str);
        // Apply the shift
        row += shift.row;
        columnIndex += shift.columnIndex;
        posString = str;

    }

    CPos(int rowNum, int columnNum) : columnIndex(columnNum), row(rowNum) {
        updateColumnFromIndex(); // Helper to update the column string from columnIndex
    }

    bool operator<(const CPos & other)const{
        if(columnIndex != other.getColumnIndex()){return columnIndex < other.getColumnIndex(); }
        return row < other.getRow();
    }
    int getColumnIndex() const { return columnIndex; }
    void setColumnIndex(int columnNum) {
        columnIndex = columnNum;
        updateColumnFromIndex(); // Update the column string anytime the column index changes
    }
    void setRow(int rowNum) { row  = rowNum;}
    int getRow() const { return row; }

    bool save(std::ostream& os) const {
        uint32_t strLength = static_cast<uint32_t>(toString().size());
        os.write(reinterpret_cast<const char*>(&strLength), sizeof(strLength));
        if ( os.fail()){
            return false;
        }
        os.write(toString().data(), strLength);
        if ( os.fail()){
            return false;
        }
        return true;
    }

    bool load(std::istream& is) {
        uint32_t strLength;
        is.read(reinterpret_cast<char*>(&strLength), sizeof(strLength));  // Read the length of the string
        if ( is.fail()){
            return false;
        }
        posString.resize(strLength);  // Resize the string to the correct size
        is.read(&posString[0], strLength);  // Read the string data
        if ( is.fail()){
            return false;
        }
        if ( ! parsePosition(posString)) {
            return false;
        }  // Update the internal state based on the loaded string
        return true;
    }

    std::string toString() const {
        return column + std::to_string(row );  // Excel-style 1-based index
    }
    bool operator==(const CPos& other) const {
        return (row == other.row && columnIndex == other.columnIndex);
    }

    // It's also common practice to provide the inequality operator
    bool operator!=(const CPos& other) const {
        return !(*this == other);
    }
private:
    std::string column;   // Optional, only if needed for other operations
    int columnIndex;      // Numeric index of the column
    int row;              // Stores the row as an integer
    string posString;


    bool parsePosition(std::string_view str) {//modify a bit to make it work for the relative
        if (str.empty() || !std::isalpha(str[0])) {
            cout << "Position must start with an alphabetical character. " << str <<  endl;
            return false;
        }

        size_t i = 0;
        while (i < str.size() && std::isalpha(str[i])) {
            i++;
        }

        if (i == str.size() || str.find_first_not_of("0123456789", i) != std::string_view::npos) {
            cout << ("Position must end with numerical digits after the alphabetical characters.") << endl;
            return false;
        }

        std::string_view columnStr = str.substr(0, i);
        std::string_view rowStr = str.substr(i);

        column = std::string(columnStr); // Storing the column as a string if needed elsewhere
        columnIndex = 0;
        int columnLen = columnStr.size();
        int power = 1;
        for(int i = columnLen - 1; i >= 0; i--) {
            columnIndex += (toupper(columnStr[i]) - 'A' + 1) * power;
            power *= 26;
        }
        row = std::stoi(std::string(rowStr));
        return true;
    }

    void updateColumnFromIndex() {
        column = ""; // Clear previous value
        int index = columnIndex;
        while (index > 0) {
            index--; // Adjust index to be 0-based
            char letter = 'A' + (index % 26);
            column = letter + column;
            index /= 26;
        }
    }

};

struct StringToken {
    string value;
};

struct NumberToken {
    double value;
};

struct OperatorToken {
    char op;
};

struct ReferenceToken { // TO DO make it have Creference info
    CPos position;
    bool isColumnAbsolute;
    bool isRowAbsolute;
    friend std::ostream& operator<<(std::ostream& os, const ReferenceToken& token) {
        os << "Cell pos: " << token.position.toString() << ", RowAbsolute: " << (token.isRowAbsolute ? "Yes" : "No")
           << ", ColumAbsolute: " << (token.isColumnAbsolute ? "Yes" : "No") ;
        return os;
    }
};

void strToReferenceToken(const string & ref, ReferenceToken & token){
    size_t idx = 0;
    token.isColumnAbsolute = (ref[idx] == '$');
    if (token.isColumnAbsolute) ++idx;

    // Extract the column part
    std::string columnPart;
    while (idx < ref.size() && std::isalpha(ref[idx])) {
        columnPart += std::toupper(ref[idx++]); // Convert to upper case for standardization
    }

    // Check for absolute row reference
    token.isRowAbsolute = (idx < ref.size() && ref[idx] == '$');
    if (token.isRowAbsolute) ++idx;

    // Extract the row part
    std::string rowPart;
    while (idx < ref.size() && std::isdigit(ref[idx])) {
        rowPart += ref[idx++];
    }
    string val = columnPart+rowPart;
    string_view val2 = val;
    token.position = CPos(val2);
}

struct NegationToken {}; // This token doesn't need any data

using Token = std::variant<NumberToken, OperatorToken, ReferenceToken, StringToken, NegationToken>;

bool operator==(const Token& lhs, const Token& rhs) {
    return std::visit([](const auto& arg1, const auto& arg2) -> bool {
        using T1 = std::decay_t<decltype(arg1)>;
        using T2 = std::decay_t<decltype(arg2)>;
        if constexpr (!std::is_same_v<T1, T2>) {
            // If types differ, they can't be equal
            return false;
        } else {
            // Compare based on type
            if constexpr (std::is_same_v<T1, NumberToken>) {
                return arg1.value == arg2.value;
            } else if constexpr (std::is_same_v<T1, StringToken>) {
                return arg1.value == arg2.value;
            } else if constexpr (std::is_same_v<T1, OperatorToken>) {
                return arg1.op == arg2.op;
            } else if constexpr (std::is_same_v<T1, ReferenceToken>) {
                // Assuming ReferenceToken can be compared based on position and absolutes
                return arg1.position == arg2.position &&
                       arg1.isColumnAbsolute == arg2.isColumnAbsolute &&
                       arg1.isRowAbsolute == arg2.isRowAbsolute;
            } else if constexpr (std::is_same_v<T1, NegationToken>) {
                // NegationToken likely does not have data to compare
                return true;
            }
        }
    }, lhs, rhs);
}


void printTokens(const std::vector<Token>& tokens) {
    for (const auto& token : tokens) {
        std::visit([](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, NumberToken>) {
                std::cout << "Number Token: " << arg.value << std::endl;
            } else if constexpr (std::is_same_v<T, OperatorToken>) {
                std::cout << "Operator Token: " << arg.op << std::endl;
            } else if constexpr (std::is_same_v<T, ReferenceToken>) {
                std::cout << "Reference Token: " << arg << std::endl;  // Directly use the overloaded operator<<
            } else if constexpr (std::is_same_v<T, StringToken>) {
                std::cout << "String Token: " << arg.value << std::endl;
            } else if constexpr (std::is_same_v<T, NegationToken>) {
                std::cout << "Negation Token" << std::endl;
            }
        }, token);
    }
}

class CCell {
public:
    CCell() {
        // Initialize members if necessary
    }

    // Virtual methods to be implemented by derived classes
    virtual bool saveCell(std::ostream &out) const = 0;

    virtual bool loadCell(std::istream &in) = 0;

    virtual CValue getValue() const = 0;

    virtual shared_ptr<CCell> clone() const = 0;

    virtual void printDetails() const = 0;

};

void printCValue(const CValue& value) {
    std::visit([](const auto& v) {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, std::monostate>) {
            std::cout << "Empty value" << std::endl;
        } else if constexpr (std::is_same_v<T, double>) {
            std::cout << "Double: " << v << std::endl;
        } else if constexpr (std::is_same_v<T, std::string>) {
            std::cout << "String: " << v << std::endl;
        }
    }, value);
}

class CCellDouble : public CCell {
private:
    double value;

public:
    CCellDouble(double val) : CCell(), value(val) {}

    void printDetails() const override {
        printCValue(getValue());
    }

    bool saveCell(std::ostream& out) const override {
        unsigned char type = 1;  // Type byte for double
        out.write(reinterpret_cast<const char*>(&type), sizeof(type));  // Write the type byte
        if ( out.fail()){
            return false;
        }

        out.write(reinterpret_cast<const char*>(&value), sizeof(value));  // Write the double in binary form
        if ( out.fail()){
            return false;
        }
        return true;
    }

    bool loadCell(std::istream& in) override {
        in.read(reinterpret_cast<char*>(&value), sizeof(value));  // Read the double in binary form
        if ( in.fail()){
            return false;
        }
        return true;

    }

    CValue getValue() const override {  // ensure 'const' is used
        return CValue(value);
    }


    shared_ptr<CCell> clone() const override {
        // Just copy the value, no need to adjust anything
        return make_shared<CCellDouble>(value);
    }
};


class CCellString : public CCell {
private:
    std::string value;

public:
    CCellString(const std::string& val) : value(val) {}
    void printDetails() const override {
        printCValue(getValue());
    }

    bool saveCell(std::ostream& out) const override {
        unsigned char type = 2; // Type byte for strings
        out.write(reinterpret_cast<const char*>(&type), sizeof(type)); // Write the type byte
        if (out.fail()){
            return false;
        }

        size_t length = value.size();
        out.write(reinterpret_cast<const char*>(&length), sizeof(length)); // Write the length of the string
        if ( out.fail()){
            return false;
        }

        out.write(value.data(), length); // Write the string itself
        if ( out.fail()){
            return false;
        }
        return true;
    }

    bool loadCell(std::istream& in) override {
        size_t length;
        in.read(reinterpret_cast<char*>(&length), sizeof(length)); // Read the length of the string
        if ( in.fail()){
            return false;
        }

        value.resize(length); // Resize the string to hold the incoming data
        in.read(&value[0], length); // Read the string data
        if ( in.fail()){
            return false;
        }
        return true;
    }

    CValue getValue() const override {
        return CValue(value);
    }


    shared_ptr<CCell> clone() const override {
        // Just copy the string, no need to adjust anything
        return make_shared<CCellString>(value);
    }

};

class CCellExpression : public CCell {
private:

    std::vector<Token> expressionTokens;
    std::string expression;
    std::map<CPos, shared_ptr<CCell>>& dataSheet;
public:

    explicit CCellExpression(std::map<CPos, shared_ptr<CCell>>& sheet) : dataSheet(sheet){}

    bool saveTokens(std::ostream& out) const ;
    bool loadTokens(std::istream& in);

    CValue evaluateExpression()const;

    bool saveCell(std::ostream &out) const override {
        unsigned char type = 3; // Type to the exppressions
        out.write(reinterpret_cast<const char*>(&type), sizeof(type)); // Write the type byte
        if(out.fail()){
            return false;
        }
        saveTokens(out);
        return true;
    }


    bool loadCell(std::istream &in) override {
        if(!loadTokens(in)){
            return false;
        }
        return true;
    }

    const std::vector<Token>& getTokens() const { return expressionTokens; }

    CValue getValue() const override {
        try {
            return evaluateExpression();
        } catch (const std::exception& e) {
            return CValue();
        }
    }

    void printDetails() const override {
        std::cout << "Type: CCellExpression, Expression: " << std::endl;
        cout << "Expression = " << expression << endl;
        printCValue(getValue());
        cout << "TOKENS" << endl;
        printTokens(expressionTokens);
    }

    std::string& getExpression(){return expression;}
    void setToken(vector<Token> &tokens){expressionTokens = tokens;}
    void setExpression(const string & str){expression = str;}

    shared_ptr<CCell> clone(std::map<CPos, shared_ptr<CCell>>& sheet) const  {
        auto newCell = make_shared<CCellExpression>(sheet);
        newCell->setExpression(expression);  // Copy the expression string
        vector<Token> newTokens(expressionTokens);  // Deep copy of tokens
        newCell->setToken(newTokens);
        return newCell;
    }

    shared_ptr<CCell> clone() const override {
        return nullptr;
    }

    shared_ptr<CCell> clone(const CPos& shift) const {
        auto newCell = make_shared<CCellExpression>(dataSheet);
//        newCell->setExpression(expression); // Copy the expression string

        vector<Token> newTokens;
        for (const auto& token : expressionTokens) {
            std::visit([&](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, ReferenceToken>) {
                    ReferenceToken newToken = arg;
                    if (!newToken.isRowAbsolute) {
                        newToken.position.setRow(newToken.position.getRow() + shift.getRow());
                    }
                    if (!newToken.isColumnAbsolute) {
                        newToken.position.setColumnIndex(newToken.position.getColumnIndex() + shift.getColumnIndex());
                    }
                    newTokens.push_back(newToken);
                } else {
                    newTokens.push_back(arg);
                }
            }, token);
        }

        newCell->setToken(newTokens);  // Set the cloned and potentially adjusted tokens
        return newCell;
    }

    void handleOperation(char op, const CValue& left, const CValue& right, std::stack<CValue>& stack) const;

};
using CellMap = std::map<CPos, std::shared_ptr<CCell>>;

double getDoubleFromCValue(const CValue& value) {
    return std::visit([](auto&& arg) -> double {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, double>) {
            return arg;  // Directly return the double
        } else if constexpr (std::is_same_v<T, std::string>) {
            // Convert string to double, or handle as error
            try {
                return std::stod(arg);
            } catch (const std::invalid_argument& ia) {
                throw std::runtime_error("String could not be converted to double");
            }
        } else {
            throw std::runtime_error("Value is not a double or convertible string");
        }
    },value);
}

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

void CCellExpression::handleOperation(char op, const CValue& left, const CValue& right, std::stack<CValue>& stack) const {
    std::visit(overloaded{
            [&](double l, double r) {
                switch (op) {
                    case '+': stack.push(l + r); break;
                    case '*': stack.push(l * r); break;
                    case '-': stack.push(l - r); break;
                    case '/': if (r == 0) throw std::runtime_error("Division by zero.");
                        stack.push(l / r); break;
                    case '^': stack.push(std::pow(l, r)); break;
                    case '=': stack.push(l == r ? 1.0 : 0.0); break;
                    case '!': stack.push(l != r ? 1.0 : 0.0); break;
                    case '<': stack.push(l < r ? 1.0 : 0.0); break;
                    case 'L': stack.push(l <= r ? 1.0 : 0.0); break;
                    case '>': stack.push(l > r ? 1.0 : 0.0); break;
                    case 'G': stack.push(l >= r ? 1.0 : 0.0); break;
                    default:  throw std::runtime_error("Unsupported operation for doubles.");
                }
            },
            [&](const std::string& l, double r) {
                if (op == '+') stack.push(l + std::to_string(r));
                else throw std::runtime_error("Invalid operation for string and double.");
            },
            [&](double l, const std::string& r) {
                if (op == '+') stack.push(std::to_string(l) + r);
                else throw std::runtime_error("Invalid operation for double and string.");
            },
            [&](const std::string& l, const std::string& r) {
                switch (op) {
                    case '+': stack.push(l + r); break;  // Concatenation for strings
                    case '=': stack.push(l == r ? 1.0 : 0.0); break;
                    case '!': stack.push(l != r ? 1.0 : 0.0); break;
                    case '<': stack.push(l < r ? 1.0 : 0.0); break;
                    case 'L': stack.push(l <= r ? 1.0 : 0.0); break;
                    case '>': stack.push(l > r ? 1.0 : 0.0); break;
                    case 'G': stack.push(l >= r ? 1.0 : 0.0); break;
                    default:  throw std::runtime_error("Unsupported operation for strings.");
                }
            },
            [&](auto, auto) { throw std::runtime_error("Unsupported types for operation."); }
    }, left, right);
}


CValue CCellExpression::evaluateExpression() const {
    std::stack<CValue> evalStack;

    for (const auto& token : expressionTokens) {
        std::visit(overloaded{
                [&](const NumberToken& num) { evalStack.push(num.value); },
                [&](const StringToken& str) { evalStack.push(str.value); },
                [&](const OperatorToken& op) {
                    if (evalStack.size() < 2) throw std::runtime_error("Insufficient values for operation.");
                    CValue right = evalStack.top(); evalStack.pop();
                    CValue left = evalStack.top(); evalStack.pop();
                    handleOperation(op.op, left, right, evalStack);
                },
                [&](const NegationToken& ne){
                    (void)ne;
                    if (evalStack.size() < 1) throw std::runtime_error("No operand available for negation.");
                    CValue value = evalStack.top(); evalStack.pop();
                    std::visit(overloaded{
                            [&](double val) { evalStack.push(-val); },
                            [](auto&) { throw std::runtime_error("Negation only applicable to numbers."); }
                    }, value);
                },
                [&](const ReferenceToken& ref) {
                    auto it = dataSheet.find(ref.position);
                    if (it == dataSheet.end()) throw std::runtime_error("Reference to undefined cell.");
                    evalStack.push(it->second->getValue()); // Assuming CCell has a method to get its value as double
                }
        }, token);
    }

    if (evalStack.size() != 1) {
        throw std::runtime_error("The expression does not evaluate to a single value.");
    }

    return evalStack.top();
}

class CExpressionBuilderImpl : public CExprBuilder{
private:
    std::vector<Token> expressionTokens;
    map<CPos, shared_ptr<CCell>>& dataSheet;
    CPos shift;
    set<CPos>references;
public:

    CExpressionBuilderImpl(map<CPos, shared_ptr<CCell>>  sheet, const CPos&  shift = CPos(0,0)) : dataSheet(sheet), shift(shift){}

    void opAdd() override{
        expressionTokens.push_back(OperatorToken{ '+' });
    }

    void opSub() override{
        expressionTokens.push_back(OperatorToken{ '-' });
    }

    void opMul() override{
        expressionTokens.push_back(OperatorToken{ '*' });
    }

    void opDiv() override{
        expressionTokens.push_back(OperatorToken{ '/' });
    }

    void opPow() override{
        expressionTokens.push_back(OperatorToken{ '^' });

    }

    void opNeg() override{
        expressionTokens.emplace_back(NegationToken{});
    }

    void opEq() override{
        expressionTokens.push_back(OperatorToken{ '=' });
    }

    void opNe() override{
        expressionTokens.push_back(OperatorToken{ '!' });
    }
    void opLt() override{
        expressionTokens.push_back(OperatorToken{ '<' });
    }

    void opLe() override{
        expressionTokens.push_back(OperatorToken{ 'L' });
    }

    void opGt() override{
        expressionTokens.push_back(OperatorToken{ '>' });
    }

    void opGe() override{
        expressionTokens.push_back(OperatorToken{ 'G' });
    }

    void valNumber(double val) override {
        expressionTokens.push_back(NumberToken{ val });
    }

    void valString(std::string val) override {
        expressionTokens.push_back(StringToken{ val });
    }

    void  valReference(std::string val) override
    {
        ReferenceToken refToken;
        strToReferenceToken(val, refToken);
        expressionTokens.push_back(refToken);
        // parse the val and create
        references.insert(refToken.position);
    }

    void valRange(std::string val) override {
    }

    void funcCall(std::string fnName, int paramCount) override {

    }


    std::vector<Token>& getExpressionTokens() {
        return expressionTokens;
    }

    std::set<CPos> & getReference()  {
        return references;
    }

};
bool isDouble(const std::string& s, double& outValue) {
    std::istringstream iss(s);
    iss >> outValue;
    return iss.eof() && !iss.fail();  // Ensure entire string was used and successfully parsed
}
class CSpreadsheet
{
public:
    void printDetails(){
        for (const auto& entry : references) {
            std::cout << "Cell " << entry.first.toString() << " references: ";
            if (entry.second.empty()) {
                std::cout << "No references";
            } else {
                for (const auto& ref : entry.second) {
                    std::cout << ref.toString() << " ";
                }
            }
            std::cout << std::endl;
        }
    }
    void printSpreadsheetDetails();
    bool saveDependencies(std::ostream& out) const;
    bool loadDependencies(std::istream& in);
    // Copy Assignment Operator
    CSpreadsheet& operator=(const CSpreadsheet& other) {
        if (this != &other) {  // Protection against self-assignment
            // Clear existing data
            cells.clear();
            references.clear();

            // Debug output
//            std::cout << "Starting deep copy of cells and references...\n";

            // Deep copy cells
            for (const auto& item : other.cells) {
                std::shared_ptr<CCell> cell = item.second;
                if (auto expressionCell = std::dynamic_pointer_cast<CCellExpression>(cell)) {
                    // It's a CCellExpression, handle specifically
                    cells[item.first] = expressionCell->clone(cells);;
                } else {
                    // Normal deep copy for other types of cells
                    cells[item.first] = cell->clone();
                }
            }

            // Deep copy references
            references = other.references;

//            std::cout << "Spreadsheet deep copy completed successfully.\n";
        }
        return *this;
    }

    // Destructor
    ~CSpreadsheet() {
        // Clean up resources if necessary
    }

    static unsigned                    capabilities                            ()
    {
        return SPREADSHEET_CYCLIC_DEPS ;
    }
    bool                               load                                    ( std::istream                        & is );
    bool                               save                                    ( std::ostream                        & os ) const;
    bool                               setCell                                 ( CPos                                  pos,
                                                                                 std::string                           contents );
    CValue                             getValue                                ( CPos                                  pos );
    void                               copyRect                                ( CPos                                  dst,
                                                                                 CPos                                  src,
                                                                                 int                                   w = 1,
                                                                                 int                                   h = 1 );

    map<CPos, shared_ptr<CCell>>& get_cells(){return cells;}
    bool detectCyclicDependencies(const CPos& startPosition);
    map<CPos, shared_ptr<CCell>> cells;
    map<CPos, set<CPos>> references;
    bool exists(const string& contents, CellMap::iterator it) {

        if (it == cells.end()) {
            return false;  // No cell at the given position.
        }
        // Retrieve the cell and compare based on its type.
        const auto &cell = it->second;
        if (auto doubleCell = dynamic_pointer_cast<CCellDouble>(cell)) {
            // Check if contents can be converted to double and compare.
            try {
                double value = stod(contents);
                return doubleCell->getValue() == CValue(value);
            } catch (const invalid_argument &e) {
                return false;  // Contents is not a double.
            }
        } else if (auto stringCell = dynamic_pointer_cast<CCellString>(cell)) {
            // Compare the string directly.
            return stringCell->getValue() == CValue(contents);
        }
        return false;
    }
private:
};

bool CSpreadsheet::save( std::ostream & os) const {
    if ( ! saveDependencies(os)){
        cout << "dependencies error " << endl;
        return false;
    }
    uint32_t numCells = cells.size();
    os.write(reinterpret_cast<const char*>(&numCells), sizeof(numCells));
    if ( os.fail()){
        cout << "write number of cells " << endl;
        return false;
    }
    for (const auto &pair : cells) {
        pair.first.save(os);
        pair.second->saveCell(os);
    }
    return true;
}


bool CSpreadsheet::load(std::istream &is) {
    if ( ! loadDependencies(is)){
//        cout << "error dependencies" << endl;
        return false;
    }
//    cout << "after dependencies " << endl;
    uint32_t numCells;
    is.read(reinterpret_cast<char*>(&numCells), sizeof(numCells));
    if ( is.fail()){
        return false;
    }
    cells.clear();  // Clear existing cells
    while (numCells--) {
        CPos pos;
        if ( ! pos.load(is)) {
            return false;
        }
        // Read the type of the cell
        uint8_t cellType;
        is.read(reinterpret_cast<char*>(&cellType), sizeof(cellType));
        if ( is.fail()){
            return false;
        }
        std::shared_ptr<CCell> cell;
        switch (cellType) {
            case 1:  // CCellDouble
                cell = std::make_shared<CCellDouble>(0);
                break;
            case 2:  // CCellString
                cell = std::make_shared<CCellString>("");
                break;
            case 3:  // CCellExpression
                cell = std::make_shared<CCellExpression>(cells);
                break;
            default:
                throw std::runtime_error("Unknown cell type encountered.");
        }

        cell->loadCell(is);  // Load the cell data
        cells[pos] = cell;  // Insert the cell into the map
    }
    return true;
}


//void CSpreadsheet::printSpreadsheetDetails() {
//    for (const auto& pair : cells) {
//        cout << "Cell Position: " << pair.first.toString() << endl;
//
//        // Attempt to cast to each known cell type and print details
//        auto cellDouble = dynamic_pointer_cast<CCellDouble>(pair.second);
//        auto cellString = dynamic_pointer_cast<CCellString>(pair.second);
//        auto cellExpression = dynamic_pointer_cast<CCellExpression>(pair.second);
//
//        if (cellDouble) {
//            cout << "Double Value: ";
//            cellDouble->printDetails();
//        } else if (cellString) {
//            cout << "String Value: ";
//            cellString->printDetails();
//        } else if (cellExpression) {
//            cout << "Expression Details: ";
//            cellExpression->printDetails();
//            cout << "set<Cpos>" << endl;
//            for( auto ref : references[pair.first]){
//                cout << ref.toString() << " " ;
//            }
//
//        } else {
//            cout << "{}"; // Print empty brackets for unrecognized cell types
//        }
//
//        cout << endl << "##################################" << endl << endl;
//    }
//}

bool CSpreadsheet::setCell(CPos pos, std::string contents) {

    if (!contents.empty() && contents[0] == '=') {
        try {
            CExpressionBuilderImpl builder(cells);
            parseExpression(contents, builder);  // create stack
            //store the stack and references in CCell
            CCellExpression cell(cells);
            cell.setToken(builder.getExpressionTokens());
            //store in map<CPos, shared_ptr<CCell>>
            cells[pos] = make_shared<CCellExpression>(cell);
            references[pos]=  builder.getReference();
//            cout << "got value" << getDoubleFromCValue( getValue(CPos("A2"))) << endl;
            // Add dependencies for the new expression
        } catch (const std::exception& e) {
            std::cerr << "Error evaluating expression: " << e.what() << std::endl;
            return false;
        }
    } else {
        // raw value
        double value;
        if(isDouble(contents, value)) {
            cells[pos] = make_shared<CCellDouble>(value);
            references[pos]= {};

        } else {
            cells[pos] = make_shared<CCellString>(contents);
            references[pos]= {};
        }

    }
    return true;
}

CValue                             CSpreadsheet::getValue                                ( CPos                                  pos ){

    auto it = cells.find(pos);
    if(it == cells.end()){
        return CValue{};
    }
    if(detectCyclicDependencies(pos)) {
        return CValue{};
    }

    return it->second->getValue();

}

bool CSpreadsheet::detectCyclicDependencies(const CPos& start) {
    std::set<CPos> visited;
    std::set<CPos> recStack;

    std::function<bool(const CPos&)> dfs = [&](const CPos& pos) {
        if (recStack.find(pos) != recStack.end()) {
            return true; // Cycle detected
        }

        if (visited.find(pos) != visited.end()) {
            return false; // Already visited this node and no cycle from this path
        }
        visited.insert(pos);
        recStack.insert(pos);
        std::set<CPos> refs = references[pos];
        for (const CPos& ref : refs) {
            if (dfs(ref)) {
                return true; // Cycle detected in DFS from the adjacent vertex
            }
        }

        recStack.erase(pos);
        return false;
    };

    // Start DFS from the given position
    return dfs(start);
}

std::set<CPos> getReferencesFromTokens(shared_ptr<CCell> cell) {
    auto casted = std::dynamic_pointer_cast<CCellExpression>(cell);
    const auto& tokens = casted->getTokens();
    std::set<CPos> references;

    // Loop through each token and check if it is a ReferenceToken
    for (const auto& token : tokens) {
        if (std::holds_alternative<ReferenceToken>(token)) {
            // Extract the position from the ReferenceToken and add to the set
            const ReferenceToken& refToken = std::get<ReferenceToken>(token);
            references.insert(refToken.position);
        }
    }

    return references;
}
void CSpreadsheet::copyRect(CPos dst, CPos src, int w, int h) {
    if (w <= 0 || h <= 0) return; // Safety check for non-positive width or height

//    cout << "Copying rectangle from " << src.toString() << " to " << dst.toString()
//         << " with width " << w << " and height " << h << endl;

    // Create a temporary storage to hold the cells that need to be copied
    vector<pair<CPos, shared_ptr<CCell>>> tempCells;
    vector<pair<CPos, set<CPos>>> tempReferences;
    // First, collect all cells to be copied to avoid modifying the source cells directly
    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < h; ++j) {
            CPos srcPos(src.getRow() + j, src.getColumnIndex() + i);
            if (cells.count(srcPos) > 0) { // Check if source cell exists
                auto srcCell = cells[srcPos];
                CPos dstPos(dst.getRow() + j, dst.getColumnIndex() + i);
                CPos shift(dstPos.getRow() - srcPos.getRow(), dstPos.getColumnIndex() - srcPos.getColumnIndex());
                auto casted = std::dynamic_pointer_cast<CCellExpression>(srcCell);
                shared_ptr<CCell> clonedCell;
                set<CPos> referenceSet;
                if(casted) {
                    clonedCell = casted->clone(shift);
                    referenceSet = getReferencesFromTokens(clonedCell);//shifted references of clonedCell
                }else{
                    clonedCell = srcCell->clone();
                }
                tempCells.push_back({dstPos, clonedCell});
                //from cloned Cell
                tempReferences.push_back({dstPos, referenceSet});
            }
        }
    }

    // Now place all the cloned cells into their new positions
    for (auto& cell : tempCells) {
        cells[cell.first] = cell.second;
    }
    for(auto & cell : tempReferences){
        references[cell.first] = cell.second;
    }

}

bool CSpreadsheet::saveDependencies(std::ostream& os) const {
    uint32_t numEntries = references.size();
    os.write(reinterpret_cast<const char*>(&numEntries), sizeof(numEntries));
    if ( os.fail()){
        return false;
    }
    for (const auto& pair : references) {
        // Use the save method of CPos
        if ( ! pair.first.save(os)){
            return false;
        }

        // Serialize the set size
        uint32_t setSize = pair.second.size();
        os.write(reinterpret_cast<const char*>(&setSize), sizeof(setSize));
        if ( os.fail()){
            return false;
        }
        // Serialize each position in the set using CPos save method
        for (const CPos& pos : pair.second) {
            if ( ! pos.save(os)){
                return false;
            }
        }
    }
    return true;
}



bool CSpreadsheet::loadDependencies(std::istream& is) {
    uint32_t numEntries;
    is.read(reinterpret_cast<char*>(&numEntries), sizeof(numEntries));
    if (is.fail()){
        return false;
    }
    while (numEntries--) {
        CPos key;
        if ( ! key.load(is)){
            return false;
        }  // Load using CPos's load method

        uint32_t setSize;
        is.read(reinterpret_cast<char*>(&setSize), sizeof(setSize));
        if (is.fail()){
            return false;
        }
        std::set<CPos> posSet;

        while (setSize--) {
            CPos pos;
            if ( ! pos.load(is) ) {
                return false;
            }  // Load using CPos's load method
            posSet.insert(std::move(pos));
        }

        references[key] = std::move(posSet);
    }
    return true;
}


bool CCellExpression::saveTokens(std::ostream& out) const {
    std::stringstream buffer;
    bool badFlag = false;
    for (const auto& token : expressionTokens) {
        std::visit([&](const auto& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, NumberToken>) {
                buffer.put(1); // NUMBER
                buffer.write(reinterpret_cast<const char*>(&arg.value), sizeof(arg.value));
                if (buffer.fail()) {
                    badFlag = true;  // Set flag on error
                }
            } else if constexpr (std::is_same_v<T, OperatorToken>) {
                buffer.put(2); // OPERATOR
                buffer.put(arg.op);
                if (buffer.fail()) {
                    badFlag = true;  // Set flag on error
                }
            } else if constexpr (std::is_same_v<T, ReferenceToken>) {
                buffer.put(3); // REFERENCE
                arg.position.save(buffer);  // Assume save() will internally manage its own errors
                buffer.write(reinterpret_cast<const char*>(&arg.isColumnAbsolute), sizeof(arg.isColumnAbsolute));
                buffer.write(reinterpret_cast<const char*>(&arg.isRowAbsolute), sizeof(arg.isRowAbsolute));
                if (buffer.fail()) {
                    badFlag = true;  // Set flag on error
                }
            } else if constexpr (std::is_same_v<T, NegationToken>) {
                buffer.put(4); // NEGATION
                if (buffer.fail()) {
                    badFlag = true;  // Set flag on error
                }
            }
        }, token);
        if (badFlag) break;  // Exit early on first error
    }

    if (badFlag) {
        cout << "BAD FLAG FROM SAVE TOKENS" << endl;
        return false;
    }

    std::string data = buffer.str();
    std::size_t totalLength = data.size();
    out.write(reinterpret_cast<const char*>(&totalLength), sizeof(totalLength));
    if (out.fail()){
        return false;
    }
    out.write(data.data(), totalLength);
    if (out.fail()){
        return false;
    }
    return true;
}


bool CCellExpression::loadTokens(std::istream& in) {
    std::size_t totalLength;
    in.read(reinterpret_cast<char*>(&totalLength), sizeof(totalLength));
    if ( in.fail()){
        return false;
    }


    std::vector<char> buffer(totalLength);
    in.read(buffer.data(), totalLength);
    if ( in.fail()){
        return false;
    }

    std::istringstream dataStream(std::string(buffer.begin(), buffer.end()));

    while (dataStream.tellg() < static_cast<std::streampos>(totalLength)) {
        int type = dataStream.get();
        switch (type) {
            case 1: { // NUMBER
                double value;
                dataStream.read(reinterpret_cast<char*>(&value), sizeof(value));
                if ( dataStream.fail()){
                    return false;
                }
                expressionTokens.emplace_back(NumberToken{value});
                break;
            }
            case 2: { // OPERATOR
                char op = dataStream.get();
                expressionTokens.emplace_back(OperatorToken{op});
                break;
            }
            case 3: { // REFERENCE
                CPos pos;
                if ( ! pos.load(dataStream)){
                    return false;
                }
                bool colAbs, rowAbs;
                dataStream.read(reinterpret_cast<char*>(&colAbs), sizeof(colAbs));
                if ( dataStream.fail()){
                    return false;
                }
                dataStream.read(reinterpret_cast<char*>(&rowAbs), sizeof(rowAbs));
                if ( dataStream.fail()){
                    return false;
                }
                expressionTokens.emplace_back(ReferenceToken{pos, colAbs, rowAbs});
                break;
            }
            case 4: // NEGATION
                expressionTokens.emplace_back(NegationToken{});
                break;
            default:
                std::cerr << "Unknown token type encountered: " << type << std::endl;
                return false;
        }
    }
    return true;
}
#ifndef __PROGTEST__

bool                                   valueMatch                              ( const CValue                        & r,
                                                                                 const CValue                        & s )

{
    if ( r . index () != s . index () )
        return false;
    if ( r . index () == 0 )
        return true;
    if ( r . index () == 2 )
        return std::get<std::string> ( r ) == std::get<std::string> ( s );
    if ( std::isnan ( std::get<double> ( r ) ) && std::isnan ( std::get<double> ( s ) ) )
        return true;
    if ( std::isinf ( std::get<double> ( r ) ) && std::isinf ( std::get<double> ( s ) ) )
        return ( std::get<double> ( r ) < 0 && std::get<double> ( s ) < 0 )
               || ( std::get<double> ( r ) > 0 && std::get<double> ( s ) > 0 );
    return fabs ( std::get<double> ( r ) - std::get<double> ( s ) ) <= 1e8 * DBL_EPSILON * fabs ( std::get<double> ( r ) );
}



void testExpressionHandling() {
    CSpreadsheet sheet;
    sheet.cells[CPos("A1")] = std::make_shared<CCellDouble>(3.14159);

    CExpressionBuilderImpl builder(sheet.cells);
//    parseExpression("=A1 + A2 * 3", builder);  // create stack
    parseExpression("=A1 + 2", builder);  // create stack
    //store the stack and references in CCell
    CCellExpression cell(sheet.cells);

    cell.setToken(builder.getExpressionTokens());
    //store in map<CPos, shared_ptr<CCell>>
    sheet.cells[CPos("C1")] = make_shared<CCellExpression>(cell);
    sheet.references.emplace(CPos("C1"), builder.getReference());


    // Save to a file
    std::ofstream outFile("spreadsheet_with_expressions.bin", std::ios::binary);
    sheet.save(outFile);
    outFile.close();

    // Load from a file
    std::ifstream inFile("spreadsheet_with_expressions.bin", std::ios::binary);
    CSpreadsheet loadedSheet;
    loadedSheet.load(inFile);
    inFile.close();

    // Display loaded expressions and evaluate them
    for (const auto& pair : loadedSheet.cells) {
        auto exprCell = std::dynamic_pointer_cast<CCellExpression>(pair.second);
        if (exprCell) {
            printCValue(exprCell->evaluateExpression());
        }
    }
}

void testDFS(){
    CSpreadsheet sheet;
    assert(sheet.setCell(CPos("A15"), "=A1"));
    assert ( sheet . setCell ( CPos ( "A1" ), "15" ) );

    assert ( valueMatch ( sheet . getValue ( CPos ( "A15" ) ), CValue () ) );
//    printCValue(sheet.getValue(CPos("A15")));
}


void test_CopyRect() {
    CSpreadsheet sheet;
    sheet.setCell(CPos("A1"), "=A2 + A3");
    sheet.setCell(CPos("A4"), "10");
    sheet.setCell(CPos("C1"), "10");
    sheet.setCell(CPos("C2"), "10");

    sheet.copyRect(CPos("A4"), CPos("A1"), 1, 3); // Copy C1 to C2

    assert(valueMatch(sheet.getValue(CPos("A1")), CValue(15.0))); // C1 should now be 35
    assert(valueMatch(sheet.getValue(CPos("B1")), CValue())); // C2 should remain 30 since it should refer to original cells
}
void testCyclicDependencies() {
    CSpreadsheet sheet;

    // Scenario 1: Direct cycle
    assert(sheet.setCell(CPos("A1"), "=B1"));
    assert(sheet.setCell(CPos("B1"), "=A1"));
    assert(sheet.detectCyclicDependencies(CPos("A1")) == true);

    // Scenario 2: Longer cycle
    assert(sheet.setCell(CPos("C1"), "=D1"));
    assert(sheet.setCell(CPos("D1"), "=E1"));
    assert(sheet.setCell(CPos("E1"), "=C1"));
    assert(sheet.detectCyclicDependencies(CPos("C1")) == true);

    // Scenario 3: No cycle (but linked cells)
    assert(sheet.setCell(CPos("F1"), "=G1"));
    assert(sheet.setCell(CPos("G1"), "20"));
    assert(sheet.detectCyclicDependencies(CPos("F1")) == false);

    // Scenario 4: Self-reference (immediate cycle)
    assert(sheet.setCell(CPos("H1"), "=H1"));
    assert(sheet.detectCyclicDependencies(CPos("H1")) == true);

    // Output for debugging
    cout << "Test Cyclic Dependencies: All tests passed." << endl;
}
void testCyclicDependencyDetection() {
    CSpreadsheet sheet;

    sheet.setCell(CPos("A1"), "=B1");
    sheet.setCell(CPos("B1"), "=A1"); // Creates a cycle with A1

    assert(sheet.detectCyclicDependencies(CPos("A1")) == true);
    cout << "b1########################################" << endl;
    for( auto ref : sheet.references[CPos("B1")]){
        cout << ref.toString() << " " ;
    }
    cout << "\n" ;
    cout << "####################################################" << endl;

sheet.setCell(CPos("B1"), "10"); // Breaks the cycle
    cout << "AFTERRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR" << endl;
    for( auto ref : sheet.references[CPos("B1")]){
        cout << ref.toString() << " " ;
    }
    cout << "\n" ;

    cout << "####################################################" << endl;

    assert(sheet.detectCyclicDependencies(CPos("A1")) == false); // Should now return false
}

int main ()
{

    CSpreadsheet x0, x1;
    std::ostringstream oss;
    std::istringstream iss;
    std::string data;

    assert(x0. setCell ( CPos ( "D0" ), "=A0+((B0*(-C0)))" ));
    assert(x0. setCell ( CPos ( "D0" ), "=A0+((B0*(-C0)))" ));
    assert ( x0 . setCell ( CPos ( "A1" ), "10" ) );
    assert ( x0 . setCell ( CPos ( "A2" ), "20.5" ) );
    assert ( x0 . setCell ( CPos ( "A3" ), "3e1" ) );
    assert ( x0 . setCell ( CPos ( "A4" ), "=40" ) );
    assert ( x0 . setCell ( CPos ( "A5" ), "=5e+1" ) );
    assert ( x0 . setCell ( CPos ( "A6" ), "raw text with any characters, including a quote \" or a newline\n" ) );
    assert ( x0 . setCell ( CPos ( "A7" ), "=\"quoted string, quotes must be doubled: \"\". Moreover, backslashes are needed for C++.\"" ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "A1" ) ), CValue ( 10.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "A2" ) ), CValue ( 20.5 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "A3" ) ), CValue ( 30.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "A4" ) ), CValue ( 40.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "A5" ) ), CValue ( 50.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "A6" ) ), CValue ( "raw text with any characters, including a quote \" or a newline\n" ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "A7" ) ), CValue ( "quoted string, quotes must be doubled: \". Moreover, backslashes are needed for C++." ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "A8" ) ), CValue() ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "AAAA9999" ) ), CValue() ) );
    assert ( x0 . setCell ( CPos ( "B1" ), "=A1+A2*A3" ) );
    assert ( x0 . setCell ( CPos ( "B2" ), "= -A1 ^ 2 - A2 / 2   " ) );
    assert ( x0 . setCell ( CPos ( "B3" ), "= 2 ^ $A$1" ) );
    assert ( x0 . setCell ( CPos ( "B4" ), "=($A1+A$2)^2" ) );
    assert ( x0 . setCell ( CPos ( "B5" ), "=B1+B2+B3+B4" ) );
    assert ( x0 . setCell ( CPos ( "B6" ), "=B1+B2+B3+B4+B5" ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B1" ) ), CValue ( 625.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B2" ) ), CValue ( -110.25 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B3" ) ), CValue ( 1024.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B4" ) ), CValue ( 930.25 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B5" ) ), CValue ( 2469.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B6" ) ), CValue ( 4938.0 ) ) );
    assert ( x0 . setCell ( CPos ( "A1" ), "12" ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B1" ) ), CValue ( 627.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B2" ) ), CValue ( -154.25 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B4" ) ), CValue ( 1056.25 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B5" ) ), CValue ( 5625.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B6" ) ), CValue ( 11250.0 ) ) );
    x1 = x0;
    assert ( x0 . setCell ( CPos ( "A2" ), "100" ) );
    assert ( x1 . setCell ( CPos ( "A2" ), "=A3+A5+A4" ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B1" ) ), CValue ( 3012.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B2" ) ), CValue ( -194.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B4" ) ), CValue ( 12544.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B5" ) ), CValue ( 19458.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "B6" ) ), CValue ( 38916.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "B1" ) ), CValue ( 3612.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "B2" ) ), CValue ( -204.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "B4" ) ), CValue ( 17424.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "B5" ) ), CValue ( 24928.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "B6" ) ), CValue ( 49856.0 ) ) );
    oss . clear ();
    oss . str ( "" );
    assert ( x0 . save ( oss ) );
    data = oss . str ();

    iss . clear ();
    iss . str ( data );

    assert ( x1 . load ( iss ) );

    assert ( valueMatch ( x1 . getValue ( CPos ( "B1" ) ), CValue ( 3012.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "B2" ) ), CValue ( -194.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "B4" ) ), CValue ( 12544.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "B5" ) ), CValue ( 19458.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "B6" ) ), CValue ( 38916.0 ) ) );
    assert ( x0 . setCell ( CPos ( "A3" ), "4e1" ) );
    assert ( valueMatch ( x1. getValue ( CPos ( "B1" ) ), CValue ( 3012.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "B2" ) ), CValue ( -194.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "B4" ) ), CValue ( 12544.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "B5" ) ), CValue ( 19458.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "B6" ) ), CValue ( 38916.0 ) ) );

    oss . clear ();
    oss . str ( "" );
    assert ( x0 . save ( oss ) );
    data = oss . str ();
    for ( size_t i = 0; i < std::min<size_t> ( data . length (), 10 ); i ++ )
        data[i] ^=0x5a;
    iss . clear ();
    iss . str ( data );
    assert ( ! x1 . load ( iss ) );

    assert ( x0 . setCell ( CPos ( "D0" ), "10" ) );
    assert ( x0 . setCell ( CPos ( "D1" ), "20" ) );
    assert ( x0 . setCell ( CPos ( "D2" ), "30" ) );
    assert ( x0 . setCell ( CPos ( "D3" ), "40" ) );
    assert ( x0 . setCell ( CPos ( "D4" ), "50" ) );
    assert ( x0 . setCell ( CPos ( "E0" ), "60" ) );
    assert ( x0 . setCell ( CPos ( "E1" ), "70" ) );
    assert ( x0 . setCell ( CPos ( "E2" ), "80" ) );
    assert ( x0 . setCell ( CPos ( "E3" ), "90" ) );
    assert ( x0 . setCell ( CPos ( "E4" ), "100" ) );
    assert ( x0 . setCell ( CPos ( "F10" ), "=D0+5" ) );
    assert ( x0 . setCell ( CPos ( "F11" ), "=$D0+5" ) );
    assert ( x0 . setCell ( CPos ( "F12" ), "=D$0+5" ) );
    assert ( x0 . setCell ( CPos ( "F13" ), "=$D$0+5" ) );
    x0 . copyRect ( CPos ( "G11" ), CPos ( "F10" ), 1, 4 );
    assert ( valueMatch ( x0 . getValue ( CPos ( "F10" ) ), CValue ( 15.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "F11" ) ), CValue ( 15.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "F12" ) ), CValue ( 15.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "F13" ) ), CValue ( 15.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "F14" ) ), CValue() ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "G10" ) ), CValue() ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "G11" ) ), CValue ( 75.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "G12" ) ), CValue ( 25.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "G13" ) ), CValue ( 65.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "G14" ) ), CValue ( 15.0 ) ) );


    oss . clear ();
    oss . str ( "" );
    assert ( x0 . save ( oss ) );
//    std::string data2 = oss.str();
//
//    // Specify the file path
//    std::string filePath = "output_file.txt";
//
//    // Create an ofstream to write to the file
//    std::ofstream outputFile(filePath);
//
//    // Check if the file is open and ready for writing
//    if (outputFile.is_open()) {
//        // Write the data to the file
//        outputFile << data2;
//
//        // Close the file
//        outputFile.close();
//        std::cout << "Data successfully saved to " << filePath << std::endl;
//    } else {
//        std::cerr << "Failed to open file: " << filePath << std::endl;
//    }
    data = oss . str ();
    iss . clear ();
    iss . str ( data );
    assert ( x1 . load ( iss ) );

    assert ( valueMatch ( x1 . getValue ( CPos ( "F10" ) ), CValue ( 15.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "F11" ) ), CValue ( 15.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "F12" ) ), CValue ( 15.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "F13" ) ), CValue ( 15.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "F14" ) ), CValue() ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "G10" ) ), CValue() ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "G11" ) ), CValue ( 75.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "G12" ) ), CValue ( 25.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "G13" ) ), CValue ( 65.0 ) ) );
    assert ( valueMatch ( x1 . getValue ( CPos ( "G14" ) ), CValue ( 15.0 ) ) );

    x0 . copyRect ( CPos ( "G11" ), CPos ( "F10" ), 2, 4 );
    assert ( valueMatch ( x0 . getValue ( CPos ( "F10" ) ), CValue ( 15.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "F11" ) ), CValue ( 15.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "F12" ) ), CValue ( 15.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "F13" ) ), CValue ( 15.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "F14" ) ), CValue() ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "G10" ) ), CValue() ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "G11" ) ), CValue ( 75.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "G12" ) ), CValue ( 25.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "G13" ) ), CValue ( 65.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "G14" ) ), CValue ( 15.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "H10" ) ), CValue() ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "H11" ) ), CValue() ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "H12" ) ), CValue() ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "H13" ) ), CValue ( 35.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "H14" ) ), CValue() ) );
    assert ( x0 . setCell ( CPos ( "F0" ), "-27" ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "H14" ) ), CValue ( -22.0 ) ) );
    x0 . copyRect ( CPos ( "H12" ), CPos ( "H13" ), 1, 2 );
    assert ( valueMatch ( x0 . getValue ( CPos ( "H12" ) ), CValue ( 25.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "H13" ) ), CValue ( -22.0 ) ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "H14" ) ), CValue ( -22.0 ) ) );

    cout << "ASSERTION PASSED!!!" << endl;
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
