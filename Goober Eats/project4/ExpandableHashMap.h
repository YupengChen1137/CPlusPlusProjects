// ExpandableHashMap.h

// Skeleton for the ExpandableHashMap class template.  You must implement the first six
// member functions.

#include <iostream>
using namespace std;
const int INITIAL_BUCKETS = 8;

template<typename KeyType, typename ValueType>
class ExpandableHashMap
{
public:
	ExpandableHashMap(double maximumLoadFactor = 0.5);
	~ExpandableHashMap();
	void reset();
	int size() const;
    
    //DELELTE THIS!!!!!!!!!!!
    int bucketNum() const { return m_slot; }
    double load() const { return double(m_size)/m_slot; }
    //DELELTE THIS!!!!!!!!!!!
    
	void associate(const KeyType& key, const ValueType& value);

	  // for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	  // for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
	}

	  // C++11 syntax for preventing copying and assignment
	ExpandableHashMap(const ExpandableHashMap&) = delete;
	ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;

private:
    struct Node {
        KeyType m_key;
        ValueType m_value;
        Node* m_next;
    };
    class LinkedList {
    public:
        Node* m_head;
        LinkedList() {
            m_head = nullptr;
        }
        ~LinkedList() {
            Node* p;
            p = m_head;
            while(p != nullptr)
            {
                Node* n = p->m_next;
                delete p;
                p = n;
            }
        }
        void insert(const KeyType& key, const ValueType& value)
        {
            Node* added = new Node;
            added->m_key = key;
            added->m_value = value;
            
            if(m_head == nullptr)
            {
                added->m_next = nullptr;
                m_head = added;
            }
            else
            {
                added->m_next = m_head;
                m_head = added;
            }
        }
    };
    int m_size;
    int m_slot;
    double m_maxLoad;
    LinkedList* m_buckets;
    unsigned int getBucketNumber(const KeyType& key) const
    {
        unsigned int hasher(const KeyType& k);
        unsigned int h = hasher(key);
        return h % m_slot;
    }
};

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::ExpandableHashMap(double maximumLoadFactor)
{
    m_size = 0;
    m_slot = INITIAL_BUCKETS;
    if (maximumLoadFactor > 0)
        m_maxLoad = maximumLoadFactor;
    else
        m_maxLoad = 0.5;
        
    m_buckets = new LinkedList[INITIAL_BUCKETS];
    //std::cerr << "maximumLoadFactor: " << m_maxLoad << std::endl;
}

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::~ExpandableHashMap()
{
    delete [] m_buckets;
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::reset()
{
    delete [] m_buckets;
    m_size = 0;
    m_slot = INITIAL_BUCKETS;
    m_buckets = new LinkedList[m_slot];
}

template<typename KeyType, typename ValueType>
int ExpandableHashMap<KeyType, ValueType>::size() const
{
    return m_size;
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
    int bucket_num = getBucketNumber(key);
    Node* p;
    p = m_buckets[bucket_num].m_head;
    while(p != nullptr)
    {
        if (p->m_key == key)
        {
            p->m_value = value;
            return;
        }
        p = p->m_next;
    }
    
    double load = double (m_size) / m_slot;
    if (load > m_maxLoad)
    {
        LinkedList* newBuckets = new LinkedList[m_slot*2];
        m_slot = m_slot*2;
        for (int i = 0; i < (m_slot / 2); i++)
        {
            Node* copy = m_buckets[i].m_head;
            while(copy != nullptr)
            {
                int new_bucket_num = getBucketNumber(copy->m_key);
                newBuckets[new_bucket_num].insert(copy->m_key, copy->m_value);
                copy = copy->m_next;
            }
        }
        delete [] m_buckets;
        m_buckets = newBuckets;
    }
    
    bucket_num = getBucketNumber(key);
    m_buckets[bucket_num].insert(key,value);
    m_size++;
}

template<typename KeyType, typename ValueType>
const ValueType* ExpandableHashMap<KeyType, ValueType>::find(const KeyType& key) const
{
    int bucket_num = getBucketNumber(key);
    Node* p;
    p = m_buckets[bucket_num].m_head;
    while(p != nullptr)
    {
        if (p->m_key == key)
            return &(p->m_value);
        p = p->m_next;
    }
    return nullptr;
}

