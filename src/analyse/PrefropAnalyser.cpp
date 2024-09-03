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

// Riverでベットした時のfold率を調べます

void PrefropAnalyser(Environment env){
    string target="FFFFRR";
    int playernum=1; // BTN : 0, SB : 1, BB : 2, UTG : 3, HJ : 4, CO : 5
    vector<int> counter(3,0);
    cerr << env.HandHistory.size() << endl;
    for (History history:env.HandHistory){
        if (history.Players[playernum].Name==env.PlayerName) continue;
        if (history.ActionLine.size()<=target.size()) continue;
        bool ok=true;
        for (int i=0;i<target.size();i++){
            if (target[i]!=history.ActionLine[i]) ok=false;
        }
        if (!ok) continue;
        if (history.ActionLine[target.size()]=='F') counter[0]++;
        if (history.ActionLine[target.size()]=='C') counter[1]++;
        if (history.ActionLine[target.size()]=='R') counter[2]++;
    }
    double sum=0;
    for (int i=0;i<3;i++) sum+=counter[i];
    cerr << sum << endl;
    cerr << "Fold  : " << (double)counter[0]/sum*100 << endl;
    cerr << "Call  : " << (double)counter[1]/sum*100 << endl;
    cerr << "Raise : " << (double)counter[2]/sum*100 << endl;
}