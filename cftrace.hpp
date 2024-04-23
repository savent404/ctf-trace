/**
 * @file cftrace.hpp
 * @author  savent_gate@outlook.com
 * @brief common format trace public header
 * @date 2024-04-23
 * 
 * Copyright 2023 savent_gate
 * 
 */
#pragma once

#include "cftrace_conf.h"
#include <string_view>
#include <tuple>
#include <span>
#include <array>

namespace cftrace {

    namespace impl {
        struct backend_if {
            virtual void write(std::span<char> buffer) = 0;
        };

        struct backend {
            static backend_if* get() { return instance; }
            static void set(backend_if* b) { instance = b; }
          private:
            static backend_if* instance;
        };
    }

    template <typename...Args>
    constexpr auto pack(Args&&...args) {
        return std::make_tuple(std::forward<Args>(args)...);
    }

    template <typename T>
    constexpr size_t size(T t) {
        return sizeof(t);
    }

    template <>
    constexpr size_t size(std::string_view t) {
        return t.size();
    }

    template <typename T, size_t N>
    constexpr size_t size(std::array<T, N> t) {
        return N * sizeof(T);
    }

    template <typename T, typename... Args>
    constexpr size_t size(T t, Args ... args) {
        // explicit namespace for avoiding std::size ambiguity
        return cftrace::size(t) + cftrace::size(args...);
    }

    template <typename T>
    constexpr void serialize(std::span<char> buffer, T t) {
        auto ptr = buffer.data();
        *reinterpret_cast<T*>(ptr) = t;
    }

    template <typename T, typename... Args>
    constexpr void serialize(std::span<char> buffer, T t, Args ... args) {
        serialize<T>(buffer, t);
        serialize(buffer.subspan(sizeof(T)), args...);
    }

    template <typename T, typename... Args>
    constexpr std::span<char> serialize(T t, Args ... args) {
        static constexpr auto s = cftrace::size(t, args...);
        std::array<char, s> buf;
        serialize(std::span<char>(buf), t, args...);
        return std::span<char>(buf);
    }

    template <typename... Args>
    void trace(Args... args) {
        static char _buf[128];
        auto s = cftrace::size(args...);
        std::span<char> buf(_buf, s);
        serialize(buf, args...);
        impl::backend{}.get()->write(buf);
    }
}
