#pragma once

#include <memory>

namespace notstd {

template<typename T>
class optional {
  protected:
    using value_type = T;

  public:
    optional() : value_(nullptr) {}

    optional(const optional& other) : value_(new value_type(*other.value_)) {}

    optional(const value_type& value) : value_(new value_type(value)) {}

    optional& operator=(const optional& other) {
        optional tmp(other);
        std::swap(tmp.value_, value_);

        return *this;
    }

    optional& operator=(const value_type& value) {
        value_ = std::make_unique<value_type>(value);

        return *this;
    }

    [[nodiscard]] bool has_value() const {
        return value_ != nullptr;
    }

    value_type& value() {
        return *value_;
    }

    const value_type& value() const {
        return *value_;
    }

  private:
    std::unique_ptr<value_type> value_;
};

} // notstd
