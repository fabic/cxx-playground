#ifndef SF_UI_ELEMENTS_H
#define SF_UI_ELEMENTS_H

#include <string>
#include <list>
#include <memory>
#include <type_traits>
#include <algorithm>

# include "dude/surface/cairo/surface.hpp"
# include "dude/util/pointers.hpp"
# include "dude/util/logging.hpp"
# include "dude/surface/maths/rectangle.hpp"

namespace sf {
  namespace ui {

    using std::string;
    using dude::ptr::inheritable_shared_from_this;

    typedef const string& string_cref;

    class Element;
    typedef std::shared_ptr< Element > ElementPtr;
    typedef std::list< ElementPtr >    ElementList;
    typedef ElementList&               ElementListRef;


    class ContainerTrait {
    protected:
      ElementPtr   parent_;
      ElementList _children;
    public:
      ContainerTrait(ElementPtr parent_);
      virtual ~ContainerTrait();

      ElementList::reference emplaceChildLast(ElementPtr&& elt);
      ElementList::reference appendChild(ElementPtr&& elt);
    };


    /**
     * \brief Base class for HTML-like elements.
     *
     * * MUST NOT: resort to std::make_shared< Concrete >(...) for instanciating
     *             any sub-type due to the inheritable_shared_from_this<...>
     *             base class magic.
     */
    class Element
        : public ContainerTrait,
          public inheritable_shared_from_this< Element >
    {
    public:
      typedef Element& self;
      typedef Element* self_ptr;

    protected:
      string         _id;
      Rectangle<>    _bounding_box;
      cairo::Surface _surface;

    public:
      explicit Element(ElementPtr parent_);

      virtual ~Element() override ;

      Element(const Element &) = delete;
      Element& operator=(const Element &) = delete;

      self_ptr id(string&& id) noexcept
      {
        this->_id = std::move(id);
        return this;
      };

      string_cref id() const noexcept { return this->_id; }

      cairo::Surface::self_ref surface() noexcept { return this->_surface; }

      Rectangle<> boundingBox() const noexcept { return this->_bounding_box; }

      self_ptr boundingBox(Rectangle<> rect) noexcept {
        this->_bounding_box = rect;
        return this;
      }

    protected:
//        virtual void _initChildrenElementsHierarchy();
    };

  } // ui ns
} // sf ns

#include "dude/surface/ui/elements-inlines.hpp"

#endif // SF_UI_ELEMENTS_H
