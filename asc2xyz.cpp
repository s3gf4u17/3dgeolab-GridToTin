#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define BREAKY 1000
#define BREAKX 1000

int main(int argc, char **argv) {
    int COLS,ROWS;
    float XLLCENTER,YLLCENTER,CELLSIZE;
    std::string NODATAVALUE;
    std::ifstream input(argv[1]);
    std::ofstream output(argv[2]);
    std::string line;
    int rowcounter=0;
    while(std::getline(input,line)) {
        std::istringstream ss(line);
        std::string first_token=line.substr(0,line.find(' '));
        if (first_token=="ncols"||first_token=="NCOLS") {
            ss >> line >> COLS;
        } else if (first_token=="nrows"||first_token=="NROWS") {
            ss >> line >> ROWS;
        } else if (first_token=="xllcenter"||first_token=="XLLCENTER") {
            ss >> line >> XLLCENTER;
        } else if (first_token=="yllcenter"||first_token=="yLLCENTER") {
            ss >> line >> YLLCENTER;
        } else if (first_token=="cellsize"||first_token=="CELLSIZE") {
            ss >> line >> CELLSIZE;
        } else if (first_token=="nodata_value"||first_token=="NODATA_VALUE") {
            ss >> line >> NODATAVALUE;
        } else {
            rowcounter++;
            if (rowcounter>BREAKY) return 0;
            float rowy=YLLCENTER-CELLSIZE*(ROWS-rowcounter);
            float colx=XLLCENTER;
            std::string buffer;
            float bufferstof;
            for (int i=0;i<COLS;i++) {
                if (i>BREAKX) break;
                ss >> buffer;
                if (buffer!=NODATAVALUE) {
                    bufferstof=std::stof(buffer);
                    output << colx << " " << rowy << " " << bufferstof << std::endl;
                }
                colx+=CELLSIZE;
            }
        }
    }
    input.close();
    output.close();
}