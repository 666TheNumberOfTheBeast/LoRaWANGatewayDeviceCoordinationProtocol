//
// Generated file, do not edit! Created by nedtool 6.0 from src/LoRaFrames/LoRaDatalinkFrame.msg.
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
#include "LoRaDatalinkFrame_m.h"

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

Register_Class(LoRaDatalinkFrame)

LoRaDatalinkFrame::LoRaDatalinkFrame(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

LoRaDatalinkFrame::LoRaDatalinkFrame(const LoRaDatalinkFrame& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

LoRaDatalinkFrame::~LoRaDatalinkFrame()
{
}

LoRaDatalinkFrame& LoRaDatalinkFrame::operator=(const LoRaDatalinkFrame& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void LoRaDatalinkFrame::copy(const LoRaDatalinkFrame& other)
{
    this->FType = other.FType;
    this->RFU = other.RFU;
    this->major = other.major;
    for (size_t i = 0; i < 19; i++) {
        this->payload[i] = other.payload[i];
    }
    for (size_t i = 0; i < 4; i++) {
        this->MIC[i] = other.MIC[i];
    }
}

void LoRaDatalinkFrame::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->FType);
    doParsimPacking(b,this->RFU);
    doParsimPacking(b,this->major);
    doParsimArrayPacking(b,this->payload,19);
    doParsimArrayPacking(b,this->MIC,4);
}

void LoRaDatalinkFrame::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->FType);
    doParsimUnpacking(b,this->RFU);
    doParsimUnpacking(b,this->major);
    doParsimArrayUnpacking(b,this->payload,19);
    doParsimArrayUnpacking(b,this->MIC,4);
}

uint8_t LoRaDatalinkFrame::getFType() const
{
    return this->FType;
}

void LoRaDatalinkFrame::setFType(uint8_t FType)
{
    this->FType = FType;
}

uint8_t LoRaDatalinkFrame::getRFU() const
{
    return this->RFU;
}

void LoRaDatalinkFrame::setRFU(uint8_t RFU)
{
    this->RFU = RFU;
}

uint8_t LoRaDatalinkFrame::getMajor() const
{
    return this->major;
}

void LoRaDatalinkFrame::setMajor(uint8_t major)
{
    this->major = major;
}

size_t LoRaDatalinkFrame::getPayloadArraySize() const
{
    return 19;
}

uint8_t LoRaDatalinkFrame::getPayload(size_t k) const
{
    if (k >= 19) throw omnetpp::cRuntimeError("Array of size 19 indexed by %lu", (unsigned long)k);
    return this->payload[k];
}

void LoRaDatalinkFrame::setPayload(size_t k, uint8_t payload)
{
    if (k >= 19) throw omnetpp::cRuntimeError("Array of size 19 indexed by %lu", (unsigned long)k);
    this->payload[k] = payload;
}

size_t LoRaDatalinkFrame::getMICArraySize() const
{
    return 4;
}

uint8_t LoRaDatalinkFrame::getMIC(size_t k) const
{
    if (k >= 4) throw omnetpp::cRuntimeError("Array of size 4 indexed by %lu", (unsigned long)k);
    return this->MIC[k];
}

void LoRaDatalinkFrame::setMIC(size_t k, uint8_t MIC)
{
    if (k >= 4) throw omnetpp::cRuntimeError("Array of size 4 indexed by %lu", (unsigned long)k);
    this->MIC[k] = MIC;
}

class LoRaDatalinkFrameDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
    enum FieldConstants {
        FIELD_FType,
        FIELD_RFU,
        FIELD_major,
        FIELD_payload,
        FIELD_MIC,
    };
  public:
    LoRaDatalinkFrameDescriptor();
    virtual ~LoRaDatalinkFrameDescriptor();

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

Register_ClassDescriptor(LoRaDatalinkFrameDescriptor)

LoRaDatalinkFrameDescriptor::LoRaDatalinkFrameDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(LoRaDatalinkFrame)), "omnetpp::cPacket")
{
    propertynames = nullptr;
}

LoRaDatalinkFrameDescriptor::~LoRaDatalinkFrameDescriptor()
{
    delete[] propertynames;
}

bool LoRaDatalinkFrameDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<LoRaDatalinkFrame *>(obj)!=nullptr;
}

const char **LoRaDatalinkFrameDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *LoRaDatalinkFrameDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int LoRaDatalinkFrameDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount() : 5;
}

unsigned int LoRaDatalinkFrameDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_FType
        FD_ISEDITABLE,    // FIELD_RFU
        FD_ISEDITABLE,    // FIELD_major
        FD_ISARRAY | FD_ISEDITABLE,    // FIELD_payload
        FD_ISARRAY | FD_ISEDITABLE,    // FIELD_MIC
    };
    return (field >= 0 && field < 5) ? fieldTypeFlags[field] : 0;
}

const char *LoRaDatalinkFrameDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "FType",
        "RFU",
        "major",
        "payload",
        "MIC",
    };
    return (field >= 0 && field < 5) ? fieldNames[field] : nullptr;
}

int LoRaDatalinkFrameDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0] == 'F' && strcmp(fieldName, "FType") == 0) return base+0;
    if (fieldName[0] == 'R' && strcmp(fieldName, "RFU") == 0) return base+1;
    if (fieldName[0] == 'm' && strcmp(fieldName, "major") == 0) return base+2;
    if (fieldName[0] == 'p' && strcmp(fieldName, "payload") == 0) return base+3;
    if (fieldName[0] == 'M' && strcmp(fieldName, "MIC") == 0) return base+4;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *LoRaDatalinkFrameDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "uint8_t",    // FIELD_FType
        "uint8_t",    // FIELD_RFU
        "uint8_t",    // FIELD_major
        "uint8_t",    // FIELD_payload
        "uint8_t",    // FIELD_MIC
    };
    return (field >= 0 && field < 5) ? fieldTypeStrings[field] : nullptr;
}

const char **LoRaDatalinkFrameDescriptor::getFieldPropertyNames(int field) const
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

const char *LoRaDatalinkFrameDescriptor::getFieldProperty(int field, const char *propertyname) const
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

int LoRaDatalinkFrameDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    LoRaDatalinkFrame *pp = (LoRaDatalinkFrame *)object; (void)pp;
    switch (field) {
        case FIELD_payload: return 19;
        case FIELD_MIC: return 4;
        default: return 0;
    }
}

void LoRaDatalinkFrameDescriptor::setFieldArraySize(void *object, int field, int size) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount()) {
            basedesc->setFieldArraySize(object, field, size);
            return;
        }
        field -= basedesc->getFieldCount();
    }
    LoRaDatalinkFrame *pp = (LoRaDatalinkFrame *)object; (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'LoRaDatalinkFrame'", field);
    }
}

const char *LoRaDatalinkFrameDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    LoRaDatalinkFrame *pp = (LoRaDatalinkFrame *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string LoRaDatalinkFrameDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    LoRaDatalinkFrame *pp = (LoRaDatalinkFrame *)object; (void)pp;
    switch (field) {
        case FIELD_FType: return ulong2string(pp->getFType());
        case FIELD_RFU: return ulong2string(pp->getRFU());
        case FIELD_major: return ulong2string(pp->getMajor());
        case FIELD_payload: return ulong2string(pp->getPayload(i));
        case FIELD_MIC: return ulong2string(pp->getMIC(i));
        default: return "";
    }
}

void LoRaDatalinkFrameDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount()) {
            basedesc->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= basedesc->getFieldCount();
    }
    LoRaDatalinkFrame *pp = (LoRaDatalinkFrame *)object; (void)pp;
    switch (field) {
        case FIELD_FType: pp->setFType(string2ulong(value)); break;
        case FIELD_RFU: pp->setRFU(string2ulong(value)); break;
        case FIELD_major: pp->setMajor(string2ulong(value)); break;
        case FIELD_payload: pp->setPayload(i,string2ulong(value)); break;
        case FIELD_MIC: pp->setMIC(i,string2ulong(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'LoRaDatalinkFrame'", field);
    }
}

const char *LoRaDatalinkFrameDescriptor::getFieldStructName(int field) const
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

void *LoRaDatalinkFrameDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    LoRaDatalinkFrame *pp = (LoRaDatalinkFrame *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

void LoRaDatalinkFrameDescriptor::setFieldStructValuePointer(void *object, int field, int i, void *ptr) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount()) {
            basedesc->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= basedesc->getFieldCount();
    }
    LoRaDatalinkFrame *pp = (LoRaDatalinkFrame *)object; (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'LoRaDatalinkFrame'", field);
    }
}

