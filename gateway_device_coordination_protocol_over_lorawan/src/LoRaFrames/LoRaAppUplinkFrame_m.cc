//
// Generated file, do not edit! Created by nedtool 6.0 from src/LoRaFrames/LoRaAppUplinkFrame.msg.
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
#include "LoRaAppUplinkFrame_m.h"

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

LoRaControlUplink::LoRaControlUplink()
{
    for (size_t i = 0; i < 4; i++)
        this->FOptsLen[i] = false;
}

void __doPacking(omnetpp::cCommBuffer *b, const LoRaControlUplink& a)
{
    doParsimArrayPacking(b,a.FOptsLen,4);
    doParsimPacking(b,a.ClassB);
    doParsimPacking(b,a.ACK);
    doParsimPacking(b,a.ADRACKReq);
    doParsimPacking(b,a.ADR);
}

void __doUnpacking(omnetpp::cCommBuffer *b, LoRaControlUplink& a)
{
    doParsimArrayUnpacking(b,a.FOptsLen,4);
    doParsimUnpacking(b,a.ClassB);
    doParsimUnpacking(b,a.ACK);
    doParsimUnpacking(b,a.ADRACKReq);
    doParsimUnpacking(b,a.ADR);
}

class LoRaControlUplinkDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
    enum FieldConstants {
        FIELD_FOptsLen,
        FIELD_ClassB,
        FIELD_ACK,
        FIELD_ADRACKReq,
        FIELD_ADR,
    };
  public:
    LoRaControlUplinkDescriptor();
    virtual ~LoRaControlUplinkDescriptor();

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

Register_ClassDescriptor(LoRaControlUplinkDescriptor)

LoRaControlUplinkDescriptor::LoRaControlUplinkDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(LoRaControlUplink)), "")
{
    propertynames = nullptr;
}

LoRaControlUplinkDescriptor::~LoRaControlUplinkDescriptor()
{
    delete[] propertynames;
}

bool LoRaControlUplinkDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<LoRaControlUplink *>(obj)!=nullptr;
}

const char **LoRaControlUplinkDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *LoRaControlUplinkDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int LoRaControlUplinkDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount() : 5;
}

unsigned int LoRaControlUplinkDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISARRAY | FD_ISEDITABLE,    // FIELD_FOptsLen
        FD_ISEDITABLE,    // FIELD_ClassB
        FD_ISEDITABLE,    // FIELD_ACK
        FD_ISEDITABLE,    // FIELD_ADRACKReq
        FD_ISEDITABLE,    // FIELD_ADR
    };
    return (field >= 0 && field < 5) ? fieldTypeFlags[field] : 0;
}

const char *LoRaControlUplinkDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "FOptsLen",
        "ClassB",
        "ACK",
        "ADRACKReq",
        "ADR",
    };
    return (field >= 0 && field < 5) ? fieldNames[field] : nullptr;
}

int LoRaControlUplinkDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0] == 'F' && strcmp(fieldName, "FOptsLen") == 0) return base+0;
    if (fieldName[0] == 'C' && strcmp(fieldName, "ClassB") == 0) return base+1;
    if (fieldName[0] == 'A' && strcmp(fieldName, "ACK") == 0) return base+2;
    if (fieldName[0] == 'A' && strcmp(fieldName, "ADRACKReq") == 0) return base+3;
    if (fieldName[0] == 'A' && strcmp(fieldName, "ADR") == 0) return base+4;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *LoRaControlUplinkDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "bool",    // FIELD_FOptsLen
        "bool",    // FIELD_ClassB
        "bool",    // FIELD_ACK
        "bool",    // FIELD_ADRACKReq
        "bool",    // FIELD_ADR
    };
    return (field >= 0 && field < 5) ? fieldTypeStrings[field] : nullptr;
}

const char **LoRaControlUplinkDescriptor::getFieldPropertyNames(int field) const
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

const char *LoRaControlUplinkDescriptor::getFieldProperty(int field, const char *propertyname) const
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

int LoRaControlUplinkDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    LoRaControlUplink *pp = (LoRaControlUplink *)object; (void)pp;
    switch (field) {
        case FIELD_FOptsLen: return 4;
        default: return 0;
    }
}

void LoRaControlUplinkDescriptor::setFieldArraySize(void *object, int field, int size) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount()) {
            basedesc->setFieldArraySize(object, field, size);
            return;
        }
        field -= basedesc->getFieldCount();
    }
    LoRaControlUplink *pp = (LoRaControlUplink *)object; (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'LoRaControlUplink'", field);
    }
}

const char *LoRaControlUplinkDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    LoRaControlUplink *pp = (LoRaControlUplink *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string LoRaControlUplinkDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    LoRaControlUplink *pp = (LoRaControlUplink *)object; (void)pp;
    switch (field) {
        case FIELD_FOptsLen: if (i >= 4) return "";
                return bool2string(pp->FOptsLen[i]);
        case FIELD_ClassB: return bool2string(pp->ClassB);
        case FIELD_ACK: return bool2string(pp->ACK);
        case FIELD_ADRACKReq: return bool2string(pp->ADRACKReq);
        case FIELD_ADR: return bool2string(pp->ADR);
        default: return "";
    }
}

void LoRaControlUplinkDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount()) {
            basedesc->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= basedesc->getFieldCount();
    }
    LoRaControlUplink *pp = (LoRaControlUplink *)object; (void)pp;
    switch (field) {
        case FIELD_FOptsLen: if (i < 0 || i >= 4) throw omnetpp::cRuntimeError("Array index %d out of bounds for field %d of class 'LoRaControlUplink'", i, field);
                pp->FOptsLen[i] = string2bool(value); break;
        case FIELD_ClassB: pp->ClassB = string2bool(value); break;
        case FIELD_ACK: pp->ACK = string2bool(value); break;
        case FIELD_ADRACKReq: pp->ADRACKReq = string2bool(value); break;
        case FIELD_ADR: pp->ADR = string2bool(value); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'LoRaControlUplink'", field);
    }
}

const char *LoRaControlUplinkDescriptor::getFieldStructName(int field) const
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

void *LoRaControlUplinkDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    LoRaControlUplink *pp = (LoRaControlUplink *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

void LoRaControlUplinkDescriptor::setFieldStructValuePointer(void *object, int field, int i, void *ptr) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount()) {
            basedesc->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= basedesc->getFieldCount();
    }
    LoRaControlUplink *pp = (LoRaControlUplink *)object; (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'LoRaControlUplink'", field);
    }
}

Register_Class(LoRaAppUplinkFrame)

LoRaAppUplinkFrame::LoRaAppUplinkFrame(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

LoRaAppUplinkFrame::LoRaAppUplinkFrame(const LoRaAppUplinkFrame& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

LoRaAppUplinkFrame::~LoRaAppUplinkFrame()
{
}

LoRaAppUplinkFrame& LoRaAppUplinkFrame::operator=(const LoRaAppUplinkFrame& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void LoRaAppUplinkFrame::copy(const LoRaAppUplinkFrame& other)
{
    for (size_t i = 0; i < 4; i++) {
        this->deviceAddress[i] = other.deviceAddress[i];
    }
    this->control = other.control;
    this->counter = other.counter;
    this->port = other.port;
    for (size_t i = 0; i < 11; i++) {
        this->payload[i] = other.payload[i];
    }
}

void LoRaAppUplinkFrame::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimArrayPacking(b,this->deviceAddress,4);
    doParsimPacking(b,this->control);
    doParsimPacking(b,this->counter);
    doParsimPacking(b,this->port);
    doParsimArrayPacking(b,this->payload,11);
}

void LoRaAppUplinkFrame::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimArrayUnpacking(b,this->deviceAddress,4);
    doParsimUnpacking(b,this->control);
    doParsimUnpacking(b,this->counter);
    doParsimUnpacking(b,this->port);
    doParsimArrayUnpacking(b,this->payload,11);
}

size_t LoRaAppUplinkFrame::getDeviceAddressArraySize() const
{
    return 4;
}

uint8_t LoRaAppUplinkFrame::getDeviceAddress(size_t k) const
{
    if (k >= 4) throw omnetpp::cRuntimeError("Array of size 4 indexed by %lu", (unsigned long)k);
    return this->deviceAddress[k];
}

void LoRaAppUplinkFrame::setDeviceAddress(size_t k, uint8_t deviceAddress)
{
    if (k >= 4) throw omnetpp::cRuntimeError("Array of size 4 indexed by %lu", (unsigned long)k);
    this->deviceAddress[k] = deviceAddress;
}

const LoRaControlUplink& LoRaAppUplinkFrame::getControl() const
{
    return this->control;
}

void LoRaAppUplinkFrame::setControl(const LoRaControlUplink& control)
{
    this->control = control;
}

uint16_t LoRaAppUplinkFrame::getCounter() const
{
    return this->counter;
}

void LoRaAppUplinkFrame::setCounter(uint16_t counter)
{
    this->counter = counter;
}

uint8_t LoRaAppUplinkFrame::getPort() const
{
    return this->port;
}

void LoRaAppUplinkFrame::setPort(uint8_t port)
{
    this->port = port;
}

size_t LoRaAppUplinkFrame::getPayloadArraySize() const
{
    return 11;
}

uint8_t LoRaAppUplinkFrame::getPayload(size_t k) const
{
    if (k >= 11) throw omnetpp::cRuntimeError("Array of size 11 indexed by %lu", (unsigned long)k);
    return this->payload[k];
}

void LoRaAppUplinkFrame::setPayload(size_t k, uint8_t payload)
{
    if (k >= 11) throw omnetpp::cRuntimeError("Array of size 11 indexed by %lu", (unsigned long)k);
    this->payload[k] = payload;
}

class LoRaAppUplinkFrameDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
    enum FieldConstants {
        FIELD_deviceAddress,
        FIELD_control,
        FIELD_counter,
        FIELD_port,
        FIELD_payload,
    };
  public:
    LoRaAppUplinkFrameDescriptor();
    virtual ~LoRaAppUplinkFrameDescriptor();

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

Register_ClassDescriptor(LoRaAppUplinkFrameDescriptor)

LoRaAppUplinkFrameDescriptor::LoRaAppUplinkFrameDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(LoRaAppUplinkFrame)), "omnetpp::cPacket")
{
    propertynames = nullptr;
}

LoRaAppUplinkFrameDescriptor::~LoRaAppUplinkFrameDescriptor()
{
    delete[] propertynames;
}

bool LoRaAppUplinkFrameDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<LoRaAppUplinkFrame *>(obj)!=nullptr;
}

const char **LoRaAppUplinkFrameDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *LoRaAppUplinkFrameDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int LoRaAppUplinkFrameDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount() : 5;
}

unsigned int LoRaAppUplinkFrameDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISARRAY | FD_ISEDITABLE,    // FIELD_deviceAddress
        FD_ISCOMPOUND,    // FIELD_control
        FD_ISEDITABLE,    // FIELD_counter
        FD_ISEDITABLE,    // FIELD_port
        FD_ISARRAY | FD_ISEDITABLE,    // FIELD_payload
    };
    return (field >= 0 && field < 5) ? fieldTypeFlags[field] : 0;
}

const char *LoRaAppUplinkFrameDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "deviceAddress",
        "control",
        "counter",
        "port",
        "payload",
    };
    return (field >= 0 && field < 5) ? fieldNames[field] : nullptr;
}

int LoRaAppUplinkFrameDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0] == 'd' && strcmp(fieldName, "deviceAddress") == 0) return base+0;
    if (fieldName[0] == 'c' && strcmp(fieldName, "control") == 0) return base+1;
    if (fieldName[0] == 'c' && strcmp(fieldName, "counter") == 0) return base+2;
    if (fieldName[0] == 'p' && strcmp(fieldName, "port") == 0) return base+3;
    if (fieldName[0] == 'p' && strcmp(fieldName, "payload") == 0) return base+4;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *LoRaAppUplinkFrameDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "uint8_t",    // FIELD_deviceAddress
        "LoRaControlUplink",    // FIELD_control
        "uint16_t",    // FIELD_counter
        "uint8_t",    // FIELD_port
        "uint8_t",    // FIELD_payload
    };
    return (field >= 0 && field < 5) ? fieldTypeStrings[field] : nullptr;
}

const char **LoRaAppUplinkFrameDescriptor::getFieldPropertyNames(int field) const
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

const char *LoRaAppUplinkFrameDescriptor::getFieldProperty(int field, const char *propertyname) const
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

int LoRaAppUplinkFrameDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    LoRaAppUplinkFrame *pp = (LoRaAppUplinkFrame *)object; (void)pp;
    switch (field) {
        case FIELD_deviceAddress: return 4;
        case FIELD_payload: return 11;
        default: return 0;
    }
}

void LoRaAppUplinkFrameDescriptor::setFieldArraySize(void *object, int field, int size) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount()) {
            basedesc->setFieldArraySize(object, field, size);
            return;
        }
        field -= basedesc->getFieldCount();
    }
    LoRaAppUplinkFrame *pp = (LoRaAppUplinkFrame *)object; (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'LoRaAppUplinkFrame'", field);
    }
}

const char *LoRaAppUplinkFrameDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    LoRaAppUplinkFrame *pp = (LoRaAppUplinkFrame *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string LoRaAppUplinkFrameDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    LoRaAppUplinkFrame *pp = (LoRaAppUplinkFrame *)object; (void)pp;
    switch (field) {
        case FIELD_deviceAddress: return ulong2string(pp->getDeviceAddress(i));
        case FIELD_control: {std::stringstream out; out << pp->getControl(); return out.str();}
        case FIELD_counter: return ulong2string(pp->getCounter());
        case FIELD_port: return ulong2string(pp->getPort());
        case FIELD_payload: return ulong2string(pp->getPayload(i));
        default: return "";
    }
}

void LoRaAppUplinkFrameDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount()) {
            basedesc->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= basedesc->getFieldCount();
    }
    LoRaAppUplinkFrame *pp = (LoRaAppUplinkFrame *)object; (void)pp;
    switch (field) {
        case FIELD_deviceAddress: pp->setDeviceAddress(i,string2ulong(value)); break;
        case FIELD_counter: pp->setCounter(string2ulong(value)); break;
        case FIELD_port: pp->setPort(string2ulong(value)); break;
        case FIELD_payload: pp->setPayload(i,string2ulong(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'LoRaAppUplinkFrame'", field);
    }
}

const char *LoRaAppUplinkFrameDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case FIELD_control: return omnetpp::opp_typename(typeid(LoRaControlUplink));
        default: return nullptr;
    };
}

void *LoRaAppUplinkFrameDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    LoRaAppUplinkFrame *pp = (LoRaAppUplinkFrame *)object; (void)pp;
    switch (field) {
        case FIELD_control: return toVoidPtr(&pp->getControl()); break;
        default: return nullptr;
    }
}

void LoRaAppUplinkFrameDescriptor::setFieldStructValuePointer(void *object, int field, int i, void *ptr) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount()) {
            basedesc->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= basedesc->getFieldCount();
    }
    LoRaAppUplinkFrame *pp = (LoRaAppUplinkFrame *)object; (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'LoRaAppUplinkFrame'", field);
    }
}

