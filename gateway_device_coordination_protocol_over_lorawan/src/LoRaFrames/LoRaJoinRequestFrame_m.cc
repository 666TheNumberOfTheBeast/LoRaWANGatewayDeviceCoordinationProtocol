//
// Generated file, do not edit! Created by nedtool 6.0 from src/LoRaFrames/LoRaJoinRequestFrame.msg.
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
#include "LoRaJoinRequestFrame_m.h"

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

Register_Class(LoRaJoinRequestFrame)

LoRaJoinRequestFrame::LoRaJoinRequestFrame(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

LoRaJoinRequestFrame::LoRaJoinRequestFrame(const LoRaJoinRequestFrame& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

LoRaJoinRequestFrame::~LoRaJoinRequestFrame()
{
}

LoRaJoinRequestFrame& LoRaJoinRequestFrame::operator=(const LoRaJoinRequestFrame& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void LoRaJoinRequestFrame::copy(const LoRaJoinRequestFrame& other)
{
    for (size_t i = 0; i < 8; i++) {
        this->joinEUI[i] = other.joinEUI[i];
    }
    for (size_t i = 0; i < 8; i++) {
        this->devEUI[i] = other.devEUI[i];
    }
    this->devNonce = other.devNonce;
}

void LoRaJoinRequestFrame::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimArrayPacking(b,this->joinEUI,8);
    doParsimArrayPacking(b,this->devEUI,8);
    doParsimPacking(b,this->devNonce);
}

void LoRaJoinRequestFrame::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimArrayUnpacking(b,this->joinEUI,8);
    doParsimArrayUnpacking(b,this->devEUI,8);
    doParsimUnpacking(b,this->devNonce);
}

size_t LoRaJoinRequestFrame::getJoinEUIArraySize() const
{
    return 8;
}

uint8_t LoRaJoinRequestFrame::getJoinEUI(size_t k) const
{
    if (k >= 8) throw omnetpp::cRuntimeError("Array of size 8 indexed by %lu", (unsigned long)k);
    return this->joinEUI[k];
}

void LoRaJoinRequestFrame::setJoinEUI(size_t k, uint8_t joinEUI)
{
    if (k >= 8) throw omnetpp::cRuntimeError("Array of size 8 indexed by %lu", (unsigned long)k);
    this->joinEUI[k] = joinEUI;
}

size_t LoRaJoinRequestFrame::getDevEUIArraySize() const
{
    return 8;
}

uint8_t LoRaJoinRequestFrame::getDevEUI(size_t k) const
{
    if (k >= 8) throw omnetpp::cRuntimeError("Array of size 8 indexed by %lu", (unsigned long)k);
    return this->devEUI[k];
}

void LoRaJoinRequestFrame::setDevEUI(size_t k, uint8_t devEUI)
{
    if (k >= 8) throw omnetpp::cRuntimeError("Array of size 8 indexed by %lu", (unsigned long)k);
    this->devEUI[k] = devEUI;
}

uint16_t LoRaJoinRequestFrame::getDevNonce() const
{
    return this->devNonce;
}

void LoRaJoinRequestFrame::setDevNonce(uint16_t devNonce)
{
    this->devNonce = devNonce;
}

class LoRaJoinRequestFrameDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
    enum FieldConstants {
        FIELD_joinEUI,
        FIELD_devEUI,
        FIELD_devNonce,
    };
  public:
    LoRaJoinRequestFrameDescriptor();
    virtual ~LoRaJoinRequestFrameDescriptor();

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

Register_ClassDescriptor(LoRaJoinRequestFrameDescriptor)

LoRaJoinRequestFrameDescriptor::LoRaJoinRequestFrameDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(LoRaJoinRequestFrame)), "omnetpp::cPacket")
{
    propertynames = nullptr;
}

LoRaJoinRequestFrameDescriptor::~LoRaJoinRequestFrameDescriptor()
{
    delete[] propertynames;
}

bool LoRaJoinRequestFrameDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<LoRaJoinRequestFrame *>(obj)!=nullptr;
}

const char **LoRaJoinRequestFrameDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *LoRaJoinRequestFrameDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int LoRaJoinRequestFrameDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount() : 3;
}

unsigned int LoRaJoinRequestFrameDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISARRAY | FD_ISEDITABLE,    // FIELD_joinEUI
        FD_ISARRAY | FD_ISEDITABLE,    // FIELD_devEUI
        FD_ISEDITABLE,    // FIELD_devNonce
    };
    return (field >= 0 && field < 3) ? fieldTypeFlags[field] : 0;
}

const char *LoRaJoinRequestFrameDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "joinEUI",
        "devEUI",
        "devNonce",
    };
    return (field >= 0 && field < 3) ? fieldNames[field] : nullptr;
}

int LoRaJoinRequestFrameDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0] == 'j' && strcmp(fieldName, "joinEUI") == 0) return base+0;
    if (fieldName[0] == 'd' && strcmp(fieldName, "devEUI") == 0) return base+1;
    if (fieldName[0] == 'd' && strcmp(fieldName, "devNonce") == 0) return base+2;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *LoRaJoinRequestFrameDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "uint8_t",    // FIELD_joinEUI
        "uint8_t",    // FIELD_devEUI
        "uint16_t",    // FIELD_devNonce
    };
    return (field >= 0 && field < 3) ? fieldTypeStrings[field] : nullptr;
}

const char **LoRaJoinRequestFrameDescriptor::getFieldPropertyNames(int field) const
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

const char *LoRaJoinRequestFrameDescriptor::getFieldProperty(int field, const char *propertyname) const
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

int LoRaJoinRequestFrameDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    LoRaJoinRequestFrame *pp = (LoRaJoinRequestFrame *)object; (void)pp;
    switch (field) {
        case FIELD_joinEUI: return 8;
        case FIELD_devEUI: return 8;
        default: return 0;
    }
}

void LoRaJoinRequestFrameDescriptor::setFieldArraySize(void *object, int field, int size) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount()) {
            basedesc->setFieldArraySize(object, field, size);
            return;
        }
        field -= basedesc->getFieldCount();
    }
    LoRaJoinRequestFrame *pp = (LoRaJoinRequestFrame *)object; (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'LoRaJoinRequestFrame'", field);
    }
}

const char *LoRaJoinRequestFrameDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    LoRaJoinRequestFrame *pp = (LoRaJoinRequestFrame *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string LoRaJoinRequestFrameDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    LoRaJoinRequestFrame *pp = (LoRaJoinRequestFrame *)object; (void)pp;
    switch (field) {
        case FIELD_joinEUI: return ulong2string(pp->getJoinEUI(i));
        case FIELD_devEUI: return ulong2string(pp->getDevEUI(i));
        case FIELD_devNonce: return ulong2string(pp->getDevNonce());
        default: return "";
    }
}

void LoRaJoinRequestFrameDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount()) {
            basedesc->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= basedesc->getFieldCount();
    }
    LoRaJoinRequestFrame *pp = (LoRaJoinRequestFrame *)object; (void)pp;
    switch (field) {
        case FIELD_joinEUI: pp->setJoinEUI(i,string2ulong(value)); break;
        case FIELD_devEUI: pp->setDevEUI(i,string2ulong(value)); break;
        case FIELD_devNonce: pp->setDevNonce(string2ulong(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'LoRaJoinRequestFrame'", field);
    }
}

const char *LoRaJoinRequestFrameDescriptor::getFieldStructName(int field) const
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

void *LoRaJoinRequestFrameDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    LoRaJoinRequestFrame *pp = (LoRaJoinRequestFrame *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

void LoRaJoinRequestFrameDescriptor::setFieldStructValuePointer(void *object, int field, int i, void *ptr) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount()) {
            basedesc->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= basedesc->getFieldCount();
    }
    LoRaJoinRequestFrame *pp = (LoRaJoinRequestFrame *)object; (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'LoRaJoinRequestFrame'", field);
    }
}

