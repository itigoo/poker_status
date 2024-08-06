#include <algorithm>
#include <bitset>
#include <complex>
#include <deque>
#include <exception>
#include <fstream>
#include <functional>
#include <iomanip>
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

struct Card{
    public:
        int Number;
        int Suit; //club : 0, dia : 1, hart : 2, spade : 3      
};

struct Player{
    public:
        string Name;
        int Stack;
        vector<Card> Hand;
};

struct Action{
    public:
        string Type;
        int player;
        int amount;
};

struct FLOP{
    public:
        int Pot;
        vector<Action> Actions;
};

struct TURN{
    public:
        int Pot;
        vector<Action> Actions;
};

struct RIVER{
    public:
        int Pot;
        vector<Action> Actions;
};