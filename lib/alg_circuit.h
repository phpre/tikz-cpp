#pragma once
#include <deque>
#include <functional>
#include <map>
#include <memory>
#include <string>

#include "defines.h"

namespace ALG {
    template <typename T>
    struct symbolic_variable {
        std::string m_name;
        T           m_data;
    };

    template <typename R>
    R add( R p_a, R p_b );

    template <>
    inline double add<double>( double p_a, double p_b ) {
        return p_a + p_b;
    }

    template <typename R>
    R multiply( R p_a, R p_b );

    template <>
    inline double multiply<double>( double p_a, double p_b ) {
        return p_a * p_b;
    }

    template <typename R>
    R multiplicative_inverse( R p_a );

    template <>
    inline double multiplicative_inverse<double>( double p_a ) {
        return 1 / p_a;
    }

    template <typename T, typename R>
    using symbolic_simple_function = std::function<R( T )>;

    template <typename T>
    using function_input = std::map<std::string, symbolic_variable<T>>;

    template <typename R>
    using circuit_input = std::map<std::string, R>;

    template <typename T, typename R>
    using symbolic_function = std::function<R( function_input<T> )>;

    template <typename T, typename R>
    inline symbolic_function<T, R> inverse( symbolic_function<T, R> p_f ) {
        return [ & ]( function_input<T> p_input ) {
            return multiplicative_inverse<R>( p_f( p_input ) );
        };
    }

    template <typename T, typename R>
    inline symbolic_function<T, R> multiply( symbolic_function<T, R> p_f,
                                             symbolic_function<T, R> p_g ) {
        return [ & ]( function_input<T> p_input ) {
            return multiply<R>( p_f( p_input ), p_g( p_input ) );
        };
    }

    template <typename T, typename R>
    inline symbolic_function<T, R> add( symbolic_function<T, R> p_f, symbolic_function<T, R> p_g ) {
        return
            [ & ]( function_input<T> p_input ) { return add<R>( p_f( p_input ), p_g( p_input ) ); };
    }

    template <typename R>
    struct gate {
        inline virtual ~gate( ) {
        }

        virtual R evaluate( circuit_input<R> p_input ) const = 0;
    };

    template <typename T, typename R>
    struct meta_gate {
        inline virtual ~meta_gate( ) {
        }

        virtual std::shared_ptr<gate<R>> instantiate( function_input<T> p_input ) const = 0;
    };

    template <typename R>
    struct constant_gate : public gate<R> {
        R m_constant;

        inline R evaluate( circuit_input<R> ) const {
            return m_constant;
        }
    };

    template <typename T, typename R>
    struct meta_constant_gate : public meta_gate<T, R> {
        R m_constant;

        inline std::shared_ptr<gate<R>> instantiate( function_input<T> ) const {
            return std::make_shared<constant_gate<R>>( m_constant );
        }
    };

    template <typename T, typename R>
    struct meta_function_gate : public meta_gate<T, R> {
        symbolic_function<T, R> m_function;

        inline std::shared_ptr<gate<R>> instantiate( function_input<T> p_input ) const {
            return std::make_shared<constant_gate<R>>( m_function( p_input ) );
        }
    };

    template <typename R>
    struct input_gate : public gate<R> {
        std::string m_name;

        inline R evaluate( circuit_input<R> p_input ) const {
            return p_input.at( m_name );
        }
    };

    template <typename T, typename R>
    struct meta_input_gate : public meta_gate<T, R> {
        std::string m_name;

        inline std::shared_ptr<gate<R>> instantiate( function_input<T> ) const {
            return std::make_shared<input_gate<R>>( m_name );
        }
    };

    template <typename R>
    struct addition_gate : public gate<R> {
        std::deque<std::shared_ptr<gate<R>>> m_children;

        inline std::shared_ptr<gate<R>> evaluate( circuit_input<R> p_input ) const {
            R res = m_children.front( )->evaluate( p_input );
            for( u64 i = 1; i < m_children.size( ); ++i ) {
                const auto& c = m_children[ i ];
                res           = add<R>( res, c->evaluate( p_input ) );
            }
            return res;
        }
    };

    template <typename T, typename R>
    struct meta_addition_gate : public meta_gate<T, R> {
        std::deque<std::shared_ptr<meta_gate<T, R>>> m_children;

        inline std::shared_ptr<gate<R>> instantiate( function_input<T> p_input ) const {
            std::shared_ptr<gate<R>> res{ };
            for( const auto& c : m_children ) { res.push_back( c->instantiate( p_input ) ); }
            return std::make_shared<addition_gate<R>>( res );
        }
    };

    template <typename R>
    struct multiplication_gate : public gate<R> {
        std::deque<std::shared_ptr<gate<R>>> m_children;

        inline std::shared_ptr<gate<R>> evaluate( circuit_input<R> p_input ) const {
            R res = m_children.front( )->evaluate( p_input );
            for( u64 i = 1; i < m_children.size( ); ++i ) {
                const auto& c = m_children[ i ];
                res           = multiply<R>( res, c->evaluate( p_input ) );
            }
            return res;
        }
    };

    template <typename T, typename R>
    struct meta_multiplication_gate : public meta_gate<T, R> {
        std::deque<std::shared_ptr<meta_gate<T, R>>> m_children;

        inline std::shared_ptr<gate<R>> instantiate( function_input<T> p_input ) const {
            std::shared_ptr<gate<R>> res{ };
            for( const auto& c : m_children ) { res.push_back( c->instantiate( p_input ) ); }
            return std::make_shared<multiplication_gate<R>>( res );
        }
    };

    template <typename R>
    struct output_gate : public gate<R> {
        std::string              m_name;
        std::shared_ptr<gate<R>> m_child;

        inline std::shared_ptr<gate<R>> instantiate( circuit_input<R> p_input ) const {
            return m_child->evaluate( p_input );
        }
    };

    template <typename T, typename R>
    struct meta_output_gate : public meta_gate<T, R> {
        std::string                      m_name;
        std::shared_ptr<meta_gate<T, R>> m_child;

        inline std::shared_ptr<gate<R>> instantiate( function_input<T> p_input ) const {
            return std::make_shared<output_gate<R>>( m_name, m_child->instantiate( p_input ) );
        }
    };

    template <typename R>
    struct circuit {
        std::deque<std::shared_ptr<output_gate<R>>> m_outputs;

        inline std::map<std::string, R> evaluate( circuit_input<R> p_input ) const {
            std::map<std::string, R> res{ };
            for( const auto& c : m_outputs ) { res[ c->m_name ] = c->evaluate( p_input ); }
            return res;
        }
    };

    template <typename T, typename R>
    struct meta_circuit {
        std::deque<std::shared_ptr<meta_output_gate<T, R>>> m_outputs;

        inline circuit<R> instantiate( function_input<T> p_input ) const {
            std::deque<std::shared_ptr<output_gate<R>>> res{ };
            for( const auto& c : m_outputs ) { res.push_back( c->instantiate( p_input ) ); }
            return circuit<R>{ res };
        }
    };
} // namespace ALG
