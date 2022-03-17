//
// Generated file, do not edit! Created by nedtool 6.0 from src/TransportSegments/TCPSegment.msg.
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
#include "TCPSegment_m.h"

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

Register_Class(TCPSegment)

TCPSegment::TCPSegment(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

TCPSegment::TCPSegment(const TCPSegment& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

TCPSegment::~TCPSegment()
{
}

TCPSegment& TCPSegment::operator=(const TCPSegment& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void TCPSegment::copy(const TCPSegment& other)
{
    this->srcPort = other.srcPort;
    this->destPort = other.destPort;
    this->sequenceNumber = other.sequenceNumber;
    this->ACKnumber = other.ACKnumber;
    this->dataOffset = other.dataOffset;
    this->reserved = other.reserved;
    this->NS = other.NS;
    this->CWR = other.CWR;
    this->ECE = other.ECE;
    this->URG = other.URG;
    this->ACK = other.ACK;
    this->PSH = other.PSH;
    this->RST = other.RST;
    this->SYN = other.SYN;
    this->FIN = other.FIN;
    this->windowSize = other.windowSize;
    this->checksum = other.checksum;
    this->urgentPointer = other.urgentPointer;
    for (size_t i = 0; i < 40; i++) {
        this->options[i] = other.options[i];
    }
}

void TCPSegment::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->srcPort);
    doParsimPacking(b,this->destPort);
    doParsimPacking(b,this->sequenceNumber);
    doParsimPacking(b,this->ACKnumber);
    doParsimPacking(b,this->dataOffset);
    doParsimPacking(b,this->reserved);
    doParsimPacking(b,this->NS);
    doParsimPacking(b,this->CWR);
    doParsimPacking(b,this->ECE);
    doParsimPacking(b,this->URG);
    doParsimPacking(b,this->ACK);
    doParsimPacking(b,this->PSH);
    doParsimPacking(b,this->RST);
    doParsimPacking(b,this->SYN);
    doParsimPacking(b,this->FIN);
    doParsimPacking(b,this->windowSize);
    doParsimPacking(b,this->checksum);
    doParsimPacking(b,this->urgentPointer);
    doParsimArrayPacking(b,this->options,40);
}

void TCPSegment::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->srcPort);
    doParsimUnpacking(b,this->destPort);
    doParsimUnpacking(b,this->sequenceNumber);
    doParsimUnpacking(b,this->ACKnumber);
    doParsimUnpacking(b,this->dataOffset);
    doParsimUnpacking(b,this->reserved);
    doParsimUnpacking(b,this->NS);
    doParsimUnpacking(b,this->CWR);
    doParsimUnpacking(b,this->ECE);
    doParsimUnpacking(b,this->URG);
    doParsimUnpacking(b,this->ACK);
    doParsimUnpacking(b,this->PSH);
    doParsimUnpacking(b,this->RST);
    doParsimUnpacking(b,this->SYN);
    doParsimUnpacking(b,this->FIN);
    doParsimUnpacking(b,this->windowSize);
    doParsimUnpacking(b,this->checksum);
    doParsimUnpacking(b,this->urgentPointer);
    doParsimArrayUnpacking(b,this->options,40);
}

uint16_t TCPSegment::getSrcPort() const
{
    return this->srcPort;
}

void TCPSegment::setSrcPort(uint16_t srcPort)
{
    this->srcPort = srcPort;
}

uint16_t TCPSegment::getDestPort() const
{
    return this->destPort;
}

void TCPSegment::setDestPort(uint16_t destPort)
{
    this->destPort = destPort;
}

uint32_t TCPSegment::getSequenceNumber() const
{
    return this->sequenceNumber;
}

void TCPSegment::setSequenceNumber(uint32_t sequenceNumber)
{
    this->sequenceNumber = sequenceNumber;
}

uint32_t TCPSegment::getACKnumber() const
{
    return this->ACKnumber;
}

void TCPSegment::setACKnumber(uint32_t ACKnumber)
{
    this->ACKnumber = ACKnumber;
}

uint8_t TCPSegment::getDataOffset() const
{
    return this->dataOffset;
}

void TCPSegment::setDataOffset(uint8_t dataOffset)
{
    this->dataOffset = dataOffset;
}

uint8_t TCPSegment::getReserved() const
{
    return this->reserved;
}

void TCPSegment::setReserved(uint8_t reserved)
{
    this->reserved = reserved;
}

bool TCPSegment::getNS() const
{
    return this->NS;
}

void TCPSegment::setNS(bool NS)
{
    this->NS = NS;
}

bool TCPSegment::getCWR() const
{
    return this->CWR;
}

void TCPSegment::setCWR(bool CWR)
{
    this->CWR = CWR;
}

bool TCPSegment::getECE() const
{
    return this->ECE;
}

void TCPSegment::setECE(bool ECE)
{
    this->ECE = ECE;
}

bool TCPSegment::getURG() const
{
    return this->URG;
}

void TCPSegment::setURG(bool URG)
{
    this->URG = URG;
}

bool TCPSegment::getACK() const
{
    return this->ACK;
}

void TCPSegment::setACK(bool ACK)
{
    this->ACK = ACK;
}

bool TCPSegment::getPSH() const
{
    return this->PSH;
}

void TCPSegment::setPSH(bool PSH)
{
    this->PSH = PSH;
}

bool TCPSegment::getRST() const
{
    return this->RST;
}

void TCPSegment::setRST(bool RST)
{
    this->RST = RST;
}

bool TCPSegment::getSYN() const
{
    return this->SYN;
}

void TCPSegment::setSYN(bool SYN)
{
    this->SYN = SYN;
}

bool TCPSegment::getFIN() const
{
    return this->FIN;
}

void TCPSegment::setFIN(bool FIN)
{
    this->FIN = FIN;
}

uint16_t TCPSegment::getWindowSize() const
{
    return this->windowSize;
}

void TCPSegment::setWindowSize(uint16_t windowSize)
{
    this->windowSize = windowSize;
}

uint16_t TCPSegment::getChecksum() const
{
    return this->checksum;
}

void TCPSegment::setChecksum(uint16_t checksum)
{
    this->checksum = checksum;
}

uint16_t TCPSegment::getUrgentPointer() const
{
    return this->urgentPointer;
}

void TCPSegment::setUrgentPointer(uint16_t urgentPointer)
{
    this->urgentPointer = urgentPointer;
}

size_t TCPSegment::getOptionsArraySize() const
{
    return 40;
}

uint8_t TCPSegment::getOptions(size_t k) const
{
    if (k >= 40) throw omnetpp::cRuntimeError("Array of size 40 indexed by %lu", (unsigned long)k);
    return this->options[k];
}

void TCPSegment::setOptions(size_t k, uint8_t options)
{
    if (k >= 40) throw omnetpp::cRuntimeError("Array of size 40 indexed by %lu", (unsigned long)k);
    this->options[k] = options;
}

class TCPSegmentDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
    enum FieldConstants {
        FIELD_srcPort,
        FIELD_destPort,
        FIELD_sequenceNumber,
        FIELD_ACKnumber,
        FIELD_dataOffset,
        FIELD_reserved,
        FIELD_NS,
        FIELD_CWR,
        FIELD_ECE,
        FIELD_URG,
        FIELD_ACK,
        FIELD_PSH,
        FIELD_RST,
        FIELD_SYN,
        FIELD_FIN,
        FIELD_windowSize,
        FIELD_checksum,
        FIELD_urgentPointer,
        FIELD_options,
    };
  public:
    TCPSegmentDescriptor();
    virtual ~TCPSegmentDescriptor();

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

Register_ClassDescriptor(TCPSegmentDescriptor)

TCPSegmentDescriptor::TCPSegmentDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(TCPSegment)), "omnetpp::cPacket")
{
    propertynames = nullptr;
}

TCPSegmentDescriptor::~TCPSegmentDescriptor()
{
    delete[] propertynames;
}

bool TCPSegmentDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<TCPSegment *>(obj)!=nullptr;
}

const char **TCPSegmentDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *TCPSegmentDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int TCPSegmentDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 19+basedesc->getFieldCount() : 19;
}

unsigned int TCPSegmentDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_srcPort
        FD_ISEDITABLE,    // FIELD_destPort
        FD_ISEDITABLE,    // FIELD_sequenceNumber
        FD_ISEDITABLE,    // FIELD_ACKnumber
        FD_ISEDITABLE,    // FIELD_dataOffset
        FD_ISEDITABLE,    // FIELD_reserved
        FD_ISEDITABLE,    // FIELD_NS
        FD_ISEDITABLE,    // FIELD_CWR
        FD_ISEDITABLE,    // FIELD_ECE
        FD_ISEDITABLE,    // FIELD_URG
        FD_ISEDITABLE,    // FIELD_ACK
        FD_ISEDITABLE,    // FIELD_PSH
        FD_ISEDITABLE,    // FIELD_RST
        FD_ISEDITABLE,    // FIELD_SYN
        FD_ISEDITABLE,    // FIELD_FIN
        FD_ISEDITABLE,    // FIELD_windowSize
        FD_ISEDITABLE,    // FIELD_checksum
        FD_ISEDITABLE,    // FIELD_urgentPointer
        FD_ISARRAY | FD_ISEDITABLE,    // FIELD_options
    };
    return (field >= 0 && field < 19) ? fieldTypeFlags[field] : 0;
}

const char *TCPSegmentDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "srcPort",
        "destPort",
        "sequenceNumber",
        "ACKnumber",
        "dataOffset",
        "reserved",
        "NS",
        "CWR",
        "ECE",
        "URG",
        "ACK",
        "PSH",
        "RST",
        "SYN",
        "FIN",
        "windowSize",
        "checksum",
        "urgentPointer",
        "options",
    };
    return (field >= 0 && field < 19) ? fieldNames[field] : nullptr;
}

int TCPSegmentDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0] == 's' && strcmp(fieldName, "srcPort") == 0) return base+0;
    if (fieldName[0] == 'd' && strcmp(fieldName, "destPort") == 0) return base+1;
    if (fieldName[0] == 's' && strcmp(fieldName, "sequenceNumber") == 0) return base+2;
    if (fieldName[0] == 'A' && strcmp(fieldName, "ACKnumber") == 0) return base+3;
    if (fieldName[0] == 'd' && strcmp(fieldName, "dataOffset") == 0) return base+4;
    if (fieldName[0] == 'r' && strcmp(fieldName, "reserved") == 0) return base+5;
    if (fieldName[0] == 'N' && strcmp(fieldName, "NS") == 0) return base+6;
    if (fieldName[0] == 'C' && strcmp(fieldName, "CWR") == 0) return base+7;
    if (fieldName[0] == 'E' && strcmp(fieldName, "ECE") == 0) return base+8;
    if (fieldName[0] == 'U' && strcmp(fieldName, "URG") == 0) return base+9;
    if (fieldName[0] == 'A' && strcmp(fieldName, "ACK") == 0) return base+10;
    if (fieldName[0] == 'P' && strcmp(fieldName, "PSH") == 0) return base+11;
    if (fieldName[0] == 'R' && strcmp(fieldName, "RST") == 0) return base+12;
    if (fieldName[0] == 'S' && strcmp(fieldName, "SYN") == 0) return base+13;
    if (fieldName[0] == 'F' && strcmp(fieldName, "FIN") == 0) return base+14;
    if (fieldName[0] == 'w' && strcmp(fieldName, "windowSize") == 0) return base+15;
    if (fieldName[0] == 'c' && strcmp(fieldName, "checksum") == 0) return base+16;
    if (fieldName[0] == 'u' && strcmp(fieldName, "urgentPointer") == 0) return base+17;
    if (fieldName[0] == 'o' && strcmp(fieldName, "options") == 0) return base+18;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *TCPSegmentDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "uint16_t",    // FIELD_srcPort
        "uint16_t",    // FIELD_destPort
        "uint32_t",    // FIELD_sequenceNumber
        "uint32_t",    // FIELD_ACKnumber
        "uint8_t",    // FIELD_dataOffset
        "uint8_t",    // FIELD_reserved
        "bool",    // FIELD_NS
        "bool",    // FIELD_CWR
        "bool",    // FIELD_ECE
        "bool",    // FIELD_URG
        "bool",    // FIELD_ACK
        "bool",    // FIELD_PSH
        "bool",    // FIELD_RST
        "bool",    // FIELD_SYN
        "bool",    // FIELD_FIN
        "uint16_t",    // FIELD_windowSize
        "uint16_t",    // FIELD_checksum
        "uint16_t",    // FIELD_urgentPointer
        "uint8_t",    // FIELD_options
    };
    return (field >= 0 && field < 19) ? fieldTypeStrings[field] : nullptr;
}

const char **TCPSegmentDescriptor::getFieldPropertyNames(int field) const
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

const char *TCPSegmentDescriptor::getFieldProperty(int field, const char *propertyname) const
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

int TCPSegmentDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    TCPSegment *pp = (TCPSegment *)object; (void)pp;
    switch (field) {
        case FIELD_options: return 40;
        default: return 0;
    }
}

void TCPSegmentDescriptor::setFieldArraySize(void *object, int field, int size) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount()) {
            basedesc->setFieldArraySize(object, field, size);
            return;
        }
        field -= basedesc->getFieldCount();
    }
    TCPSegment *pp = (TCPSegment *)object; (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'TCPSegment'", field);
    }
}

const char *TCPSegmentDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    TCPSegment *pp = (TCPSegment *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string TCPSegmentDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    TCPSegment *pp = (TCPSegment *)object; (void)pp;
    switch (field) {
        case FIELD_srcPort: return ulong2string(pp->getSrcPort());
        case FIELD_destPort: return ulong2string(pp->getDestPort());
        case FIELD_sequenceNumber: return ulong2string(pp->getSequenceNumber());
        case FIELD_ACKnumber: return ulong2string(pp->getACKnumber());
        case FIELD_dataOffset: return ulong2string(pp->getDataOffset());
        case FIELD_reserved: return ulong2string(pp->getReserved());
        case FIELD_NS: return bool2string(pp->getNS());
        case FIELD_CWR: return bool2string(pp->getCWR());
        case FIELD_ECE: return bool2string(pp->getECE());
        case FIELD_URG: return bool2string(pp->getURG());
        case FIELD_ACK: return bool2string(pp->getACK());
        case FIELD_PSH: return bool2string(pp->getPSH());
        case FIELD_RST: return bool2string(pp->getRST());
        case FIELD_SYN: return bool2string(pp->getSYN());
        case FIELD_FIN: return bool2string(pp->getFIN());
        case FIELD_windowSize: return ulong2string(pp->getWindowSize());
        case FIELD_checksum: return ulong2string(pp->getChecksum());
        case FIELD_urgentPointer: return ulong2string(pp->getUrgentPointer());
        case FIELD_options: return ulong2string(pp->getOptions(i));
        default: return "";
    }
}

void TCPSegmentDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount()) {
            basedesc->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= basedesc->getFieldCount();
    }
    TCPSegment *pp = (TCPSegment *)object; (void)pp;
    switch (field) {
        case FIELD_srcPort: pp->setSrcPort(string2ulong(value)); break;
        case FIELD_destPort: pp->setDestPort(string2ulong(value)); break;
        case FIELD_sequenceNumber: pp->setSequenceNumber(string2ulong(value)); break;
        case FIELD_ACKnumber: pp->setACKnumber(string2ulong(value)); break;
        case FIELD_dataOffset: pp->setDataOffset(string2ulong(value)); break;
        case FIELD_reserved: pp->setReserved(string2ulong(value)); break;
        case FIELD_NS: pp->setNS(string2bool(value)); break;
        case FIELD_CWR: pp->setCWR(string2bool(value)); break;
        case FIELD_ECE: pp->setECE(string2bool(value)); break;
        case FIELD_URG: pp->setURG(string2bool(value)); break;
        case FIELD_ACK: pp->setACK(string2bool(value)); break;
        case FIELD_PSH: pp->setPSH(string2bool(value)); break;
        case FIELD_RST: pp->setRST(string2bool(value)); break;
        case FIELD_SYN: pp->setSYN(string2bool(value)); break;
        case FIELD_FIN: pp->setFIN(string2bool(value)); break;
        case FIELD_windowSize: pp->setWindowSize(string2ulong(value)); break;
        case FIELD_checksum: pp->setChecksum(string2ulong(value)); break;
        case FIELD_urgentPointer: pp->setUrgentPointer(string2ulong(value)); break;
        case FIELD_options: pp->setOptions(i,string2ulong(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'TCPSegment'", field);
    }
}

const char *TCPSegmentDescriptor::getFieldStructName(int field) const
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

void *TCPSegmentDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    TCPSegment *pp = (TCPSegment *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

void TCPSegmentDescriptor::setFieldStructValuePointer(void *object, int field, int i, void *ptr) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount()) {
            basedesc->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= basedesc->getFieldCount();
    }
    TCPSegment *pp = (TCPSegment *)object; (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'TCPSegment'", field);
    }
}

