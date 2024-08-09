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
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
using namespace std;
namespace fs = std::filesystem;

vector<History> input(){
    // 対象ディレクトリのパス
    std::string path = "input/data/";
    vector<History> Historys;
    // ディレクトリ内のファイルを順番に処理
    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            std::string file_path = entry.path().string();
            std::ifstream file(file_path);

            if (!file) {
                std::cerr << "ファイルを開くことができませんでした: " << file_path << std::endl;
                continue;
            }

            std::string line;
            string s;
            int sum=0;
            while (std::getline(file, line)) {
                sum++;
                if (line.size()<=1){
                    if (s.size()==0) continue;
                    try{
                        History h(s);
                        Historys.push_back(h);
                    } catch(...){

                    }
                    s="";
                    continue;
                }
                if (s.size()!=0) s.append("\n").append(line);
                else s.append(line);
            }

            file.close();
        }
    }
    return Historys;
}