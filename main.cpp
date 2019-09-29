#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <sstream>
#include "totalpairing.h"

using namespace std;

using Int = std::size_t;


int main(int argc, char* argv[]){
    unsigned int n=0;
    char mode;
//Getting the input arguments.
    const char* error_msg = "Invalid arguments. Expected:\n1) [int, >0]  [char, \'s\'||\'p\']\n2) [int, >0]\n";
    if(argc == 1){
    //No args, ask for user input.
        std::cout<<"Symbol count[2*n]: n = ";
        if(!(std::cin>>n)){
            std::cout<<"Invalid input.\n";
            return 1;
        }
        std::cin.ignore(1000,'\n');
        std::cout<<"Save[s] or Print[p]: mode = ";
        std::cin>>mode;
        if(mode != 's' && mode !='p'){
            std::cout<<"Invalid input.\n";
            return 1;
        }
    }else if(argc ==2){
    //by default print results.
        std::istringstream arg1(argv[1]);
        if(!(arg1>>n) || n<=0 ){
            std::cout<<error_msg;
            return 1;
        }
        mode = 'p';
    }else if(argc == 3){
        std::istringstream arg1(argv[1]);
        if(!(arg1>>n) || n<=0 ){
            std::cout<<error_msg;
            return 1;
        }
        std::istringstream arg2(argv[2]);
        arg2.str(argv[2]);
        if(!(arg2>>mode) || (mode != 's' && mode != 'p')){
            std::cout<<error_msg;
            return 1;
        }

    }else{
        std::cout<<error_msg;
        return 1;
    }

    std::fstream save_file;
    std::string file_path;
    if(mode == 's'){
        std::ostringstream out_format;
        out_format<<"n = "<<n<<".txt";
        file_path = out_format.str();
        save_file.open(file_path.c_str(), std::fstream::out | std::fstream::trunc);
        if(!save_file.good()){
            std::cout<<"Error creating save file.\n";
            return 1;
        }
    }

    std::ostream& output = (mode == 'p'? std::cout : save_file);
//Doing the calculation after preparing buffers
    n*=2;
    std::vector<Int> symbols (n);
    std::vector<Int> pairings(n);
    for(Int i=0;i<n;++i){
        symbols[i]=i+1;
    }
    TotalPairing<std::vector<Int>::iterator> alg(symbols.begin(),pairings.begin(),n);

    int fill_size=0;
    while(n)++fill_size,n/=10;

    for(auto itr:symbols){
        output<<std::setw(fill_size)<<std::setfill(' ')<<itr<<' ';
    }
    output<<'\n';
    while(alg()){
        for(auto itr:pairings){
            output<<std::setw(fill_size)<<std::setfill(' ')<<itr<<' ';
        }
        output<<'\n';
    }
    output<<std::flush;
    if(mode == 's'){
        std::cout<<"Result saved in: \""<<file_path<<"\"."<<std::endl;
    }
    return 0;
}


