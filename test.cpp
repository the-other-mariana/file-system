
Conversación abierta. 1 mensaje leído.

Ir al contenido
        Uso de Correo de Universidad Panamericana con lectores de pantalla
9 de 3.554
os 5
Recibidos
        x
Mariana Ávalos Arce <0197495@up.edu.mx>

Archivos adjuntos8 may. 2019 23:45 (hace 4 días)

para mí

Zona de los archivos adjuntos




#include<iostream>
#include <stdio.h>
#include<string.h>
#include <sstream>
#include <bits/stdc++.h>
#include <fstream>
#include <math.h>
using namespace std;

string currentFS;
int bSize;
int nBlocks;
char *fs;
char *blockState;

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

int readInt(const char *file, int begin){
    ifstream is(file, ios::binary);

    is.seekg (0, is.end);
    int length = is.tellg();
    is.seekg (0, is.beg);

    char * buffer = new char [length];
    is.read(buffer,length);
    is.close();


    int num = 0;

    num = (num << 8) + buffer[begin + 3];
    num = (num << 8) + buffer[begin + 2];
    num = (num << 8) + buffer[begin + 1];
    num = (num << 8) + buffer[begin + 0];

    return num;
}

char readChar(const char *file, int pos){
    ifstream is(file, ios::binary);

    is.seekg (0, is.end);
    int length = is.tellg();
    is.seekg (0, is.beg);

    char * buffer = new char [length];
    is.read(buffer,length);
    is.close();

    return (char)buffer[pos];
}

char *readString(const char *file, int begin){
    ifstream is(file, ios::binary);

    is.seekg (0, is.end);
    int length = is.tellg();
    is.seekg (0, is.beg);

    char *buffer = new char [length];
    is.read(buffer,length);
    is.close();

    int strlen = 0;
    for(int i = 0; i < length; i++){
        if ((int)buffer[i] == 35) break;
        strlen++;
    }

    char * result = new char[strlen];
    for(int i = begin; i < begin + strlen; i++){
        result[i] = buffer[i];
    }

    return result;
}

void printSB(string name){
    char *nameSB = readString("superblock.bin", 0);

    cout << "------------------------------------" << endl;
    for(int i = 0; i < name.length(); i++){
        cout<<nameSB[i];
    }
    cout << endl;

    for(int i = 0; i < nBlocks; i++){
        cout << "Block" << i << ": " << (int)readChar("superblock.bin", name.length() + 1 + i) << endl;
    }

    cout << "Block size: " << readInt("superblock.bin", name.length() + 1 + nBlocks) << endl;
    cout << "Number of blocks: " << readInt("superblock.bin", name.length() + 1 + nBlocks + 4) << endl;
    cout << "------------------------------------" << endl;
}

void initBlockState(int nb){
    blockState = new char[nb];
    for(int i = 0; i < nb; i++){
        blockState[i] = 0;
    }
}

void updateSB(const char *file){
    ofstream sb(file, std::ios::out | std::ios::binary);
    sb << currentFS;
    sb << "#";

    for(int i = 0; i < nBlocks; i++){
        sb << blockState[i];
    }

    sb.write((char*) &bSize, sizeof(bSize));
    sb.write((char*) &nBlocks, sizeof(nBlocks));

    sb.close();
}

void create(string name, string blocksize, string numberOfBlocks){
    int bs, nb;

    istringstream (blocksize) >> bs;
    istringstream (numberOfBlocks) >> nb;
    bSize = bs;
    nBlocks = nb;

    int bytes = bs * nb;
    fs = (char*) malloc(bytes * sizeof(char));
    if (fs != NULL) cout << "File System created successfully" << endl;

    initBlockState(nb);
    updateSB("superblock.bin");
}

void assignBlocks(int num){
    srand(time(NULL));
    int cont = 0;
    while(cont < num){
        int index = rand()%((nBlocks - 1) - 0 + 1) + 0;
        if((int)blockState[index] == 0){
            blockState[index] = 1;
            cont++;
        }
    }
    updateSB("superblock.bin");
}

void loadFile(const char *file, const char *virtualFile){
    ifstream is(file, ios::binary);

    is.seekg (0, is.end);
    int length = is.tellg();
    is.seekg (0, is.beg);

    char * buffer = new char [length];
    is.read(buffer,length);
    is.close();

    for(int i = 0; i < length; i++){
        cout<<buffer[i];
    }

    cout << endl;
    cout << "bytes: " << length << endl;

    int blocks = ceil((length * 1.00) / (bSize * 1.00));
    cout << "blocks: " << blocks << endl;

    assignBlocks(blocks);

}

int main(){
    vector<string> cmds = createCmds();

    int cmdIndex = -1;
    while(cmdIndex < 9){

        cout << (char)38 << " ";

        string line;
        getline(cin, line);
        vector<string> lineParts = split(line, ' ');

        cmdIndex = getCmdIndex(lineParts[0], cmds);
        if(cmdIndex >= 0){
            cout << "user cmd: " << cmds[cmdIndex] << endl;

            switch(cmdIndex){
                case 0:
                    currentFS = lineParts[1];
                    create(lineParts[1], lineParts[2], lineParts[3]);

                    break;
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    loadFile(lineParts[1].c_str(), lineParts[2].c_str());
                    break;
                case 4:
                    break;
                case 5:
                    break;
                case 6:
                    break;
                case 7:
                    printSB(currentFS);
                    break;
                case 8:
                    break;
            }


        }
    }

}


