#include <iostream>
#include <string>
#include <stack>
#include <map>

using namespace std;

int CheckBrackets(string expression)
{
    stack<char> brackets;
    int position{};

    string brOpen = "([{";
    string brClose = ")]}";


    while (position < expression.length())
    {
        if (strchr(brOpen.c_str(), expression[position]) &&
            strchr(brClose.c_str(), expression[position]))
        {
            position++;
            continue;
        }

        if (strchr(brOpen.c_str(), expression[position]))
        {
            brackets.push(expression[position]);
            position++;
            continue;
        }
        
        if(brackets.empty())
            return position;

        int brCloseType = brClose.find(expression[position]);
        char brTop = brackets.top();
        int brOpenType = brOpen.find(brTop);
        if (brOpenType == brCloseType)
        {
            brackets.pop();
            position++;
            continue;
        }

        return position;
    }

    if (!brackets.empty())
        return position;

    return -1;
}

string StringToOPZ(string strInput, map<string, double> vars)
{
    stack<char> operations;

    string brOpen = "([{";
    string brClose = ")]}";

    string strOutput = "";
    
    int position{};
    while (position < strInput.length())
    {
        if (strInput[position] == ' ')
        {
            position++; 
            continue;
        }

        if (isdigit(strInput[position]))
        {
            while (position < strInput.length() && (isdigit(strInput[position]) || strchr("-.e", strInput[position])))
            {
                strOutput.push_back(strInput[position]);
                position++;
            }
            strOutput.push_back('#');
            continue;
        }

        if (isalpha(strInput[position]))
        {
            string var;
            while (position < strInput.length() && (isalnum(strInput[position]) || strInput[position] == '_'))
            {
                var.push_back(strInput[position]);
                position++;
            }
            cout << var << " = ";
            double value = vars[var];
            cout << value << "\n";
            strOutput.append(to_string(value));
            strOutput.push_back('#');
        }

        if (strchr(brOpen.c_str(), strInput[position]))
        {
            operations.push(strInput[position]);
            position++;
            continue;
        }

        if (strchr(brClose.c_str(), strInput[position]))
        {
            while (!operations.empty() && operations.top() != '(')
            {
                strOutput.push_back(operations.top());
                operations.pop();
            }
            operations.pop();
        }

        if (strchr("*/", strInput[position]))
        {
            while (!operations.empty() && strchr("*/", operations.top()))
            {
                strOutput.push_back(operations.top());
                operations.pop();
            }
            operations.push(strInput[position]);
        }

        if (strchr("+-", strInput[position]))
        {
            while (!operations.empty() && strchr("+-*/", operations.top()))
            {
                strOutput.push_back(operations.top());
                operations.pop();
            }
            operations.push(strInput[position]);
        }

        position++;
    }

    while (!operations.empty())
    {
        strOutput.push_back(operations.top());
        operations.pop();
    }

    return strOutput;
}

double CalculateOPZ(string strOpz)
{
    stack<double> numbers;

    int position{};

    while (position < strOpz.length())
    {
        if (isdigit(strOpz[position]))
        {
            string number = "";
            while (position < strOpz.length() && strOpz[position] != '#')
            {
                number.push_back(strOpz[position]);
                position++;
            }
            position++;
            numbers.push(stod(number));
            continue;
        }

        if (strchr("*/+-", strOpz[position]))
        {
            double operand2{ numbers.top() };
            numbers.pop();
            double operand1{ numbers.top() };
            numbers.pop();

            double result;

            switch (strOpz[position]) 
            {
            case '+': result = operand1 + operand2; break;
            case '-': result = operand1 - operand2; break;
            case '*': result = operand1 * operand2; break;
            case '/': result = operand1 / operand2; break;
            default:
                break;
            }

            numbers.push(result);
        }
        position++;

    }

    return numbers.top();
}


int main()
{
    map<string, double> vars;

    vars.emplace("a", 7);
    vars.emplace("count", 16);
    vars.emplace("var_1", 1);

    string expresion = "((2 + a) * 3 - count / (3 + var_1))";
    // 25+3*38-4/-
    int error = CheckBrackets(expresion);
    if (error == -1)
        cout << "no error\n";
    else
        cout << "error in " << error << " position\n";

    string opz = StringToOPZ(expresion, vars);

    cout << opz << "\n";
    cout << CalculateOPZ(opz) << "\n";

    return 0;
}
