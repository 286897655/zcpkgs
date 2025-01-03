#ifndef ZPKG_FW_HPP_
#define ZPKG_FW_HPP_

#include <memory>
#include <functional>

namespace fw{
template<typename T>
class TSingleton{
public:
    template<typename ...Args>
    static T* Singleton(Args &&... args){
        static T singleton(std::forward<Args>(args)...);

        return &singleton;
    }

protected:
    TSingleton() = default;
    ~TSingleton() = default;
};

template<typename Event>
class EventListener{
public:
    virtual void OnEvent(Event&& event) = 0;
};

template<typename Event>
class EventDispatch{
public:
    using Listener = EventListener<Event>;
    void RegisterW(std::weak_ptr<Listener> listenr){
        listener_ = listenr;
    }
    void RegisterP(Listener* listener){
        listener_p_ = listener;
    }
    template<typename Fn>
    void RegisterC(Fn&& fn){
        callback_ = fn;
    }
protected:
    void postEventP(Event&& event){
        if(listener_p_){
            return listener_p_->OnEvent(std::forward<Event>(event));
        }
    }

    void postEventW(Event&& event){
        if(auto shared_listener = listener_.lock()){
            return shared_listener->OnEvent(std::forward<Event>(event));
        }
    }

    void postEventC(Event&& event){
        if(callback_){
            callback_(std::forward<Event>(event));
        }
    }
private:
    std::weak_ptr<Listener> listener_;
    Listener* listener_p_ = nullptr;
    std::function<void(Event&& event)> callback_;
};

};//!namespace fw


#endif//!ZPKG_FW_HPP_