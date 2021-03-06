# include "dude/xcb+.hpp"

#include <random>

/**
 * MAIN !
 */
int main(int argc, const char *argv[])
{
  loginfo << argv[0]
          << "Displays an X window through XCB+,"
              " and setup an XCB Cairo Surface.";

  using sf::xcb::Xcb;
  using sf::xcb::Window;
  using sf::cairo::Surface;

  auto xcb_ = std::make_shared< Xcb >();

  auto win_ = std::make_shared< Window >( xcb_ );

  win_->create(320, 240, nullptr);
  win_->map();

  win_->setHandleExposeCallback(
      [](Window &win, uint16_t width, uint16_t height) {
        logtrace << "HEY!! " << width << 'x' << height
                 << " / Cairo surface status : " << win.surface().status();
        std::random_device r;

        auto sf = win.surface();

        auto dim = sf.dimensions();

        double a = r() / (double) r.max();
        double b = r() / (double) r.max();
        double x = a * dim.width();
        double y = b * dim.height();

        sf.move_to(sf::Vector<>(x, y));
        sf.rel_line_to(sf::Vector<>(64, 92));
        sf.set_line_width(10);
        sf.source_rgba(sf::rgba<double>(1.0, .9, .7, .75));
        sf.stroke();
      }
  );

  xcb_->run();

  return 0;
}
