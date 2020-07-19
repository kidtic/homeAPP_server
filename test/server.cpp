#include "homeAppSys.h"
using namespace std;

int main(int argc, char const *argv[])
{
    homeAppSys homesys("config/homeAppConfig.json");

    homesys.run();

}
