#pragma once

#include <memory>

namespace notstd {

class any {
    struct Inner {
        [[nodiscard]] virtual Inner* clone() const = 0;

        virtual ~Inner() = default;
    };

    template<typename T>
    class InnerImpl : public Inner {
      public:
        explicit InnerImpl(const T& value) : value_(value) {}

        [[nodiscard]] Inner* clone() const override {
            return new InnerImpl(value_);
        }

        T& operator*() {
            return value_;
        }

        const T& operator*() const {
            return value_;
        }

      private:
        T value_;
    };

  public:
    any() : inner_(new InnerImpl<bool>(false)) {}

    any(const any& other) : inner_(other.inner_->clone()) {}

    template<typename T>
    any(const T& value) : inner_(new InnerImpl<T>(value)) {}

    any& operator=(const any& other) {
        any tmp(other);
        std::swap(tmp.inner_, inner_);

        return *this;
    }

    template<typename T>
    any& operator=(const T& value) {
        inner_ = std::make_unique<InnerImpl<T>>(value);

        return *this;
    }

    template<typename T>
    T& cast() {
        return *dynamic_cast<InnerImpl<T>&>(*inner_);
    }

    template<typename T>
    const T& cast() const {
        return *dynamic_cast<InnerImpl<T>&>(*inner_);
    }

  private:
    std::unique_ptr<Inner> inner_;
};

} // notstd
