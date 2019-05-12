#include <vector>
#include <string>
#include <sstream>
#include <cstring>
#include <fstream>
#include <cmath>
#include <iostream>

using namespace std;

struct GlobalInfo {
    char currentFS[40];
    int bSize;
    int nBlocks;
    char *fs;
    char *blockState;
    bool initialized;
};

struct BlockStatus {
    char *data; //arreglo de chars de tamaño [bSize]
    void *nextData; // Si el archivo es mas grande que BSize apunta al siguiente bloque
    bool hasMoreData; // Pregunta si hay mas info
    bool isAvailable; // Pregunta si esta disponible el bloque
};

vector<string> split(string line, char separator);

vector<string> createCmds();

int getCmdIndex(string value, vector<string> cmds);

BlockStatus *createFileSystem(GlobalInfo &globalInfo, vector<string> parameters);

void UpdateFile(GlobalInfo &globalInfo);

void loadFile(string name = "test-file.cpp", int blockSize = 45, int nBlock = 4) {

    ifstream fentrada(name, ios::binary);
    fentrada.seekg(0, fentrada.end);
    int length = fentrada.tellg();
    fentrada.seekg(0, fentrada.beg);
    char *buffer = new char[length];
    fentrada.read(buffer, length);
    fentrada.close();
    int blocks = ceil((length * 1.0) / (blockSize * 1.0));
    cout << length;
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

BlockStatus *createFileSystem(GlobalInfo &globalInfo, vector<string> parameters) {
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
    BlockStatus *tempStatus = new BlockStatus[globalInfo.nBlocks];
    //Save Global info to file
    ofstream fileGlobalInfo("my-fs.info", ios::out | ios::binary);
    fileGlobalInfo.write((char *) &globalInfo, sizeof(GlobalInfo));
    globalInfo.fs = static_cast<char *>(malloc(globalInfo.bSize * globalInfo.nBlocks * sizeof(char)));
    fileGlobalInfo.close();


    UpdateFile(globalInfo);
    return tempStatus;

}

void UpdateFile(GlobalInfo &globalInfo) {
    ofstream fileblockStatus("my-fs.data", ios::out | ios::binary);


    fileblockStatus.close();
}

void readFileSystem(GlobalInfo &newInfo) {
    string name = "";
    string temp = "";
    cout << endl << "Name of the file system to read" << endl;
    cin >> temp;
    name = temp + ".info";
    ifstream fentrada(name, ios::in | ios::binary);
    fentrada.read(reinterpret_cast<char *>(&newInfo), sizeof(GlobalInfo));
    cout << newInfo.currentFS << endl;
    cout << newInfo.nBlocks << endl;
    cout << newInfo.bSize << endl;
    newInfo.initialized = true;
    fentrada.close();
}