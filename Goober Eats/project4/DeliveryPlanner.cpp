#include "provided.h"
#include <vector>
#include <list>
#include <map>

using namespace std;

class DeliveryPlannerImpl
{
public:
    DeliveryPlannerImpl(const StreetMap* sm);
    ~DeliveryPlannerImpl();
    DeliveryResult generateDeliveryPlan(
        const GeoCoord& depot,
        const vector<DeliveryRequest>& deliveries,
        vector<DeliveryCommand>& commands,
        double& totalDistanceTravelled) const;
    
private:
    const StreetMap* m_sm;
    PointToPointRouter m_router;
    DeliveryOptimizer m_opt;
    map<double, string> ang2dir;
    void makeCommand(const list<StreetSegment> partial, vector<DeliveryCommand>& commands) const;
};

DeliveryPlannerImpl::DeliveryPlannerImpl(const StreetMap* sm)
: m_sm(sm), m_router(sm), m_opt(sm)
{
    ang2dir[-1] = "east";
    ang2dir[0] = "northeast";
    ang2dir[1] = "north";
    ang2dir[2] = "northwest";
    ang2dir[3] = "west";
    ang2dir[4] = "southwest";
    ang2dir[5] = "south";
    ang2dir[6] = "southeast";
    ang2dir[7] = "east";
}

DeliveryPlannerImpl::~DeliveryPlannerImpl()
{
}

DeliveryResult DeliveryPlannerImpl::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    //initialize and optimize
    commands.clear();
    totalDistanceTravelled = 0;
    double oldDistance;
    double newDistance;
    vector<DeliveryRequest> opt_deliveries = deliveries;
    m_opt.optimizeDeliveryOrder(depot, opt_deliveries, oldDistance, newDistance);
    
    GeoCoord lastStop = depot;
    
    //delivery cycle
    for (auto p : opt_deliveries)
    {
        list<StreetSegment> routePart;
        double disPart;
        switch (m_router.generatePointToPointRoute(lastStop, p.location, routePart, disPart))
        {
            case BAD_COORD:
                cerr << "ERROR: fail to produce routes! --bad coords--Planner" << endl;
                return BAD_COORD;
                
            case NO_ROUTE:
                cerr << "ERROR: fail to produce routes! --no routes--Planner" << endl;
                return NO_ROUTE;
            
            default:
                makeCommand(routePart, commands);
        }

        DeliveryCommand delivered;
        delivered.initAsDeliverCommand(p.item);
        commands.push_back(delivered);
        
        totalDistanceTravelled += disPart;
        lastStop = p.location;
    }
    
    list<StreetSegment> routePart;
    double disPart;
    m_router.generatePointToPointRoute(lastStop, depot, routePart, disPart);
    makeCommand(routePart, commands);
    totalDistanceTravelled += disPart;

    return DELIVERY_SUCCESS;
}

void DeliveryPlannerImpl::makeCommand(const list<StreetSegment> partial, vector<DeliveryCommand>& commands) const
{
    StreetSegment lastStreet = partial.front();
    DeliveryCommand lastProceed;
    int DirStart = floor((angleOfLine(partial.front())-22.5) / 45);
    auto it = ang2dir.find(DirStart);
    string dirStart = it->second;
    lastProceed.initAsProceedCommand(dirStart, partial.front().name, 0);
    
    
    auto q = partial.end();
    q --;
    for (auto p = partial.begin(); p!= q; p++)
    {
        //some calculations
        double dis = distanceEarthMiles(p->start, p->end);
        double angle = angleBetween2Lines(lastStreet, *p);
        int Dir = floor((angleOfLine(*p) - 22.5) / 45);
        auto it = ang2dir.find(Dir);
        string dir = it->second;
            
        //proceed on the same street
        if (p->name == lastStreet.name)
        {
            //cerr << "===================================" << endl;
            lastProceed.increaseDistance(dis);
            continue;
        }
        
        //go to next street
        DeliveryCommand proceedNext;
        DeliveryCommand turnNext;
        
        //proceed onto next
        if (angle < 1 || angle > 359)
        {
            proceedNext.initAsProceedCommand(dir, p->name, dis);
        }
        
        //turn to next
        else
        {
            string dirTurn;
            if (angle < 180)
                dirTurn = "left";
            else
                dirTurn = "right";
            turnNext.initAsTurnCommand(dirTurn, p->name);
            proceedNext.initAsProceedCommand(dir, p->name, dis);
        }
        
        lastStreet = (*p);
        commands.push_back(lastProceed);
        commands.push_back(turnNext);
        lastProceed = proceedNext;
    }
    //the last segments
    commands.push_back(lastProceed);
}
//******************** DeliveryPlanner functions ******************************

// These functions simply delegate to DeliveryPlannerImpl's functions.
// You probably don't want to change any of this code.

DeliveryPlanner::DeliveryPlanner(const StreetMap* sm)
{
    m_impl = new DeliveryPlannerImpl(sm);
}

DeliveryPlanner::~DeliveryPlanner()
{
    delete m_impl;
}

DeliveryResult DeliveryPlanner::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    return m_impl->generateDeliveryPlan(depot, deliveries, commands, totalDistanceTravelled);
}
