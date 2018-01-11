#include <stdexcept>

#include "Repository.hpp"

namespace clong {
namespace plugin {

  using namespace clang;

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  Artifact&
    Repository::Add(const Decl *D, DBIdentifier_t ID)
    {
      Key_t K = KeyOf( D );
      auto pair = Artifacts.insert( std::make_pair(K, Artifact(D)) );

      bool ok = pair.second;
      if (!ok)
        throw std::runtime_error("Decl. is already in repository (!)");

      Map_t::iterator elt = pair.first ;
      Artifact& A = elt->second ;
      return A ;
    }

  // void
  //   Repository::AddDeclContext(const DeclContext *DC)
  //   {
  //     const Decl *D = dyn_cast<Decl>( DC );
  //     assert( D != nullptr && "Ouch! Could not obtain a Decl* by casting a DeclContext* (!)");
  //     AddDecl( D );
  //   }
  //



} // plugin ns.
} // clong ns.
