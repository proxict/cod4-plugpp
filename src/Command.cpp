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

namespace detail {
    template <std::size_t TCount, std::size_t... TIndices>
    auto createDynamicCallbackInstances(std::index_sequence<TIndices...>) {
        return std::array<std::unique_ptr<CallbackBase>, TCount>{
            { std::make_unique<DynamicCallback<TIndices>>()... }
        };
    }
} // namespace detail

template <std::size_t TCount>
auto createDynamicCallbackInstances() {
    return detail::createDynamicCallbackInstances<TCount>(std::make_index_sequence<TCount>{});
}

static auto gCallbackInstances = createDynamicCallbackInstances<20>();

class MemberFunctionCallbackWrapper {
public:
    MemberFunctionCallbackWrapper(Command* instance, MemberFunctionCallback method) {
        for (mContext = 0; mContext < gCallbackInstances.size(); ++mContext) {
            mCallback = gCallbackInstances.at(mContext)->reserve(instance, method);
            if (mCallback) {
                break;
            }
        }
    }

    ~MemberFunctionCallbackWrapper() noexcept {
        if (mCallback) {
            gCallbackInstances.at(mContext)->release();
        }
    }

public:
    xcommand_t getCallback() const { return mCallback; }

private:
    xcommand_t mCallback = nullptr;
    std::size_t mContext;

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

Command::Command(std::string commandName, std::function<void()> callback, bool replace)
    : mImpl(std::make_unique<Impl>(this, std::move(callback)))
    , mCommandName(std::move(commandName)) {
    xcommand_t cb = mImpl->getCallback();
    if (!cb) {
        throw plugpp::Exception(
            "Failed to register command ", commandName, " - number of available command slots exceeded");
    }
    if (replace) {
        removeCommand(mCommandName);
    }
    Plugin_AddCommand(mCommandName.c_str(), cb, 100);
}

Command::~Command() noexcept {
    removeCommand(mCommandName);
}

void Command::callback() {
    mImpl->callback();
}

void removeCommand(const std::string& commandName) {
    std::string cmd(commandName);
    Plugin_RemoveCommand(&cmd[0]);
}

} // namespace plugpp
