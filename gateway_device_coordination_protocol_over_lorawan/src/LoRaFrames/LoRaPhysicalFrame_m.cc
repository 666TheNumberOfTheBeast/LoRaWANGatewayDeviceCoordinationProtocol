//
// Generated file, do not edit! Created by nedtool 6.0 from src/LoRaFrames/LoRaPhysicalFrame.msg.
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
#include "LoRaPhysicalFrame_m.h"

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

Register_Class(LoRaPhysicalFrame)

LoRaPhysicalFrame::LoRaPhysicalFrame(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

LoRaPhysicalFrame::LoRaPhysicalFrame(const LoRaPhysicalFrame& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

LoRaPhysicalFrame::~LoRaPhysicalFrame()
{
}

LoRaPhysicalFrame& LoRaPhysicalFrame::operator=(const LoRaPhysicalFrame& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void LoRaPhysicalFrame::copy(const LoRaPhysicalFrame& other)
{
    this->preamble = other.preamble;
    this->syncWord = other.syncWord;
    this->payloadLength = other.payloadLength;
    this->FECRate = other.FECRate;
    this->CRCPresence = other.CRCPresence;
    this->headerCRC = other.headerCRC;
    this->transmissionPower = other.transmissionPower;
    this->channelFrequency = other.channelFrequency;
    this->codingRate = other.codingRate;
    this->spreadingFactor = other.spreadingFactor;
    this->bandwidth = other.bandwidth;
    this->IQPolarizationInverted = other.IQPolarizationInverted;
    for (size_t i = 0; i < 59; i++) {
        this->payload[i] = other.payload[i];
    }
    this->payloadCRC = other.payloadCRC;
}

void LoRaPhysicalFrame::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->preamble);
    doParsimPacking(b,this->syncWord);
    doParsimPacking(b,this->payloadLength);
    doParsimPacking(b,this->FECRate);
    doParsimPacking(b,this->CRCPresence);
    doParsimPacking(b,this->headerCRC);
    doParsimPacking(b,this->transmissionPower);
    doParsimPacking(b,this->channelFrequency);
    doParsimPacking(b,this->codingRate);
    doParsimPacking(b,this->spreadingFactor);
    doParsimPacking(b,this->bandwidth);
    doParsimPacking(b,this->IQPolarizationInverted);
    doParsimArrayPacking(b,this->payload,59);
    doParsimPacking(b,this->payloadCRC);
}

void LoRaPhysicalFrame::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->preamble);
    doParsimUnpacking(b,this->syncWord);
    doParsimUnpacking(b,this->payloadLength);
    doParsimUnpacking(b,this->FECRate);
    doParsimUnpacking(b,this->CRCPresence);
    doParsimUnpacking(b,this->headerCRC);
    doParsimUnpacking(b,this->transmissionPower);
    doParsimUnpacking(b,this->channelFrequency);
    doParsimUnpacking(b,this->codingRate);
    doParsimUnpacking(b,this->spreadingFactor);
    doParsimUnpacking(b,this->bandwidth);
    doParsimUnpacking(b,this->IQPolarizationInverted);
    doParsimArrayUnpacking(b,this->payload,59);
    doParsimUnpacking(b,this->payloadCRC);
}

uint32_t LoRaPhysicalFrame::getPreamble() const
{
    return this->preamble;
}

void LoRaPhysicalFrame::setPreamble(uint32_t preamble)
{
    this->preamble = preamble;
}

uint16_t LoRaPhysicalFrame::getSyncWord() const
{
    return this->syncWord;
}

void LoRaPhysicalFrame::setSyncWord(uint16_t syncWord)
{
    this->syncWord = syncWord;
}

uint8_t LoRaPhysicalFrame::getPayloadLength() const
{
    return this->payloadLength;
}

void LoRaPhysicalFrame::setPayloadLength(uint8_t payloadLength)
{
    this->payloadLength = payloadLength;
}

uint8_t LoRaPhysicalFrame::getFECRate() const
{
    return this->FECRate;
}

void LoRaPhysicalFrame::setFECRate(uint8_t FECRate)
{
    this->FECRate = FECRate;
}

bool LoRaPhysicalFrame::getCRCPresence() const
{
    return this->CRCPresence;
}

void LoRaPhysicalFrame::setCRCPresence(bool CRCPresence)
{
    this->CRCPresence = CRCPresence;
}

uint16_t LoRaPhysicalFrame::getHeaderCRC() const
{
    return this->headerCRC;
}

void LoRaPhysicalFrame::setHeaderCRC(uint16_t headerCRC)
{
    this->headerCRC = headerCRC;
}

uint8_t LoRaPhysicalFrame::getTransmissionPower() const
{
    return this->transmissionPower;
}

void LoRaPhysicalFrame::setTransmissionPower(uint8_t transmissionPower)
{
    this->transmissionPower = transmissionPower;
}

float LoRaPhysicalFrame::getChannelFrequency() const
{
    return this->channelFrequency;
}

void LoRaPhysicalFrame::setChannelFrequency(float channelFrequency)
{
    this->channelFrequency = channelFrequency;
}

uint8_t LoRaPhysicalFrame::getCodingRate() const
{
    return this->codingRate;
}

void LoRaPhysicalFrame::setCodingRate(uint8_t codingRate)
{
    this->codingRate = codingRate;
}

uint8_t LoRaPhysicalFrame::getSpreadingFactor() const
{
    return this->spreadingFactor;
}

void LoRaPhysicalFrame::setSpreadingFactor(uint8_t spreadingFactor)
{
    this->spreadingFactor = spreadingFactor;
}

float LoRaPhysicalFrame::getBandwidth() const
{
    return this->bandwidth;
}

void LoRaPhysicalFrame::setBandwidth(float bandwidth)
{
    this->bandwidth = bandwidth;
}

bool LoRaPhysicalFrame::getIQPolarizationInverted() const
{
    return this->IQPolarizationInverted;
}

void LoRaPhysicalFrame::setIQPolarizationInverted(bool IQPolarizationInverted)
{
    this->IQPolarizationInverted = IQPolarizationInverted;
}

size_t LoRaPhysicalFrame::getPayloadArraySize() const
{
    return 59;
}

uint8_t LoRaPhysicalFrame::getPayload(size_t k) const
{
    if (k >= 59) throw omnetpp::cRuntimeError("Array of size 59 indexed by %lu", (unsigned long)k);
    return this->payload[k];
}

void LoRaPhysicalFrame::setPayload(size_t k, uint8_t payload)
{
    if (k >= 59) throw omnetpp::cRuntimeError("Array of size 59 indexed by %lu", (unsigned long)k);
    this->payload[k] = payload;
}

uint16_t LoRaPhysicalFrame::getPayloadCRC() const
{
    return this->payloadCRC;
}

void LoRaPhysicalFrame::setPayloadCRC(uint16_t payloadCRC)
{
    this->payloadCRC = payloadCRC;
}

class LoRaPhysicalFrameDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
    enum FieldConstants {
        FIELD_preamble,
        FIELD_syncWord,
        FIELD_payloadLength,
        FIELD_FECRate,
        FIELD_CRCPresence,
        FIELD_headerCRC,
        FIELD_transmissionPower,
        FIELD_channelFrequency,
        FIELD_codingRate,
        FIELD_spreadingFactor,
        FIELD_bandwidth,
        FIELD_IQPolarizationInverted,
        FIELD_payload,
        FIELD_payloadCRC,
    };
  public:
    LoRaPhysicalFrameDescriptor();
    virtual ~LoRaPhysicalFrameDescriptor();

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

Register_ClassDescriptor(LoRaPhysicalFrameDescriptor)

LoRaPhysicalFrameDescriptor::LoRaPhysicalFrameDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(LoRaPhysicalFrame)), "omnetpp::cPacket")
{
    propertynames = nullptr;
}

LoRaPhysicalFrameDescriptor::~LoRaPhysicalFrameDescriptor()
{
    delete[] propertynames;
}

bool LoRaPhysicalFrameDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<LoRaPhysicalFrame *>(obj)!=nullptr;
}

const char **LoRaPhysicalFrameDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *LoRaPhysicalFrameDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int LoRaPhysicalFrameDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 14+basedesc->getFieldCount() : 14;
}

unsigned int LoRaPhysicalFrameDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_preamble
        FD_ISEDITABLE,    // FIELD_syncWord
        FD_ISEDITABLE,    // FIELD_payloadLength
        FD_ISEDITABLE,    // FIELD_FECRate
        FD_ISEDITABLE,    // FIELD_CRCPresence
        FD_ISEDITABLE,    // FIELD_headerCRC
        FD_ISEDITABLE,    // FIELD_transmissionPower
        FD_ISEDITABLE,    // FIELD_channelFrequency
        FD_ISEDITABLE,    // FIELD_codingRate
        FD_ISEDITABLE,    // FIELD_spreadingFactor
        FD_ISEDITABLE,    // FIELD_bandwidth
        FD_ISEDITABLE,    // FIELD_IQPolarizationInverted
        FD_ISARRAY | FD_ISEDITABLE,    // FIELD_payload
        FD_ISEDITABLE,    // FIELD_payloadCRC
    };
    return (field >= 0 && field < 14) ? fieldTypeFlags[field] : 0;
}

const char *LoRaPhysicalFrameDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "preamble",
        "syncWord",
        "payloadLength",
        "FECRate",
        "CRCPresence",
        "headerCRC",
        "transmissionPower",
        "channelFrequency",
        "codingRate",
        "spreadingFactor",
        "bandwidth",
        "IQPolarizationInverted",
        "payload",
        "payloadCRC",
    };
    return (field >= 0 && field < 14) ? fieldNames[field] : nullptr;
}

int LoRaPhysicalFrameDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0] == 'p' && strcmp(fieldName, "preamble") == 0) return base+0;
    if (fieldName[0] == 's' && strcmp(fieldName, "syncWord") == 0) return base+1;
    if (fieldName[0] == 'p' && strcmp(fieldName, "payloadLength") == 0) return base+2;
    if (fieldName[0] == 'F' && strcmp(fieldName, "FECRate") == 0) return base+3;
    if (fieldName[0] == 'C' && strcmp(fieldName, "CRCPresence") == 0) return base+4;
    if (fieldName[0] == 'h' && strcmp(fieldName, "headerCRC") == 0) return base+5;
    if (fieldName[0] == 't' && strcmp(fieldName, "transmissionPower") == 0) return base+6;
    if (fieldName[0] == 'c' && strcmp(fieldName, "channelFrequency") == 0) return base+7;
    if (fieldName[0] == 'c' && strcmp(fieldName, "codingRate") == 0) return base+8;
    if (fieldName[0] == 's' && strcmp(fieldName, "spreadingFactor") == 0) return base+9;
    if (fieldName[0] == 'b' && strcmp(fieldName, "bandwidth") == 0) return base+10;
    if (fieldName[0] == 'I' && strcmp(fieldName, "IQPolarizationInverted") == 0) return base+11;
    if (fieldName[0] == 'p' && strcmp(fieldName, "payload") == 0) return base+12;
    if (fieldName[0] == 'p' && strcmp(fieldName, "payloadCRC") == 0) return base+13;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *LoRaPhysicalFrameDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "uint32_t",    // FIELD_preamble
        "uint16_t",    // FIELD_syncWord
        "uint8_t",    // FIELD_payloadLength
        "uint8_t",    // FIELD_FECRate
        "bool",    // FIELD_CRCPresence
        "uint16_t",    // FIELD_headerCRC
        "uint8_t",    // FIELD_transmissionPower
        "float",    // FIELD_channelFrequency
        "uint8_t",    // FIELD_codingRate
        "uint8_t",    // FIELD_spreadingFactor
        "float",    // FIELD_bandwidth
        "bool",    // FIELD_IQPolarizationInverted
        "uint8_t",    // FIELD_payload
        "uint16_t",    // FIELD_payloadCRC
    };
    return (field >= 0 && field < 14) ? fieldTypeStrings[field] : nullptr;
}

const char **LoRaPhysicalFrameDescriptor::getFieldPropertyNames(int field) const
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

const char *LoRaPhysicalFrameDescriptor::getFieldProperty(int field, const char *propertyname) const
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

int LoRaPhysicalFrameDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    LoRaPhysicalFrame *pp = (LoRaPhysicalFrame *)object; (void)pp;
    switch (field) {
        case FIELD_payload: return 59;
        default: return 0;
    }
}

void LoRaPhysicalFrameDescriptor::setFieldArraySize(void *object, int field, int size) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount()) {
            basedesc->setFieldArraySize(object, field, size);
            return;
        }
        field -= basedesc->getFieldCount();
    }
    LoRaPhysicalFrame *pp = (LoRaPhysicalFrame *)object; (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'LoRaPhysicalFrame'", field);
    }
}

const char *LoRaPhysicalFrameDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    LoRaPhysicalFrame *pp = (LoRaPhysicalFrame *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string LoRaPhysicalFrameDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    LoRaPhysicalFrame *pp = (LoRaPhysicalFrame *)object; (void)pp;
    switch (field) {
        case FIELD_preamble: return ulong2string(pp->getPreamble());
        case FIELD_syncWord: return ulong2string(pp->getSyncWord());
        case FIELD_payloadLength: return ulong2string(pp->getPayloadLength());
        case FIELD_FECRate: return ulong2string(pp->getFECRate());
        case FIELD_CRCPresence: return bool2string(pp->getCRCPresence());
        case FIELD_headerCRC: return ulong2string(pp->getHeaderCRC());
        case FIELD_transmissionPower: return ulong2string(pp->getTransmissionPower());
        case FIELD_channelFrequency: return double2string(pp->getChannelFrequency());
        case FIELD_codingRate: return ulong2string(pp->getCodingRate());
        case FIELD_spreadingFactor: return ulong2string(pp->getSpreadingFactor());
        case FIELD_bandwidth: return double2string(pp->getBandwidth());
        case FIELD_IQPolarizationInverted: return bool2string(pp->getIQPolarizationInverted());
        case FIELD_payload: return ulong2string(pp->getPayload(i));
        case FIELD_payloadCRC: return ulong2string(pp->getPayloadCRC());
        default: return "";
    }
}

void LoRaPhysicalFrameDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount()) {
            basedesc->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= basedesc->getFieldCount();
    }
    LoRaPhysicalFrame *pp = (LoRaPhysicalFrame *)object; (void)pp;
    switch (field) {
        case FIELD_preamble: pp->setPreamble(string2ulong(value)); break;
        case FIELD_syncWord: pp->setSyncWord(string2ulong(value)); break;
        case FIELD_payloadLength: pp->setPayloadLength(string2ulong(value)); break;
        case FIELD_FECRate: pp->setFECRate(string2ulong(value)); break;
        case FIELD_CRCPresence: pp->setCRCPresence(string2bool(value)); break;
        case FIELD_headerCRC: pp->setHeaderCRC(string2ulong(value)); break;
        case FIELD_transmissionPower: pp->setTransmissionPower(string2ulong(value)); break;
        case FIELD_channelFrequency: pp->setChannelFrequency(string2double(value)); break;
        case FIELD_codingRate: pp->setCodingRate(string2ulong(value)); break;
        case FIELD_spreadingFactor: pp->setSpreadingFactor(string2ulong(value)); break;
        case FIELD_bandwidth: pp->setBandwidth(string2double(value)); break;
        case FIELD_IQPolarizationInverted: pp->setIQPolarizationInverted(string2bool(value)); break;
        case FIELD_payload: pp->setPayload(i,string2ulong(value)); break;
        case FIELD_payloadCRC: pp->setPayloadCRC(string2ulong(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'LoRaPhysicalFrame'", field);
    }
}

const char *LoRaPhysicalFrameDescriptor::getFieldStructName(int field) const
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

void *LoRaPhysicalFrameDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    LoRaPhysicalFrame *pp = (LoRaPhysicalFrame *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

void LoRaPhysicalFrameDescriptor::setFieldStructValuePointer(void *object, int field, int i, void *ptr) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount()) {
            basedesc->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= basedesc->getFieldCount();
    }
    LoRaPhysicalFrame *pp = (LoRaPhysicalFrame *)object; (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'LoRaPhysicalFrame'", field);
    }
}

