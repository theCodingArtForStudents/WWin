#ifndef WPUSHBUTTON_H
#define WPUSHBUTTON_H

#include <functional>
#include <vector>
#include "wwin/wwidget.h"

/**
 * @brief WPushButton Класс самой обычной кнопки
 */
class WPushButton : public WWidget
{
private:
    /// Функции обратного вызова на событие клика
    std::vector< std::function<void(WMouseEvent*)> > _callbacks;

    int style() override;

protected:
    virtual bool mouseReleaseEvent(WMouseEvent *e);
public:
    WPushButton(WWidget* parent = nullptr);
    WPushButton(WString title, WWidget* parent = nullptr);
    int on_click(std::function<void(WMouseEvent *)> callback );
    bool nativeEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

#endif // WPUSHBUTTON_H
