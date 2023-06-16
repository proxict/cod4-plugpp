#include "cod4-plugpp/Command.hpp"

#include "cod4-plugpp/Exception.hpp"
#include "cod4-plugpp/PluginApi.h"

#include <memory>

namespace plugpp {

typedef void (Command::*MemberFunctionCallback)();

class CallbackBase {
public:
    CallbackBase(xcommand_t callback)
        : mCallback(callback)
        , mCommandInstance(nullptr)
        , mMethod(nullptr) {}

    void release() {
        mMethod = nullptr;
        mCommandInstance = nullptr;
    }

    xcommand_t reserve(Command* instance, MemberFunctionCallback method) {
        if (mCommandInstance) {
            return nullptr;
        }

        mCommandInstance = instance;
        mMethod = method;
        return mCallback;
    }

protected:
    static void staticInvoke(const int context);

private:
    xcommand_t mCallback;
    Command* mCommandInstance;
    MemberFunctionCallback mMethod;
};

template <int TContext>
class DynamicCallback : public CallbackBase {
public:
    DynamicCallback()
        : CallbackBase(&DynamicCallback<TContext>::contextualStaticFunction) {}

private:
    static void contextualStaticFunction() { staticInvoke(TContext); }
};

// clang-format off
static CallbackBase* gCallbackInstances[] = {
    new DynamicCallback<0>(),
    new DynamicCallback<1>(),
    new DynamicCallback<2>(),
    new DynamicCallback<3>(),
    new DynamicCallback<4>(),
    new DynamicCallback<5>(),
    new DynamicCallback<6>(),
    new DynamicCallback<7>(),
    new DynamicCallback<8>(),
    new DynamicCallback<9>(),
    new DynamicCallback<10>(),
    new DynamicCallback<11>(),
    new DynamicCallback<12>(),
    new DynamicCallback<13>(),
    new DynamicCallback<14>(),
    new DynamicCallback<15>(),
    new DynamicCallback<16>(),
    new DynamicCallback<17>(),
    new DynamicCallback<18>(),
    new DynamicCallback<19>(),
};
static constexpr const int NUMBER_OF_INSTANCES = sizeof(gCallbackInstances) / sizeof(gCallbackInstances[0]);
// clang-format on

extern "C" __attribute__((destructor)) void destructGlobalObjects() {
    for (int i = 0; i < NUMBER_OF_INSTANCES; ++i) {
        if (gCallbackInstances[i]) {
            delete gCallbackInstances[i];
            gCallbackInstances[i] = nullptr;
        }
    }
}

class MemberFunctionCallbackWrapper {
public:
    MemberFunctionCallbackWrapper(Command* instance, MemberFunctionCallback method) {
        for (mContext = 0; mContext < NUMBER_OF_INSTANCES; ++mContext) {
            mCallback = gCallbackInstances[mContext]->reserve(instance, method);
            if (mCallback) {
                break;
            }
        }
    }

    ~MemberFunctionCallbackWrapper() noexcept {
        if (mCallback) {
            gCallbackInstances[mContext]->release();
        }
    }

public:
    xcommand_t getCallback() const { return mCallback; }

private:
    xcommand_t mCallback = nullptr;
    int mContext;

private:
    MemberFunctionCallbackWrapper(const MemberFunctionCallbackWrapper& os);
    MemberFunctionCallbackWrapper& operator=(const MemberFunctionCallbackWrapper& os);
};

void CallbackBase::staticInvoke(const int context) {
    ((gCallbackInstances[context]->mCommandInstance)->*(gCallbackInstances[context]->mMethod))();
}

class Command::Impl {
public:
    Impl(Command* command, std::function<void()> functor)
        : mCallbackWrapper(command, &Command::callback)
        , mFunctor(std::move(functor)) {}

    xcommand_t getCallback() { return mCallbackWrapper.getCallback(); }

    void callback() {
        if (mFunctor) {
            mFunctor();
        }
    }

private:
    MemberFunctionCallbackWrapper mCallbackWrapper;
    std::function<void()> mFunctor;
};

Command::Command(std::string functionName, std::function<void()> functor, bool replace)
    : mImpl(std::make_unique<Impl>(this, std::move(functor)))
    , mFunctionName(std::move(functionName)) {
    xcommand_t cb = mImpl->getCallback();
    if (!cb) {
        throw plugpp::Exception("Failed to register command ", functionName, " - number of available command slots exceeded");
    }
    if (replace) {
        removeCommand(mFunctionName);
    }
    Plugin_AddCommand(mFunctionName.c_str(), cb, 100);
}

Command::~Command() noexcept {
    removeCommand(mFunctionName);
}

void Command::callback() {
    mImpl->callback();
}

void removeCommand(const std::string& commandName) {
    std::string cmd(commandName);
    Plugin_RemoveCommand(&cmd[0]);
}

} // namespace plugpp
