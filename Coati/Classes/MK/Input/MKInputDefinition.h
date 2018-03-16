#ifndef MK_INPUTDEFINITION_H
#define MK_INPUTDEFINITION_H

// Include STL
#include <functional>

// Include MK
#include "MK/Common/MKMacros.h"
#include "MK/Common/MKPasskey.h"
#include "MKInputName.h"
#include "MKInput.h"

NS_MK_BEGIN

class MKInputDefinition
{
public:
    typedef std::function<void(MKPasskey<MKInputDefinition>, mkU64 _mask, MKInputName _inputName)> HandlerRegisterFunction;
    typedef std::function<void(MKPasskey<MKInputDefinition>, mkU64 _mask, MKInputName _inputName)> HandlerUnregisterFunction;

    static const mkU32 MAX_INPUTS_PER_DEFINITION = 3;

private:
    struct InputRegisterData
    {
        mkU64 m_Mask = 0;
        HandlerRegisterFunction m_RegisterFunction;
        HandlerUnregisterFunction m_UnregisterFunction;
    };

    const MKInputName m_InputName;
    InputRegisterData* m_InputRegisterData[MAX_INPUTS_PER_DEFINITION] = { nullptr, nullptr, nullptr };

public:
    MKInputDefinition(MKInputName _inputName)
        : m_InputName(_inputName) {}
    virtual ~MKInputDefinition()
    {
        for (mkU32 i = 0; i < MAX_INPUTS_PER_DEFINITION; ++i) { Unregister(i); }
    }

    MKInputName GetInputName() const { return m_InputName; }

    void Register(const HandlerRegisterFunction& _registerFunction, const HandlerUnregisterFunction& _unregisterFunction, mkU64 _mask, mkU32 _index);
    void Unregister(mkU32 _index);
};

NS_MK_END

#endif // MK_INPUTDEFINITION_H