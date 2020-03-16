//
////  main.cpp
////  project4
////
////  Created by Yupeng Chen on 3/4/20.
////  Copyright © 2020 Yupeng Chen. All rights reserved.
//
//
//#include <iostream>
//#include <string>
//#include <cassert>
//#include "ExpandableHashMap.h"
//#include "provided.h"
//
//using namespace std;
//
//
//void foo() {
//    // Define a hashmap that maps strings to doubles and has a maximum
//    // load factor of 0.2. It will initially have 8 buckets when empty.
//    ExpandableHashMap<string,double> nameToGPA(0.2);
//
//    // Add new items to the hashmap. Inserting the third item will cause
//    // the hashmap to increase the number of buckets (since the maximum
//    // load factor is 0.2), forcing a rehash of all items.
//    nameToGPA.associate("Carey", 3.5); // Carey has a 3.5 GPA
//    nameToGPA.associate("David", 3.99); // David beat Carey
//    nameToGPA.associate("Abe", 3.2); // Abe has a 3.2 GPA
//    //cerr << nameToGPA.load() << endl;
//    assert(nameToGPA.bucketNum() == 16);
//
//    double* davidsGPA = nameToGPA.find("David");
//    assert(*davidsGPA == 3.99);
//    if (davidsGPA != nullptr)
//        *davidsGPA = 1.5; // after a re-grade of David’s exam
//    assert(*davidsGPA == 1.5);
//    nameToGPA.associate("Carey", 4.0); // Carey deserves a 4.0 // replaces old 3.5 GPA
//    assert(*nameToGPA.find("Carey") == 4.0);
//
//    double* lindasGPA = nameToGPA.find("Linda");
//    assert(lindasGPA == nullptr);
//
//    nameToGPA.reset();
//    assert(nameToGPA.find("Carey") == nullptr);
//    assert(nameToGPA.find("Abe") == nullptr);
//    assert(nameToGPA.find("David") == nullptr);
//    assert(nameToGPA.bucketNum() == 8);
//}
//
//int main() {
//    foo();
//    ExpandableHashMap<string, int> sim;
//    ExpandableHashMap<string, int> sim2(0.4);
//    ExpandableHashMap<string, int> sim3(0);
//
//    sim.associate("a", 1);
//    assert(*sim.find("a") == 1);
//    sim.associate("b", 1);
//    assert(sim.size() == 2);
//    sim.associate("b", 2);
//    assert(sim.size() == 2);
//    assert(*sim.find("b") == 2);
//
//    StreetMap map;
//    map.load("mapdata.txt");
//    GeoCoord a("34.0732851", "-118.4931016");
//    GeoCoord b("34.0724746", "-118.4923463");
//    GeoCoord c("34.0589680", "-118.4424895");
//    GeoCoord d("34.0572000", "-118.4417620");
//    GeoCoord e("34.0591340", "-118.4426546");
//
//    vector<StreetSegment> found;
//    cerr.setf(ios::fixed);
//    cerr.precision(10);
//
//    assert(map.getSegmentsThatStartWith(a, found));
//    for (auto p:found)
//        cerr << p.start.latitude << " " << p.end.latitude << endl;
//    assert(map.getSegmentsThatStartWith(b, found));
//    for (auto p:found)
//        cerr << p.start.latitude << " " << p.end.latitude << endl;
//    assert(map.getSegmentsThatStartWith(c, found));
//    cerr << found.size() << endl;
//    for (auto p:found)
//        cerr << p.end.latitudeText << " " << p.end.longitudeText << endl;
//    assert(map.getSegmentsThatStartWith(d, found));
//    cerr << found.size() << endl;
//    for (auto p:found)
//        cerr << p.end.latitudeText << " " << p.end.longitudeText << endl;
//    assert(map.getSegmentsThatStartWith(e, found));
//    cerr << found.size() << endl;
//    for (auto p:found)
//        cerr << p.end.latitudeText << " " << p.end.longitudeText << endl;
//
//
//    PointToPointRouter router(&map);
//    GeoCoord start("34.0625329", "-118.4470263");
//    GeoCoord stop1("34.0712323", "-118.4505969");
//    GeoCoord stop2("34.0687443", "-118.4449195");
//    GeoCoord stop3("34.0685657", "-118.4489289");
//    GeoCoord stop4("34.0724746", "-118.4923463");
//    list<StreetSegment> route;
//    double distance;
//    assert(router.generatePointToPointRoute(a, b, route, distance) == DELIVERY_SUCCESS);
//    cerr << "found first..." << endl;
//    for(auto p:route)
//        cerr << p.start.latitudeText << " " << p.start.longitudeText << " "
//            << p.end.latitudeText << " " << p.end.longitudeText << " " << p.name << endl;
//    cerr << "Travelled: " << distance << " miles" << endl;
//
//    assert(router.generatePointToPointRoute(b, a, route, distance) == DELIVERY_SUCCESS);
//    cerr << "found second..." << endl;
//    for(auto p:route)
//    cerr << p.start.latitudeText << " " << p.start.longitudeText << " "
//        << p.end.latitudeText << " " << p.end.longitudeText << " " << p.name << endl;
//    cerr << "Travelled: " << distance << " miles" << endl;
//
//    assert(router.generatePointToPointRoute(start, stop1, route, distance) == DELIVERY_SUCCESS);
//    cerr << "found third..." << endl;
//    for(auto p:route)
//    cerr << p.start.latitudeText << " " << p.start.longitudeText << " "
//        << p.end.latitudeText << " " << p.end.longitudeText << " " << p.name << endl;
//    cerr << "Travelled: " << distance << " miles" << endl;
//
//    assert(router.generatePointToPointRoute(stop1, stop2, route, distance) == DELIVERY_SUCCESS);
//    cerr << "found fourth..." << endl;
//    for(auto p:route)
//    cerr << p.start.latitudeText << " " << p.start.longitudeText << " "
//        << p.end.latitudeText << " " << p.end.longitudeText << " " << p.name << endl;
//    cerr << "Travelled: " << distance << " miles" << endl;
//
//    assert(router.generatePointToPointRoute(stop3, stop4, route, distance) == DELIVERY_SUCCESS);
//    cerr << "found fifth..." << endl;
//    assert(router.generatePointToPointRoute(start, stop4, route, distance) == DELIVERY_SUCCESS);
//    cerr << "found sixth..." << endl;
//
//    DeliveryPlanner planner(&map);
//
//    GeoCoord depot("34.0625329", "-118.4470263");
//    GeoCoord Sproul("34.0712323", "-118.4505969");
//    string Landing = "Chicken tenders";
//    DeliveryRequest First(Landing, Sproul);
//    GeoCoord Eng("34.0687443", "-118.4449195");
//    string IV = "B-Plate salmon";
//    DeliveryRequest Second(IV, Eng);
//    GeoCoord Beta("34.0685657", "-118.4489289");
//    string ThetaPi = "Pabst Blue Ribbon beer";
//    DeliveryRequest Third(ThetaPi, Beta);
//
//    GeoCoord four("34.0724746", "-118.4923463");
//    string stop = "Pabst Blue Ribbon beer";
//    DeliveryRequest Fourth(stop, four);
//
//    GeoCoord five("34.0732851", "-118.4931016");
//    DeliveryRequest Fifth(stop, five);
//
//
//    GeoCoord p("34.0436968", "-118.4800519");
//    GeoCoord q("34.0721826", "-118.4435144");
//    assert(router.generatePointToPointRoute(depot, Eng, route, distance) == DELIVERY_SUCCESS);
//    cerr << "found fourth..." << endl;
//    for(auto p:route)
//    cerr << p.start.latitudeText << " " << p.start.longitudeText << " "
//        << p.end.latitudeText << " " << p.end.longitudeText << " " << p.name << endl;
//    cerr << "Travelled: " << distance << " miles" << endl;
//
//    vector<DeliveryRequest> request;
//    //request.push_back(Fourth);
//    //request.push_back(Fifth);
//    request.push_back(Third);
//    request.push_back(Second);
//    request.push_back(First);
//    vector<DeliveryCommand> commands;
//    commands.clear();
//
//    assert(planner.generateDeliveryPlan(depot, request, commands, distance) == DELIVERY_SUCCESS);
//    cout << distance << endl;
//    for(auto p: commands)
//        cerr << p.description() << endl;
//
////    DeliveryOptimizer optimizer(&map);
////    double oldDis;
////    double newDis;
////    optimizer.optimizeDeliveryOrder(depot, request, oldDis, newDis);
////    cout << oldDis << endl;
////    cout << newDis << endl;
//}

#include "provided.h"
#include <list>
using namespace std;

int main()
{
    StreetMap map;
    map.load("mapdata.txt");
    PointToPointRouter route(&map);
    GeoCoord s("34.0555267", "-118.4796954");
    GeoCoord e("34.0547000", "-118.4794734");
    list<StreetSegment> list1;
    double distance = 0;
    route.generatePointToPointRoute(s, e, list1, distance);
    list<StreetSegment>::iterator i = list1.begin();
    for (; i!=list1.end(); i++)
    {
        cout << (*i).start.latitudeText << " " << (*i).start.longitudeText << endl;
        // << "  " << (*i).end.latitudeText << " " << (*i).end.longitudeText;
    }
}
//
//#include "provided.h"
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <string>
//#include <vector>
//using namespace std;
//
//bool loadDeliveryRequests(string deliveriesFile, GeoCoord& depot, vector<DeliveryRequest>& v);
//bool parseDelivery(string line, string& lat, string& lon, string& item);
//
//int main(int argc, char *argv[])
//{
//    if (argc != 3)
//    {
//        cout << "Usage: " << argv[0] << " mapdata.txt deliveries.txt" << endl;
//        return 1;
//    }
//
//    StreetMap sm;
//        
//    if (!sm.load(argv[1]))
//    {
//        cout << "Unable to load map data file " << argv[1] << endl;
//        return 1;
//    }
//
//    GeoCoord depot;
//    vector<DeliveryRequest> deliveries;
//    if (!loadDeliveryRequests(argv[2], depot, deliveries))
//    {
//        cout << "Unable to load delivery request file " << argv[2] << endl;
//        return 1;
//    }
//
//    cout << "Generating route...\n\n";
//
//    DeliveryPlanner dp(&sm);
//    vector<DeliveryCommand> dcs;
//    double totalMiles;
//    DeliveryResult result = dp.generateDeliveryPlan(depot, deliveries, dcs, totalMiles);
//    if (result == BAD_COORD)
//    {
//        cout << "One or more depot or delivery coordinates are invalid." << endl;
//        return 1;
//    }
//    if (result == NO_ROUTE)
//    {
//        cout << "No route can be found to deliver all items." << endl;
//        return 1;
//    }
//    cout << "Starting at the depot...\n";
//    for (const auto& dc : dcs)
//        cout << dc.description() << endl;
//    cout << "You are back at the depot and your deliveries are done!\n";
//    cout.setf(ios::fixed);
//    cout.precision(2);
//    cout << totalMiles << " miles travelled for all deliveries." << endl;
//}
//
//bool loadDeliveryRequests(string deliveriesFile, GeoCoord& depot, vector<DeliveryRequest>& v)
//{
//    ifstream inf(deliveriesFile);
//    if (!inf)
//        return false;
//    string lat;
//    string lon;
//    inf >> lat >> lon;
//    inf.ignore(10000, '\n');
//    depot = GeoCoord(lat, lon);
//    string line;
//    while (getline(inf, line))
//    {
//        string item;
//        if (parseDelivery(line, lat, lon, item))
//            v.push_back(DeliveryRequest(item, GeoCoord(lat, lon)));
//    }
//    return true;
//}
//
//bool parseDelivery(string line, string& lat, string& lon, string& item)
//{
//    const size_t colon = line.find(':');
//    if (colon == string::npos)
//    {
//        cout << "Missing colon in deliveries file line: " << line << endl;
//        return false;
//    }
//    istringstream iss(line.substr(0, colon));
//    if (!(iss >> lat >> lon))
//    {
//        cout << "Bad format in deliveries file line: " << line << endl;
//        return false;
//    }
//    item = line.substr(colon + 1);
//    if (item.empty())
//    {
//        cout << "Missing item in deliveries file line: " << line << endl;
//        return false;
//    }
//    return true;
//}
