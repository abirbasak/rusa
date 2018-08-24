#ifndef AZUIK_TEST_UNIT_TEST_HPP
#define AZUIK_TEST_UNIT_TEST_HPP
#include <cstdio>

namespace azuik
{
    namespace test
    {
        struct test_case
        {
          public:
            explicit test_case(char const *name) noexcept
                : m_name{name}, m_prev{head()->m_prev}, m_next{head()->m_prev->m_next}
            {
                head()->m_prev->m_next = this;
                head()->m_prev = this;
            }
            test_case() noexcept : m_name{"TEST_HEAD"}, m_prev{this}, m_next{this}
            {
            }
            constexpr char const *name() const noexcept
            {
                return m_name;
            }
            constexpr auto prev() const noexcept
            {
                return m_prev;
            }
            constexpr auto next() const noexcept
            {
                return m_next;
            }
            static test_case *head()
            {
                static test_case head{};
                return &head;
            }
            virtual void exec()
            {
            }
            virtual ~test_case()
            {
            }

          private:
            char const *m_name;
            test_case *m_prev;
            test_case *m_next;
        };

        namespace detail_
        {
            template <bool V>
            struct bool_c
            {
                constexpr static bool value = V;
            };
            using true_c = bool_c<true>;
            using false_c = bool_c<false>;

            template <class T>
            struct remove_cv_t
            {
                using type = T;
            };
            template <class T>
            struct remove_cv_t<T const>
            {
                using type = T;
            };
            template <class T>
            using remove_cv = typename remove_cv_t<T>::type;

            template <class T>
            struct remove_ref_t
            {
                using type = T;
            };
            template <class T>
            struct remove_ref_t<T &>
            {
                using type = T;
            };
            template <class T>
            using remove_ref = typename remove_ref_t<T>::type;

            template <class T>
            using remove_cv_ref = remove_ref<remove_cv<T>>;

            template <class T1, class T2>
            struct is_same_t : false_c
            {
            };
            template <class T>
            struct is_same_t<T, T> : true_c
            {
            };
            template <class T1, class T2>
            constexpr bool is_same = is_same_t<T1, T2>::value;

            template <bool B, class E = void>
            struct enable_if_t
            {
                using type = E;
            };
            template <class E>
            struct enable_if_t<false, E>
            {
            };
            template <bool B, class E = void>
            using enable_if = typename enable_if_t<B, E>::type;

            template <class T>
            constexpr bool is_bool = is_same<T, bool>;
        } // namespace detail_

        template <class T>
        struct is_printable : detail_::false_c
        {
        };

#define AZUIK_IS_PRINTABLE_SPCL(TYPE, FMT, EXPR)                                                                       \
    template <>                                                                                                        \
    struct is_printable<TYPE> : detail_::true_c                                                                        \
    {                                                                                                                  \
        static constexpr auto format()                                                                                 \
        {                                                                                                              \
            return #FMT;                                                                                               \
        }                                                                                                              \
        template <class T>                                                                                             \
        static constexpr auto formatted(T &&v)                                                                         \
        {                                                                                                              \
            return EXPR;                                                                                               \
        }                                                                                                              \
    }

        AZUIK_IS_PRINTABLE_SPCL(char, % c, v);
        AZUIK_IS_PRINTABLE_SPCL(bool, % s, v ? "true" : "false");
        AZUIK_IS_PRINTABLE_SPCL(unsigned char, % i, v);
        AZUIK_IS_PRINTABLE_SPCL(signed char, % i, v);
        AZUIK_IS_PRINTABLE_SPCL(unsigned short, % i, v);
        AZUIK_IS_PRINTABLE_SPCL(signed short, % i, v);
        AZUIK_IS_PRINTABLE_SPCL(unsigned int, % i, v);
        AZUIK_IS_PRINTABLE_SPCL(signed int, % i, v);
        AZUIK_IS_PRINTABLE_SPCL(unsigned long, % i, v);
        AZUIK_IS_PRINTABLE_SPCL(signed long, % i, v);
        AZUIK_IS_PRINTABLE_SPCL(unsigned long long, % i, v);
        AZUIK_IS_PRINTABLE_SPCL(signed long long, % i, v);
        AZUIK_IS_PRINTABLE_SPCL(float, % d, v);
        AZUIK_IS_PRINTABLE_SPCL(double, % d, v);
        AZUIK_IS_PRINTABLE_SPCL(long double, % d, v);
#undef AZUIK_IS_PRINTABLE_SPCL

        namespace detail_
        {
            template <class T, std::size_t N>
            struct array
            {
                T value[N];
            };

            struct result_value
            {
                bool value;
                array<char, 128> str;
            };

            struct attach
            {
            };

#define AZUIK_LOGICAL_OPERATOR(OP)                                                                                     \
    template <class U>                                                                                                 \
    result_value operator OP(U &&that) const noexcept                                                                  \
    {                                                                                                                  \
        using is_printable_lhs = is_printable<remove_cv_ref<T>>;                                                       \
        using is_printable_rhs = is_printable<remove_cv_ref<U>>;                                                       \
        if constexpr (is_printable_lhs::value && is_printable_rhs::value)                                              \
        {                                                                                                              \
            array<char, 128> fmt{};                                                                                    \
            std::snprintf(fmt.value, 128, "%s %s %s", is_printable_lhs::format(), #OP, is_printable_rhs::format());    \
            array<char, 128> str{};                                                                                    \
            std::snprintf(str.value, 128, fmt.value, is_printable_lhs::formatted(m_value),                             \
                          is_printable_rhs::formatted(that));                                                          \
            return result_value{m_value OP that, str};                                                                 \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            bool result = m_value OP that;                                                                             \
            array<char, 128> str{};                                                                                    \
            std::snprintf(str.value, 128, "%s", is_printable<bool>::formatted(result));                                \
            return result_value{result, str};                                                                          \
        }                                                                                                              \
    }
            template <class T>
            struct attached_value
            {
                using value_type = T;
                explicit attached_value(T &&x) : m_value{static_cast<T &&>(x)}
                {
                }
                template <class U = T, class = enable_if<is_bool<U>>>
                operator result_value()
                {
                    array<char, 128> fmt{};
                    using is_printable_this = is_printable<remove_cv_ref<T>>;
                    std::snprintf(fmt.value, 128, "value = %s", is_printable_this::format());
                    return result_value{m_value, fmt};
                }
                AZUIK_LOGICAL_OPERATOR(==)
                AZUIK_LOGICAL_OPERATOR(!=)
                AZUIK_LOGICAL_OPERATOR(<)
                AZUIK_LOGICAL_OPERATOR(<=)
                AZUIK_LOGICAL_OPERATOR(>)
                AZUIK_LOGICAL_OPERATOR(>=)
                AZUIK_LOGICAL_OPERATOR(&&)
                AZUIK_LOGICAL_OPERATOR(||)
              private:
                T m_value;
            };
            template <class T>
            attached_value<T> operator->*(attach const &, T &&x)
            {
                return attached_value<T>{static_cast<T &&>(x)};
            }
        } // namespace detail_

        struct formatter
        {
            static void test(detail_::result_value const &result, char const *expr, char const *file, int line) noexcept
            {
                char const *kind = result.value ? "SUCCESS" : "FAILED";
                std::printf("%s(%s,%d) : %s [%s]\n", kind, file, line, expr, result.str.value);
            }
        };

        struct execute_fn
        {
            void operator()(int argc, char *argv[]) const
            {
                test_case const *head = test_case::head();
                for (test_case *cur = head->next(); cur != head; cur = cur->next())
                {
                    std::printf("Executing %s\n", cur->name());
                    cur->exec();
                }
            }
        } constexpr execute{};
    } // namespace test
} // namespace azuik

#define AZUIK_TEST(expr)                                                                                               \
    ::azuik::test::formatter::test(::azuik::test::detail_::attach{}->*expr, #expr, __FILE__, __LINE__)

#define AZUIK_TEST_SUIT(NAME) namespace azuik_test_##NAME

#define AZUIK_TEST_CASE(NAME)                                                                                          \
    struct azuik_test_##NAME : ::azuik::test::test_case                                                                \
    {                                                                                                                  \
        azuik_test_##NAME() : ::azuik::test::test_case(#NAME)                                                          \
        {                                                                                                              \
        }                                                                                                              \
        void exec();                                                                                                   \
    } const azuik_test_instance_##NAME{};                                                                              \
    inline void azuik_test_##NAME::exec()

#endif
