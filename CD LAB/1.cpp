// Write a program to check whether an entered string is a keyword or not.
// made by Aaditya Bhatia

#include <iostream>
#include <queue>

using namespace std;

string keywords[] = { // this is a list of C++ keywords
    "alignas", "alignof", "and", "and_eq", "asm", "atomic_cancel",
    "atomic_commit", "atomic_noexcept", "auto", "bitand", "bitor", "bool",
    "break", "case", "catch", "char", "char16_t", "char32_t", "class",
    "compl", "concept", "const", "consteval", "constexpr", "constinit",
    "const_cast", "continue", "co_await", "co_return", "co_yield", "decltype",
    "default", "delete", "do", "double", "dynamic_cast", "else", "enum",
    "explicit", "export", "extern", "false", "float", "for", "friend", "goto",
    "if", "inline", "int", "long", "mutable", "namespace", "new", "noexcept",
    "not", "not_eq", "nullptr", "operator", "or", "or_eq", "private",
    "protected", "public", "register", "reinterpret_cast", "requires", "return",
    "short", "signed", "sizeof", "static", "static_assert", "static_cast",
    "struct", "switch", "synchronized", "template", "this", "thread_local",
    "throw", "true", "try", "typedef", "typeid", "typename", "union",
    "unsigned", "using", "virtual", "void", "volatile", "wchar_t", "while",
    "xor", "xor_eq"};

bool isKeyword(const string &str)
{
    for (const string &keyword : keywords)
    {
        if (str == keyword)
        {
            return true;
        }
    }
    return false;
}

int main()
{
    string input;
    cout << "Enter a string: ";
    cin >> input;

    if (isKeyword(input))
    {
        cout << input << " is a keyword." << endl;
    }
    else
    {
        cout << input << " is not a keyword." << endl;
    }

    return 0;
}