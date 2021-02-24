#pragma once

#include <string>
#include <fstream>
#include <iostream>

class XjIoHelper {
public:
    XjIoHelper();

    ~XjIoHelper() {}

    void testlog(std::string const &message);

    static int setSystemPath(std::string sysDir);

    int xjgetSystemPath(std::string &sysDir);

    std::string xjgetTmpDir(int &flag);

    std::string xjgetTmpDir(const char *name = 0);

    static std::string xjgetWorkingPath();

    int setFilePath(std::string filePath);

    int xjfwrite(const void *buf, size_t size, size_t count, const char *fileName, const char *folderName = 0,
                 const char *mode = "wb");

    int xjfread(void *buf, size_t size, size_t count, const char *fileName, const char *folderName = 0);

    // folderName can be 0.
    int saveString(const std::string &s, const char *fileName, const char *folderName, bool binary, bool append=0);

    // folderName can be 0.
    int readString(std::string *buf, const char *fileName, const char *folderName, bool binary);

private:
    static bool xjDirExists(const char *name);

    static int xjmkdir(const char *);

    static int xjrmdir(const char *);

    // path is absolute.
    static int xjLoadFileRaw(const char *path, bool binary, std::string *buf);

    static int xjSaveFileRaw(const char *path, const std::string &buf, std::ios::openmode mode);
public:
    void setIsClearCache(int isClearCache, std::string cmd = "");

    std::string xjgetFileName(int &flag);

private:
    int used;
    int isClearCache;

    static std::string sysDir;
    static int MAX_PATH_LEN;
    std::string fileName;
    std::string filePar;
    std::string fileFullPath;

    static int xjIsNull(std::string &filePath);
};
