#include "rtt/batteries/niststs/battery-sts.h"

namespace rtt {
namespace batteries {
namespace niststs {

const std::string Battery::XPATH_LOG_DIRECTORY = "TOOLKIT_SETTINGS/LOGGER/NIST_STS_DIR";
const std::string Battery::XPATH_DEFAULT_TESTS = "NIST_STS_SETTINGS/DEFAULT_TESTS";

std::unique_ptr<Battery> Battery::getInstance(const Globals & globals) {
    std::unique_ptr<Battery> b (new Battery());
    b->cliOptions = globals.getCliOptions();
    b->batteryConfiguration = globals.getBatteryConfiguration();
    b->toolkitSettings = globals.getToolkitSettings();
    b->battery = b->cliOptions->getBattery();
    b->objectInfo = Constants::batteryToString(b->battery);
    b->creationTime = Utils::getRawTime();

    std::cout << "[INFO] Processing file: " << b->cliOptions->getBinFilePath() << std::endl;

    b->logFilePath = b->toolkitSettings->getLoggerBatteryDir(b->battery);
    b->logFilePath.append(Utils::formatRawTime(b->creationTime , "%Y%m%d%H%M%S"));
    b->logFilePath.append(
                "-" + Utils::getLastItemInPath(b->cliOptions->getBinFilePath() + ".log"));

    /* Creating storage for results */
    b->storage = output::IOutput::getInstance(globals , b->creationTime);

    /* Getting constants of tests to be executed */
    std::vector<int> testConsts = b->cliOptions->getTestConsts();
    if(testConsts.empty())
        testConsts = b->batteryConfiguration->getBatteryDefaultTests(b->battery);
    if(testConsts.empty())
        throw RTTException(b->objectInfo , "no tests were set for execution");

    for(int i : testConsts) {
        std::unique_ptr<ITest> test = ITest::getInstance(i , globals);
        b->tests.push_back(std::move(test));
    }

    return b;
}

void Battery::runTests() {
    /* Executing all tests in sequence here */
    /* In time, it's possible to add some multithreading (added now) */
    if(executed)
        throw RTTException(objectInfo , "battery was already executed");

    TestRunner::executeTests(std::ref(tests));

    /* Setting executed to true, allowing postprocessing */
    executed = true;
}

void Battery::processStoredResults() {
    if(!executed)
        throw RTTException(objectInfo , "battery must be executed before result processing");

    std::cout << "Storing log and results." << std::endl;

    /* Log storage */
    std::string batteryLog;
    for(auto & i : tests)
        i->appendTestLog(batteryLog);

    Utils::createDirectory(Utils::getPathWithoutLastItem(logFilePath));
    Utils::saveStringToFile(logFilePath , batteryLog);

    /* Result storage */
    for(const auto & test : tests) {
        storage->addNewTest(test->getLogicName());
        storage->setTestOptions(test->getParameters());

        std::vector<tTestPvals> results = test->getResults();
        /* There are always two statistics in NIST STS, namely
         * Chi-square and Proportion */
        std::vector<std::string> statistics = test->getStatistics();
        bool propStatFailed;
        std::string propStat;

        if(results.size() == 1) { /* Single test */
            storage->addStatisticResult(statistics.at(0) ,
                                        chi2_stat(results.at(0)) , 6);
            propStat = proportionStat(results.at(0) , &propStatFailed);
            storage->addStatisticResult(statistics.at(1) , propStat ,
                                        propStatFailed);
            storage->addPValues(results.at(0) , 6);
        } else { /* Multiple subtests */
            for(const auto & result : results) {
                storage->addSubTest();
                storage->addStatisticResult(statistics.at(0) ,
                                            chi2_stat(result) , 6);
                propStat = proportionStat(result , &propStatFailed);
                storage->addStatisticResult(statistics.at(1) , propStat ,
                                            propStatFailed);
                storage->addPValues(result , 6);
                storage->finalizeSubTest();
            }
        }
        storage->finalizeTest();
    }
    storage->finalizeReport();
}

std::string Battery::proportionStat(tTestPvals pvals , bool * failed) {
    int passCount = 0;
    double p_hat = 1.0 - Constants::MATH_ALPHA;
    double proportion_threshold_max =
            (p_hat + 3.0 * sqrt((p_hat*Constants::MATH_ALPHA)/pvals.size())) * pvals.size();
    double proportion_threshold_min =
            (p_hat - 3.0 * sqrt((p_hat*Constants::MATH_ALPHA)/pvals.size())) * pvals.size();
    for(const auto & val : pvals) {
        if(val >= Constants::MATH_ALPHA)
            ++passCount;
    }
    if(passCount < proportion_threshold_min || passCount > proportion_threshold_max)
        *failed = true;
    else
        *failed = false;

    return {Utils::itostr(passCount) + "/" + Utils::itostr(pvals.size())};
}

/* Following code is taken from NIST STS source code */
/* Used for calculation of Chi2 statistic */
double Battery::chi2_stat(tTestPvals pvals) {
    int streamCount = pvals.size();
    int j, pos , expCount;
    double chi2 , uniformity;

    std::vector<int> freqPerBin(10 , 0);

    std::sort(pvals.begin() , pvals.end());
    for(j = 0 ; j <  streamCount ; ++j) {
        pos = (int)floor(pvals.at(j)*10);
        if(pos == 10)
            pos--;
        ++freqPerBin[pos];
    }
    chi2 = 0.0;
    expCount =  streamCount/10;
    if(expCount == 0) {
        uniformity = 0.0;
    } else {
        for (j = 0 ; j < 10 ; ++j)
            chi2 += pow(freqPerBin[j]-expCount, 2)/expCount;

        uniformity = Cephes::cephes_igamc(9.0/2.0, chi2/2.0);
    }
    return uniformity;
}



} // namespace niststs
} // namespace batteries
} // namespace rtt