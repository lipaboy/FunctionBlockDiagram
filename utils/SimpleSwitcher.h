#ifndef SIMPLESWITCHER_H
#define SIMPLESWITCHER_H

#include <optional>
#include <functional>

template < class T >
class SimpleSwitcher
{
public:
    using TriggerType = std::function< void ( T, bool ) >;

public:
    void turnOn( T newVal )
    {
        if ( m_var.has_value() )
        {
            runTrigger( false );
        }
        m_var = newVal;
        runTrigger( true );
    }

    void turnOff()
    {
        if ( m_var.has_value() )
        {
            runTrigger( false );
        }
        m_var.reset();
    }

    void setSwitchFunc( TriggerType switchFunc )
    {
        m_trigger = switchFunc;
    }

    bool isOn() const { return m_var.has_value(); }
    T value() { return m_var.value(); }

private:
    void runTrigger( bool isOn )
    {
        if ( m_trigger != nullptr )
        {
            m_trigger( m_var.value(), isOn );
        }
    }

private:
    std::optional< T > m_var{ std::nullopt };
    TriggerType m_trigger{};
};

#endif // SIMPLESWITCHER_H
