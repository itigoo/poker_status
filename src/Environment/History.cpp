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

#include "BaseStruct.cpp"

// 文字列を二次元配列に分割する関数
std::vector<std::vector<std::string>> splitStringTo2DArray(const std::string& s) {
    std::vector<std::vector<std::string>> result;
    std::istringstream stream(s);
    std::string line;

    // 各行ごとに処理
    while (std::getline(stream, line)) {
        std::istringstream lineStream(line);
        std::string word;
        std::vector<std::string> words;

        // 行内の単語を空白で分割
        while (lineStream >> word) {
            words.push_back(word);
        }

        if (words.size()>=2){
            if (words[1]=="said,") {
                continue;
            }
        }

        // 行内の単語が空でない場合のみ追加
        if (!words.empty()) {
            result.push_back(words);
        }
    }

    return result;
}

int tostack(string s){
    s.erase(0,1);
    int stack=stod(s)*100;
    return stack;
}

vector<Player> getplayer(vector<vector<string>> &array){
    vector<Player> Players(6);
    for (int i=2;i<8;i++){
        if (array[i][0]!="Seat"){
            Players[0].Stack=-1;
            break;
        }
        int k=2;
        while(true){
            if (array[i][k][0]=='(' && array[i][k][1]=='$') break;
            Players[i-2].Name.append(array[i][k]);
            k++;
        }
        array[i][k].erase(0,1);
        Players[i-2].Stack=tostack(array[i][k]);
    }
    return Players;
}

Card getcard(string s){
    string u;
    Card card;
    for (char c:s){
        if (c!=']' && c!='[') u.push_back(c);
    }
    s=u;
    if (s[0]=='A') card.Number=14;
    else if (s[0]=='K') card.Number=13;
    else if (s[0]=='Q') card.Number=12;
    else if (s[0]=='J') card.Number=11;
    else if (s[0]=='T') card.Number=10;
    else card.Number=s[0]-'0';
    if (s[1]=='c') card.Suit=0;
    if (s[1]=='d') card.Suit=1;
    if (s[1]=='h') card.Suit=2;
    if (s[1]=='s') card.Suit=3;
    return card;
}

Action getaction(vector<string> array,vector<Player> Players){
    int k=0;
    string name;
    Action action;
    if (array[array.size()-1]=="out" || array[array.size()-1]=="disconnected" || array[array.size()-1]=="connected"){
        action.player=-1;
        return action;
    }
    while(true){
        if (array[k][array[k].size()-1]==':'){ // usernameに': 'が含まれない事前提だが、流石にないよね？
            for (int i=0;i<array[k].size()-1;i++){
                name.push_back(array[k][i]);
            }
            k++;
            break;
        } else {
            name.append(array[k]);
        }
        k++;
    }
    for (int i=0;i<6;i++){
        if (Players[i].Name==name) action.player=i;
    }
    action.Type=array[k];
    if (action.Type=="calls" || action.Type=="bets") {
        action.amount=tostack(array[k+1]);
    }
    if (action.Type=="raises") action.amount=tostack(array[k+3]);
    return action;
}

struct History{
    public:
        string RawData;
        int SB;
        int BB;
        vector<Player> Players;
        int PostflopType; // not supported : 0, Linp : 1, SRP : 2, 3-Bet : 3, 4-Bet : 4, ...
        int Player1,Player2; // Only support HU(todo).  Player1 is original.
        vector<Card> Board;
        FLOP Flop;
        TURN Turn;
        RIVER River;

        History(string s){
            RawData = s;
            std::vector<std::vector<std::string>> array = splitStringTo2DArray(s);
            Players=getplayer(array);
            if (Players[0].Stack==-1){
                PostflopType=0;
                return;
            }
            SB=tostack(array[8][array[8].size()-1]);
            BB=tostack(array[9][array[9].size()-1]);
            vector<int> paied(6,0);
            paied[1]=SB;paied[2]=BB;Player1=2;
            int pot=SB+BB;
            int playernum=1,raisetimes=1;
            int line=12;

            // PREFLOP
            while(true){
                if (array[line][0]=="Uncalled") break;
                if (array[line][array[line].size()-1]=="pot") break;
                if (array[line][0]=="***") break;
                Action temp=getaction(array[line],Players);
                line++;
                if (temp.player==-1) continue;
                if (temp.Type=="raises"){
                    pot+=temp.amount-paied[temp.player];
                    paied[temp.player]=temp.amount;
                    playernum=1;raisetimes++;
                    Player1=temp.player;
                } else if (temp.Type=="calls"){
                    pot+=temp.amount;
                    paied[temp.player]+=temp.amount;
                    playernum++;
                    Player2=temp.player;
                }
            }
            if (playernum!=2 || array[line][0]!="***"){
                PostflopType=0;
                return;
            }
            if (array[line][1]!="FLOP"){
                PostflopType=0;
                return;
            }
            PostflopType=raisetimes;

            // FLOP
            Flop.Pot=pot;
            for (int i=3;i<=5;i++){
                Board.push_back(getcard(array[line][i]));
            }
            line++;
            for (int i=0;i<6;i++) paied[i]=0;
            while(true){
                if (array[line][0]=="Uncalled") break;
                if (array[line][array[line].size()-1]=="pot") break;
                if (array[line][0]=="***") break;
                Action temp=getaction(array[line],Players);
                line++;
                if (temp.player==-1) continue;
                if (temp.Type=="raises"){
                    pot+=temp.amount-paied[temp.player];
                    paied[temp.player]=temp.amount;
                } else if (temp.Type=="calls"){
                    pot+=temp.amount;
                    paied[temp.player]+=temp.amount;
                } else if (temp.Type=="bets"){
                    pot+=temp.amount-paied[temp.player];
                    paied[temp.player]=temp.amount;
                }
                Flop.Actions.push_back(temp);
            }
            if (array[line][0]!="***") return;
            if (array[line][1]!="TURN") return;

            // TURN
            Turn.Pot=pot;
            for (int i=6;i<=6;i++){
                Board.push_back(getcard(array[line][i]));
            }
            line++;
            for (int i=0;i<6;i++) paied[i]=0;
            while(true){
                if (array[line][0]=="Uncalled") break;
                if (array[line][array[line].size()-1]=="pot") break;
                if (array[line][0]=="***") break;
                Action temp=getaction(array[line],Players);
                line++;
                if (temp.player==-1) continue;
                if (temp.Type=="raises"){
                    pot+=temp.amount-paied[temp.player];
                    paied[temp.player]=temp.amount;
                } else if (temp.Type=="calls"){
                    pot+=temp.amount;
                    paied[temp.player]+=temp.amount;
                } else if (temp.Type=="bets"){
                    pot+=temp.amount-paied[temp.player];
                    paied[temp.player]=temp.amount;
                }
                Turn.Actions.push_back(temp);
            }
            if (array[line][0]!="***") return;
            if (array[line][1]!="RIVER") return;

            // RIVER
            River.Pot=pot;
            for (int i=7;i<=7;i++){
                Board.push_back(getcard(array[line][i]));
            }
            line++;
            for (int i=0;i<6;i++) paied[i]=0;
            while(true){
                if (array[line][0]=="Uncalled") break;
                if (array[line][array[line].size()-1]=="pot") break;
                if (array[line][0]=="***") break;
                Action temp=getaction(array[line],Players);
                line++;
                if (temp.player==-1) continue;
                if (temp.Type=="raises"){
                    pot+=temp.amount-paied[temp.player];
                    paied[temp.player]=temp.amount;
                } else if (temp.Type=="calls"){
                    pot+=temp.amount;
                    paied[temp.player]+=temp.amount;
                } else if (temp.Type=="bets"){
                    pot+=temp.amount-paied[temp.player];
                    paied[temp.player]=temp.amount;
                }
                River.Actions.push_back(temp);
            }
            if (array[line][0]!="***") return;
            if (array[line][1]!="SHOW") return;

            // SHOU DOWN (todo)
        }
};