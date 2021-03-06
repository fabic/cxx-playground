#ifndef SF_UI_SURFACE_H
#define SF_UI_SURFACE_H

#include "dude/xcbx/window.hpp"
#include "dude/surface/text/TextBuffers.hpp"

namespace sf {
  namespace ui {

    using std::shared_ptr;
    using Xcb = xcb::Xcb;


    /**
     *
     */
    class Surface
        : public xcb::Window
    {
    public:
      typedef Surface * self_ptr;
      typedef xcb::Window parent;

    protected:
      std::shared_ptr< TextBuffers > textBuffers_;

      /// Currently displayed buffer.
      TextBuffer::pointer currentTextBuffer_ = nullptr ;

    public:
      Surface(shared_ptr< Xcb > xcb_, std::shared_ptr< TextBuffers > buffers_);

      virtual ~Surface() override;

      virtual self_ptr render() override;

      TextBuffer::pointer textBuffer_();

    protected:
    };


    // // // // // // // // // // // // // // // // // // // // // //


    inline
    TextBuffer::pointer
      Surface::textBuffer_()
    {
      if (currentTextBuffer_ == nullptr)
        currentTextBuffer_ = textBuffers_->any();
      return currentTextBuffer_ ;
    }


  } // ui ns.
} // sf ns.

#endif // SF_UI_SURFACE_H
