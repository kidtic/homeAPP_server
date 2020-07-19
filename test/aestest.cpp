#include "homeAppComm.h"
using namespace std;

int main(int argc, char const *argv[])
{
    homeAppComm comm(2333,"133z195");
    string str;
    cin>>str;
    string decrtptstr= comm.decrypt(str);
    cout<<decrtptstr<<endl;
    return 0;
}
