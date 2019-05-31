#include "Commands.h"
#include <iostream>

using namespace std;

GlobalInfo globalInfo;
bool *blockStatus;

int main() {
    globalInfo.initialized = false;
    vector<string> cmds = createCmds();
    int cmdIndex = -1;

    cout << "-- Virtual File System Program. 2019-2. Version 1.0 --" << endl;
    cout << "Please enter a command. Enter << exit >> to terminate the program." << endl;

    while (cmdIndex < 9) {
        cout << (char) 38 << " ";

        string line;
        getline(cin, line);
        vector<string> lineParts = split(line, ' ');

        cmdIndex = getCmdIndex(lineParts[0], cmds);
        if (cmdIndex >= 0) {

            switch (cmdIndex) {
                case 0://create *name* *blockSize* *numberOfBlocks*
                    if (!globalInfo.initialized && lineParts.size() == 4) {
                        if(validFS(lineParts)) createFileSystem(globalInfo, lineParts);
                    } else cout << "You have a file system loaded already." << endl;
                    break;
                case 1://save
                    if (globalInfo.initialized) {
                        cout << "File system saved successfully." << endl;
                    }
                    break;
                case 2://open *name*
                    if (!globalInfo.initialized && lineParts.size() == 2) {
                        loadFileSystem(globalInfo, lineParts[1]);
                    } else cout << "You have a file system loaded already." << endl;
                    break;
                case 3://load original.txt copy.txt
                    if (globalInfo.initialized && lineParts.size() == 3) {
                        loadFile(globalInfo, lineParts[1], lineParts[2]);
                    }
                    break;
                case 4://download copy.txt newCopy.txt
                    if (globalInfo.initialized && lineParts.size() == 3) {
                        downloadFile(globalInfo, lineParts[1], lineParts[2]);
                    }
                    break;
                case 5://ls
                    if (globalInfo.initialized) {
                        listFiles(globalInfo);
                    }
                    break;
                case 6://rm archivo.txt
                    if (globalInfo.initialized && lineParts.size() == 2) {
                        removeFile(globalInfo, lineParts[1]);
                    }
                    break;
                case 7://info
                    if (globalInfo.initialized) {
                        displayInfo(globalInfo);
                    }
                    break;
                case 8://details *file.txt*
                    if (globalInfo.initialized && lineParts.size() == 2) {
                        details(globalInfo, lineParts[1]);
                    }
                    break;
            }
            if (!globalInfo.initialized)
                cout << "Please open a file system first" << endl;
        }
    }

}


