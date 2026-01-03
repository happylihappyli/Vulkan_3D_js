#include "button_manager.h"
#include <imgui.h>
#include <iostream>

namespace ScriptViewer {

ButtonManager::ButtonManager()
    : m_nextButtonId(1)
    , m_jsContext(nullptr)
{
    std::cout << "ButtonManager 初始化" << std::endl;
}

ButtonManager::~ButtonManager() {
    clearAllButtons();
    std::cout << "ButtonManager 销毁" << std::endl;
}

// 创建按钮，返回按钮ID
uint32_t ButtonManager::createButton(const std::string& text) {
    uint32_t buttonId = m_nextButtonId++;
    Button* button = new Button(buttonId, text);
    button->manager = this;
    m_buttons.push_back(button);
    std::cout << "创建按钮: ID=" << buttonId << ", 文本=\"" << text << "\"" << std::endl;
    return buttonId;
}

// 设置按钮位置
bool ButtonManager::setButtonPosition(uint32_t buttonId, float x, float y) {
    Button* button = getButton(buttonId);
    if (!button) {
        std::cerr << "设置按钮位置失败: 找不到按钮ID " << buttonId << std::endl;
        return false;
    }
    
    button->x = x;
    button->y = y;
    std::cout << "设置按钮位置: ID=" << buttonId << ", 位置=(" << x << ", " << y << ")" << std::endl;
    return true;
}

// 设置按钮大小
bool ButtonManager::setButtonSize(uint32_t buttonId, float width, float height) {
    Button* button = getButton(buttonId);
    if (!button) {
        std::cerr << "设置按钮大小失败: 找不到按钮ID " << buttonId << std::endl;
        return false;
    }
    
    button->width = width;
    button->height = height;
    std::cout << "设置按钮大小: ID=" << buttonId << ", 大小=" << width << "x" << height << std::endl;
    return true;
}

// 设置按钮颜色
bool ButtonManager::setButtonColor(uint32_t buttonId, float r, float g, float b) {
    Button* button = getButton(buttonId);
    if (!button) {
        std::cerr << "设置按钮颜色失败: 找不到按钮ID " << buttonId << std::endl;
        return false;
    }
    
    button->r = r;
    button->g = g;
    button->b = b;
    std::cout << "设置按钮颜色: ID=" << buttonId << ", 颜色=(" << r << ", " << g << ", " << b << ")" << std::endl;
    return true;
}

// 设置按钮可见性
bool ButtonManager::setButtonVisible(uint32_t buttonId, bool visible) {
    Button* button = getButton(buttonId);
    if (!button) {
        std::cerr << "设置按钮可见性失败: 找不到按钮ID " << buttonId << std::endl;
        return false;
    }
    
    button->visible = visible;
    std::cout << "设置按钮可见性: ID=" << buttonId << ", 可见=" << (visible ? "true" : "false") << std::endl;
    return true;
}

// 设置按钮启用状态
bool ButtonManager::setButtonEnabled(uint32_t buttonId, bool enabled) {
    Button* button = getButton(buttonId);
    if (!button) {
        std::cerr << "设置按钮启用状态失败: 找不到按钮ID " << buttonId << std::endl;
        return false;
    }
    
    button->enabled = enabled;
    std::cout << "设置按钮启用状态: ID=" << buttonId << ", 启用=" << (enabled ? "true" : "false") << std::endl;
    return true;
}

// 设置按钮点击事件（C++）
bool ButtonManager::setButtonClickHandler(uint32_t buttonId, std::function<void()> handler) {
    Button* button = getButton(buttonId);
    if (!button) {
        std::cerr << "设置按钮点击事件失败: 找不到按钮ID " << buttonId << std::endl;
        return false;
    }
    
    button->clickHandler = handler;
    std::cout << "设置按钮点击事件: ID=" << buttonId << std::endl;
    return true;
}

// 设置按钮点击事件（JavaScript）
bool ButtonManager::setButtonClickHandler(uint32_t buttonId, JSValue handler) {
    Button* button = getButton(buttonId);
    if (!button) {
        std::cerr << "设置按钮点击事件失败: 找不到按钮ID " << buttonId << std::endl;
        return false;
    }
    
    if (!m_jsContext) {
        std::cerr << "设置按钮点击事件失败: JSContext未初始化" << std::endl;
        return false;
    }
    
    // 释放旧的JavaScript函数
    if (JS_IsFunction(m_jsContext, button->jsClickHandler)) {
        JS_FreeValue(m_jsContext, button->jsClickHandler);
    }
    
    // 保存新的JavaScript函数
    button->jsClickHandler = JS_DupValue(m_jsContext, handler);
    
    // 设置C++点击处理函数来调用JavaScript函数
    button->clickHandler = [button]() {
        if (button->manager && button->manager->m_jsContext) {
            JSContext* ctx = button->manager->m_jsContext;
            if (JS_IsFunction(ctx, button->jsClickHandler)) {
                JSValueConst args[1];
                args[0] = JS_NewInt32(ctx, (int32_t)button->id);
                JS_Call(ctx, button->jsClickHandler, JS_UNDEFINED, 1, args);
                JS_FreeValue(ctx, args[0]);
            }
        }
    };
    
    std::cout << "设置按钮点击事件（JavaScript）: ID=" << buttonId << std::endl;
    return true;
}

// 删除按钮
bool ButtonManager::deleteButton(uint32_t buttonId) {
    for (auto it = m_buttons.begin(); it != m_buttons.end(); ++it) {
        if ((*it)->id == buttonId) {
            // 释放JavaScript函数
            if (m_jsContext && JS_IsFunction(m_jsContext, (*it)->jsClickHandler)) {
                JS_FreeValue(m_jsContext, (*it)->jsClickHandler);
            }
            std::cout << "删除按钮: ID=" << buttonId << std::endl;
            delete *it;
            m_buttons.erase(it);
            return true;
        }
    }
    
    std::cerr << "删除按钮失败: 找不到按钮ID " << buttonId << std::endl;
    return false;
}

// 清除所有按钮
void ButtonManager::clearAllButtons() {
    std::cout << "清除所有按钮，共 " << m_buttons.size() << " 个" << std::endl;
    for (auto button : m_buttons) {
        delete button;
    }
    m_buttons.clear();
}

// 获取按钮
Button* ButtonManager::getButton(uint32_t buttonId) {
    for (auto button : m_buttons) {
        if (button->id == buttonId) {
            return button;
        }
    }
    return nullptr;
}

// 获取所有按钮
const std::vector<Button*>& ButtonManager::getAllButtons() const {
    return m_buttons;
}

// 渲染所有按钮（在ImGui中调用）
void ButtonManager::render() {
    for (auto button : m_buttons) {
        if (!button->visible) {
            continue;
        }
        
        // 设置按钮位置
        ImGui::SetCursorPos(ImVec2(button->x, button->y));
        
        // 设置按钮颜色
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(button->r, button->g, button->b, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(button->r + 0.1f, button->g + 0.1f, button->b + 0.1f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(button->r + 0.2f, button->g + 0.2f, button->b + 0.2f, 1.0f));
        
        // 设置按钮是否启用
        if (!button->enabled) {
            ImGui::BeginDisabled();
        }
        
        // 渲染按钮
        bool clicked = ImGui::Button(button->text.c_str(), ImVec2(button->width, button->height));
        
        // 恢复按钮启用状态
        if (!button->enabled) {
            ImGui::EndDisabled();
        }
        
        // 恢复按钮颜色
        ImGui::PopStyleColor(3);
        
        // 处理点击事件
        if (clicked && button->enabled && button->clickHandler) {
            std::cout << "按钮被点击: ID=" << button->id << ", 文本=\"" << button->text << "\"" << std::endl;
            button->clickHandler();
        }
    }
}

// 获取下一个可用的按钮ID
uint32_t ButtonManager::getNextButtonId() {
    return m_nextButtonId;
}

} // namespace ScriptViewer
