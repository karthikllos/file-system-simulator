#include "fileSystem.h"
#include <iostream>
#include <string>
using namespace std;

void displayHelperBox() {
    cout << "====================== COMMANDS ======================\n";
    cout << "mkdir <dirname>    : Create a directory.\n";
    cout << "cd <dirname>       : Change directory.\n";
    cout << "cd ..              : Move to parent directory.\n";
    cout << "mkfile <filename>  : Create a file.\n";
    cout << "edit <filename>    : Edit a file (append content).\n";
    cout << "read <filename>    : Read and display file content.\n";
    cout << "del <filename>     : Delete a file.\n";
    cout << "deld <dirname>     : Delete a directory\n";
    cout << "ls                 : List contents of the current directory.\n";
    cout << "cpy <src>          : Copy a file (enter destination when prompted).\n";
    cout << "cut <src>          : Cut a file (enter destination when prompted).\n";
    cout << "move <src>         : Move a file (enter destination when prompted).\n";
    cout << "rename <src> <dst> : Rename a file from <src> to <dst>.\n";
    cout << "exit               : Exit the program.\n";
    cout << "clear              : Clears the screen\n";
    cout << "Del                : List of files to choose which one to delete\n";
    cout << "======================================================\n\n";
}


int main() {
    FileSystem fs;
    string input;

    // Display helper box at the start
    displayHelperBox();

    int t = 1;
    while (t) {
        cout << endl;
        fs.showCurrentPath();
        getline(cin, input);

        if (input == "exit") {
            t = 0;
        } else if (input.rfind("mkdir", 0) == 0) {
            fs.createDirectory(input.substr(6));
        } else if (input.rfind("cd", 0) == 0) {
            fs += input.substr(3);
        } else if (input.rfind("mkfile", 0) == 0) {
            fs + input.substr(7);
        } else if (input == "ls") {
            *fs;
        } else if (input.rfind("edit", 0) == 0) {
            fs >> input.substr(5);
            fs << input.substr(5);
        } else if (input.rfind("read", 0) == 0) {
            fs << input.substr(5);
        } else if (input.rfind("del", 0) == 0 && input.rfind("deld", 0) != 0 && input.substr(4)!="") {
            fs - input.substr(4);
        } else if (input.rfind("Del", 0) == 0){
            fs.del();
        } else if (input == "cd ..") {
            --fs;
        } else if (input.rfind("cpy", 0) == 0) {
            string dest;
            cout << "Enter the destination: ";
            cin >> dest;
            fs.copyFile(input.substr(4), dest);
            cout << "Copied '" << input.substr(4) << "' to '" << dest << "'\n";
            cin.ignore();
        } else if (input.rfind("cut", 0) == 0) {
            string dest;
            cout << "Enter the destination: ";
            cin >> dest;
            fs.cutFile(input.substr(4), dest);
            cout << "Cut '" << input.substr(3) << "' to '" << dest << "'\n";
            cin.ignore();
        } else if (input.rfind("move", 0) == 0) {
            string dest;
            cout << "Enter the destination: ";
            cin >> dest;
            fs.moveFile(input.substr(5), dest);
            cout << "Moved '" << input.substr(5) << "' to '" << dest << "'\n";
            cin.ignore();
        } else if (input.rfind("rename", 0) == 0) {
            size_t pos = input.find(' ', 7); // Find space after "rename "
            if (pos == string::npos) {
                cerr << "Invalid command format. Use: rename <src> <dst>\n";
                continue;
            }
            string oldName = input.substr(7, pos - 7);
            string newName = input.substr(pos + 1);
            fs.renameFile(oldName, newName);
        } else if(input.rfind("deld", 0) == 0){
            fs.delDirectory(input.substr(5));
        } else if(input.rfind("clear", 0) == 0) {
            fs.clearScreen();
        } else if(input.rfind("open",0) == 0){
            fs.openFile(input.substr(5));
        } else {
            if(input != ""){
                cerr << "Unknown command: " << input << "\n";
                displayHelperBox();
            }
        }
    }

    return 0;
}
