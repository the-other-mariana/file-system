#include <iostream>
#include "Functions.h"

int main() {
    vector<string> cmds = createCmds();

    int cmdIndex = -1;
    while(cmdIndex < 9){

        string line;
        getline(cin, line);
        vector<string> lineParts = split(line, ' ');

        cmdIndex = getCmdIndex(lineParts[0], cmds);
        if(cmdIndex >= 0){
            cout << "user cmd: " << cmds[cmdIndex] << endl;

            switch(cmdIndex){
                case 0:
                    create(lineParts[1], lineParts[2], lineParts[3]);
                    break;
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    break;
                case 4:
                    break;
            }


        }
    }


}