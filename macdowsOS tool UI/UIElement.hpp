#ifndef UI_ELEMENT_HPP
#define UI_ELEMENT_HPP

#include <SFML/Graphics.hpp>

// UIElement: 抽象基类，定义所有 UI 组件的接口
class UIElement {
public:
    virtual ~UIElement() {}
    // 在每帧更新元素状态（如鼠标悬停检测）
    virtual void update(const sf::Vector2f& mousePos) = 0;
    // 处理事件（鼠标/键盘等）
    virtual void handleEvent(const sf::Event& event) = 0;
    // 绘制到渲染目标
    virtual void draw(sf::RenderTarget& target) const = 0;
    // 判断鼠标点是否在此元素范围内
    virtual bool contains(const sf::Vector2f& point) const = 0;
    // 设置元素位置（会影响内部图形对象）
    virtual void setPosition(const sf::Vector2f& pos) = 0;
    // 获取元素尺寸（可用于布局），未必每个元素都需要使用
    virtual sf::Vector2f getSize() const = 0;
    // 获取元素位置（用于容器拖动时调整子元素）
    virtual sf::Vector2f getPosition() const = 0;
};
#endif
