#pragma once

#include <memory>

namespace notstd {

template<typename TFunc>
class function;

template<typename R, typename... Args>
class function<R(Args...)> {
    struct ICallable {
        virtual R invoke(Args... args) = 0;

        virtual ~ICallable() = default;
    };

    template<typename TCallable>
    class TCallableImpl : public ICallable {
      public:
        explicit TCallableImpl(const TCallable& callable) : callable_(callable) {}

        R invoke(Args... args) override {
            return callable_(args...);
        }

      private:
        TCallable callable_;
    };

  public:
    template<typename TCallable>
    explicit function(const TCallable& callable) : callable_(new TCallableImpl<TCallable>(callable)) {}

    R operator()(Args... args) {
        return callable_->invoke(args...);
    }

  private:
    std::unique_ptr<ICallable> callable_;
};

} // notstd
