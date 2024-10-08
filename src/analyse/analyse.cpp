#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <typeinfo>
#include <utility>
#include <valarray>
#include <vector>
using namespace std;

#include "./FrequencyOfFoldToRiverBet.cpp"
#include "./GameTreeExplorer.cpp"
#include "./PrefropAnalyser.cpp"

void analyse(string analysetype,Environment env){
    if (analysetype=="FrequencyOfFoldToRiverBet"){
        FrequencyOfFoldToRiverBet(env);
    } else if (analysetype == "GameTreeExplorer") {
        GameTreeExplorer(env);
    } else if (analysetype == "PrefropAnalyser"){
        PrefropAnalyser(env);
    }
}