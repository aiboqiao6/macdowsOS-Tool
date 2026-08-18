#ifndef UI_MANAGER_HPP
#define UI_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "UIElement.hpp"

// UIManager: 管理所有顶层 UI 元素，负责事件分发、更新和绘制
class UIManager {
public:
    // 添加 UI 元素
    void add(std::shared_ptr<UIElement> element) {
        elements.push_back(element);
    }

    // 事件分发：根据事件类型传递给对应元素
    void handleEvent(const sf::Event& event) {
        // 鼠标左键按下：查找顶层元素处理点击
        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f pt(event.mouseButton.x, event.mouseButton.y);
            // 从后向前遍历，先检查最上层的元素
            for (auto it = elements.rbegin(); it != elements.rend(); ++it) {
                if ((*it)->contains(pt)) {
                    focused = *it;
                    (*it)->handleEvent(event);
                    return;
                }
            }
        }
        // 鼠标释放：将释放事件传给当前焦点元素
        else if (event.type == sf::Event::MouseButtonReleased &&
            event.mouseButton.button == sf::Mouse::Left)
        {
            if (focused) focused->handleEvent(event);
        }
        // 滚轮滚动：传给鼠标所在的元素
        else if (event.type == sf::Event::MouseWheelScrolled) {
            sf::Vector2f pt(event.mouseWheelScroll.x, event.mouseWheelScroll.y);
            for (auto& el : elements) {
                if (el->contains(pt)) {
                    el->handleEvent(event);
                    return;
                }
            }
        }
        // 键盘事件：传给当前焦点元素
        else if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
            if (focused) focused->handleEvent(event);
        }
    }

    // 更新所有元素（通常传入当前鼠标坐标）
    void update(const sf::Vector2f& mousePos) {
        for (auto& el : elements) {
            el->update(mousePos);
        }
    }

    // 绘制所有元素
    void draw(sf::RenderTarget& target) const {
        for (auto& el : elements) {
            el->draw(target);
        }
    }

private:
    std::vector<std::shared_ptr<UIElement>> elements;
    std::shared_ptr<UIElement> focused;  // 当前获得焦点的元素
};

#endif
