#include "Commands.h"
#include <iostream>

using namespace std;

GlobalInfo globalInfo;
bool *blockStatus;

int main() {
    globalInfo.initialized = false;
    vector<string> cmds = createCmds();
    int cmdIndex = -1;
    while (cmdIndex < 9) {
        cout << (char) 38 << " ";

        string line;
        getline(cin, line);
        vector<string> lineParts = split(line, ' ');

        cmdIndex = getCmdIndex(lineParts[0], cmds);
        if (cmdIndex >= 0) {
            cout << "user cmd: " << cmds[cmdIndex] << endl;

            switch (cmdIndex) {
                case 0://create
                    if (!globalInfo.initialized) {
                        blockStatus = createFileSystem(globalInfo, lineParts);
                    } else cout << "You have a file system loaded already" << endl;
                    break;
                case 1://save
                    if (globalInfo.initialized) {
                        cout << sizeof(blockStatus) << endl;
                    }
                    break;
                case 2://open
                    if (!globalInfo.initialized) {
                        loadFileSystem(globalInfo, lineParts[1]);
                    } else cout << "You have a file system loaded already" << endl;
                    break;
                case 3://load
                    if (globalInfo.initialized) {
                        loadFile(globalInfo, blockStatus);
                    }
                    break;
                case 4://download
                    if (globalInfo.initialized) {
                        downloadFile(globalInfo);
                    }
                    break;
                case 5://ls
                    if (globalInfo.initialized) {
                        listFiles(globalInfo);
                    }
                    break;
                case 6://rm
                    if (globalInfo.initialized) {
                        removeFile(globalInfo, blockStatus);
                    }
                    break;
                case 7://info
                    if (globalInfo.initialized) {
                        readFileSystem(globalInfo);
                        readDataFile(globalInfo);
                    }
                    break;
                case 8://details
                    if (globalInfo.initialized) {}
                    break;
            }
            if (!globalInfo.initialized)
                cout << "please open a file system first" << endl;
        }
    }

}


