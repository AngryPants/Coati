#ifndef MK_ENUM_MACROS_H
#define MK_ENUM_MACROS_H

// Include MK
#include "MKMacros.h"
#include "MKMacros2.h"

// Include STL
#include <unordered_map>

NS_MK_BEGIN

class MKEnumValue
{
private:
    // Variable(s)
    const mkS64 m_Value;
    const mkString& m_Name;

    // Do not allow the use of these operator(s)
    mkBool operator=(const MKEnumValue& _other) {}

public:
    // Constructor(s) & Destructor
    MKEnumValue(const mkString& _name, mkS64 _value)
     : m_Name(_name), m_Value(_value)
    {}
    virtual ~MKEnumValue() {}

    // Interface Function(s)
    mkS64 getValue() const { return m_Value; }
    const mkString& getName() const { return m_Name; }

    // Operator Overload(s)
    mkBool operator==(mkS64 _value) const { return m_Value == _value; }
    mkBool operator==(const MKEnumValue& _other) const { return (*this) == _other.m_Value; }
    friend mkBool operator==(mkS64 _value, const MKEnumValue& _enum) { return _value == _enum.m_Value; }

    mkS64 operator&(mkS64 _value) const { return m_Value & _value; }
    mkS64 operator&(const MKEnumValue& _other) const { return (*this) & _other.m_Value; }
    friend mkS64 operator&(mkS64 _value, const MKEnumValue& _enum) { return _value & _enum.m_Value; }

    mkBool operator&&(mkS64 _value) const { return (m_Value != 0) && (_value != 0); }
    mkBool operator&&(const MKEnumValue& _other) const { return (*this) && _other.m_Value; }
    friend mkBool operator&&(mkS64 _value, const MKEnumValue& _enum) { return (_value != 0) && (_enum.m_Value != 0); }

    mkS64 operator|(mkS64 _value) const { return m_Value | _value; }
    mkS64 operator|(const MKEnumValue& _other) const { return (*this) | _other.m_Value; }
    friend mkS64 operator|(mkS64 _value, const MKEnumValue& _enum) { return _value | _enum.m_Value; }

    mkS64 operator||(mkS64 _value) const { return (m_Value != 0) || (_value != 0); }
    mkS64 operator||(const MKEnumValue& _other) const { return (*this) || _other.m_Value; }
    friend mkS64 operator||(mkS64 _value, const MKEnumValue& _enum) { return (_enum.m_Value != 0) || (_value != 0); }

    mkS64 operator*(mkS64 _value) const { return m_Value * _value; }
    mkS64 operator*(const MKEnumValue& _other) const { return (*this) * _other.m_Value; }
    friend mkS64 operator*(mkS64 _value, const MKEnumValue& _enum) { return _value * _enum.m_Value; }

    mkS64 operator/(mkS64 _value) const { return m_Value / _value; }
    mkS64 operator/(const MKEnumValue& _other) const { return (*this) / _other.m_Value; }
    friend mkS64 operator/(mkS64 _value, const MKEnumValue& _enum) { return _value / _enum.m_Value; }

    mkS64 operator+(mkS64 _value) const { return m_Value + _value; }
    mkS64 operator+(const MKEnumValue& _other) const { return (*this) + _other.m_Value; }
    friend mkS64 operator+(mkS64 _value, const MKEnumValue& _enum) { return _value + _enum.m_Value; }

    mkS64 operator-(mkS64 _value) const { return m_Value - _value; }
    mkS64 operator-(const MKEnumValue& _other) const { return (*this) - _other.m_Value; }
    friend mkS64 operator-(mkS64 _value, const MKEnumValue& _enum) { return _value - _enum.m_Value; }
};

#define MK_ENUM_VALUE(__NAME__, __VALUE__) \
    std::pair<mkString, mkS64>(__NAME__, __VALUE__);

#define MK_ENUM(__NAME__, ...) \
class __NAME__ \
{ \
public: \
    static const mkU64 NUM_VALUES = MK_GET_ELEMENT_COUNT(__VA_ARGS__); \
    MK_FOR_EACH(, __VA_ARGS__);
\
}; \

NS_MK_END

#endif // MK_ENUM_MACROS_H