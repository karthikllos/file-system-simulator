#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <cstdio>
#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#else
#include <sys/stat.h>
#endif

using namespace std;
namespace fs = filesystem;

// Abstract Base Class for Polymorphism
class FileSystemBase {
public:
    virtual void createFile(const string &filename) = 0;
    virtual void deleteFile(const string &filename) = 0;
    virtual void readFile(const string &filename) = 0;
    virtual void createDirectory(const string &dirname) = 0;
    virtual void changeDirectory(const string &dirname) = 0;
    virtual void delDirectory(const string &dirname) = 0;
    virtual void listCurrentDirectory() = 0;
    virtual void del() = 0;
    virtual ~FileSystemBase() {} // Virtual destructor for proper cleanup
};

// Derived Class
class FileSystem : public FileSystemBase {
    private:
        wstring currentPath;
        string wstringToString(const wstring &wstr);

    public:
    FileSystem() {
    try {
        #ifdef _WIN32
            wchar_t buffer[MAX_PATH];
            if (GetCurrentDirectoryW(MAX_PATH, buffer)) {
                currentPath = buffer;
            } else {
                throw runtime_error("Failed to get the current working directory.");
            }
        #else
            char buffer[PATH_MAX];
            if (getcwd(buffer, PATH_MAX)) {
                currentPath = wstring(buffer, buffer + strlen(buffer));
            } else {
                throw runtime_error("Failed to get the current working directory.");
            }
        #endif
            wcout << L"Initialized with current path: " << currentPath << L"\n";
            } catch (const exception &e) {
                cerr << e.what() << "\n";
                currentPath = L"."; // Fallback to default
            }
    }


    void createFile(const string &filename) override;
    void deleteFile(const string &filename) override;
    void readFile(const string &filename) override;
    void createDirectory(const string &dirname) override;
    void changeDirectory(const string &dirname) override;
    void delDirectory(const string &dirname) override;
    void listCurrentDirectory() override;
    void showCurrentPath() const;
    void editFile(const string &filename);
    void copyFile(const string &sourcePath, const string &destinationPath);
    void cutFile(const string &filename, const string &destinationDir);
    void moveFile(const string &filename, const string &destinationDir);
    void renameFile(const string &oldName, const string &newName);
    void openFile(const string &filename);
    void del() override;
    void clearScreen() {
    #ifdef _WIN32
            system("cls");
    #else
            system("clear");
    #endif
    }

    // Operator Overloading
    FileSystem &operator<<(const string &filename) {
        readFile(filename);
        return *this;
    }
    FileSystem &operator>>(const string &filename) {
        editFile(filename);
        return *this;
    }
    FileSystem &operator+=(const string &filename) {
        changeDirectory(filename);
        return *this;
    }
    FileSystem &operator--() {
        changeDirectory("..");
        return *this;
    }
    void operator*() {
        listCurrentDirectory();
    }
    FileSystem &operator-(const string &filename) {
        deleteFile(filename);
        return *this;
    }
    FileSystem &operator+(const string &filename) {
        createFile(filename);
        return *this;
    }
};
