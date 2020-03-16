#include "provided.h"
#include <vector>
#include <cmath>
#include <random>
#include <list>
#include <algorithm>

using namespace std;

class DeliveryOptimizerImpl
{
public:
    DeliveryOptimizerImpl(const StreetMap* sm);
    ~DeliveryOptimizerImpl();
    void optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const;
private:
    const StreetMap* m_sm;
    double calculateDistance(vector<DeliveryRequest> deliveries, GeoCoord depot) const;
    void anneal(vector<DeliveryRequest>& deliveries, GeoCoord depot) const;
};

DeliveryOptimizerImpl::DeliveryOptimizerImpl(const StreetMap* sm)
:m_sm(sm)
{
}

DeliveryOptimizerImpl::~DeliveryOptimizerImpl()
{
}

void DeliveryOptimizerImpl::optimizeDeliveryOrder(
    const GeoCoord& depot,
    vector<DeliveryRequest>& deliveries,
    double& oldCrowDistance,
    double& newCrowDistance) const
{
    //calculating the old distance;
    oldCrowDistance = 0;
    oldCrowDistance = calculateDistance(deliveries, depot);
    anneal(deliveries, depot);
    newCrowDistance = calculateDistance(deliveries, depot);
}

double DeliveryOptimizerImpl::calculateDistance(vector<DeliveryRequest> deliveries, GeoCoord depot) const
{
    double totalDis = 0;
    list<StreetSegment> route;
    double dis = 0;
    
    GeoCoord last = depot;
    for (auto p:deliveries)
    {
        //m_router.generatePointToPointRoute(last, p.location, route, dis);
        dis = distanceEarthMiles(last, p.location);
        last = p.location;
        totalDis += dis;
    }
    //m_router.generatePointToPointRoute(last, depot, route, dis);
    dis = distanceEarthMiles(last, depot);
    totalDis += dis;
    
    return totalDis;
}

void DeliveryOptimizerImpl::anneal(vector<DeliveryRequest>& deliveries, GeoCoord depot) const
{
    double T = 1000;
    double deltaDis = 0;
    double cooling = 0.997;
    double absT = 0.01;
    double oldDis = calculateDistance(deliveries, depot);
    //cerr << "OriginalDis: " << oldDis << endl;
    double bestDis = oldDis;
    vector<DeliveryRequest> bestDeliveries(deliveries.begin(), deliveries.end());
    while (T > absT)
    {
        vector<DeliveryRequest> newDeliveries(deliveries.begin(), deliveries.end());
        double random;
        random = double(rand())/RAND_MAX;
//        DeliveryRequest temp = newVisits[int(random)*newVisits.size()];
//        newVisits[int(random)*newVisits.size()] = newVisits[0];
//        newVisits[0] = temp;
        random_shuffle(newDeliveries.begin(), newDeliveries.end());
        double newDis = calculateDistance(newDeliveries, depot);
        deltaDis = newDis - oldDis;
        if ( deltaDis < 0 || (oldDis > 0 && exp(-deltaDis/T) > random))
        {
//            if (deltaDis > 0)
//                cout << exp(-deltaDis/T) << endl;
            deliveries = newDeliveries;
            oldDis = newDis;
            if (newDis < bestDis)
            {
                bestDeliveries = newDeliveries;
                bestDis = newDis;
            }
        }
        T *= cooling;
    }
    deliveries = bestDeliveries;
    //cerr << "BestDis: " << bestDis << endl;
    
}


//******************** DeliveryOptimizer functions ****************************

// These functions simply delegate to DeliveryOptimizerImpl's functions.
// You probably don't want to change any of this code.

DeliveryOptimizer::DeliveryOptimizer(const StreetMap* sm)
{
    m_impl = new DeliveryOptimizerImpl(sm);
}

DeliveryOptimizer::~DeliveryOptimizer()
{
    delete m_impl;
}

void DeliveryOptimizer::optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const
{
    return m_impl->optimizeDeliveryOrder(depot, deliveries, oldCrowDistance, newCrowDistance);
}
