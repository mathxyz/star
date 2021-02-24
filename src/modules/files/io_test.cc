#include <sstream>
#include "XjIoHelper.h"
#include "io_test.h"
#include "config_parser.h"
int writeToString() {
    std::ostringstream oss;
    oss << "One hundred and one: " << 101;
    std::string s = oss.str();
    std::cout << s << '\n';
}

int readFromString() {
    std::istringstream iss;
    std::string strvalues = "32 240 2 1450";

    iss.str(strvalues);

    for (int n = 0; n < 4; n++) {
        int val;
        iss >> val;
        std::cout << val << '\n';
    }
    std::cout << "Finished writing the numbers in: ";
    std::cout << iss.str() << '\n';
}

void IoHelper_test() {
    XjIoHelper ioHelper;
    XjIoHelper::setSystemPath("D:/z4");
    ioHelper.saveString("yyes! abctt\n", "hello.txt", 0, 1, 0);
    std::string str = "eee";
    ioHelper.readString(&str, "hello.txt", 0, 1);
    std::cout << str << std::endl;
}

void io_test() {
//    IoHelper_test();
//    X2string_test();
    config_parser_save();
    config_parser_load();
}
