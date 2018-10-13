/**
 * @file       Benchmark.cpp
 *
 * @brief      Benchmarking tests for Zerocoin.
 *
 * @author     Ian Miers, Christina Garman and Matthew Green
 * @date       June 2013
 *
 * @copyright  Copyright 2013 Ian Miers, Christina Garman and Matthew Green
 * @license    This project is released under the MIT license.
 **/


#include "Timer.h"
#include "libzerocoin/Accumulator.h"
#include "libzerocoin/PrivateCoin.h"
#include "libzerocoin/CoinSpend.h"
#include "libzerocoin/Denominations.h"
#include "libzerocoin/ParamGeneration.h"
#include "streams.h"
#include "random.h"
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>

#include <sodium/core.h>

using namespace std;
using namespace libzerocoin;

#define TESTS_COINS_TO_ACCUMULATE 50

// Global test counters
uint32_t global_NumTests = 0;
uint32_t global_SuccessfulTests = 0;

// Global coin array
PrivateCoin* global_Coins[TESTS_COINS_TO_ACCUMULATE];

// Global params
ZerocoinParams* global__Params;

// Global timer
Timer timer;

void LogTestResult(string testName, bool (*testPtr)())
{
    cout << "Testing if " << testName << "...";

    bool testResult = testPtr();

    if (testResult == true) {
        cout << "\t"
             << "[PASS]" << endl;
        global_SuccessfulTests++;
    } else {
        cout << "\t[FAIL]" << endl;
    }

    global_NumTests++;
}

// Use RSA String as in main PIVX code
CBigNum GetTestModulus()
{
    static CBigNum testModulus("25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
                               "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
                               "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
                               "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
                               "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
                               "31438167899885040445364023527381951378636564391212010397122822120720357");
    
    return testModulus;
}
bool Test_ParamGen()
{
    bool result = true;

    try {
        timer.start();
        // Instantiating testParams runs the parameter generation code
        ZerocoinParams testParams(ZEROCOIN_DEFAULT_SECURITYLEVEL);
        timer.stop();

        cout << "\tPARAMGEN ELAPSED TIME: " << timer.duration() << " ms\t" << timer.duration() * 0.001 << " s ";
    } catch (runtime_error e) {
        cout << e.what() << endl;
        result = false;
    }

    return result;
}

bool Test_Accumulator()
{
    // This test assumes a list of coins were generated during the Testb_MintCoin() test.
    if (global_Coins[0] == NULL) {
        return false;
    }
    try {
        // Accumulate the coin list from first to last into one accumulator
        Accumulator accOne(&global__Params->accumulatorParams, libzerocoin::CoinDenomination::ZQ_ONE);
        Accumulator accTwo(&global__Params->accumulatorParams, libzerocoin::CoinDenomination::ZQ_ONE);
        Accumulator accThree(&global__Params->accumulatorParams, libzerocoin::CoinDenomination::ZQ_ONE);
        Accumulator accFour(&global__Params->accumulatorParams, libzerocoin::CoinDenomination::ZQ_ONE);
        AccumulatorWitness wThree(global__Params, accThree, global_Coins[0]->getPublicCoin());

        for (uint32_t i = 0; i < TESTS_COINS_TO_ACCUMULATE; i++) {
            accOne += global_Coins[i]->getPublicCoin();
            accTwo += global_Coins[TESTS_COINS_TO_ACCUMULATE - (i + 1)]->getPublicCoin();
            accThree += global_Coins[i]->getPublicCoin();
            wThree += global_Coins[i]->getPublicCoin();
            if (i != 0) {
                accFour += global_Coins[i]->getPublicCoin();
            }
        }

        // Compare the accumulated results
        if (accOne.getValue() != accTwo.getValue() || accOne.getValue() != accThree.getValue()) {
            cout << "Accumulators don't match" << endl;
            return false;
        }

        if (accFour.getValue() != wThree.getValue()) {
            cout << "Witness math not working," << endl;
            return false;
        }

        // Verify that the witness is correct
        if (!wThree.VerifyWitness(accThree, global_Coins[0]->getPublicCoin())) {
            cout << "Witness not valid" << endl;
            return false;
        }

    } catch (runtime_error e) {
        cout << e.what() << endl;
        return false;
    }

    return true;
}

bool Test_MintCoin()
{
    try {
        // Generate a list of coins
        timer.start();
        for (uint32_t i = 0; i < TESTS_COINS_TO_ACCUMULATE; i++) {
            global_Coins[i] = new PrivateCoin(global__Params);
            std::string seed = to_string(i);
            uint512 s(uint512S(seed));
            global_Coins[i]->CoinFromSeed(s);
        }

        timer.stop();
    } catch (exception& e) {
        return false;
    }

    cout << "\tMINT ELAPSED TIME:\n\t\tTotal: " << timer.duration() << " ms\t"
         << timer.duration() * 0.001 << " s\n\t\tPer Coin: "
         << timer.duration() / TESTS_COINS_TO_ACCUMULATE << " ms\t"
         << (timer.duration() / TESTS_COINS_TO_ACCUMULATE) * 0.001 << " s ";

    return true;
}

bool Test_MintAndSpend()
{
    try {
        // This test assumes a list of coins were generated in Testb_MintCoin()
        if (global_Coins[0] == NULL) {
            // No coins: mint some.
            Test_MintCoin();
            if (global_Coins[0] == NULL) {
                return false;
            }
        }

        // Accumulate the list of generated coins into a fresh accumulator.
        // The first one gets marked as accumulated for a witness, the
        // others just get accumulated normally.
        Accumulator acc(&global__Params->accumulatorParams, CoinDenomination::ZQ_ONE);
        AccumulatorWitness wAcc(global__Params, acc, global_Coins[0]->getPublicCoin());

        timer.start();
        for (uint32_t i = 0; i < TESTS_COINS_TO_ACCUMULATE; i++) {
            acc += global_Coins[i]->getPublicCoin();
        }
        timer.stop();

        cout << "\n\tACCUMULATOR ELAPSED TIME:\n\t\tTotal: " << timer.duration() << " ms\t"
             << timer.duration() * 0.001 << " s\n\t\tPer Element: "
             << timer.duration() / TESTS_COINS_TO_ACCUMULATE << " ms\t"
             << (timer.duration() / TESTS_COINS_TO_ACCUMULATE) * 0.001 << " s" << endl;

        timer.start();
        for (uint32_t i = 0; i < TESTS_COINS_TO_ACCUMULATE; i++) {
            wAcc += global_Coins[i]->getPublicCoin();
        }
        timer.stop();

        cout << "\tWITNESS ELAPSED TIME: \n\t\tTotal: " << timer.duration() << " ms\t"
             << timer.duration() * 0.001 << " s\n\t\tPer Element: "
             << timer.duration() / TESTS_COINS_TO_ACCUMULATE << " ms\t"
             << (timer.duration() / TESTS_COINS_TO_ACCUMULATE) * 0.001 << " s" << endl;

        // Now spend the coin
        timer.start();
        uint256 ptxHash(uint256S("0"));
        CoinSpend spend(global__Params, *(global_Coins[0]), acc, 0, wAcc, ptxHash);
        timer.stop();

        cout << "\tSPEND ELAPSED TIME: " << timer.duration() << " ms\t" << timer.duration() * 0.001 << " s" << endl;

        // Serialize the proof and deserialize into newSpend
        CDataStream ss(SER_NETWORK, 900);

        timer.start();
        ss << spend;
        timer.stop();

        CoinSpend newSpend(global__Params, ss);

        cout << "\tSERIALIZE ELAPSED TIME: " << timer.duration() << " ms\t" << timer.duration() * 0.001 << " s" << endl;

        // Finally, see if we can verify the deserialized proof (return our result)
        timer.start();
        bool ret = newSpend.Verify(acc);
        timer.stop();

        cout << "\tSPEND VERIFY ELAPSED TIME: " << timer.duration() << " ms\t" << timer.duration() * 0.001 << " s" << endl;

        return ret;
    } catch (runtime_error& e) {
        cout << e.what() << endl;
        return false;
    }

    return false;
}

void Test_RunAllTests()
{
  if (sodium_init() < 0) { throw string("Libsodium initialization failed."); }
    // Make a new set of parameters from a random RSA modulus
    global__Params = new ZerocoinParams();

    global_NumTests = global_SuccessfulTests = 0;
    for (uint32_t i = 0; i < TESTS_COINS_TO_ACCUMULATE; i++) {
        global_Coins[i] = NULL;
    }

    // Run through all of the Zerocoin tests
    LogTestResult("parameter generation is correct", Test_ParamGen);
    LogTestResult("coins can be minted", Test_MintCoin);
    LogTestResult("the accumulator works", Test_Accumulator);
    LogTestResult("a minted coin can be spent", Test_MintAndSpend);

    // Summarize test results
    if (global_SuccessfulTests < global_NumTests) {
        cout << endl
             << "ERROR: SOME TESTS FAILED" << endl;
    }

    // Clear any generated coins
    for (uint32_t i = 0; i < TESTS_COINS_TO_ACCUMULATE; i++) {
        delete global_Coins[i];
    }

    cout << global_SuccessfulTests << " out of " << global_NumTests << " tests passed." << endl;
    delete global__Params;
}
int main(int argc, char* argv[])
{
    cout << "libzerocoin benchmark utility." << endl;
    Test_RunAllTests();
    if (global_SuccessfulTests < global_NumTests) return 1;
    return 0;
}
