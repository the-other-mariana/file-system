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
    char *blockState;
    bool initialized;
    int fileNumbers;
    vector<Files> myFiles;
};

void saveGlobalInfo(GlobalInfo &globalInfo);

vector<string> split(string line, char separator);

void UpdateFile(GlobalInfo &globalInfo, bool *blockStatus);

vector<string> createCmds();

int getCmdIndex(string value, vector<string> cmds);

bool *createFileSystem(GlobalInfo &globalInfo, vector<string> parameters);


void readDataFile(const GlobalInfo &newInfo);


vector<string> getFileParameters(string);

void loadFile(GlobalInfo &globalInfo, bool *blockStatus, string name = "test-file.cpp") {
    srand(time(NULL));
    if (!globalInfo.initialized)cout << "create a file system first!";
    Files newFile = *new Files;
    strcpy(newFile.name, (const char *) name.c_str());
    ifstream fentrada(name, ios::binary);
    fentrada.seekg(0, fentrada.end);
    int length = fentrada.tellg();
    fentrada.seekg(0, fentrada.beg);
    char *buffer = new char[length];
    fentrada.read(buffer, length);
    fentrada.close();
    int blocks = ceil((length * 1.0) / (globalInfo.bSize * 1.0));
    cout << length;
    if (length < globalInfo.nBlocks * globalInfo.bSize) cout << "true"; else cout << "false";
    int j = 0;
    for (int count = 0; count < blocks;) {
        int index = rand() % ((globalInfo.nBlocks - 1) + 1);
        if (!blockStatus[index]) {
            newFile.dataIndex.push_back(index);
            blockStatus[index] = true;
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

void AssignBlocks(int nBlocks) {

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
    cout << sizeof(cmds);
    return cmds;
}

int getCmdIndex(string value, vector<string> cmds) {
    int index = -1;
    for (int i = 0; i < cmds.size(); i++)
        if (value == cmds[i])
            index = i;
    return index;
}

bool *createFileSystem(GlobalInfo &globalInfo, vector<string> parameters) {
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

    //Save Global info to file
    ofstream fileGlobalInfo("my-fs.info", ios::out | ios::binary);
    fileGlobalInfo.write((char *) &globalInfo, sizeof(GlobalInfo));
    globalInfo.fs = static_cast<char *>(malloc(globalInfo.bSize * globalInfo.nBlocks * sizeof(char)));
    fileGlobalInfo.close();


    UpdateFile(globalInfo, tempStatus);
    return tempStatus;

}

void UpdateFile(GlobalInfo &globalInfo, bool *blockStatus) {
    ofstream fileblockStatus("my-fs.data", ios::out | ios::binary);
    fileblockStatus.write(reinterpret_cast<const char *>(blockStatus), sizeof(blockStatus));
    fileblockStatus.close();
}


void readFileSystem(GlobalInfo &newInfo) {
    ifstream fentrada("my-fs.info", ios::in | ios::binary);
    fentrada.read(reinterpret_cast<char *>(&newInfo), sizeof(GlobalInfo));
    cout << newInfo.currentFS << endl;
    cout << newInfo.nBlocks << endl;
    cout << newInfo.bSize << endl;
    newInfo.initialized = true;
    fentrada.close();
}

void readDataFile(const GlobalInfo &newInfo) {
    ifstream fentrada("my-fs.data", ios::in | ios::binary);
    fentrada.seekg(0, fentrada.end);
    int length = fentrada.tellg();
    fentrada.seekg(0, fentrada.beg);
    char *buffer = new char[length];
    fentrada.read(buffer, length);
    fentrada.close();
    cout << "------------------\n";
    for (int i = 0; i < newInfo.nBlocks; ++i) {
        cout << buffer[i] << endl;
    }
    cout << "------------------\n";
}


void listFiles(GlobalInfo &globalInfo) {
    for (int i = 0; i < globalInfo.myFiles.size(); ++i) {
        cout << globalInfo.myFiles.at(i).name << "\t";
    }
    cout << endl;
}

void removeFile(GlobalInfo &globalInfo, bool *blockStatus, string fileName = "test-file.cpp") {
    for (int i = 0; i < globalInfo.myFiles.size(); ++i) {
        if (fileName.compare(globalInfo.myFiles.at(i).name) == 0) {
            for (int j = 0; j < globalInfo.myFiles.at(i).dataIndex.size(); ++j) blockStatus[j] = false;
            globalInfo.myFiles.erase(globalInfo.myFiles.begin() + i);
            globalInfo.fileNumbers = globalInfo.myFiles.size();
            break;
        }
    }
}

void downloadFile(GlobalInfo &globalInfo, string sourceFileName = "test-file.cpp",
                  string destFilePath = "/home/developer/Desktop/hello.cpp") {
    for (int i = 0; i < globalInfo.myFiles.size(); ++i) {
        if (sourceFileName.compare(globalInfo.myFiles.at(i).name) == 0) {
            ofstream outfile(destFilePath, ios::out);
            for (int count = 0; count < globalInfo.myFiles.at(i).dataIndex.size();) {
                int index = globalInfo.myFiles.at(i).dataIndex.at(count);
                for (int k = index * globalInfo.bSize; k < index * globalInfo.bSize + globalInfo.bSize; ++k) {
                    outfile << globalInfo.fs[k];
                }
                count++;
            }
            outfile.close();
        }
    }
}

void loadFileSystem(GlobalInfo &globalInfo, string name) {
    string Generalname = name + ".info";
    string headsName = name + ".heads";
    vector<string> v_info = getFileParameters(Generalname);
    vector<string> v_heads = getFileParameters(headsName);
    bool *tempStatus = new bool[globalInfo.nBlocks];
    GlobalInfo tempGlobal;

    strcpy(tempGlobal.currentFS, v_info[0].c_str());
    v_info.erase(v_info.begin());
    tempGlobal.bSize = atoi(v_info[0].c_str());
    v_info.erase(v_info.begin());
    tempGlobal.nBlocks = atoi(v_info[0].c_str());
    v_info.erase(v_info.begin());
    tempGlobal.fileNumbers = atoi(v_info[0].c_str());

    vector<Files> myFiles;

    for (int i = 0; i < tempGlobal.fileNumbers; i++) {
        //pop size
        Files file;
        file.size = atoi(v_heads[0].c_str());
        v_heads.erase(v_heads.begin());
        //pop name
        strcpy(file.name, v_heads[0].c_str());
        v_heads.erase(v_heads.begin());
        //pop heads
        for (int i = 0; i < file.size; ++i) {
            file.dataIndex.push_back(atoi(v_heads[0].c_str()));
            v_heads.erase(v_heads.begin());
        }
        myFiles.push_back(file);
    }
    globalInfo = tempGlobal;
    globalInfo.myFiles = myFiles;
    globalInfo.initialized = true;

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
    ofstream fileGlobalInfo("my-fs.info", ios::out | ios::binary);
    fileGlobalInfo << globalInfo.currentFS;
    fileGlobalInfo << "|";
    fileGlobalInfo << globalInfo.bSize;
    fileGlobalInfo << "|";
    fileGlobalInfo << globalInfo.nBlocks;
    fileGlobalInfo << "|";
    fileGlobalInfo << globalInfo.fileNumbers;
    fileGlobalInfo << "|";
    fileGlobalInfo.close();
    ofstream fileGlobalHeads("my-fs.heads", ios::binary);
    for (int i = 0; i < globalInfo.myFiles.size(); ++i) {
        fileGlobalHeads << globalInfo.myFiles[i].size;
        fileGlobalHeads << '|';
        fileGlobalHeads << globalInfo.myFiles[i].name;
        for (int j = 0; j < globalInfo.myFiles[i].dataIndex.size(); ++j) {
            fileGlobalHeads << '|';
            fileGlobalHeads << globalInfo.myFiles[i].dataIndex[j];
        }
        fileGlobalHeads << '|';
    }

}