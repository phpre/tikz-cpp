#pragma once
#include <deque>
#include <format>
#include <functional>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>

#include "defines.h"

namespace ALG {
    template <typename T>
    struct symbolic_variable {
        std::string m_name;
        T           m_data;

        inline symbolic_variable( const std::string& p_name, const T& p_data )
            : m_name{ p_name }, m_data{ p_data } {
        }
    };

    template <>
    struct symbolic_variable<std::string> {
        std::string m_name;
        std::string m_data;

        inline symbolic_variable<std::string>( const std::string& p_name )
            : m_name{ p_name }, m_data{ p_name } {
        }
    };

    template <typename R>
    R add( const R& p_a, const R& p_b );
    template <>
    inline double add<double>( const double& p_a, const double& p_b ) {
        return p_a + p_b;
    }
    template <>
    inline std::string add<std::string>( const std::string& p_a, const std::string& p_b ) {
        return "(" + p_a + ")+(" + p_b + ")";
    }

    template <typename R>
    R multiply( const R& p_a, const R& p_b );
    template <>
    inline double multiply<double>( const double& p_a, const double& p_b ) {
        return p_a * p_b;
    }
    template <>
    inline std::string multiply<std::string>( const std::string& p_a, const std::string& p_b ) {
        return "(" + p_a + ")*(" + p_b + ")";
    }

    template <typename R>
    R multiplicative_inverse( const R& p_a );
    template <>
    inline double multiplicative_inverse<double>( const double& p_a ) {
        return 1 / p_a;
    }
    template <>
    inline std::string multiplicative_inverse<std::string>( const std::string& p_a ) {
        return "1/(" + p_a + ")";
    }

    template <typename R>
    R additive_inverse( const R& p_a );
    template <>
    inline double additive_inverse<double>( const double& p_a ) {
        return -p_a;
    }
    template <>
    inline std::string additive_inverse<std::string>( const std::string& p_a ) {
        return "-(" + p_a + ")";
    }

    template <typename R, typename T = R>
    struct symbolic_simple_function {
        std::string           m_name;
        std::function<R( T )> m_f;
    };
    template <typename T>
    using function_input = std::function<symbolic_variable<T>( const std::string& )>;
    template <typename R>
    using circuit_input = std::map<std::string, R>;
    template <typename R, typename T = R>
    using symbolic_function = std::function<R( function_input<T> )>;

    template <typename R, typename T = R>
    inline symbolic_function<R, T> inverse( symbolic_function<R, T> p_f ) {
        return [ p_f ]( function_input<T> p_input ) {
            return multiplicative_inverse<R>( p_f( p_input ) );
        };
    }
    template <typename R, typename T = R>
    inline symbolic_function<R, T> negate( symbolic_function<R, T> p_f ) {
        return
            [ p_f ]( function_input<T> p_input ) { return additive_inverse<R>( p_f( p_input ) ); };
    }
    template <typename R, typename T = R>
    inline symbolic_function<R, T> multiply( symbolic_function<R, T> p_f,
                                             symbolic_function<R, T> p_g ) {
        return [ p_f, p_g ]( function_input<T> p_input ) {
            return multiply<R>( p_f( p_input ), p_g( p_input ) );
        };
    }
    template <typename R, typename T = R>
    inline symbolic_function<R, T> add( symbolic_function<R, T> p_f, symbolic_function<R, T> p_g ) {
        return [ p_f, p_g ]( function_input<T> p_input ) {
            return add<R>( p_f( p_input ), p_g( p_input ) );
        };
    }

    template <typename R, typename T = R>
    struct gate {
        std::string m_name;

        inline virtual ~gate( ) {
        }

        virtual u64 depth( ) const = 0;

        virtual R evaluate( circuit_input<R> p_input ) const                       = 0;
        virtual R evaluate( std::function<R( const std::string& )> p_input ) const = 0;

        virtual std::shared_ptr<gate<R, T>>
        instantiate( function_input<T>                                   p_input,
                     std::function<std::string( const std::string&, T )> p_nameFunction ) const
            = 0;

        virtual std::shared_ptr<gate<R, T>>
        replace_input( const std::string&          p_inputGateName,
                       std::shared_ptr<gate<R, T>> p_replacement ) const
            = 0;

        inline virtual std::set<std::string> inputs( ) const {
            return { };
        }

        inline virtual std::deque<std::shared_ptr<gate<R, T>>> children( ) const {
            return { };
        }

        inline virtual bool is_input( ) const {
            return false;
        }
        inline virtual bool is_output( ) const {
            return false;
        }
        inline virtual bool is_constant( ) const {
            return false;
        }
        inline virtual bool is_operator( ) const {
            return false;
        }
    };

    template <typename R, typename T = R>
    struct constant_gate : public gate<R, T> {
        R m_constant;

        inline constant_gate( const std::string& p_name, const R& p_constant )
            : m_constant{ p_constant } {
            gate<R, T>::m_name = p_name;
        }

        inline bool is_constant( ) const {
            return true;
        }

        inline u64 depth( ) const {
            return 0;
        }

        inline R evaluate( circuit_input<R> ) const {
            return m_constant;
        }
        inline R evaluate( std::function<R( const std::string& )> ) const {
            return m_constant;
        }

        inline std::shared_ptr<gate<R, T>>
        instantiate( function_input<T>,
                     std::function<std::string( const std::string&, T )> ) const {
            return std::make_shared<constant_gate<R, T>>( *this );
        }

        virtual std::shared_ptr<gate<R, T>> replace_input( const std::string&,
                                                           std::shared_ptr<gate<R, T>> ) const {
            return std::make_shared<constant_gate<R, T>>( *this );
        }
    };

    template <typename R, typename T = R>
    struct function_gate : public gate<R, T> {
        symbolic_function<R, T> m_function;

        inline function_gate( const std::string& p_name, const symbolic_function<R, T>& p_function )
            : m_function{ p_function } {
            gate<R, T>::m_name = p_name;
        }

        inline bool is_constant( ) const {
            return true;
        }

        inline u64 depth( ) const {
            return 0;
        }

        inline R evaluate( circuit_input<R> ) const {
            throw std::logic_error( "Cannot evaluate function gate" );
        }
        inline R evaluate( std::function<R( const std::string& )> ) const {
            throw std::logic_error( "Cannot evaluate function gate" );
        }

        inline std::shared_ptr<gate<R, T>>
        instantiate( function_input<T>                                   p_input,
                     std::function<std::string( const std::string&, T )> p_nameFunction ) const {
            return std::make_shared<constant_gate<R, T>>(
                p_nameFunction( gate<R, T>::m_name, m_function( p_input ) ),
                m_function( p_input ) );
        }

        virtual std::shared_ptr<gate<R, T>> replace_input( const std::string&,
                                                           std::shared_ptr<gate<R, T>> ) const {
            return std::make_shared<function_gate<R, T>>( *this );
        }
    };

    template <typename R, typename T = R>
    struct input_gate : public gate<R, T> {
        inline input_gate( const std::string& p_name ) {
            gate<R, T>::m_name = p_name;
        }

        inline bool is_input( ) const {
            return true;
        }

        inline u64 depth( ) const {
            return 0;
        }

        inline R evaluate( circuit_input<R> p_input ) const {
            return p_input.at( gate<R, T>::m_name );
        }
        inline R evaluate( std::function<R( const std::string& )> p_input ) const {
            return p_input( gate<R, T>::m_name );
        }

        inline std::shared_ptr<gate<R, T>>
        instantiate( function_input<T>,
                     std::function<std::string( const std::string&, T )> ) const {
            return std::make_shared<input_gate<R, T>>( *this );
        }

        virtual std::shared_ptr<gate<R, T>>
        replace_input( const std::string&          p_inputGateName,
                       std::shared_ptr<gate<R, T>> p_replacement ) const {
            if( gate<R, T>::m_name == p_inputGateName ) { return p_replacement; }
            return std::make_shared<input_gate<R, T>>( *this );
        }

        inline std::set<std::string> inputs( ) const {
            return { gate<R, T>::m_name };
        }
    };

    template <typename R, typename T = R>
    struct operation_gate : public gate<R, T> {
        const std::function<R( const R&, const R& )> m_op;
        std::deque<std::shared_ptr<gate<R, T>>>      m_children;

        inline operation_gate( const std::string&                             p_name,
                               std::function<R( const R&, const R& )>         p_op,
                               const std::deque<std::shared_ptr<gate<R, T>>>& p_children )
            : m_op{ p_op }, m_children{ p_children } {
            gate<R, T>::m_name = p_name;
        }

        inline virtual bool is_operator( ) const {
            return true;
        }

        inline virtual std::deque<std::shared_ptr<gate<R, T>>> children( ) const {
            return m_children;
        }

        inline virtual u64 depth( ) const {
            u64 res = 0;
            for( const auto& c : m_children ) { res = std::max( res, c->depth( ) ); }
            return 1 + res;
        }

        inline R evaluate( circuit_input<R> p_input ) const {
            R res = m_children.front( )->evaluate( p_input );
            for( u64 i = 1; i < m_children.size( ); ++i ) {
                const auto& c = m_children[ i ];
                res           = m_op( res, c->evaluate( p_input ) );
            }
            return res;
        }

        inline R evaluate( std::function<R( const std::string& )> p_input ) const {
            R res = m_children.front( )->evaluate( p_input );
            for( u64 i = 1; i < m_children.size( ); ++i ) {
                const auto& c = m_children[ i ];
                res           = m_op( res, c->evaluate( p_input ) );
            }
            return res;
        }

        virtual inline std::shared_ptr<gate<R, T>>
        instantiate( function_input<T>                                   p_input,
                     std::function<std::string( const std::string&, T )> p_nameFunction ) const {
            std::deque<std::shared_ptr<gate<R, T>>> res{ };
            for( const auto& c : m_children ) {
                res.push_back( c->instantiate( p_input, p_nameFunction ) );
            }
            return std::make_shared<operation_gate<R, T>>( gate<R, T>::m_name, m_op, res );
        }

        virtual inline std::shared_ptr<gate<R, T>>
        replace_input( const std::string&          p_inputGateName,
                       std::shared_ptr<gate<R, T>> p_replacement ) const {
            std::deque<std::shared_ptr<gate<R, T>>> res{ };
            for( const auto& c : m_children ) {
                res.push_back( c->replace_input( p_inputGateName, p_replacement ) );
            }
            return std::make_shared<operation_gate<R, T>>( gate<R, T>::m_name, m_op, res );
        }

        virtual inline std::set<std::string> inputs( ) const {
            std::set<std::string> res;
            for( const auto& c : m_children ) { res.merge( c->inputs( ) ); }
            return res;
        }
    };

    template <typename R, typename T = R>
    struct addition_gate : public operation_gate<R, T> {
        inline addition_gate( const std::deque<std::shared_ptr<gate<R, T>>>& p_children )
            : operation_gate<R, T>{
                  "+", []( const R& p_l_a, const R& p_l_b ) { return add<R>( p_l_a, p_l_b ); },
                  p_children } {
        }
    };

    template <typename R, typename T = R>
    struct multiplication_gate : public operation_gate<R, T> {
        inline multiplication_gate( const std::deque<std::shared_ptr<gate<R, T>>>& p_children )
            : operation_gate<R, T>{
                  "\\textbullet",
                  []( const R& p_l_a, const R& p_l_b ) { return multiply<R>( p_l_a, p_l_b ); },
                  p_children } {
        }
    };

    template <typename R, typename T = R>
    struct output_gate : public gate<R, T> {
        std::shared_ptr<gate<R, T>> m_child;

        inline output_gate( const std::string& p_name, const std::shared_ptr<gate<R, T>>& p_child )
            : m_child{ p_child } {
            gate<R, T>::m_name = p_name;
        }

        inline bool is_output( ) const {
            return true;
        }

        inline virtual std::deque<std::shared_ptr<gate<R, T>>> children( ) const {
            return { m_child };
        }

        inline u64 depth( ) const {
            return 1 + m_child->depth( );
        }

        inline R evaluate( circuit_input<R> p_input ) const {
            return m_child->evaluate( p_input );
        }
        inline R evaluate( std::function<R( const std::string& )> p_input ) const {
            return m_child->evaluate( p_input );
        }

        inline std::shared_ptr<output_gate<R>>
        instantiate_s( function_input<T>                                   p_input,
                       std::function<std::string( const std::string&, T )> p_nameFunction ) const {
            return std::make_shared<output_gate<R>>(
                gate<R, T>::m_name, m_child->instantiate( p_input, p_nameFunction ) );
        }

        inline std::shared_ptr<gate<R, T>>
        instantiate( function_input<T>                                   p_input,
                     std::function<std::string( const std::string&, T )> p_nameFunction ) const {
            return instantiate_s( p_input, p_nameFunction );
        }

        virtual std::shared_ptr<gate<R, T>>
        replace_input( const std::string&          p_inputGateName,
                       std::shared_ptr<gate<R, T>> p_replacement ) const {
            return std::make_shared<output_gate<R, T>>(
                gate<R, T>::m_name, m_child->replace_input( p_inputGateName, p_replacement ) );
        }

        inline std::set<std::string> inputs( ) const {
            return m_child->inputs( );
        }
    };

    template <typename R, typename T = R>
    struct subcircuit_gate : public gate<R, T> {
        std::shared_ptr<output_gate<R, T>>                              m_op;
        std::deque<std::pair<std::string, std::shared_ptr<gate<R, T>>>> m_children;

        inline subcircuit_gate(
            const std::string& p_name, std::shared_ptr<output_gate<R, T>> p_op,
            const std::deque<std::pair<std::string, std::shared_ptr<gate<R, T>>>>& p_children )
            : m_op{ p_op }, m_children{ p_children } {
            gate<R, T>::m_name = p_name;
        }

        inline subcircuit_gate( const std::string& p_name, std::shared_ptr<output_gate<R, T>> p_op )
            : m_op{ p_op } {
            gate<R, T>::m_name = p_name;

            m_children = std::deque<std::pair<std::string, std::shared_ptr<gate<R, T>>>>{ };

            for( const auto& input : m_op->inputs( ) ) {
                m_children.push_back( { input, std::make_shared<input_gate<R, T>>( input ) } );
            }
        }

        inline virtual bool is_operator( ) const {
            return true;
        }

        inline virtual std::deque<std::shared_ptr<gate<R, T>>> children( ) const {
            std::deque<std::shared_ptr<gate<R, T>>> res{ };
            for( const auto& [ n, c ] : m_children ) { res.push_back( c ); }
            return res;
        }

        inline virtual u64 depth( ) const {
            u64 res = 0;
            for( const auto& [ n, c ] : m_children ) { res = std::max( res, c->depth( ) ); }
            return 1 + res;
        }

        inline R evaluate( circuit_input<R> p_input ) const {
            circuit_input<R> sc_in{ };
            for( const auto& [ n, c ] : m_children ) { sc_in[ n ] = c->evaluate( p_input ); }
            return m_op->evaluate( sc_in );
        }

        inline R evaluate( std::function<R( const std::string& )> p_input ) const {
            circuit_input<R> sc_in{ };
            for( const auto& [ n, c ] : m_children ) { sc_in[ n ] = c->evaluate( p_input ); }
            return m_op->evaluate( sc_in );
        }

        virtual inline std::shared_ptr<gate<R, T>>
        instantiate( function_input<T>                                   p_input,
                     std::function<std::string( const std::string&, T )> p_nameFunction ) const {
            std::deque<std::pair<std::string, std::shared_ptr<gate<R, T>>>> res{ };
            for( const auto& [ n, c ] : m_children ) {
                res.push_back( { n, c->instantiate( p_input, p_nameFunction ) } );
            }
            return std::make_shared<subcircuit_gate<R, T>>( gate<R, T>::m_name, m_op, res );
        }

        virtual inline std::shared_ptr<gate<R, T>>
        replace_input( const std::string&          p_inputGateName,
                       std::shared_ptr<gate<R, T>> p_replacement ) const {
            std::deque<std::pair<std::string, std::shared_ptr<gate<R, T>>>> res{ };
            for( const auto& [ n, c ] : m_children ) {
                res.push_back( { n, c->replace_input( p_inputGateName, p_replacement ) } );
            }
            return std::make_shared<subcircuit_gate<R, T>>( gate<R, T>::m_name, m_op, res );
        }

        virtual inline std::set<std::string> inputs( ) const {
            std::set<std::string> res;
            for( const auto& [ n, c ] : m_children ) { res.merge( c->inputs( ) ); }
            return res;
        }
    };

    template <typename R, typename T = R>
    struct circuit {
        std::deque<std::shared_ptr<output_gate<R, T>>> m_outputs;

        circuit<R, T> merge( const circuit<R, T>& p_other ) const {
            circuit<R, T> res{ m_outputs };
            res.m_outputs.append_range( p_other.m_outputs );
            return res;
        }

        inline u64 depth( ) const {
            u64 d = 0;
            for( const auto& c : m_outputs ) { d = std::max( d, c->depth( ) ); }
            return d;
        }

        inline std::map<std::string, R> evaluate( circuit_input<R> p_input ) const {
            std::map<std::string, R> res{ };
            for( const auto& c : m_outputs ) { res[ c->m_name ] = c->evaluate( p_input ); }
            return res;
        }
        inline std::map<std::string, R>
        evaluate( std::function<R( const std::string& )> p_input ) const {
            std::map<std::string, R> res{ };
            for( const auto& c : m_outputs ) { res[ c->m_name ] = c->evaluate( p_input ); }
            return res;
        }

        inline circuit<R, T>
        instantiate( function_input<T>                                   p_input,
                     std::function<std::string( const std::string&, T )> p_nameFunction ) const {
            std::deque<std::shared_ptr<output_gate<R>>> res{ };
            for( const auto& c : m_outputs ) {
                res.push_back( c->instantiate_s( p_input, p_nameFunction ) );
            }
            return circuit<R, T>{ res };
        }

        inline circuit<R, T> replace_input( const std::string&          p_inputGateName,
                                            std::shared_ptr<gate<R, T>> p_replacement ) const {
            std::deque<std::shared_ptr<output_gate<R, T>>> res;
            for( const auto& c : m_outputs ) {
                res.push_back( c->replace_input( p_inputGateName, p_replacement ) );
            }
            return circuit<R, T>{ res };
        }

        inline std::set<std::string> inputs( ) const {
            std::set<std::string> res;
            for( const auto& c : m_outputs ) { res.merge( c->inputs( ) ); }
            return res;
        }
    };
} // namespace ALG
