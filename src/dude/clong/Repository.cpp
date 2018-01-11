#include <cassert>
#include <stdexcept>
//#include <llvm/Support/Casting.h> // for dyn_cast<>
#include <clang/AST/Decl.h>

#include "Repository.hpp"

namespace clong {
namespace plugin {

  using namespace clang;

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  Artifact&
    Repository::Add(const Decl *D, DBIdentifier_t ID)
    {
      assert(D != nullptr);

      Key_t K = KeyOf( D );
      auto pair = Artifacts_.insert( std::make_pair(K, Artifact(D)) );

      bool ok = pair.second;
      if (!ok)
        throw std::runtime_error("Decl. is already in repository (!)");

      Map_t::iterator elt = pair.first ;
      Artifact& A = elt->second ;
      return A ;
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  Artifact&
    Repository::PushDeclContext(const DeclContext* DC, DBIdentifier_t ID)
    {
      assert(DC != nullptr);

      const Decl *D = dyn_cast< Decl >( DC );
      assert( D != nullptr && "Ouch! Could not obtain a Decl* by casting a DeclContext* (!)");

      Artifact& A = Add(D, ID);

      Artifact& B = DCStack_.Push( A );
      assert(&B == &A && "This isn't good.");

      return A ;
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  Artifact&
    Repository::PopDeclContext()
    {
      Artifact& A = DCStack_.Pop();
      return A ;
    }



} // plugin ns.
} // clong ns.
