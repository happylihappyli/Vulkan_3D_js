#pragma once

#include <string>
#include <vector>
#include <functional>
#include <cstdint>
#include "../quickjs/quickjs.h"

namespace ScriptViewer {

// 前向声明
class ButtonManager;

// 按钮结构体
struct Button {
    uint32_t id;                      // 按钮唯一ID
    std::string text;                 // 按钮文本
    float x, y;                       // 按钮位置
    float width, height;              // 按钮大小
    float r, g, b;                    // 按钮颜色
    bool visible;                     // 是否可见
    bool enabled;                     // 是否启用
    std::function<void()> clickHandler; // 点击事件处理函数（C++）
    JSValue jsClickHandler;           // 点击事件处理函数（JavaScript）
    ButtonManager* manager;           // 按钮管理器指针（用于访问JSContext）
    
    Button(uint32_t id, const std::string& text)
        : id(id)
        , text(text)
        , x(0.0f)
        , y(0.0f)
        , width(100.0f)
        , height(30.0f)
        , r(0.2f)
        , g(0.6f)
        , b(0.8f)
        , visible(true)
        , enabled(true)
        , clickHandler(nullptr)
        , jsClickHandler(JS_UNDEFINED)
        , manager(nullptr)
    {
    }
};

// 按钮管理器类
class ButtonManager {
public:
    ButtonManager();
    ~ButtonManager();
    
    // 创建按钮，返回按钮ID
    uint32_t createButton(const std::string& text);
    
    // 设置按钮位置
    bool setButtonPosition(uint32_t buttonId, float x, float y);
    
    // 设置按钮大小
    bool setButtonSize(uint32_t buttonId, float width, float height);
    
    // 设置按钮颜色
    bool setButtonColor(uint32_t buttonId, float r, float g, float b);
    
    // 设置按钮可见性
    bool setButtonVisible(uint32_t buttonId, bool visible);
    
    // 设置按钮启用状态
    bool setButtonEnabled(uint32_t buttonId, bool enabled);
    
    // 设置按钮点击事件（C++）
    bool setButtonClickHandler(uint32_t buttonId, std::function<void()> handler);
    
    // 设置按钮点击事件（JavaScript）
    bool setButtonClickHandler(uint32_t buttonId, JSValue handler);
    
    // 设置JSContext（用于调用JavaScript函数）
    void setJSContext(JSContext* ctx) { m_jsContext = ctx; }
    
    // 删除按钮
    bool deleteButton(uint32_t buttonId);
    
    // 清除所有按钮
    void clearAllButtons();
    
    // 获取按钮
    Button* getButton(uint32_t buttonId);
    
    // 获取所有按钮
    const std::vector<Button*>& getAllButtons() const;
    
    // 渲染所有按钮（在ImGui中调用）
    void render();
    
    // 获取下一个可用的按钮ID
    uint32_t getNextButtonId();
    
private:
    std::vector<Button*> m_buttons;    // 所有按钮的列表
    uint32_t m_nextButtonId;           // 下一个按钮ID
    JSContext* m_jsContext;            // JavaScript上下文（用于调用JS函数）
};

} // namespace ScriptViewer
