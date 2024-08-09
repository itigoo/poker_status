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

// レイズが無いアクションツリーそれぞれのbetに対するfold率を調べます

// 出力として欲しいゲームツリーを入れてください
// 二文字ごとにflop,turn,riverのアクションを表します。callは省略されます
void insertOutputTree(set<string> &s){
    s.insert("xB"); //flop IP bet
    s.insert("xxB"); //turn prob
    s.insert("xxBB"); //prob and river CB
    s.insert("xxBxB"); //prob and river check bet
    s.insert("xxxB"); //flop xx turn xB
    s.insert("xxxBxB"); //flop xx turn xB river xB
    s.insert("xBxB"); // flop IP bet CB
    s.insert("xBxBxB"); // flop IP bet CB CB
    s.insert("xBxxB"); // flop IP bet, turn xx, river OOP bet
    s.insert("xBxxxB"); // flop IP bet, turn xx, river IP bet
    s.insert("xxxxB"); // all check, OOP bet
    s.insert("xxxxxB"); // all check, IP bet
}

void RiverTree(History history,vector<int> &counter,vector<int> &foldcounter,vector<double> &betsize,int treeNumber, Environment &env){
    vector<Action> actions=history.River.Actions;
    if (actions.size()<2) return;
    if (actions[0].Type=="bets"){
        int n=treeNumber+16;
        if (history.Players[actions[1].player].Name!=env.PlayerName) {
            betsize[n]+=((double)actions[0].amount/(double)history.River.Pot)*100;
            counter[n]++;
        }
        if (actions[1].Type=="folds") {
            if (history.Players[actions[1].player].Name!=env.PlayerName) foldcounter[n]++;
        }
        else if (actions[1].Type=="calls"){
        }
    } else {
        if (actions[1].Type=="bets"){
            int n=treeNumber+32;
            if (history.Players[actions[2].player].Name!=env.PlayerName) {
                counter[n]++;
                betsize[n]+=((double)actions[1].amount/(double)history.River.Pot)*100;
            }
            if (actions[2].Type=="folds") {
                if (history.Players[actions[2].player].Name!=env.PlayerName) foldcounter[n]++;
            }
            else if (actions[2].Type=="calls"){
            }
        } else {
            int n=treeNumber+48;
        }
    }
}

void TurnTree(History history,vector<int> &counter,vector<int> &foldcounter,vector<double> &betsize,int treeNumber, Environment &env){
    vector<Action> actions=history.Turn.Actions;
    if (actions.size()<2) return;
    if (actions[0].Type=="bets"){
        int n=treeNumber+4;
        if (history.Players[actions[1].player].Name!=env.PlayerName) {
            counter[n]++;
            betsize[n]+=((double)actions[0].amount/(double)history.Turn.Pot)*100;
        }
        if (actions[1].Type=="folds") {
            if (history.Players[actions[1].player].Name!=env.PlayerName) foldcounter[n]++;
        }
        else if (actions[1].Type=="calls"){
            RiverTree(history,counter,foldcounter,betsize,n,env);
        }
    } else {
        if (actions[1].Type=="bets"){
            int n=treeNumber+8;
            if (history.Players[actions[2].player].Name!=env.PlayerName) {
                counter[n]++;
                betsize[n]+=((double)actions[1].amount/(double)history.Turn.Pot)*100;
            }
            if (actions[2].Type=="folds") {
                if (history.Players[actions[2].player].Name!=env.PlayerName) foldcounter[n]++;
            }
            else if (actions[2].Type=="calls"){
                RiverTree(history,counter,foldcounter,betsize,n,env);
            }
        } else {
            int n=treeNumber+12;
            RiverTree(history,counter,foldcounter,betsize,n,env);
        }
    }
}

void FlopTree(History history,vector<int> &counter,vector<int> &foldcounter,vector<double> &betsize,int treeNumber, Environment &env){
    vector<Action> actions=history.Flop.Actions;
    if (actions.size()<2) return;
    if (actions[0].Type=="bets"){
        int n=treeNumber+1;
        if (history.Players[actions[1].player].Name!=env.PlayerName) {
            counter[n]++;
            betsize[n]+=((double)actions[0].amount/(double)history.Flop.Pot)*100;
        }
        if (actions[1].Type=="folds") {
            if (history.Players[actions[1].player].Name!=env.PlayerName) foldcounter[n]++;
        }
        else if (actions[1].Type=="calls"){
            TurnTree(history,counter,foldcounter,betsize,n,env);
        }
    } else {
        if (actions[1].Type=="bets"){
            int n=treeNumber+2;
            if (history.Players[actions[2].player].Name!=env.PlayerName) {
                counter[n]++;
                betsize[n]+=((double)actions[1].amount/(double)history.Flop.Pot)*100;
            }
            if (actions[2].Type=="folds") {
                if (history.Players[actions[2].player].Name!=env.PlayerName) foldcounter[n]++;
            }
            else if (actions[2].Type=="calls"){
                TurnTree(history,counter,foldcounter,betsize,n,env);
            }
        } else {
            int n=treeNumber+3;
            TurnTree(history,counter,foldcounter,betsize,n,env);
        }
    }
}

void GameTreeExplorer(Environment env){
    vector<int> counter(64,0),foldcounter(64,0);
    vector<double> betsize(64,0);
    set<string> outputTree;
    insertOutputTree(outputTree);
    int treeNumber=0;
    for (History history:env.HandHistory){
        if (history.PostflopType!=2) continue; //SRP
        FlopTree(history,counter,foldcounter,betsize,treeNumber,env);
    }
    for (int i=0;i<64;i++){
        int c=i;
        string treestr;
        for (int j=0;j<3;j++){
            int k=c%4;
            c/=4;
            if (k==0) break;
            if (k==1) treestr=treestr+"B";
            if (k==2) treestr=treestr+"xB";
            if (k==3) treestr=treestr+"xx";
        }
        if (outputTree.find(treestr)==outputTree.end()) continue;
        if (counter[i]==0) continue;
        cout << treestr << " : " << "回数 :" << counter[i] << " fold回数 : " << foldcounter[i] << " fold率 : " << (double)foldcounter[i]/max((double)counter[i],0.1)*100 << " betsize : " << betsize[i]/(double)counter[i] << "%" << endl;
    }
}