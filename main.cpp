#include "Commands.h"
#include <iostream>

using namespace std;

GlobalInfo globalInfo;
void *blockStatus;

int main() {
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
                    blockStatus = createFileSystem(globalInfo, lineParts);
                    break;
                case 1://save
                    cout << sizeof(blockStatus) << endl;
                    break;
                case 2://open
                    break;
                case 3://load
                    loadFile();
                    break;
                case 4://download
                    break;
                case 5://ls
                    break;
                case 6://rm
                    break;
                case 7://info
                    readFileSystem(globalInfo);
                    break;
                case 8://details
                    break;
            }


        }
    }

}


