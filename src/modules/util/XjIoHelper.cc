#include <sys/stat.h>
#include <sstream>
#include <cstdlib>

#ifdef _WIN32

#include <io.h>

#else

#include <unistd.h>

#endif

#include "XjIoHelper.h"

std::string XjIoHelper::sysDir = std::string("");
int XjIoHelper::MAX_PATH_LEN = 1024;

int XjIoHelper::setSystemPath(std::string sysDir) {
    XjIoHelper::sysDir = sysDir;
    if (!xjDirExists(sysDir.c_str())) {
        xjmkdir(sysDir.c_str());
    }
    return 1;
}


XjIoHelper::XjIoHelper() {
    this->fileName = "";
    this->filePar = "";
    this->fileFullPath = "";
    isClearCache = 0;
}

void XjIoHelper::testlog(std::string const &message) {
//    writeToString();
//    readFromString();
//    xjwarn(fileFullPath);
//    xjwarn(fileFullPath.size());
//    xjwarn(fileName);
    std::cerr << "XjIoHelper: " << message << std::endl;
}

//, const char* mode="wb"
int
XjIoHelper::saveString(const std::string &s, const char *fileName, const char *folderName, bool binary, bool append) {
    if (fileName == NULL)
        return 0;

    std::string path;
    std::string tmpDir = xjgetTmpDir(folderName);
    if (tmpDir.empty()) {
        return 0;
    }
    path = tmpDir + "/" + std::string(fileName);

    int flag;
    std::ios::openmode mode = std::ios::out;
    if (binary) {
        mode = mode | std::ios::binary;
    }
    if (append) {
        mode = mode | std::ios::app;
    }
    flag = xjSaveFileRaw(path.c_str(), s, mode);
    return flag;
}

int XjIoHelper::xjSaveFileRaw(const char *path, const std::string &buf, std::ios::openmode mode) {

    std::ofstream ofs(path, mode);
    if (!ofs.is_open()) {
//                printf("open %s fail!\n", path);
        return 0;
    }
    if (mode & std::ios::binary) {
        // The fastest way to read a file into a string.
        ofs.write(&(buf)[0], (buf).size());
    } else {
        // This is slower, but works correctly on all platforms for text files.
        std::istringstream iss(buf);
        iss >> ofs.rdbuf();
    }
    return !ofs.bad();
}

//const char* mode="rb"
int XjIoHelper::readString(std::string *buf, const char *fileName, const char *folderName, bool binary) {
    if (fileName == NULL)
        return 0;

    std::string path;
    std::string tmpDir = xjgetTmpDir(folderName);
    if (tmpDir.empty()) {
        return 0;
    }
    path = tmpDir + "/" + std::string(fileName);

    int flag;
    flag = xjLoadFileRaw(path.c_str(), binary, buf);
    return flag;
}

int XjIoHelper::xjLoadFileRaw(const char *path, bool binary, std::string *buf) {
    if (xjDirExists(path)) return false;
    std::ifstream ifs(path, binary ? std::ifstream::binary : std::ifstream::in);
    if (!ifs.is_open()) {
//                printf("open %s fail!\n", path);
        return 0;
    }
    if (binary) {
        // The fastest way to read a file into a string.
        ifs.seekg(0, std::ios::end);
        auto size = ifs.tellg();
        (*buf).resize(static_cast<size_t>(size));
        ifs.seekg(0, std::ios::beg);
        ifs.read(&(*buf)[0], (*buf).size());
    } else {
        // This is slower, but works correctly on all platforms for text files.
        std::ostringstream oss;
        oss << ifs.rdbuf();
        *buf = oss.str();
    }
    return !ifs.bad();
}

int XjIoHelper::setFilePath(std::string filePath) {
    std::string name = filePath;
    int found = name.find_last_of("/\\");
    this->fileName = name.substr(found + 1);
    this->filePar = name.substr(0, found);
    this->fileFullPath = filePath;
    int flag;
    std::string tmpDir = xjgetTmpDir(flag);
    if (flag == 0)
        return 0;
    if (!xjDirExists(tmpDir.c_str())) {
        xjmkdir(tmpDir.c_str());
    }
    return 1;
}

std::string XjIoHelper::xjgetFileName(int &flag) {
    if (xjIsNull(this->fileName)) {
        flag = 0;
    } else {
        flag = 1;
    }
    return this->fileName;
}

std::string XjIoHelper::xjgetWorkingPath() {
    char temp[MAX_PATH_LEN];
    return (getcwd(temp, MAX_PATH_LEN) ? std::string(temp) : std::string(""));
}

std::string XjIoHelper::xjgetTmpDir(int &flag) {
    if (this->filePar.size() == 0 || fileName.size() == 0) {
        flag = 0;
    } else {
        flag = 1;
    }
    std::string tmpDir = this->filePar + "/." + this->fileName;
    return tmpDir;
}

std::string XjIoHelper::xjgetTmpDir(const char *name2) {
    std::string tmpDir = "";
    if (this->filePar.empty() || fileName.empty()) {
        if (sysDir.empty()) {
        } else {
            if (name2 == 0) {
                tmpDir = sysDir;
            } else {
                tmpDir = sysDir + "/" + std::string(name2);
                if (!xjDirExists(tmpDir.c_str())) {
                    xjmkdir(tmpDir.c_str());
                }
            }
        }
    } else {
        if (name2 == 0) {
            tmpDir = this->filePar + "/." + this->fileName;
        } else {
            tmpDir = this->filePar + "/." + this->fileName + "/" + std::string(name2);
            if (!xjDirExists(tmpDir.c_str())) {
                xjmkdir(tmpDir.c_str());
            }
        }
    }
    return tmpDir;
}

int XjIoHelper::xjfwrite(const void *buf, size_t size, size_t count, const char *fileName, const char *folderName,
                         const char *mode) {
    if (fileName == NULL)
        return 0;

    std::string path;
    std::string tmpDir = xjgetTmpDir(folderName);
    if (tmpDir == "") {
        return 0;
    }
    path = tmpDir + "/" + std::string(fileName);
    FILE *f = fopen(path.c_str(), mode);
    if (f == 0)
        return 0;
    fwrite(buf, size, count, f);
    fclose(f);
    return 1;
}

int XjIoHelper::xjfread(void *buf, size_t size, size_t count, const char *fileName, const char *folderName) {
    if (fileName == NULL)
        return 0;
    int flag = 1;
    std::string path;
    if (folderName) {
        path = xjgetTmpDir(flag) + "/" + std::string(folderName) + "/" + std::string(fileName);
    } else {
        path = xjgetTmpDir(flag) + "/" + std::string(fileName);
    }
//    xjwarn(path);
    if (flag == 0)
        return 0;
    FILE *f = fopen(path.c_str(), "rb");
    if (f == 0)
        return 0;
    fseek(f, 0L, SEEK_END);
    long int sz2 = ftell(f);
    rewind(f);
    if (sz2 != size * count) {
        flag = 0;
    }
    if (flag == 1)
        fread(buf, size, count, f);
    fclose(f);
    return flag;
}

int XjIoHelper::xjmkdir(const char *name) {
#ifdef _WIN32
    mkdir(name);
#else
    mkdir(name, S_IRWXU | S_IRGRP | S_IXGRP);
#endif
    return 1;
}

int XjIoHelper::xjrmdir(const char *name) {
    rmdir(name);
    return 1;
}

bool XjIoHelper::xjDirExists(const char *name) {
    // clang-format off
#ifdef _WIN32
#define xj_stat _stat
#define XJ_S_IFDIR _S_IFDIR
#else
#define xj_stat stat
#define XJ_S_IFDIR S_IFDIR
#endif
    // clang-format on
    struct xj_stat file_info;
    if (xj_stat(name, &file_info) != 0) return false;
    return (file_info.st_mode & XJ_S_IFDIR) != 0;
}

int XjIoHelper::xjIsNull(std::string &filePath) {
    if (filePath.size() == 0) {
        return 1;
    } else {
        return 0;
    }
}

int XjIoHelper::xjgetSystemPath(std::string &sysDir) {
    if (xjIsNull(this->sysDir))
        return 0;
    sysDir = this->sysDir;
    return 1;
}

void XjIoHelper::setIsClearCache(int isClearCache, std::string suf) {

    XjIoHelper::isClearCache = isClearCache;
    int flag;
    std::string tmpDir = xjgetTmpDir(flag);
    if (flag == 0)
        return;
    if (!xjDirExists(tmpDir.c_str())) {
        xjmkdir(tmpDir.c_str());
    } else {
        if (this->isClearCache) {
            if (suf == "")
                return;
            std::string str;
            std::stringstream ss(suf);
            while (ss >> str) {
                str = "rm -rf " + tmpDir + "/" + str;
                system(str.c_str());
            }
        }
    }
}