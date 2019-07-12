/** \file pathdependent.cpp
 * \author Andrej Leban
 * \date 4/2019
 * Ch 7 An exotics engine and the template pattern
 */

#include <algorithm>
#include <vector>

#include "payoff2.h"

#include "pathdependent.h"

namespace der
{

PathDependent::PathDependent(const std::vector<double> & p_lookAtTimes)
    : m_lookAtTimes(p_lookAtTimes)
{}

PathDependent::~PathDependent() = default;

std::vector<double> PathDependent::lookAtTimes() const
{
    return m_lookAtTimes;
}

AsianOptionArith::AsianOptionArith(const std::vector<double> & p_lookAtTimes, double p_delivery, const Payoff2 & p_payoff)
    : PathDependent(p_lookAtTimes)
    , m_delivery(p_delivery)
    , m_pPayoff(p_payoff.clone())
{}

std::unique_ptr<PathDependent> AsianOptionArith::clone() const
{
    return std::make_unique<AsianOptionArith>(*this);
}

size_t AsianOptionArith::maxNumberOfCashFlows() const
{
    return 1;
}

std::vector<double> AsianOptionArith::possibleCashFlowTimes() const
{
    return {m_delivery};
}

std::vector<CashFlow> AsianOptionArith::cashFlows(const std::vector<double> & p_spots, std::vector<CashFlow> && p_flows) const
{
    p_flows.resize(1);
    double sum = std::accumulate(p_spots.begin(), p_spots.end(), 0.0);

    p_flows[0].timeIndex = 0;
    // The payoff of the arithmetic Asian option at delivery is naturally the arithemtic mean of the
    // underlying at the times specified
    p_flows[0].amount = (*m_pPayoff)(sum/m_lookAtTimes.size());
    return std::move(p_flows);
}

der::AsianOptionArith::AsianOptionArith(const der::AsianOptionArith & p_other)
    : PathDependent(p_other)
    , m_delivery(p_other.m_delivery)
    , m_pPayoff(p_other.m_pPayoff->clone())
{}

AsianOptionArith & AsianOptionArith::operator=(const AsianOptionArith & p_other)
{
    if(this != &p_other)
    {
        // call without returning
        PathDependent::operator=(p_other);
        this->m_delivery = p_other.m_delivery;
        this->m_pPayoff= p_other.m_pPayoff->clone();
    }
    return *this;
}

} // namespace der
