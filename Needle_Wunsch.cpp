#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <array>
#include "Blosum.cpp"
using namespace std;

class Needle {

    public:
        Blosum bl = Blosum();
        int leven_distance(string str1, string str2, int gcost=-10, bool needle = false){
            if (str1 == str2){
                return(0);
            }
            else if (str1.length() == 0) {
                return str2.length();
            }
            else if (str2.length() == 0) {
                return str1.length();
            }
            vector< int > row;
            vector < vector < int > > M;
            for(int i=0; i <= str1.size(); i++) {
                row = {};
                for (int j=0; j <= str2.size(); j++) {
                    if (i == 0){
                        row.push_back(j*gcost);
                    }
                    else if (j == 0) {
                        row.push_back(i*gcost);
                    }
                    else {
                        row.push_back(0);
                    }
                }
                M.push_back(row);
            }

            for (int i=1; i < str1.length()+1; i++){
                for (int j=1; j < str2.length()+1; j++){
                    if(needle == true){
                        M[i][j] = max(max(M[i-1][j]+gcost, M[i][j-1]+gcost) , M[i-1][j-1] + bl.operator()(str1[i-1],str2[j-1]));
                    } 
                    else {
                        M[i][j] = min(min(M[i-1][j]+gcost, M[i][j-1]+gcost) , M[i-1][j-1] + bl.operator()(str1[i-1],str2[j-1]));
                    }
                        
                }
            }
            return M[str1.size()][str2.size()];
        }
        int needle(string str1, string str2,...) {
            return(leven_distance(str1, str2, -10, true));
        }

};



int main(int argc, char** argv) {
    auto bl = Blosum();
    auto nl = Needle();
    bl.read_matrix("blosum50");
    //string str1 = "SEND";
   // string str2 = "AND";
    if(argc == 3){
        string str1 = argv[1];
        string str2 = argv[2];
        //string str3 = argv[3];
        cout << nl.needle(str1, str2) << endl;
    }
    else if(argc > 3){
        cout<<"Too many arguments supplied:)" <<endl;
    }
    else {
        cout<<"The argument supplied is filename: "<<argv[0] <<endl;
    }
    
}
