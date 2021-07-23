#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <vector>
#include <map>

#if __has_include(<filesystem>)
  #include <filesystem>
  namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
  #include <experimental/filesystem> 
  namespace fs = std::experimental::filesystem;
#else
  error "Missing the <filesystem> header."
#endif

using namespace std;

map<string,string> getChildren(string filename, string PID = ""){
    ifstream infile;
    string line;
    string GO_Id;
    string GO_name;
    regex pat("^id: (GO:[0-9]{7})");
    regex name("^name: ");
    regex reg("^is_a:\\s+"+PID+".+");
    infile.open(filename);
    map<string, string> res;
    while(getline(infile,line)){
        if(regex_search(line, pat)){
            GO_Id = regex_replace(line, pat,"$1");
        }
        if(regex_search(line, name)){
            GO_name = regex_replace(line, name, "$1");
        }
        if(regex_search(line, reg)){
            res[GO_Id] = GO_name;
        }
    }
    infile.close();
    return(res);
}

vector<string> getParent(string filename, string GO_ID){
    ifstream infile;
    string line;
    vector<string> PID;
    string res;
    regex pat("^id: "+GO_ID);
    regex reg("is_a: ");
    infile.open(filename);
    bool found = false;
    while(getline(infile, line)){
        if(regex_search(line, pat)){
            found = true;
        }
        if(line.empty() && found){
            break;
        }
        if(found && regex_search(line, reg)){
            res = regex_replace(line, reg, "$1");
            PID.push_back(res);
        }
    }
    return(PID);
}
            
map<string, string> searchEC(string filename, string Xref_ID){
    ifstream infile;
    string line;
    string GO_Id;
    string GO_name;
    regex pat("^id: ");
    regex name("^name: ");
    regex reg("^xref: "+Xref_ID+".+");
    map<string, string> res;
    infile.open(filename);
    while(getline(infile, line)){
        if(regex_search(line, pat)){
            GO_Id = regex_replace(line, pat, "$1");
        }
        if(regex_search(line, name)){
            GO_name = regex_replace(line, name, "$1");
        }
        if(regex_search(line, reg)){
            res[GO_Id] = GO_name;
        }
    }
    infile.close();
    return(res);
}
            

void help(){
    exit(0);
}

void usage(){
    cout<<"Usage: GoUtil.cpp obofilename cmdname GOid"
    "\nwhere commandname is either getChildren or getParent\n";
}

bool path_is_valid(std::string path_string){
    fs::path p = path_string;
    bool exists = fs::exists(p);
    return exists;
}
int main(int argc, char** argv){
    if(argc < 4){
        help();
        //std::cout<<"Warning: using default value! \n";
        //obo_file.open("go-basic.obo");
    }
    
    if(argc == 4){
        string filename = argv[1];
        string cmdname = argv[2];
        string GoId = argv[3];
        if(path_is_valid(filename) == false){
            cout << "Given file does not exists"<<'\n';
            usage();
        }
        else {
            if(cmdname == "getChildren"){
                auto result = getChildren(filename,GoId);
                if(result.empty() == true){
                    cout<<"The Parent Id has no children"<<"\n";
                }
                else{
                    for(auto r : result){
                        cout<<r.first <<"\t" <<r.second <<"\n";
                    }
                }
                
            }
            else if(cmdname == "getParent"){
                auto result = getParent(filename, GoId);
                if(result.empty() == true){
                    cout<<"Given GO Id has no Parent" <<"\n";
                }
                else{
                    for(auto r : result){
                        cout<<r<<"\n";
                    }
                }
            }
            else if(cmdname == "searchEC"){
                auto result = searchEC(filename, GoId);
                if(result.empty() == true){
                    cout<<"Given Xref Id has no GO Id's "<<"\n";
                }
                else {
                    for(auto r: result){
                        cout<<r.first <<"\t" <<r.second << "\n";
                    }
                }
            }
            else {
                cout<<"The command is not valid"<<"\n";
                usage();
            }
        }
    }
    return(0);
}
