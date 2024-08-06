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

void RaitoOfFoldToRiverBet(Environment env){
    int blocksize=10,blockwidht=20;
    vector<int> counter(blocksize,0),foldcounter(blocksize,0);
    for (History history:env.HandHistory){
        int type=-1,is_fold=0;
        for (Action act:history.River.Actions){
            if (type!=-1){
                if (history.Players[act.player].Name==env.PlayerName) break;
                counter[type]++;
                if (act.Type=="folds"){
                    foldcounter[type]++;
                }
                break;
            }
            if (act.Type=="bets"){
                type=((double)act.amount/(double)history.River.Pot)*100/blockwidht;
                if (type>=blocksize) break;
            }
        }
    }
    for (int i=0;i<blocksize;i++) {
        cerr << "<" << blockwidht*(i+1) << "%bet : " << "回数 :" << counter[i] << " fold回数 : " << foldcounter[i] << " fold率 : " << (double)foldcounter[i]/max((double)counter[i],0.1)*100 << "%" << endl;
    }
}