#include "io.h"
#include <fstream>
#include "error.h"

using namespace std;
using namespace lg;

std::string read_txt_file(const std::string& path)
{
    ifstream f(path);

    Error::check(f.is_open(), "Can't open file '" + path + "'");

    // TODO : Use algorithm
    string s;
    string line;
    while (getline(f, line))
        s += line;
    
    return s;
}
