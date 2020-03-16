#include "provided.h"
#include "ExpandableHashMap.h"
#include <string>
#include <vector>
#include <functional>
#include <fstream>
#include <sstream>
using namespace std;

unsigned int hasher(const GeoCoord& g)
{
    return std::hash<string>()(g.latitudeText + g.longitudeText);
}

unsigned int hasher(const string& s)
{
    return std::hash<string>()(s);
}

class StreetMapImpl
{
public:
    StreetMapImpl();
    ~StreetMapImpl();
    bool load(string mapFile);
    bool getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const;
    
private:
    ExpandableHashMap<GeoCoord, vector<StreetSegment>> m_map;
};

StreetMapImpl::StreetMapImpl()
: m_map()
{
    
}

StreetMapImpl::~StreetMapImpl()
{
}

bool StreetMapImpl::load(string mapFile)
{
    ifstream infile(mapFile);
    if (!infile)
    {
        cerr << "Error: Cannot open " <<  mapFile << " !" << endl;
        return false;
    }
    
    string line1;
    string line2;
    while(getline(infile,line1) && getline(infile,line2))
    {
        string street = line1;
        
        istringstream iss2(line2);
        int seg_num = 0;
        iss2 >> seg_num;
        for (int i = 0; i < seg_num; i++)
        {
            string startLat;
            string startLon;
            string endLat;
            string endLon;
            string line3;
            getline(infile, line3);
            istringstream iss3(line3);
            if (iss3 >> startLat >> startLon >> endLat >> endLon)
            {
                GeoCoord start(startLat, startLon);
                GeoCoord end(endLat, endLon);
                
                vector<StreetSegment> startSeg;
                StreetSegment forward(start, end, street);
                if(m_map.find(start) != nullptr)
                    startSeg = *m_map.find(start);
                startSeg.push_back(forward);
                
                vector<StreetSegment> endSeg;
                StreetSegment backward(end, start, street);
                if(m_map.find(end) != nullptr)
                    endSeg = *m_map.find(end);
                endSeg.push_back(backward);
                
                m_map.associate(start, startSeg);
                m_map.associate(end, endSeg);
            }
        }
    }
    return true;
}

bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
    const vector<StreetSegment>* result = m_map.find(gc);
    if (result == nullptr)
        return false;
    segs = *result;
    return true;
}

//******************** StreetMap functions ************************************

// These functions simply delegate to StreetMapImpl's functions.
// You probably don't want to change any of this code.

StreetMap::StreetMap()
{
    m_impl = new StreetMapImpl;
}

StreetMap::~StreetMap()
{
    delete m_impl;
}

bool StreetMap::load(string mapFile)
{
    return m_impl->load(mapFile);
}

bool StreetMap::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
   return m_impl->getSegmentsThatStartWith(gc, segs);
}
