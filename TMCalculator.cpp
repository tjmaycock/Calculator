// TMCalculator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iomanip>
#include <iostream>
#include <windows.h>
#include <string>
#include <stdExcept>
#include "TMCalculator.h"
using namespace std;

int main()
{
    std::cout << "Please enter your equation: Whole numbers only, no parentheses and only 1 instance of any operand please:" << std::endl << std::flush;
    std::string str;
    std::getline(std::cin, str);
    CalculateEquation(str);
    std::cout << "The answer to the equation is - " << str;
    
    Sleep(5000);
}

// Calculate the answer to the equation, but advise of any errors 
// in the equation provided
void CalculateEquation(std::string& strEquation)
{
    try
    {
        std::size_t found;

        //run a test to see if there are any incorrect characters in the equation, e.g. a-z
        for (int i = 0; i < strEquation.size(); i++)
        {
            char ch = strEquation[i];

            std::string EqChars = ("/*+-0123456789");

            found = EqChars.find_first_of(ch);

            // if the character is not found in the list of allowable cahracters, then return 
            // with a message of failure
            if (found == std::string::npos)
            {
                strEquation = "Invalid char in equation - " + strEquation;
                return;
            }
        }

        found = strEquation.find_first_of("-+/*");

        if (found == std::string::npos)
        {
            strEquation = "No operands found, this is just a number";
            return;
        }

        // apply BODMAS, so search for divisors in the equation
        bool bBodmas = true;
        while (true == bBodmas)
        {
            if (strEquation.find_first_of('/') != std::string::npos)
            {
                std::string strCopy = strEquation;
                CalculateSubEquation('/', strCopy);

                strEquation = strCopy;
            }

            else if (strEquation.find_first_of('*') != std::string::npos)
            {
                std::string strCopy = strEquation;
                CalculateSubEquation('*', strCopy);

                strEquation = strCopy;
            }

            else if (strEquation.find_first_of('+') != std::string::npos)
            {
                std::string strCopy = strEquation;
                CalculateSubEquation('+', strCopy);

                strEquation = strCopy;
            }

            else if (strEquation.find_first_of('-') != std::string::npos)
            {
                std::string strCopy = strEquation;
                CalculateSubEquation('-', strCopy);

                strEquation = strCopy;
            }

            else if (strEquation.find_first_of("/*+-") == std::string::npos)
            {
                bBodmas = false;
            }
        }
    }
    catch (const exception& ex)
    {
        cout << "Error" << ex.what() << endl;
    }

    return;
};

// calculate the mini equation that is worked from the operand, preceding 
// and following operands
void CalculateSubEquation(char Operator, std::string& strEquation)
{
    try
    {
        std::string sub1, sub2;
        int x = (int)strEquation.find_first_of(Operator);

        std::string strOper = "";

        switch (Operator)
        {
        case ('/'):
        {
            strOper = "*+-";
            break;
        }
        case ('*'):
        {
            strOper = "+-";
            break;
        }
        case ('+'):
        {
            strOper = "-";
            break;
        }
        case ('-'):
        default:
        {
            strOper = "";
            break;
        }
        }

        //find the position of any previous operand
        int abc = 0;
        int nCharPos = 0;

        while ((abc < x) && (nCharPos != std::string::npos))
        {
            nCharPos = (int)strEquation.find_first_of(strOper, abc);

            if (nCharPos != std::string::npos)
                abc = nCharPos + 1;
        }

        if (abc == std::string::npos)
        {
            abc = 0;
        }

        sub1 = strEquation.substr(abc, x - abc);

        int p = (int)strEquation.find_first_of(Operator, x + 1);

        //if the end of the string is found, the end of the string needs to be found
        //and the final integer extracted
        if (p == std::string::npos)
        {
            sub2 = strEquation.substr(x + 1);
        }
        else
        {
            sub2 = strEquation.substr(x + 1, p - (x + 1));
        }
        std::string strBodmas = DoMaths(sub1, sub2, Operator);
        std::string strItem = sub1 + Operator + sub2;

        strEquation.replace(abc, strItem.length(), strBodmas);
    }
    catch (const exception& ex)
    {
        cout << "Error" << ex.what() << endl;
    }
}

// apply BODMAS to the equation
string DoMaths(std::string n1, std::string n2, char Operator)
{
    std::string strRetval = "abc";
    try
    {        
        float n = 0.00;

        float x = ::atof(n1.c_str());
        float p = ::atof(n2.c_str());

        switch (Operator)
        {
            case '/':
            {
                if ((x == 0) || (p == 0))
                {
                    throw runtime_error("Division by 0 is not permissible");
                    break;
                }
                else
                {
                    n = float(x / p);
                }
                break;
            }
            case '*':
            {
                n = float(x * p);
                break;
            }
            case '+':
            {
                n = float(x + p);
                break;
            }
            case '-':
            {
                n = float(x - p);
                break;
            }
            default:
            {
                break;
            }
        }

        strRetval = to_string(n);
    }
    catch (const exception& ex)
    {
        cout << "Error" << ex.what() << endl;
    }

    return strRetval;
}


