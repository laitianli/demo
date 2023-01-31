#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

bool execShellCommand(const char* cmd, std::string &buf)
{
    bool ret = true;
    char buffer[1024];
    std::stringstream os;
    FILE* fpipe = popen(cmd, "r");
    if (NULL == fpipe) {
        std::cout << "open cmd: " << cmd << std::endl;
        return false;
    }
    while (!feof(fpipe)) {
        memset(buffer, 0, sizeof(buffer));
        size_t readn = fread(buffer, sizeof(char), sizeof(buffer), fpipe);
        os << buffer;
        if (readn == 0) {
            if (errno == EINTR) {
                continue;
            }
            break;
        }
    }
    pclose(fpipe);
    buf = os.str();
    return true;
}

bool parse_result(std::string &str, vector<std::string> &vec_alg)
{
#define STRING_1 "IKE proposal: "
#define STRING_2 "bytes_i"
#define STRING_3 ":  "
#define STRING_4 ", "
   std:string tmp = str;
    std::string sub1, sub2;
    size_t pos1, pos2;
   size_t p = tmp.find(STRING_1);
   int i = 0;
   if (p == string::npos)
        return false;
    sub1 = tmp.substr(p + sizeof(STRING_1) - 1);
    cout << "sub1: " << sub1 << endl;
    size_t end  = sub1.find("\n");
    sub1 = sub1.substr(0, end);
    cout << "sub1: " << sub1 << endl;
    pos1 = 0;
    for (i = 0; i < 3; i++) {
        pos2 = sub1.find("/", pos1);
        vec_alg.push_back(sub1.substr(pos1, pos2 - pos1));
        pos1 = pos2 + 1;
    }
    vec_alg.push_back(sub1.substr(pos2 + 1));
  
   p = tmp.find(STRING_2);
   if (p == string::npos)
        return false;
   size_t a = tmp.rfind(STRING_3, p);
   size_t b = tmp.rfind(STRING_4, p);
   sub2 = tmp.substr(a + sizeof(STRING_3) - 1, b - (a + sizeof(STRING_3)));
   cout << "sub2: " << sub2 <<endl;
   pos2 = sub2.find("/");
   vec_alg.push_back(sub2.substr(0, pos2));
   vec_alg.push_back(sub2.substr(pos2 + 1));
   if (vec_alg.size() != 6)
        return false;
   return true;
}

#define ALG_NAME_1 "aes"
#define ALG_NAME_2 "EDS"
#define LLG_NAME(index) ALG_NAME_##index

int main(int argv, char** argc)
{
    vector<std::string> vec_alg;
    std::string str = "hello world!";
    std::cout << str << std::endl;
    cout << str << endl;
    execShellCommand("/usr/libexec/ipsec/stroke statusall", str);
    cout << "size: " << str.size() << str << endl;
    parse_result(str, vec_alg);
    int i = 0;
    for (i = 0; i < vec_alg.size(); i++)
        cout << i << ": " << vec_alg[i] << endl;

    cout << "alg: " << LLG_NAME(1) << " alg: " << LLG_NAME(2) << endl;
    return 0;
}
