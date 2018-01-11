#ifndef _PIMPL_PLUGIN_Repository_HPP
#define _PIMPL_PLUGIN_Repository_HPP

#include <functional>
#include <memory>

// #include <llvm/ADT/MapVector.h>
// ^ TEMP. imported :
#include "dude/llvm/MapVector.hpp"

namespace clang {
  class DeclContext;
  class Decl;
  class   TranslationUnitDecl;
  class NamedDecl;
  class   NamespaceDecl;

  class Type;
  class TypeLoc;
}

namespace clong {
namespace plugin {

  using namespace clang;

  // FIXME: Clang 5.0.1 -> 6.x : strange llvm::Type name conflict.
  using Type = clang::Type;

  /**
   *
   */
  class Artifact {
  public:
    using DBIdentifier_t = unsigned int ;
  private:
    const Decl*    Decl_ ;
    DBIdentifier_t ID_ ;

  public:
    explicit Artifact(const Decl* D, DBIdentifier_t ID = 0)
      : Decl_(D)
      , ID_(0)
    { /* noop */ }
  };


  /**
   * * http://en.cppreference.com/w/cpp/utility/functional/reference_wrapper
   */
  class DeclContextStack
          : public llvm::SmallVector<
                           std::reference_wrapper< Artifact >,
                           /* nElt= */ 32 >
  {
  public:
    Artifact& Push( Artifact& DC ) {
      push_back( DC );
      return Current();
    }

    Artifact& Pop() {
      auto& A = Current();
      pop_back();
      return A;
    }

    Artifact& Current() const {
      return back().get();
    }

    // TODO: Parent() ?
    // TODO: find closest namespace context.
    // TODO: find closest tag struct/class/... context.
  };


  /**
   *
   */
  class Repository {
  public:
    /// Key type may be a `const Decl*` or `const Type*`.
    using Key_t = size_t;
    using Map_t = MapVector< Key_t, Artifact > ;
    using DBIdentifier_t = Artifact::DBIdentifier_t ;

  public:
    ///
    static Key_t KeyOf(const Decl *D) {
      return reinterpret_cast< size_t >( D );
    }

  private:
    Map_t Artifacts_ ;
    DeclContextStack DCStack_ ;

  public:
    /// Return the `MapVector(&)` that stores the collected code artifacts.
    Map_t& getArtifactsMap() { return Artifacts_ ; }

    // TODO: return newly inserted reference.
    Artifact& Add(const Decl* D, DBIdentifier_t ID = 0);

    Artifact& PushDeclContext(const DeclContext* DC, DBIdentifier_t ID = 0);
    Artifact& PopDeclContext();

    void Add(const Type* T);
    void Add(const TypeLoc* T);
  };


} // plugin ns
} // clong ns


#endif // _PIMPL_PLUGIN_Repository_HPP
