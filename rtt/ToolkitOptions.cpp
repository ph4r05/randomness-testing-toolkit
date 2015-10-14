#include "ToolkitOptions.h"

void ToolkitOptions::init(int argc , char * argv[]) {
    int test = -1;
    int testsBot = -1;
    int testsTop = -1;

    for(int i = 1; i < argc; i += 2) {
        if(i == (argc - 1))
            throw std::runtime_error("invalid usage of command-line arguments");

        // Battery option
        if(strcmp(argv[i] , "-b") == 0) {
            if(battery != 0 || argv[i + 1][0] == '-')
                throw std::runtime_error("can't set \"-b\" option multiple times or without any value");

            if(strcmp(argv[i + 1] , "dieharder") == 0) battery = BATTERY_DIEHARDER;
            if(strcmp(argv[i + 1] , "nist_sts") == 0) battery = BATTERY_NIST_STS;
            if(strcmp(argv[i + 1] , "tu01_smallcrush") == 0) battery = BATTERY_TU01_SMALLCRUSH;
            if(strcmp(argv[i + 1] , "tu01_crush") == 0) battery = BATTERY_TU01_CRUSH;
            if(strcmp(argv[i + 1] , "tu01_bigcrush") == 0) battery = BATTERY_TU01_BIGCRUSH;
            if(strcmp(argv[i + 1] , "eacirc") == 0) battery = BATTERY_EACIRC;
        }

        // Input binary file option
        if(strcmp(argv[i] , "-f") == 0) {
            if(!binFilePath.empty() || argv[i + 1][0] == '-')
                throw std::runtime_error("can't set \"-b\" option multiple times or without any value");
            binFilePath = argv[i + 1];
        }

        if(strcmp(argv[i] , "-o") == 0) {
            if(!binFilePath.empty() || argv[i + 1][0] == '-')
                throw std::runtime_error("can't set \"-o\" option multiple times or without any value");
            outFilePath = argv[i + 1];
        }

        // Custom input config option
        if(strcmp(argv[i] , "-c") == 0) {
            if(!inputCfgPath.empty() || argv[i + 1][0] == '-')
                throw std::runtime_error("can't set \"-c\" option multiple times or without any value");
            inputCfgPath = argv[i + 1];
        }

        // Test option
        if(strcmp(argv[i] , "-t") == 0) {
            if(test != -1 || argv[i + 1][0] == '-')
                throw std::runtime_error("can't set \"-t\" option multiple times or without any value");
            test = strtoi(argv[i + 1]);
        }

        // Bottom limit test option
        if(strcmp(argv[i] , "-tbot") == 0) {
            if(testsBot != -1 || argv[i + 1][0] == '-')
                throw std::runtime_error("can't set \"-tbot\" option multiple times or without any value");
            testsBot = strtoi(argv[i + 1]);
        }

        // Top limit test option
        if(strcmp(argv[i] , "-ttop") == 0) {
            if(testsTop != -1 || argv[i + 1][0] == '-')
                throw std::runtime_error("can't set \"-tbot\" option multiple times or without any value");
            testsTop = strtoi(argv[i + 1]);
        }
    }
    if(battery == 0)
        throw std::runtime_error("option \"-b\" must be correctly set in arguments");
    if(binFilePath.empty())
        throw std::runtime_error("option \"-f\" must be set in arguments");
    if(test == -1 && testsBot == -1 && testsTop == -1)
        throw std::runtime_error("test option must be set either by \"-t\" or with \"-tbot\" and \"-ttop\"");
    if((testsBot != -1 && testsTop == -1) || (testsBot == -1 && testsTop != -1))
        throw std::runtime_error("can't set only one of options \"-tbot\" and \"ttop\"");
    if(inputCfgPath.empty()) inputCfgPath = FILE_DEFAULT_CFG_PATH;

    if(test != -1) testConsts.push_back(test);
    if(testsBot != -1 && testsTop != -1){
        for(int i = testsBot; i <= testsTop; i++) testConsts.push_back(i);
    }

    Utils::sort(testConsts);
}

int ToolkitOptions::getBattery() const {
    return battery;
}

std::string ToolkitOptions::getInputCfgPath() const {
    return inputCfgPath;
}

std::vector<int> ToolkitOptions::getTestConsts() const {
    return testConsts;
}

std::string ToolkitOptions::getBinFilePath() const {
    return binFilePath;
}

std::string ToolkitOptions::getOutFilePath() const {
    return outFilePath;
}

std::string ToolkitOptions::getUsage() const {
    std::stringstream ss;
    ss << "\n[USAGE] Randomness Testing Toolkit accepts following options.\n";
    ss << "    -b  Followed with battery that will be used. Following batteries\n";
    ss << "        are accepted: \"dieharder\", \"nist_sts\", \"tu01_smallcrush\",\n";
    ss << "        \"tu01_crush\", \"tu01_bigcrush\", \"eacirc\"\n";
    ss << "    -f  Followed with path to input binary file that will be analysed by battery.\n";
    ss << "    -o  Followed with path of output file for battery results. If left empty,\n";
    ss << "        If left empty, default value from config file will be used.\n";
    ss << "    -c  Followed with path to custom config file that will be used instead of\n";
    ss << "        default one. Argument is optional, default path is " << FILE_DEFAULT_CFG_PATH << "\n";
    ss << "    -t  Followed with constant of test in battery that will be used in testing.\n";
    ss << " -tbot  Bottom border for tests. All tests between \"-tbot\" and \"-ttop\n";
    ss << "        will be used. Can't be used without setting \"-ttop\"\n";
    ss << " -ttop  Top border for tests. All tests between \"-tbot\" and \"-ttop\"\n";
    ss << "        will be used. Can't be used without setting \"-tbot\"\n\n";
    return ss.str();
}

int ToolkitOptions::strtoi(char * str) const {
    int result = 0;
    try {
        result = std::stoi(str);
    }
    catch (std::invalid_argument) {
        throw std::runtime_error("invalid numeric argument entered");
    }
    catch (std::out_of_range) {
        throw std::runtime_error("entered numeric argument is out of range");
    }
    return result;
}

