#include <cassert>
#include <stdexcept>
//#include <llvm/Support/Casting.h> // for dyn_cast<>
#include <clang/AST/Decl.h>

#include "Repository.hpp"
#include "dude/util/Terminal.hpp"

namespace clong {
namespace plugin {

  using namespace clang;

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  bool
    Artifact::isDeclContext() const
    {
      const DeclContext* DC = dyn_cast< DeclContext >( Decl_ );
      return DC != nullptr ;
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  const DeclContext*
    Artifact::GetAsDeclContext() const
    {
      const DeclContext* DC = dyn_cast< DeclContext >( Decl_ );
      if (DC == nullptr) {
        terrs() << tred << "Hey! DC ain't no DC! :" << Decl_
          << " is-a " << Decl_->getDeclKindName()
          << tnormal << tendl;
        throw clong_error( "Artifact::GetAsDeclContext(): "
                           "Decl is-not-a DeclContext (!!)" );
      }
      return DC ;
    }

  // ~  -  ~  -  ~  -  ~  -  ~  -  ~  -  ~  -  ~  -  ~  -  ~  -  ~  -  ~  -  ~
  // ~  -  ~  REPOSITORY  -  ~  -  ~  -  ~  -  ~  -  ~  -  ~  -  ~  -  ~  -  ~

  Artifact&
    Repository::Add(const Decl *D, DBIdentifier_t ID)
    {
      assert(D != nullptr);

      Key_t K = KeyOf( D );
      auto pair = Artifacts_.insert( {K, Artifact(D)} );

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

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  bool
    Repository::Has(const Decl* D) const
    {
      Map_t::const_iterator it = Artifacts_.find( KeyOf(D) );
      bool Found = it != Artifacts_.end();
      return Found ;
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  Artifact&
    Repository::Get(const Decl* D)
    {
      Map_t::iterator it = Artifacts_.find( KeyOf(D) );
      bool Found = it != Artifacts_.end();
      if (! Found)
        throw clong_error("Repository::Get(): No such Decl* (!)");
      return it->second ;
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  bool
    Repository::HasDeclContext(const DeclContext* DC)
    {
      const Decl *D = dyn_cast< Decl >( DC );
      assert( D != nullptr && "Ouch!");
      return Has( D );
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  Artifact&
    Repository::GetDeclContext(const DeclContext* DC)
    {
      const Decl *D = dyn_cast< Decl >( DC );
      assert( D != nullptr && "Ouch!");
      return Get( D );
    }


  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  bool
    Repository::isCurrentDeclContext(const DeclContext* DC) const
    {
      return DCStack_.isCurrent( DC );
    }

} // plugin ns.
} // clong ns.
