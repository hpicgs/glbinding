
#pragma once


#include <string>
#include <set>
#include <vector>

#include <glesbinding/glesbinding_api.h>

#include <glesbinding/ProcAddress.h>
#include <glesbinding/FunctionCall.h>
#include <glesbinding/CallbackMask.h>


namespace glesbinding 
{


/**
*  @brief
*    The AbstractFunction represents an OpenGL API function by its name
*    and entry point after dynamic address resolution.
*/
class GLESBINDING_API AbstractFunction
{
    friend class Binding; ///< For state configuration


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] name
    *    The actual exported OpenGL API function name, including the 'gl' prefix
    */
    AbstractFunction(const char * name);

    /**
    *  @brief
    *    Destructor to guarantee correct memory deallocation of subclasses
    */
    virtual ~AbstractFunction();

    /**
    *  @brief
    *    Get function name
    *
    *  @return
    *    The function name
    */
    const char * name() const;

    /**
    *  @brief
    *    Lookup the function pointer and stores it in the current state
    */
    void resolveAddress();

    /**
    *  @brief
    *    Check for a valid function pointer in the current state
    *
    *  @return
    *    'true' if a valid function pointer is stored in the current state, else 'false'
    */
    bool isResolved() const;

    /**
    *  @brief
    *    Get function pointer
    *
    *  @return
    *    The function pointer
    */
    ProcAddress address() const;

    /**
    *  @brief
    *    Get callback mask
    *
    *  @return
    *    The currently configured callback mask for the current state
    */
    CallbackMask callbackMask() const;

    /**
    *  @brief
    *    Reconfigures the callback mask for the current state
    *
    *  @param[in] mask
    *    The new callback mask
    */
    void setCallbackMask(CallbackMask mask);

    /**
    *  @brief
    *    Reconfigures the callback mask for the current state in means of a bit-wise 'or' operation with the current callback mask
    *
    *  @param[in] mask
    *    The callback mask to include
    */
    void addCallbackMask(CallbackMask mask);

    /**
    *  @brief
    *    Reconfigures the callback mask for the current state in means of a bit-wise 'clear' operation of the current callback mask
    *
    *  @param[in] mask
    *    The callback mask to exclude
    */
    void removeCallbackMask(CallbackMask mask);

    /**
    *  @brief
    *    Check if all bits of the parameter are set in the currently configured callback mask of the current state
    *
    *  @param[in] mask
    *    The mask to check against
    *
    *  @return
    *    'true' if all bits are set, else 'false'
    */
    bool isEnabled(CallbackMask mask) const;

    /**
    *  @brief
    *    Check if any bit of the parameter is set in the currently configured callback mask of the current state
    *
    *  @param[in] mask
    *    The mask to check against
    *
    *  @return
    *    'true' if at least one bit is set, else 'false'
    */
    bool isAnyEnabled(CallbackMask mask) const;

    /**
    *  @brief
    *    Triggers a call of the unresolved callback
    */
    void unresolved() const;

    /**
    *  @brief
    *    Triggers a call of the before callback, passing the parameters
    *
    *  @param[in] call
    *    The parameters of the current function call
    */
    void before(const FunctionCall & call) const;

    /**
    *  @brief
    *    Triggers a call of the after callback, passing the parameters and return value
    *
    *  @param[in] call
    *    The parameters and the return value of the current function call
    */
    void after(const FunctionCall & call) const;


protected:
    /**
    *  @brief
    *    The State struct represents the configuration of an OpenGL function for one thread.
    *    This includes the driver function pointer (is allowed to differ between contexts) and the callback mask
    */
    struct State
    {
       /**
       *  @brief
       *    Constructor that initializes all values with 0 / invalid
       */
        State();

        ProcAddress  address;      ///< The function pointer to the OpenGL function
        bool         initialized;  ///< Whether this state is initialized or not
        CallbackMask callbackMask; ///< The callback mask that is considered when dispatching function calls
    };

    /**
    *  @brief
    *    Checks for existance of the current configured state
    *
    *  @return
    *    'true' if the current state still exists, else 'false'
    *
    *  @remarks
    *    This method is usually used to detect invalid state clean up
    */
    bool hasState() const;

    /**
    *  @brief
    *    Checks for existance of a state
    *
    *  @param[in] pos
    *    The index of the state to check
    *
    *  @return
    *    'true' if the state exists, else 'false'
    */
    bool hasState(int pos) const;

    /**
    *  @brief
    *    Get current state
    *
    *  @return
    *    The current state
    */
    State & state() const;

    /**
    *  @brief
    *    Get state
    *
    *  @param[in] pos
    *    The index of the state
    *
    *  @return
    *    The state
    */
    State & state(int pos) const;

    /**
    *  @brief
    *    Extend the function states to include the state identified through pos
    *
    *  @param[in] pos
    *    The index of the state to provide
    */
    static void provideState(int pos);

    /**
    *  @brief
    *    Reconfigures the states so that the state identified through pos is neglested
    *
    *  @param[in] pos
    *    The index of the state to neglest
    */
    static void neglestState(int pos);

    /**
    *  @brief
    *    Updates the currently used state
    *
    *  @param[in] pos
    *    The index of the new state
    */
    static void setStatePos(int pos);


protected:
    const char               * m_name;   ///< The OpenGL API function name, including the 'gl' prefix
    mutable std::vector<State> m_states; ///< The list of all states

    // to reduce per instance hasState checks and provide/neglest states for all instances, 
    // max pos is used to provide m_states size, which is identical for all instances.
    static int s_maxpos; ///< The global maximum of states per function
};


} // namespace glesbinding
