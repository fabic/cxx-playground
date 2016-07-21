#ifndef FABIC_XCB_WINDOW_H
#define FABIC_XCB_WINDOW_H

#include "xcb.hpp"
//#include "fabic/surface/cairo.hpp"

TL_NS_BEGIN
namespace xcb {

/**
 * An XCB Window.
 */
  class Window {
  public:
    typedef Window &self;
    typedef Window *self_ptr;

  private:
    // Prevent inadvertent copies.
    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

  protected:
    xcb_shared_ptr xcb_;
    xcb_window_t windowXid;
    xcb_visualtype_t *visual = nullptr;

  public:

    explicit Window(xcb_shared_ptr xcb_);

    // fixme: drop this.
    Window(xcb_shared_ptr xcb_, xcb_window_t xid);

    virtual ~Window();

    /// Get the associated Xcb connection (shared ptr).
    inline xcb_shared_ptr xcb() { return this->xcb_; }

    /**
     * @return this window's XID.
     */
    xcb_window_t getXid() const;
    xcb_drawable_t getDrawableXid() const;
    xcb_visualid_t getVisualXid();

    self map();

    std::unique_ptr< xcb_get_window_attributes_reply_t >
      get_attributes();

    std::unique_ptr< xcb_get_geometry_reply_t >
      get_geometry();

    xcb_visualtype_t * getVisualType();

    virtual void handleEvent(const Event &event);

    virtual void handleEventExpose(
        uint16_t width,
        uint16_t height,
        uint16_t x,
        uint16_t y
    );

  protected:
    virtual self_ptr
    _create(
        uint16_t width,
        uint16_t height,
        window_shared_ptr parentWindow = nullptr
    );
  };

} // xcb ns.
TL_NS_END

#endif // FABIC_XCB_WINDOW_H