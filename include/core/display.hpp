#ifndef _HYDRA_NGINE_CORE_DISPLAY_HPP
#define _HYDRA_NGINE_CORE_DISPLAY_HPP
namespace hydra {
    class WindowInterface {
    protected:
        uint32_t m_width = 800;
        uint32_t m_height = 800;
        std::string m_title = "Test Window";

    public:
        WindowInterface(std::string title, uint32_t width, uint32_t height) : m_title{title}, m_width{width}, m_height{height} {};
        virtual ~WindowInterface() {};
        inline auto width() & -> uint32_t&;
        inline auto width() const& -> const uint32_t&;
        inline auto height() & -> uint32_t&;
        inline auto height() const& -> const uint32_t&;
        inline auto title() & -> std::string&;
        inline auto title() const& -> const std::string&;
        virtual void init() = 0;
    };
    inline auto WindowInterface::width() & -> uint32_t& { return m_width; };
    inline auto WindowInterface::width() const& -> const uint32_t& { return m_width; };
    inline auto WindowInterface::height() & -> uint32_t& { return m_height; };
    inline auto WindowInterface::height() const& -> const uint32_t& { return m_height; };
    inline auto WindowInterface::title() & -> std::string& { return m_title; };
    inline auto WindowInterface::title() const& -> const std::string& { return m_title; };
}  // namespace hydra

#endif  //_HYDRA_NGINE_CORE_DISPLAY_HPP
