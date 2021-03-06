
#include <cairo.h>
# include "dude/surface/ui/surface.hpp"

namespace sf {
  namespace ui {

    Surface::Surface(
        shared_ptr< Xcb > xcb_,
        std::shared_ptr< TextBuffers > buffers_
      )
        : xcb::Window( xcb_ ), // parent ctor.
          textBuffers_( buffers_ )
    {
      this->create(800, 450);
    }


    Surface::~Surface()
    { }


    Surface *
      Surface::render()
    {
      parent::render();

      cairo_t * cr = _surface.context().get();

      cairo_select_font_face(cr, "Monospace",
                             CAIRO_FONT_SLANT_NORMAL,
                             CAIRO_FONT_WEIGHT_NORMAL);

      cairo_font_extents_t font_extents;
      cairo_font_extents(cr, &font_extents);

      cairo_set_font_size(cr, 14);
      //cairo_set_source_rgba(cr, 184, 184, 184, 255);
      _surface.source_rgba(rgba<uint8_t>(216, 216, 216, 255));

      int lineNb = 0;
      double x = 2,
             y = 2 + font_extents.height + font_extents.descent ;

      Dimensions< double > surfaceDimensions( _surface.dimensions() );

      const auto max_y =
          surfaceDimensions.height()
              // We draw one more line past the height of viewport.
              + font_extents.height ;

      for(auto&& line : textBuffer_()->lines())
      {
        lineNb++;

        logtrace << "Surface::render(): Line #" << lineNb << ' '
                 << line.string() ;

        const auto str = line.string().c_str();

        cairo_text_extents_t text_extents;
        cairo_text_extents(cr, str, &text_extents);

        line.renderStates().position(x, y);

        if (! line.isBlank()) {
          cairo_move_to(cr, x, y);
          cairo_show_text(cr, str);
          y += std::max(text_extents.height, font_extents.height)
                + font_extents.descent ;
        }
        else {
          y += font_extents.height + font_extents.descent ;
        }

        // Reached bottom of viewport ?
        if (y > max_y) {
          logtrace << "Surface::render(): reached bottom of surface, rendered "
                   << lineNb << " lines, y = " << y;
          break;
        }
      }


      // HELLO WORLD !
      #if 1
      {
        const double bbox_height = font_extents.height + 2;
        cairo_rectangle(cr,
                        0, surfaceDimensions.height() - bbox_height,
                        surfaceDimensions.width(), bbox_height );
        _surface.source_rgba(rgba<uint8_t>(0, 0, 0, 232));
        cairo_fill(cr);

        cairo_move_to(cr, 2, _surface.dimensions().height() - 3);
        _surface.source_rgba(rgba<uint8_t>(216, 216, 216, 255));
        cairo_show_text(cr, "Hello world !");
      }
      #endif

      return this;
    }

  } // ui ns.
} // sf ns.
