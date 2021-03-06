#include <vector>
#include <string>
#include <sstream>
#include <cstring>
#include <fstream>
#include <cmath>
#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

struct Files {
    char name[40];
    int size;
    vector<int> dataIndex;
};

struct GlobalInfo {
    char currentFS[40];
    int bSize;
    int nBlocks;
    char *fs;
    bool *blockState;
    bool initialized;
    int fileNumbers;
    vector<Files> myFiles;
};

void saveGlobalInfo(GlobalInfo &globalInfo);

vector<string> split(string line, char separator);

vector<string> createCmds();

int getCmdIndex(string value, vector<string> cmds);

void createFileSystem(GlobalInfo &globalInfo, vector<string> parameters);


void readDataFile(const GlobalInfo &newInfo);


vector<string> getFileParameters(string);

void loadFile(GlobalInfo &globalInfo, string nameOld, string nameNew) {
    //test-file.cpp
    srand(time(NULL));
    if (!globalInfo.initialized)cout << "create a file system first!";

    Files newFile = *new Files;
    strcpy(newFile.name, (const char *) nameNew.c_str());

    ifstream fentrada(nameOld, ios::binary);

    if(fentrada.fail()){
        cout << "Unable to find specified file." << endl;
        return;
    }

    fentrada.seekg(0, fentrada.end);
    int length = fentrada.tellg();
    fentrada.seekg(0, fentrada.beg);

    char *buffer = new char[length];
    fentrada.read(buffer, length);
    fentrada.close();

    int blocks = ceil((length * 1.0) / (globalInfo.bSize * 1.0));

    int available = 0;
    for(int i = 0; i < globalInfo.nBlocks; i++){
        if(!globalInfo.blockState[i]) available++;
    }

    if (blocks < available) cout << "File loaded successfully." << endl;
    else cout << "Full memory, unable to load file.";

    int j = 0;
    for (int count = 0; count < blocks;) {
        int index = rand() % ((globalInfo.nBlocks - 1) + 1);
        if (!globalInfo.blockState[index]) {
            newFile.dataIndex.push_back(index);
            globalInfo.blockState[index] = true;
            for (int i = index * globalInfo.bSize; i < index * globalInfo.bSize + globalInfo.bSize; ++i, j++) {
                if (j >= length)break;
                globalInfo.fs[i] = buffer[j];
            }
            count++;
        }
    }
    newFile.size = newFile.dataIndex.size();
    globalInfo.myFiles.push_back(newFile);
    globalInfo.fileNumbers = globalInfo.myFiles.size();
    saveGlobalInfo(globalInfo);
}

void displayInfo(GlobalInfo &globalInfo){

    int available = 0;
    for(int i = 0; i < globalInfo.nBlocks; i++){
        if(!globalInfo.blockState[i]) available++;
    }

    cout << "----------------------------------------" << endl;
    cout << "Name: " << globalInfo.currentFS << endl;
    cout << "Number of blocks: " << globalInfo.nBlocks << endl;
    cout << "Block size: " << globalInfo.bSize << endl;
    cout << "Used space: " << globalInfo.bSize * globalInfo.nBlocks - available * globalInfo.bSize << " bytes" << endl;
    cout << "Available space: " << available * globalInfo.bSize << " bytes" << endl;
    cout << "----------------------------------------" << endl;
}

vector<string> split(string line, char separator) {
    vector<string> lineParts;
    stringstream stream(line);
    string part;

    while (getline(stream, part, separator)) {
        lineParts.push_back(part);
    }
    return lineParts;
}

vector<string> createCmds() {
    vector<string> cmds;
    cmds.emplace_back("create");
    cmds.emplace_back("save");
    cmds.emplace_back("open");
    cmds.emplace_back("load");
    cmds.emplace_back("download");
    cmds.emplace_back("ls");
    cmds.emplace_back("rm");
    cmds.emplace_back("info");
    cmds.emplace_back("details");
    cmds.emplace_back("exit");

    return cmds;
}

int getCmdIndex(string value, vector<string> cmds) {
    int index = -1;
    for (int i = 0; i < cmds.size(); i++)
        if (value == cmds[i])
            index = i;
    return index;
}

bool validFS(vector<string> params){
    if(params[2] != "128" && params[2] != "256" && params[2] != "512" && params[2] != "1024"){
        cout << "Block size must be a power of two." << endl;
        return true;
    }
    if( (atoi(params[2].c_str()) * atoi(params[3].c_str())) > 1000000000 ){
        cout << "File system must not exceed 1GB." << endl;
        return false;
    }
    return true;
}

void createFileSystem(GlobalInfo &globalInfo, vector<string> parameters) {
/*
 * [0] Commando
 * [1] nombre
 * [2] tamaño de bloques
 * [3] numero de bloques
*/

    strcpy(globalInfo.currentFS, (const char *) parameters[1].c_str());
    globalInfo.bSize = atoi(parameters[2].c_str());
    globalInfo.nBlocks = atoi(parameters[3].c_str());
    globalInfo.initialized = true;
    bool *tempStatus = new bool[globalInfo.nBlocks];

    for (int i = 0; i < globalInfo.nBlocks; ++i) tempStatus[i] = false;

    globalInfo.blockState = tempStatus;
    //Save Global info to file
    string infoName = parameters[1] + ".info";
    ofstream fileGlobalInfo(infoName, ios::out | ios::binary);
    fileGlobalInfo.write((char *) &globalInfo, sizeof(GlobalInfo));
    globalInfo.fs = static_cast<char *>(malloc(globalInfo.bSize * globalInfo.nBlocks * sizeof(char)));

    for(int i = 0; i < globalInfo.nBlocks * globalInfo.bSize; i++){
        globalInfo.fs[i] = 0;
    }

    fileGlobalInfo.close();


    saveGlobalInfo(globalInfo);
}



void listFiles(GlobalInfo &globalInfo) {
    for (int i = 0; i < globalInfo.myFiles.size(); ++i) {
        cout << globalInfo.myFiles.at(i).name << "\t";
    }
    cout << endl;
}

void removeFile(GlobalInfo &globalInfo, string fileName) {
    for (int i = 0; i < globalInfo.myFiles.size(); ++i) {
        if (fileName.compare(globalInfo.myFiles.at(i).name) == 0) {
            for (int j = 0; j < globalInfo.myFiles.at(i).dataIndex.size(); ++j) globalInfo.blockState[globalInfo.myFiles.at(i).dataIndex.at(j)] = false;
            globalInfo.myFiles.erase(globalInfo.myFiles.begin() + i);
            globalInfo.fileNumbers = globalInfo.myFiles.size();
            break;
        }
    }
    saveGlobalInfo(globalInfo);
}

void downloadFile(GlobalInfo &globalInfo, string sourceFileName,
                  string destFilePath) {
    //example: test-file.cpp /home/mariana/Desktop/hello.cpp
    for (int i = 0; i < globalInfo.myFiles.size(); ++i) {
        if (sourceFileName.compare(globalInfo.myFiles.at(i).name) == 0) {
            ofstream outfile(destFilePath, ios::out);
            for (int count = 0; count < globalInfo.myFiles.at(i).dataIndex.size();) {
                int index = globalInfo.myFiles.at(i).dataIndex.at(count);
                for (int k = index * globalInfo.bSize; k < index * globalInfo.bSize + globalInfo.bSize; ++k) {
                    if((int)globalInfo.fs[k] == 0) continue;
                    outfile << globalInfo.fs[k];
                }
                count++;
            }
            outfile.close();
        }
    }
}

void details(GlobalInfo &globalInfo, string sourceFileName){
    for (int i = 0; i < globalInfo.myFiles.size(); ++i) {
        if (sourceFileName.compare(globalInfo.myFiles.at(i).name) == 0) {
            cout << "----------------------------------------" << endl;
            cout << "File name: " << globalInfo.myFiles.at(i).name << endl;
            cout << "Number of blocks: " << globalInfo.myFiles.at(i).dataIndex.size() << endl;
            cout << "Used blocks: ";
            for(int k = 0; k < globalInfo.myFiles.at(i).dataIndex.size(); k++){
                cout << globalInfo.myFiles.at(i).dataIndex[k] << " ";
            }

        }
    }
    cout << endl << "----------------------------------------" << endl;
}

void loadFileSystem(GlobalInfo &globalInfo, string name) {
    string Generalname = name + ".info";

    ifstream finputTest(Generalname, ios::binary);
    if(finputTest.fail()){
        cout << "Unable to find specified file system." << endl;
        return;
    }
    finputTest.close();

    string dataName = name + ".data";
    vector<string> v_info = getFileParameters(Generalname);
    vector<string> v_data = getFileParameters(dataName);


    bool *tempStatus = new bool[globalInfo.nBlocks];
    GlobalInfo tempGlobal;

    //info info begins
    strcpy(tempGlobal.currentFS, v_info[0].c_str());
    v_info.erase(v_info.begin());
    tempGlobal.bSize = atoi(v_info[0].c_str());
    v_info.erase(v_info.begin());
    tempGlobal.nBlocks = atoi(v_info[0].c_str());
    v_info.erase(v_info.begin());
    tempGlobal.fileNumbers = atoi(v_info[0].c_str());
    v_info.erase(v_info.begin());

    vector<Files> myFiles;

    //heads info begins
    for (int i = 0; i < tempGlobal.fileNumbers; i++) {
        //pop size
        Files file;
        file.size = atoi(v_info[0].c_str());
        v_info.erase(v_info.begin());
        //pop name
        strcpy(file.name, v_info[0].c_str());
        v_info.erase(v_info.begin());
        //pop heads
        for (int i = 0; i < file.size; ++i) {
            file.dataIndex.push_back(atoi(v_info[0].c_str()));
            v_info.erase(v_info.begin());
        }
        myFiles.push_back(file);
    }

    globalInfo = tempGlobal;
    globalInfo.myFiles = myFiles;
    globalInfo.initialized = true;

    //data info begins
    bool *tempBlockState = new bool[tempGlobal.nBlocks];
    v_data.erase(v_data.begin());
    for(int i = 0; i < tempGlobal.nBlocks; i++){
        int state = atoi(v_data[0].c_str());
        v_data.erase(v_data.begin());
        if(state == 0){
            tempBlockState[i] = false;
        }else{
            tempBlockState[i] = true;
        }
    }

    char *tempFS = static_cast<char *>(malloc(globalInfo.bSize * globalInfo.nBlocks * sizeof(char)));
    for(int i = 0; i < tempGlobal.nBlocks * tempGlobal.bSize; i++){
        int item = atoi(v_data[i].c_str());
        tempFS[i] = (char)item;
    }
    globalInfo.blockState = tempBlockState;
    globalInfo.fs = tempFS;
}

vector<string> getFileParameters(string headsName) {
    ifstream finputHeads(headsName, ios::binary);
    finputHeads.seekg(0, finputHeads.end);
    int length = finputHeads.tellg();
    finputHeads.seekg(0, finputHeads.beg);
    char *buffer = new char[length];
    finputHeads.read(buffer, length);
    finputHeads.close();
    string strBuffer(buffer);
    return split(strBuffer, '|');
}

void saveGlobalInfo(GlobalInfo &globalInfo) {
    string infoName(globalInfo.currentFS);
    infoName += ".info";
    ofstream fileGlobalInfo(infoName, ios::out | ios::binary);
    fileGlobalInfo << globalInfo.currentFS;
    fileGlobalInfo << "|";
    fileGlobalInfo << globalInfo.bSize;
    fileGlobalInfo << "|";
    fileGlobalInfo << globalInfo.nBlocks;
    fileGlobalInfo << "|";
    fileGlobalInfo << globalInfo.fileNumbers;
    fileGlobalInfo << "|";
    //fileGlobalInfo.close();

    //string headsName(globalInfo.currentFS);
    //headsName += ".heads";
    //ofstream fileGlobalHeads(headsName, ios::binary);

    for (int i = 0; i < globalInfo.myFiles.size(); ++i) {
        fileGlobalInfo << globalInfo.myFiles[i].size;
        fileGlobalInfo << '|';
        fileGlobalInfo << globalInfo.myFiles[i].name;
        for (int j = 0; j < globalInfo.myFiles[i].dataIndex.size(); ++j) {
            fileGlobalInfo << '|';
            fileGlobalInfo << globalInfo.myFiles[i].dataIndex[j];
        }
        fileGlobalInfo << '|';
    }

    string dataName(globalInfo.currentFS);
    dataName += ".data";
    ofstream fileGlobalData(dataName, ios::binary);
    for(int i = 0; i < globalInfo.nBlocks; i++){
        fileGlobalData << '|';
        fileGlobalData << globalInfo.blockState[i];
    }

    for(int i = 0; i < globalInfo.nBlocks * globalInfo.bSize; i++){
        fileGlobalData << '|';
        fileGlobalData << (int)globalInfo.fs[i];
    }

}