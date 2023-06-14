#pragma once

#include <string>
#include <unordered_map>
#include <functional>

namespace os {

enum class pixel_format : uint8_t {
    rgba8,
    rgba16,
    rgba32f,
};

// metadata for the image buffer
struct image_header {
    uint32_t width;
    uint32_t height;
    uint8_t channels;
    pixel_format fmt;
    // can contain a json with the image metadata
    std::string metadata{};
};

struct input_source_result {
    bool success{ true };
    const std::string error_message{};
};

struct imgui_context_data {
    void* (*alloc_func)(size_t sz, void* user_data); // Function signature for ImGui::SetAllocatorFunctions()
    void (*free_func)(void* ptr, void* user_data); // Function signature for ImGui::SetAllocatorFunctions()
    void* user_data{ nullptr };
    void* context{ nullptr };
};

class OSPlugin {
public:
    typedef std::unordered_map<std::string, std::string> string_map;
    typedef std::function<int(uint8_t** ptr, size_t size)> request_buf_callback;
    typedef std::function<void(int req_id, const image_header& ing_info)> finalize_buf_callback;

public:
    OSPlugin() {}
    virtual ~OSPlugin() {}

    // called on plugin load
    virtual bool init() = 0;
    // imgui related pointers (required when using imgui from dll)
    virtual bool init_imgui(imgui_context_data*) = 0;

    // called when plugin is unloaded
    virtual void release() = 0;

    // will be called on input connect / disconnect
    virtual input_source_result start() = 0;
    virtual input_source_result stop() = 0;

    // called by OmniScope on plugin load
    virtual void read_settings(const string_map& settings) {}

    // input source settings UI (available in the input settings in OmniScope)
    virtual void draw_settings_ui(string_map& settings, bool* show_settings) {}

    // called every frame - possibility to render any ImGui user interface and access plugin settings
    virtual void draw_custom_ui(string_map& settings) {}

    // callbacks used for requesting memory buffers and sending image dta to the application
    void set_callbacks(request_buf_callback request_callback, finalize_buf_callback finalize_callback)
    {
        m_request_buf_callback = std::move(request_callback);
        m_finalize_buf_callback = std::move(finalize_callback);
    }

    inline int get_unique_id() const
    {
        return m_unique_id;
    }
    inline void set_unique_id(int u_id)
    {
        m_unique_id = u_id;
    }

protected:
    request_buf_callback m_request_buf_callback{ nullptr };
    finalize_buf_callback m_finalize_buf_callback{ nullptr };
    int m_unique_id{ -1 };
}; // class OSPlugin

} // namespace os
