#include "provided.h"
#include "ExpandableHashMap.h"
#include <list>
#include <vector>
#include <queue>
#include <iterator>
#include <algorithm>
using namespace std;

class PointToPointRouterImpl
{
public:
    PointToPointRouterImpl(const StreetMap* sm);
    ~PointToPointRouterImpl();
    DeliveryResult generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const;
    
private:
    const StreetMap* m_sm;
    void produceRoute(
        ExpandableHashMap<GeoCoord, GeoCoord>* record,
        GeoCoord end, GeoCoord start,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const;
};

PointToPointRouterImpl::PointToPointRouterImpl(const StreetMap* sm)
{
    m_sm = sm;
}

PointToPointRouterImpl::~PointToPointRouterImpl()
{
}

DeliveryResult PointToPointRouterImpl::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
    //starting and ending positions are the same
    if (start == end)
    {
        route.clear();
        totalDistanceTravelled = 0;
        return DELIVERY_SUCCESS;
    }
    
    //bad coordinates
    vector<StreetSegment> dummy;
    if(!m_sm->getSegmentsThatStartWith(start, dummy) && !m_sm->getSegmentsThatStartWith(end, dummy))
        return BAD_COORD;
    
    //regular search
    route.clear();
    ExpandableHashMap<GeoCoord, GeoCoord> previousPt;
    queue<GeoCoord> searchSpace;
    list<GeoCoord> discovered;
    searchSpace.push(start);
    discovered.push_front(start);
    
    while(!searchSpace.empty())
    {
        GeoCoord toSearch = searchSpace.front();
        searchSpace.pop();
        if (toSearch == end)
        {
            produceRoute(&previousPt, end, start, route, totalDistanceTravelled);
            return DELIVERY_SUCCESS;
        }
        vector<StreetSegment> found;
        if(m_sm->getSegmentsThatStartWith(toSearch, found))
        {
            for (auto p: found)
            {
                if(find(discovered.begin(), discovered.end(), p.end) == discovered.end())
                {
                    searchSpace.push(p.end);
                    previousPt.associate(p.end, toSearch);
                    discovered.push_front(p.end);
                }
            }
        }
    }
    
    
    return NO_ROUTE;  // Delete this line and implement this function correctly
}

void PointToPointRouterImpl::produceRoute(
        ExpandableHashMap<GeoCoord, GeoCoord>* record,
        GeoCoord end, GeoCoord start,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
    totalDistanceTravelled = 0;
    while (end != start)
    {
        if (record->find(end) == nullptr)
        {
            cerr << "ERROR: fail to produce a route! --1" << endl;
            return;
        }
        
        vector<StreetSegment> result;
        GeoCoord last = end;
        end = *record->find(end);
        
        if (!m_sm->getSegmentsThatStartWith(end, result))
        {
            cerr << "ERROR: fail to produce a route! --2" << endl;
            return;
        }
        
        StreetSegment seg(end, last, "");
        auto p = find(result.begin(), result.end(), seg);
        if (p == result.end())
        {
            cerr << "ERROR: fail to produce a route! --3" << endl;
            return;
        }
        totalDistanceTravelled += distanceEarthMiles(end, last);
        route.push_front(*p);
            
    }
    return;
}


//******************** PointToPointRouter functions ***************************

// These functions simply delegate to PointToPointRouterImpl's functions.
// You probably don't want to change any of this code.

PointToPointRouter::PointToPointRouter(const StreetMap* sm)
{
    m_impl = new PointToPointRouterImpl(sm);
}

PointToPointRouter::~PointToPointRouter()
{
    delete m_impl;
}

DeliveryResult PointToPointRouter::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
    return m_impl->generatePointToPointRoute(start, end, route, totalDistanceTravelled);
}
