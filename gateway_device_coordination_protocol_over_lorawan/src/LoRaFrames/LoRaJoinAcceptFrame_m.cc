//
// Generated file, do not edit! Created by nedtool 6.0 from src/LoRaFrames/LoRaJoinAcceptFrame.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "LoRaJoinAcceptFrame_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

namespace {
template <class T> inline
typename std::enable_if<std::is_polymorphic<T>::value && std::is_base_of<omnetpp::cObject,T>::value, void *>::type
toVoidPtr(T* t)
{
    return (void *)(static_cast<const omnetpp::cObject *>(t));
}

template <class T> inline
typename std::enable_if<std::is_polymorphic<T>::value && !std::is_base_of<omnetpp::cObject,T>::value, void *>::type
toVoidPtr(T* t)
{
    return (void *)dynamic_cast<const void *>(t);
}

template <class T> inline
typename std::enable_if<!std::is_polymorphic<T>::value, void *>::type
toVoidPtr(T* t)
{
    return (void *)static_cast<const void *>(t);
}

}


// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule to generate operator<< for shared_ptr<T>
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const std::shared_ptr<T>& t) { return out << t.get(); }

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline typename std::enable_if<!std::is_base_of<omnetpp::cObject, T>::value, std::ostream&>::type
operator<<(std::ostream& out,const T&) {const char *s = omnetpp::opp_typename(typeid(T)); out.put('<'); out.write(s, strlen(s)); out.put('>'); return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');

    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

Register_Class(LoRaJoinAcceptFrame)

LoRaJoinAcceptFrame::LoRaJoinAcceptFrame(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

LoRaJoinAcceptFrame::LoRaJoinAcceptFrame(const LoRaJoinAcceptFrame& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

LoRaJoinAcceptFrame::~LoRaJoinAcceptFrame()
{
}

LoRaJoinAcceptFrame& LoRaJoinAcceptFrame::operator=(const LoRaJoinAcceptFrame& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void LoRaJoinAcceptFrame::copy(const LoRaJoinAcceptFrame& other)
{
    this->joinNonce = other.joinNonce;
    for (size_t i = 0; i < 3; i++) {
        this->netID[i] = other.netID[i];
    }
    for (size_t i = 0; i < 4; i++) {
        this->devAddr[i] = other.devAddr[i];
    }
    this->DLSettings = other.DLSettings;
    this->RXDelay = other.RXDelay;
    for (size_t i = 0; i < 16; i++) {
        this->CFList[i] = other.CFList[i];
    }
}

void LoRaJoinAcceptFrame::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->joinNonce);
    doParsimArrayPacking(b,this->netID,3);
    doParsimArrayPacking(b,this->devAddr,4);
    doParsimPacking(b,this->DLSettings);
    doParsimPacking(b,this->RXDelay);
    doParsimArrayPacking(b,this->CFList,16);
}

void LoRaJoinAcceptFrame::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->joinNonce);
    doParsimArrayUnpacking(b,this->netID,3);
    doParsimArrayUnpacking(b,this->devAddr,4);
    doParsimUnpacking(b,this->DLSettings);
    doParsimUnpacking(b,this->RXDelay);
    doParsimArrayUnpacking(b,this->CFList,16);
}

uint32_t LoRaJoinAcceptFrame::getJoinNonce() const
{
    return this->joinNonce;
}

void LoRaJoinAcceptFrame::setJoinNonce(uint32_t joinNonce)
{
    this->joinNonce = joinNonce;
}

size_t LoRaJoinAcceptFrame::getNetIDArraySize() const
{
    return 3;
}

uint8_t LoRaJoinAcceptFrame::getNetID(size_t k) const
{
    if (k >= 3) throw omnetpp::cRuntimeError("Array of size 3 indexed by %lu", (unsigned long)k);
    return this->netID[k];
}

void LoRaJoinAcceptFrame::setNetID(size_t k, uint8_t netID)
{
    if (k >= 3) throw omnetpp::cRuntimeError("Array of size 3 indexed by %lu", (unsigned long)k);
    this->netID[k] = netID;
}

size_t LoRaJoinAcceptFrame::getDevAddrArraySize() const
{
    return 4;
}

uint8_t LoRaJoinAcceptFrame::getDevAddr(size_t k) const
{
    if (k >= 4) throw omnetpp::cRuntimeError("Array of size 4 indexed by %lu", (unsigned long)k);
    return this->devAddr[k];
}

void LoRaJoinAcceptFrame::setDevAddr(size_t k, uint8_t devAddr)
{
    if (k >= 4) throw omnetpp::cRuntimeError("Array of size 4 indexed by %lu", (unsigned long)k);
    this->devAddr[k] = devAddr;
}

uint8_t LoRaJoinAcceptFrame::getDLSettings() const
{
    return this->DLSettings;
}

void LoRaJoinAcceptFrame::setDLSettings(uint8_t DLSettings)
{
    this->DLSettings = DLSettings;
}

uint8_t LoRaJoinAcceptFrame::getRXDelay() const
{
    return this->RXDelay;
}

void LoRaJoinAcceptFrame::setRXDelay(uint8_t RXDelay)
{
    this->RXDelay = RXDelay;
}

size_t LoRaJoinAcceptFrame::getCFListArraySize() const
{
    return 16;
}

uint8_t LoRaJoinAcceptFrame::getCFList(size_t k) const
{
    if (k >= 16) throw omnetpp::cRuntimeError("Array of size 16 indexed by %lu", (unsigned long)k);
    return this->CFList[k];
}

void LoRaJoinAcceptFrame::setCFList(size_t k, uint8_t CFList)
{
    if (k >= 16) throw omnetpp::cRuntimeError("Array of size 16 indexed by %lu", (unsigned long)k);
    this->CFList[k] = CFList;
}

class LoRaJoinAcceptFrameDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
    enum FieldConstants {
        FIELD_joinNonce,
        FIELD_netID,
        FIELD_devAddr,
        FIELD_DLSettings,
        FIELD_RXDelay,
        FIELD_CFList,
    };
  public:
    LoRaJoinAcceptFrameDescriptor();
    virtual ~LoRaJoinAcceptFrameDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;
    virtual void setFieldArraySize(void *object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual void setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
    virtual void setFieldStructValuePointer(void *object, int field, int i, void *ptr) const override;
};

Register_ClassDescriptor(LoRaJoinAcceptFrameDescriptor)

LoRaJoinAcceptFrameDescriptor::LoRaJoinAcceptFrameDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(LoRaJoinAcceptFrame)), "omnetpp::cPacket")
{
    propertynames = nullptr;
}

LoRaJoinAcceptFrameDescriptor::~LoRaJoinAcceptFrameDescriptor()
{
    delete[] propertynames;
}

bool LoRaJoinAcceptFrameDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<LoRaJoinAcceptFrame *>(obj)!=nullptr;
}

const char **LoRaJoinAcceptFrameDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *LoRaJoinAcceptFrameDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int LoRaJoinAcceptFrameDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 6+basedesc->getFieldCount() : 6;
}

unsigned int LoRaJoinAcceptFrameDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_joinNonce
        FD_ISARRAY | FD_ISEDITABLE,    // FIELD_netID
        FD_ISARRAY | FD_ISEDITABLE,    // FIELD_devAddr
        FD_ISEDITABLE,    // FIELD_DLSettings
        FD_ISEDITABLE,    // FIELD_RXDelay
        FD_ISARRAY | FD_ISEDITABLE,    // FIELD_CFList
    };
    return (field >= 0 && field < 6) ? fieldTypeFlags[field] : 0;
}

const char *LoRaJoinAcceptFrameDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "joinNonce",
        "netID",
        "devAddr",
        "DLSettings",
        "RXDelay",
        "CFList",
    };
    return (field >= 0 && field < 6) ? fieldNames[field] : nullptr;
}

int LoRaJoinAcceptFrameDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0] == 'j' && strcmp(fieldName, "joinNonce") == 0) return base+0;
    if (fieldName[0] == 'n' && strcmp(fieldName, "netID") == 0) return base+1;
    if (fieldName[0] == 'd' && strcmp(fieldName, "devAddr") == 0) return base+2;
    if (fieldName[0] == 'D' && strcmp(fieldName, "DLSettings") == 0) return base+3;
    if (fieldName[0] == 'R' && strcmp(fieldName, "RXDelay") == 0) return base+4;
    if (fieldName[0] == 'C' && strcmp(fieldName, "CFList") == 0) return base+5;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *LoRaJoinAcceptFrameDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "uint32_t",    // FIELD_joinNonce
        "uint8_t",    // FIELD_netID
        "uint8_t",    // FIELD_devAddr
        "uint8_t",    // FIELD_DLSettings
        "uint8_t",    // FIELD_RXDelay
        "uint8_t",    // FIELD_CFList
    };
    return (field >= 0 && field < 6) ? fieldTypeStrings[field] : nullptr;
}

const char **LoRaJoinAcceptFrameDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *LoRaJoinAcceptFrameDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int LoRaJoinAcceptFrameDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    LoRaJoinAcceptFrame *pp = (LoRaJoinAcceptFrame *)object; (void)pp;
    switch (field) {
        case FIELD_netID: return 3;
        case FIELD_devAddr: return 4;
        case FIELD_CFList: return 16;
        default: return 0;
    }
}

void LoRaJoinAcceptFrameDescriptor::setFieldArraySize(void *object, int field, int size) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount()) {
            basedesc->setFieldArraySize(object, field, size);
            return;
        }
        field -= basedesc->getFieldCount();
    }
    LoRaJoinAcceptFrame *pp = (LoRaJoinAcceptFrame *)object; (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'LoRaJoinAcceptFrame'", field);
    }
}

const char *LoRaJoinAcceptFrameDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    LoRaJoinAcceptFrame *pp = (LoRaJoinAcceptFrame *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string LoRaJoinAcceptFrameDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    LoRaJoinAcceptFrame *pp = (LoRaJoinAcceptFrame *)object; (void)pp;
    switch (field) {
        case FIELD_joinNonce: return ulong2string(pp->getJoinNonce());
        case FIELD_netID: return ulong2string(pp->getNetID(i));
        case FIELD_devAddr: return ulong2string(pp->getDevAddr(i));
        case FIELD_DLSettings: return ulong2string(pp->getDLSettings());
        case FIELD_RXDelay: return ulong2string(pp->getRXDelay());
        case FIELD_CFList: return ulong2string(pp->getCFList(i));
        default: return "";
    }
}

void LoRaJoinAcceptFrameDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount()) {
            basedesc->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= basedesc->getFieldCount();
    }
    LoRaJoinAcceptFrame *pp = (LoRaJoinAcceptFrame *)object; (void)pp;
    switch (field) {
        case FIELD_joinNonce: pp->setJoinNonce(string2ulong(value)); break;
        case FIELD_netID: pp->setNetID(i,string2ulong(value)); break;
        case FIELD_devAddr: pp->setDevAddr(i,string2ulong(value)); break;
        case FIELD_DLSettings: pp->setDLSettings(string2ulong(value)); break;
        case FIELD_RXDelay: pp->setRXDelay(string2ulong(value)); break;
        case FIELD_CFList: pp->setCFList(i,string2ulong(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'LoRaJoinAcceptFrame'", field);
    }
}

const char *LoRaJoinAcceptFrameDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *LoRaJoinAcceptFrameDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    LoRaJoinAcceptFrame *pp = (LoRaJoinAcceptFrame *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

void LoRaJoinAcceptFrameDescriptor::setFieldStructValuePointer(void *object, int field, int i, void *ptr) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount()) {
            basedesc->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= basedesc->getFieldCount();
    }
    LoRaJoinAcceptFrame *pp = (LoRaJoinAcceptFrame *)object; (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'LoRaJoinAcceptFrame'", field);
    }
}

