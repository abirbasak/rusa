#ifndef AZUIK_TOOL_COMMAND_LINE_HPP
#define AZUIK_TOOL_COMMAND_LINE_HPP
#include <string.h>
namespace azuik
{
    namespace tool
    {
        class command_line {
            using str_type = char const*;

        public:
            command_line(int argc, str_type* argv)
                : m_argc{argc}
                , m_argv{argv}
            {}

            struct span {
                auto constexpr begin() const noexcept
                {
                    return m_ptr;
                }
                auto constexpr end() const noexcept
                {
                    return m_ptr + m_size;
                }
                int m_size;
                str_type* m_ptr;
            };
            struct starts_with_fn {
                auto operator()(str_type s, str_type prefix) const noexcept -> bool
                {
                    return ::strncmp(prefix, s, ::strlen(prefix)) == 0;
                }
            };
            inline static starts_with_fn starts_with{};

        public:
            auto constexpr contains(char const* key) const noexcept -> bool
            {
                for (auto const& s : span{m_argc, m_argv})
                {
                    if (starts_with(s, key))
                    {
                        return true;
                    }
                }
                return false;
            }
            //auto constexpr value(char const* key) const noexcept -> char const* {}

        private:
            int m_argc;
            str_type* m_argv;
        };
    } // namespace tool
} // namespace azuik
#endif
