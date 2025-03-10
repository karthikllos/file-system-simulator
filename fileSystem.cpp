#include"fileSystem.h"


string FileSystem::wstringToString(const wstring &wstr) {
    return string(wstr.begin(), wstr.end());
}



void FileSystem::createFile(const string &filename) {
    try {
        wstring fullPath = currentPath + L"\\" + wstring(filename.begin(), filename.end());
        string fullPathStr = wstringToString(fullPath);
        ofstream file(fullPathStr);
        if (!file) throw runtime_error("Failed to create file: " + filename);
        cout << "File created: " << filename << "\n";
    } catch (const exception &e) {
        cerr << e.what() << "\n";
    }
}





void FileSystem::deleteFile(const string &filename) {
    try {
        wstring fullPath = currentPath + L"\\" + wstring(filename.begin(), filename.end());
        string fullPathStr = wstringToString(fullPath);
        if (remove(fullPathStr.c_str()) != 0) throw runtime_error("Failed to delete file: " + filename);
        cout << "File deleted: " << filename << "\n";
    } catch (const exception &e) {
        cerr << e.what() << "\n";
    }
}






void FileSystem::readFile(const string &filename) {
    try {
        wstring fullPath = currentPath + L"\\" + wstring(filename.begin(), filename.end());
        string fullPathStr = wstringToString(fullPath);
        ifstream file(fullPathStr);
        if (!file) throw runtime_error("Failed to read file: " + filename);
        cout << "Reading file: " << filename << "\n";
        string line;
        while (getline(file, line)) cout << line << "\n";
    } catch (const exception &e) {
        cerr << e.what() << "\n";
    }
}






void FileSystem::createDirectory(const string &dirname) {
    try {
        wstring fullDirPath = currentPath + L"\\" + wstring(dirname.begin(), dirname.end());
        if (!CreateDirectoryW(fullDirPath.c_str(), NULL)) throw runtime_error("Failed to create directory: " + dirname);
        cout << "Directory created: " << dirname << "\n";
    } catch (const exception &e) {
        cerr << e.what() << "\n";
    }
}







void FileSystem::changeDirectory(const string &dirname) {
    try {
        // Convert the input directory path to a wide string
        wstring wdirname(dirname.begin(), dirname.end());
        wstring newPath;

        // Check if the given path is absolute or relative
        if (fs::path(dirname).is_absolute()) {
            // For absolute paths, directly use the provided path
            newPath = wdirname;
        } else if (dirname == "..") {
            // Handle parent directory
            size_t pos = currentPath.find_last_of(L"\\/");
            if (pos != wstring::npos) {
                newPath = currentPath.substr(0, pos);
            } else {
                throw runtime_error("Cannot move up from the root directory.");
            }
        } else {
            // For relative paths, append to the current path
            newPath = currentPath + L"\\" + wdirname;
        }

        // Verify if the new path exists and is a directory
        if (!(GetFileAttributesW(newPath.c_str()) & FILE_ATTRIBUTE_DIRECTORY)) {
            throw runtime_error("Error: Directory not found: " + dirname);
        }

        // Update the current path
        currentPath = newPath;

        // Notify the user
        wcout << L"Changed directory to: " << currentPath << L"\n";
    } catch (const exception &e) {
        cerr << e.what() << "\n";
    }
}





void FileSystem::listCurrentDirectory() {
    try {
        wcout << L"Contents of directory " << currentPath << L":\n";
        wstring searchPath = currentPath + L"\\*";
        WIN32_FIND_DATAW findFileData;
        HANDLE hFind = FindFirstFileW(searchPath.c_str(), &findFileData);
        if (hFind == INVALID_HANDLE_VALUE) throw runtime_error("Failed to open directory.");
        do {
            wcout << (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ? L"[DIR] " : L"[FILE] ") << findFileData.cFileName << L"\n";
        } while (FindNextFileW(hFind, &findFileData) != 0);
        FindClose(hFind);
    } catch (const exception &e) {
        cerr << e.what() << "\n";
    }
}







void FileSystem::editFile(const string &filename) {
    wstring fullPath = currentPath + L"\\" + wstring(filename.begin(), filename.end());
    string fullPathStr = wstringToString(fullPath);

    ifstream checkFile(fullPathStr);
    if (!checkFile) {
        cerr << "File not found: " << filename << "\n";
        return;
    }
    checkFile.close();

    ofstream file(fullPathStr, ios::app);
    if (!file) {
        cerr << "Failed to open file for editing: " << filename << "\n";
        return;
    }

    cout << "Enter content to append to the file:\n";
    string newContent;
    getline(cin, newContent);

    file << newContent << "\n";
    file.close();

    cout << "Content added to " << filename << "\n";
}







void FileSystem::copyFile(const string &sourcePath, const string &destinationPath)
{
    // Convert paths to wide strings for better handling on Windows
    wstring fullSourcePath = currentPath + L"\\" + wstring(sourcePath.begin(), sourcePath.end());
    wstring fullDestinationPath = currentPath + L"\\" + wstring(destinationPath.begin(), destinationPath.end());

    string fullSourcePathStr = wstringToString(fullSourcePath);
    string fullDestinationPathStr = wstringToString(fullDestinationPath);

    try {
        // Check if the source file exists
        if (!fs::exists(fullSourcePathStr)) {
            cerr << "Error: Source file does not exist: " << sourcePath << endl;
            return;
        }

        // Perform the copy, allowing overwrite if destination exists
        fs::copy(fullSourcePathStr, fullDestinationPathStr, fs::copy_options::overwrite_existing);
        cout << "File copied from " << sourcePath << " to " << destinationPath << endl;
    } catch (const fs::filesystem_error &e) {
        cerr << "Error copying file: " << e.what() << endl;
    }
}









void FileSystem::cutFile(const string &filename, const string &destinationDir)
{
    // Convert paths to wide strings for better handling on Windows
    wstring fullSourcePath = currentPath + L"\\" + wstring(filename.begin(), filename.end());
    wstring fullDestinationPath = wstring(destinationDir.begin(), destinationDir.end()) + L"\\" + wstring(filename.begin(), filename.end());

    string fullSourcePathStr = wstringToString(fullSourcePath);
    string fullDestinationPathStr = wstringToString(fullDestinationPath);

    try {
        // Check if the source file exists
        if (!fs::exists(fullSourcePathStr)) {
            cerr << "Error: Source file does not exist: " << filename << endl;
            return;
        }

        // Ensure the destination directory exists
        if (!fs::exists(destinationDir)) {
            cerr << "Error: Destination directory does not exist: " << destinationDir << endl;
            return;
        }

        // Perform the move (copy + delete source)
        fs::copy(fullSourcePathStr, fullDestinationPathStr, fs::copy_options::overwrite_existing);
        fs::remove(fullSourcePathStr);

        cout << "File cut (moved) from " << filename << " to " << destinationDir << endl;
    } catch (const fs::filesystem_error &e) {
        cerr << "Error cutting file: " << e.what() << endl;
    }
}







void FileSystem::moveFile(const string &filename, const string &destinationDir)
{
    wstring fullSourcePath = currentPath + L"\\" + wstring(filename.begin(), filename.end());
    wstring fullDestinationPath = wstring(destinationDir.begin(), destinationDir.end()) + L"\\" + wstring(filename.begin(), filename.end());

    string fullSourcePathStr = wstringToString(fullSourcePath);
    string fullDestinationPathStr = wstringToString(fullDestinationPath);

    try {
        // Check if the source file exists
        if (!fs::exists(fullSourcePathStr)) {
            cerr << "Error: Source file does not exist: " << fullSourcePathStr << endl;
            return;
        }

        // Ensure the destination directory exists
        if (!fs::exists(destinationDir)) {
            cerr << "Error: Destination directory does not exist: " << destinationDir << endl;
            return;
        }

        // Try to rename (move the file)
        try {
            fs::rename(fullSourcePathStr, fullDestinationPathStr);
            cout << "File moved from " << fullSourcePathStr << " to " << fullDestinationPathStr << endl;
        } catch (const fs::filesystem_error &e) {
            cerr << "Direct rename failed, attempting fallback: " << e.what() << endl;

            // Fallback to copy + delete
            try {
                fs::copy(fullSourcePathStr, fullDestinationPathStr, fs::copy_options::overwrite_existing);
                fs::remove(fullSourcePathStr);

                cout << "File moved (via copy + delete) from " << fullSourcePathStr << " to " << fullDestinationPathStr << endl;
            } catch (const fs::filesystem_error &copyError) {
                cerr << "Error during fallback copy: " << copyError.what() << endl;
            }
        }
    } catch (const fs::filesystem_error &e) {
        cerr << "Error moving file: " << e.what() << endl;
    }
}

#include <conio.h> // For _getch()
#include <vector>

void FileSystem::del() {
    try {
        vector<wstring> items;
        wcout << L"Fetching contents of: " << currentPath << L"\n";

        // Populate the list of files and directories
        for (const auto &entry : fs::directory_iterator(currentPath)) {
            items.push_back(entry.path().wstring());
        }

        if (items.empty()) {
            wcout << L"No files or folders to delete.\n";
            return;
        }

        size_t selectedIndex = 0;

        // Interactive selection loop
        while (true) {
            system("cls"); // Clear the screen
            wcout << L"Select a file or folder to delete (Press Enter to confirm, Esc to cancel):\n";

            // Display the list with the current selection
            for (size_t i = 0; i < items.size(); ++i) {
                if (i == selectedIndex) {
                    wcout << L">> " << items[i] << L" <<\n"; // Highlight the selected item
                } else {
                    wcout << L"   " << items[i] << L"\n";
                }
            }

            // Handle user input
            char key = _getch();
            if (key == 27) { // Escape key
                wcout << L"Deletion canceled.\n";
                return;
            } else if (key == 13) { // Enter key
                // Confirm deletion
                wstring selectedItem = items[selectedIndex];
                wcout << L"Are you sure you want to delete " << selectedItem << L"? (y/n): ";
                char confirm = _getch();
                if (confirm == 'y' || confirm == 'Y') {
                    if (fs::is_directory(selectedItem)) {
                        fs::remove_all(selectedItem); // Delete directory and contents
                    } else {
                        fs::remove(selectedItem); // Delete file
                    }
                    wcout << L"Deleted: " << selectedItem << L"\n";
                } else {
                    wcout << L"Deletion aborted for: " << selectedItem << L"\n";
                }
                return;
            } else if (key == 72) { // Up arrow key
                if (selectedIndex > 0) {
                    selectedIndex--;
                }
            } else if (key == 80) { // Down arrow key
                if (selectedIndex < items.size() - 1) {
                    selectedIndex++;
                }
            }
        }
    } catch (const exception &e) {
        cerr << e.what() << "\n";
    }
}

void FileSystem::delDirectory(const string &dirname) {
    // Convert dirname to a wide string for Windows API compatibility
    wstring dirPath = fs::path(currentPath).append(dirname).wstring();

    try {
        if (fs::exists(dirPath)) {
            if (fs::is_directory(dirPath)) {
                if (!fs::is_empty(dirPath)) {
                    wcout << L"Directory \"" << dirPath << L"\" is not empty. Do you want to delete it and all its contents? (yes/no): ";
                    string response;
                    cin >> response;
                    if (response == "yes" || response == "Yes" || response == "YES") {
                        fs::remove_all(dirPath); // Delete directory and its contents
                        wcout << L"Directory \"" << dirPath << L"\" and its contents have been deleted.\n";
                    } else {
                        wcout << L"Deletion of \"" << dirPath << L"\" canceled.\n";
                    }
                } else {
                    fs::remove(dirPath); // Delete the empty directory
                    wcout << L"Directory \"" << dirPath << L"\" deleted successfully.\n";
                }
            } else {
                wcerr << L"Error: \"" << dirPath << L"\" is not a directory.\n";
            }
        } else {
            wcerr << L"Error: Directory \"" << dirPath << L"\" does not exist.\n";
        }
    } catch (const fs::filesystem_error &e) {
        wcerr << L"Filesystem error: " << e.what() << endl;
    } catch (const exception &e) {
        wcerr << L"Exception: " << e.what() << endl;
    }
}




void FileSystem::showCurrentPath() const {
    wcout << currentPath << L": ";
}
void FileSystem::renameFile(const string &oldName, const string &newName) {
    try {
        wstring oldFullPath = currentPath + L"\\" + wstring(oldName.begin(), oldName.end());
        wstring newFullPath = currentPath + L"\\" + wstring(newName.begin(), newName.end());
        string oldFullPathStr = wstringToString(oldFullPath);
        string newFullPathStr = wstringToString(newFullPath);
        if (!fs::exists(oldFullPathStr)) throw runtime_error("File not found: " + oldName);
        fs::rename(oldFullPathStr, newFullPathStr);
        cout << "File renamed from " << oldName << " to " << newName << "\n";
    } catch (const exception &e) {
        cerr << e.what() << "\n";
    }
}


void FileSystem::openFile(const string &filename) {
    try {
        // Construct the full path to the file
        wstring filePath = fs::path(currentPath).append(filename).wstring();

        // Check if the file exists
        if (!fs::exists(filePath)) {
            throw runtime_error("Error: File not found: " + filename);
        }

#ifdef _WIN32
        // Use ShellExecuteW on Windows to open the file
        HINSTANCE result = ShellExecuteW(NULL, L"open", filePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
        if (reinterpret_cast<intptr_t>(result) <= 32) { // Proper check for failure
            throw runtime_error("Error: Failed to open the file: " + filename);
        }
#else
        // Use system command to open file on Unix-based systems
        string command = "xdg-open \"" + string(filePath.begin(), filePath.end()) + "\" &";
        if (system(command.c_str()) != 0) {
            throw runtime_error("Error: Failed to open the file: " + filename);
        }
#endif
        wcout << L"Opened file: " << filePath << L"\n";
    } catch (const exception &e) {
        cerr << e.what() << "\n";
    }
}
