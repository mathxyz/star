#include <sstream>
#include "XjIoHelper.h"
#include "config_parser.h"
#include "SimpleStreamBuf.h"

/*
ioHelper.saveString("debug: true\n", "config.txt", ".tensorflow", 1);
ioHelper.saveString("imagePath:\n", "config.txt", ".tensorflow", 1,
1);
ioHelper.saveString("/sdcard/tensorflow/detect_inputs/a.png\n", "config.txt", ".tensorflow", 1,
1);
ioHelper.saveString("modelPath:\n", "config.txt", ".tensorflow", 1,
1);
ioHelper.saveString("/sdcard/tensorflow/c.tflite\n", "config.txt", ".tensorflow", 1,
1);
ioHelper.saveString("labelsPath:\n", "config.txt", ".tensorflow", 1,
1);
ioHelper.saveString("/sdcard/tensorflow/c.txt\n", "config.txt", ".tensorflow", 1,
1);*/

struct X {
public:
    int id;
    std::string name;

    X() {
    }

    X(const std::string &name, int id) {
        this->name = name;
        this->id = id;
    }

    void log() {
        SimpleStreamBuf ssb;
        std::cout.rdbuf(&ssb);
        std::cout << "name: " << name << '\n';
        std::cout << "id: " << id << '\n';
    }

    virtual ~X() {

    }
};

void writeToString(const X &x, std::string &s) {
    std::ostringstream oss;
    oss << x.name << '\n';
    oss << x.id << '\n';;
    s = oss.str();
}

void readFromString(X &x, const std::string &s) {
    std::istringstream iss;
    iss.str(s);

    if (!iss.eof()) {
        std::getline(iss, x.name);
    }
    if (!iss.eof()) {
        iss >> x.id;
    }
}

void X2string_test() {
    X x("abc de", 23);
    std::string s;
    writeToString(x, s);
    std::cout << s << '\n';

    X x2;
    readFromString(x2, s);
    x2.log();
}

void config_parser_save() {
    X x("abc de", 23);
    std::string s;
    writeToString(x, s);
    std::cout << s << '\n';

    XjIoHelper ioHelper;
    XjIoHelper::setSystemPath("D:/z");
    ioHelper.saveString(s, "config.txt", ".tensorflow", 1);
}

void config_parser_load() {
    std::string str;

    XjIoHelper ioHelper;
    XjIoHelper::setSystemPath("D:/z");
    ioHelper.readString(&str, "config.txt", ".tensorflow", 1);

    X x;
    readFromString(x, str);
    x.log();
}
