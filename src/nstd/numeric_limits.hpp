#ifndef _NSTD_NUMERIC_LIMITS_H
#define _NSTD_NUMERIC_LIMITS_H

// libcxx/include/type_traits

namespace nstd {

  // remove_const/volatile/cv ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ {

  template <class T> struct remove_const          {typedef T type;};
  template <class T> struct remove_const<const T> {typedef T type;};

  template <class T> struct remove_volatile             {typedef T type;};
  template <class T> struct remove_volatile<volatile T> {typedef T type;};

  template <class T> struct remove_cv {
    typedef typename remove_volatile<
        typename remove_const<T>::type
      >::type type;
  };

  // }

  // integral / bool constant, true/false type ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ {

  template <class T, T _val>
    struct integral_constant
    {
        static constexpr const T      value = _val;
        typedef T                     value_type;
        typedef integral_constant     type;

        inline constexpr operator value_type() const noexcept {return value;}
        inline constexpr value_type operator ()() const noexcept {return value;}
    };

  template <class T, T _val>
    constexpr const T integral_constant<T, _val>::value;

  template <bool _val>
    using bool_constant = integral_constant<bool, _val>;

  using  true_type = bool_constant< true >;
  using false_type = bool_constant< false >;

  // }


  // ~ ~ is_integral ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ {

  template <class T>   struct is_integral                     : public false_type {};
  template <>          struct is_integral<bool>               : public true_type {};
  template <>          struct is_integral<char>               : public true_type {};
  template <>          struct is_integral<signed char>        : public true_type {};
  template <>          struct is_integral<unsigned char>      : public true_type {};
  template <>          struct is_integral<short>              : public true_type {};
  template <>          struct is_integral<unsigned short>     : public true_type {};
  template <>          struct is_integral<int>                : public true_type {};
  template <>          struct is_integral<unsigned int>       : public true_type {};
  template <>          struct is_integral<long>               : public true_type {};
  template <>          struct is_integral<unsigned long>      : public true_type {};
  template <>          struct is_integral<long long>          : public true_type {};
  template <>          struct is_integral<unsigned long long> : public true_type {};

  template <class T> struct is_integral
      : public is_integral<typename remove_cv<T>::type> {};

  // }


  // ~ ~ is_floating_point ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ {

  template <class T>   struct is_floating_point              : public false_type {};
  template <>          struct is_floating_point<float>       : public true_type {};
  template <>          struct is_floating_point<double>      : public true_type {};
  template <>          struct is_floating_point<long double> : public true_type {};

  template <class T> struct is_floating_point
      : public is_floating_point< typename remove_cv<T>::type > {};

  // }


  // ~ ~ is_arithmetic ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ {

  template <class T> struct is_arithmetic
      : public integral_constant<bool, is_integral<T>::value ||
                                       is_floating_point<T>::value> {};

  // }


  // ~ ~ numeric_limits<T> ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ {

  /**
   * Inspired by LLVM-Clang's `libcxx/include/limits`
   *
   * \link http://en.cppreference.com/w/cpp/types/numeric_limits
   */
  template <class T>
  class numeric_limits<T, bool IsArithmetic = true>
  {
  protected:
      typedef T type;

      static constexpr const bool is_specialized = true;

      static constexpr const bool is_signed = type(-1) < type(0);

      /// Count of boolean digits.
      static constexpr const int  digits = static_cast<int>(sizeof(type) * 8 - is_signed);

      inline static constexpr type min() noexcept {
        return type( type(1) << digits );
      }

      inline static constexpr type max() noexcept {
        return is_signed ? type(type(~0) ^ min()): type(~0);
      }
  };

  static_assert(numeric_limits<char>::max() == 255);

  // }

  } // nstd ns.

#endif // _NSTD_NUMERIC_LIMITS_H
