/** \file mc4.cpp
 * \author Andrej Leban
 * \date 2/2019
 * Ch 4.2 Bridging with a virtual ctor
 */

#include <iostream>
#include <sstream>

#include "../src/derivatives.h"
#include "../src/simspot.h"
#include "../src/vanillaoption.h"

using namespace der;

double doMonteCarlo(const VanillaOption & option, double sigma, double r, double S0, int nScen)
{
    double sum = 0.0;
    const simSpot spot{S0, option.expiry(), sigma, r};

    for (int i = 0; i < nScen; ++i)
    {
        sum += option.optionPayoff(spot());
    }

    return std::exp(-r * option.expiry()) * (sum / nScen);
}

int main(int /*argc*/, char * /*argv*/ [])
{
    double S0, K, T, sigma, r;
    int nScen;

#ifndef NDEBUG
    S0 = 100;
    K = 90;
    T = 30;
    sigma = 0.5;
    r = 0.02;
    nScen = 10000000;
#else
    std::cout << "enter spot, strike, time to expiry, vol, r and number of scenarios:\n";
    std::string inputParams;
    std::getline(std::cin, inputParams);
    std::istringstream iss{inputParams};

    iss >> S0 >> K >> T >> sigma >> r >> nScen;
    std::cin >> S0 >> K >> T >> sigma >> r >> nScen;
#endif

    std::cout << S0 << " " << K << " " << T << " " << sigma << " " << r << " " << nScen << "\n";

    std::cout << "the price is: " << doMonteCarlo(VanillaOption{std::make_unique<Payoff2call>(K), T}, sigma, r, S0, nScen) << "\n";

    return 0;
}
