//
// Created by mariana on 6/05/19.
//

#ifndef FILE_SYSTEM_FUNCTIONS_H
#define FILE_SYSTEM_FUNCTIONS_H
#include<iostream>
#include <stdio.h>
#include<string.h>
#include <sstream>
#include <bits/stdc++.h>
using namespace std;

vector<string> split (string line, char separator){
    vector<string> lineParts;
    stringstream stream(line);
    string part;

    while(getline(stream, part, separator)){
        lineParts.push_back(part);
    }
    return lineParts;
}

vector<string> createCmds(){
    vector<string> cmds;
    cmds.push_back("create");
    cmds.push_back("save");
    cmds.push_back("open");
    cmds.push_back("load");
    cmds.push_back("download");
    cmds.push_back("ls");
    cmds.push_back("rm");
    cmds.push_back("info");
    cmds.push_back("details");
    cmds.push_back("exit");

    return cmds;
}

int getCmdIndex(string value, vector<string> cmds){
    int index = -1;

    for(int i = 0; i < cmds.size(); i++){
        if(value == cmds[i]){
            index = i;
        }
    }
    return index;
}

void create(string name, string blocksize, string numberOfBlocks){

    int bs, nb;
    // parsing string params to ints
    istringstream (blocksize) >> bs;
    istringstream (numberOfBlocks) >> nb;

    // assigning memory
    int bytes = bs * nb;
    char *fs = (char*) malloc(bytes * sizeof(char));
    if (fs != NULL) cout << "File System created successfully" << endl;

    /*
    // writing superblock
    ofstream sb("superblock.bin", std::ios::out | std::ios::binary);
    sb.write((char*) &bytes, sizeof(int));
    sb.close();

    ifstream is;
    //is.open ("superblock.bin", ios::binary);

    for(int i = 0; i < bytes; i++){
        cout << (char)fs[i] << endl;
    }
    */
}

#endif //FILE_SYSTEM_FUNCTIONS_H


