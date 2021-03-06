#include "wwidget.h"
#include "wwin/wapplication.h"
#include "wwin/helpers/winapiwindowbuilder.h"
#include "wwin/wscreen.h"

#include <iostream>

int WWidget::_componentCount = 0; /// < Количество компонентов в системе

/**
 * @brief WWidget::hwnd получить хендл окна виджета
 * @return
 */
HWND WWidget::hwnd() const
{
    return _hwnd;
}

/**
 * @brief WWidget::hwnd установить хендл окнна виджета
 * @param hwnd
 */
void WWidget::hwnd(HWND hwnd)
{
    _hwnd = hwnd;
}

/**
 * @brief WWidget::parentHwnd получить хендл родительского окна
 * @return
 */
HWND WWidget::parentHwnd() const
{
    HWND p_hwnd = HWND_DESKTOP;
    WWidget *parent = parentWidget();
    if( parent && parent->hwnd() ){
        p_hwnd = parent->hwnd();
    }
    return p_hwnd;
}

/**
 * @brief WWidget::parentWidget получить родительский виджет
 * @return родительский виджет или nullptr
 */
WWidget* WWidget::parentWidget() const
{
    WObject* parent = WObject::parent();
    if( parent && parent->type() == WObjectType::Widget ){
        return static_cast<WWidget*>( parent );
    }
    return nullptr;
}

/**
 * @brief WWidget::cid Получить ID компонента
 * @return id компонента
 */
WORD WWidget::cid() const
{
    return _cid;
}

/**
 * @brief WWidget::style Стиль виджета.
 * Стили для главного окна приложения
 * @return
 */
int WWidget::style()
{
    return WS_OVERLAPPEDWINDOW | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
}

/**
 * @brief WWidget::setStyle установить стиль виджета
 * @warning Не работает
 * \todo реализовать метод
 * @param style2
 */
void WWidget::setStyle(int style2)
{
//    SetWindowLong(this->hwnd(), GWL_STYLE, style);
//    SendMessage(this->hwnd(), ES_, (WPARAM)GWL_STYLE, 0);
//        SetDlgItemInt(this->hwnd(), this->cid(), ES_PASSWORD, false);
  ///  SendMessage ((HWND) lParam, BM_SETCHECK, !SendMessage ((HWND) lParam, BM_GETCHECK, 0, 0), 0) ;
//    DWORD style = GetWindowLong(this->hwnd(), GWL_STYLE);
//    SetWindowLong(this->hwnd(), GWL_STYLE, style | ES_PASSWORD);
//    SetWindowLongPtr(this->hwnd(), GWL_EXSTYLE, 0);
//    UpdateWindow(this->hwnd());
//    SetWindowPos(this->hwnd(), nullptr, _x, _y, _width, _height, 0);
//    UpdateWindow( this->parentHwnd() );
//    MessageBox(this->hwnd(), L"TEST", L"TEST", MB_OK);
}
/**
 * @brief WWidget::initWndClass инициализировать окно класса.
 * @warning Обязательно должен быть вызван в конструкторе при унаследовании данного класса.
 * @param className - класс окна
 * @return успех инициализации окна
 */
bool WWidget::initWndClass(WString className)
{
    _className = className;
    HWND x = WinApiWindowBuilder()
         .className( _className )
         .title( this->title() )
         .style( this->style() )
         .geometry( _x, _y, _width, _height )
         .parent( this->parentHwnd() )
         .hinstance( wApp->getHinstance() )
         .param( reinterpret_cast<LPVOID>( _windowParams ) )
         .menu( reinterpret_cast<HMENU>( this->cid() ) )
         .build();
    this->hwnd(x);
    wApp->addComponent(this);

    return (x != nullptr);
}

/**
 * @brief WWidget::mouseReleaseEvent событие отпускания кнопки мыши
 * @param e - экземпляр WMouseEvent
 * @return WEvent::isAccepted()
 */
bool WWidget::mouseReleaseEvent(WMouseEvent *e)
{
    return e->isAccepted();
}

/**
 * @brief WWidget::mouseReleaseEvent событие двойного клика
 * @param e - экземпляр WMouseEvent
 * @return WEvent::isAccepted()
 */
bool WWidget::mouseDoubleClickEvent(WMouseEvent *e)
{
    return e->isAccepted();
}

/**
 * @brief WWidget::mouseReleaseEvent событие изменения виджета
 * @param e - экземпляр WEvent
 * @return WEvent::isAccepted()
 */
bool WWidget::changeEvent(WEvent *e)
{
    return e->isAccepted();
}

/**
 * @brief WWidget::mouseReleaseEvent обработка потока событий
 * @param e - экземпляр WEvent
 * @return WEvent::isAccepted()
 */
bool WWidget::event(WEvent *e)
{
    if( e->type() == WEvent::Type::MouseReleaseEvent ){
        this->mouseReleaseEvent( static_cast<WMouseEvent*>(e) );
    }
    return WObject::event(e);
}

/**
 * @brief WWidget::WWidget Конструктор с инииализацией базового класса и компонента окна
 * @param parent - родительское окно
 * @param params - параметры окна
 */
WWidget::WWidget(WWidget *parent, int params)
    : WObject(parent), _windowParams(params)
{
    _cid = _componentCount++;
    this->setType(WObjectType::Widget);

    if( parent == nullptr ) {
        this->initWndClass(L"WWIDGET");
    }
}

/**
 * @brief WWidget::~WWidget Деструктор с удалением компонента из системы
 */
WWidget::~WWidget()
{
    wApp->removeComponent(this);
    DestroyWindow(this->hwnd());
}

/**
 * @brief WWidget::show отобразить виджет
 * @warning Если WWidget::initWndClass ещё небыло вызвано и _hwnd не установлен, вызовет самостоятельно
 */
void WWidget::show()
{
    if( ! this->hwnd() ) {
       this->initWndClass(L"WWIDGET");
    }
    ShowWindow( this->hwnd(), _windowParams );
    UpdateWindow( this->hwnd() );
}

/**
 * @brief WWidget::setGeometry установить размеры и положение виджета
 * @param x
 * @param y
 * @param width
 * @param height
 */
void WWidget::setGeometry(int x, int y, int width, int height)
{
    _x = x;
    _y = y;

    // Можно проще
    if( width < _minWidth ) {
        width = _minWidth;
    } else if( width > _maxWidth ) {
        width = _maxWidth;
    }
    if( height < _minHeight ) {
        height = _minHeight;
    } else if( height > _maxHeight ) {
        height = _maxHeight;
    }

    _width = width;
    _height = height;

    SetWindowPos(this->hwnd(), nullptr, _x, _y, _width, _height, 0);

}

/**
 * @brief WWidget::title получить заголовок виджета
 * @return WString - заголовок виджета
 */
WString WWidget::title() const
{
    return _title;
}

/**
 * @brief WWidget::setTitle Установить заголовок виджета
 * @param title - заголовок виджета
 */
void WWidget::setTitle(const WString &title)
{
    _title = title;
    SetWindowText(this->hwnd(), _title.c_str());
}

/**
 * @brief WWidget::nativeEvent Обработка нативных системных событий и их преобразование
 * в систему событий WWin
 * @param hWnd
 * @param message
 * @param wParam
 * @param lParam
 * @return
 */
bool WWidget::nativeEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  if( message == WM_DESTROY ){
    PostQuitMessage( EXIT_SUCCESS );
    return true;
  }
  if(WM_SIZE == message || WM_MOVE == message) {
    RECT rect;
    if( ! GetWindowRect(hWnd, &rect) ){
      /// \todo process error
    }

    _x = rect.left;
    _y = rect.top;
    _width = rect.right-rect.left;
    _height = rect.bottom - rect.top;
    return true;
  }
  if (WM_PAINT) {
    /// \todo repaint something
  }

  return WObject::nativeEvent(hWnd, message, wParam, lParam);
}

/**
 * @brief WWidget::setFocus Установить фокус на виджет
 */
void WWidget::setFocus()
{
    SetFocus( this->hwnd() );
}

