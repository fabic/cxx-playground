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

      A.SetIndex( GetIndexOfLastArtifact() );

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

      DCStack_.push_back( A.GetIndex0() );

      return A ;
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  Artifact&
    Repository::PopDeclContext()
    {
      Artifact& A = CurrentDeclContext();
      DCStack_.pop_back();
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

  Artifact&
    Repository::CurrentDeclContext()
    {
      if (DCStack_.empty())
        throw clong_error("Repository: we have no current DeclContext on the stack.");

      size_t Index = DCStack_.back();
      Artifact& A = Artifacts_.GetVector()[ Index ].second ;

      assert( A.GetAsDeclContext() != nullptr );
      return A;
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  const Artifact&
    Repository::CurrentDeclContext() const
    {
      return const_cast<Repository *>(this)->CurrentDeclContext();
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  Artifact&
    Repository::PreviousDeclContext(unsigned int Distance)
    {
      size_t Last = DCStack_.size() - 1;

      assert(Last >= 0
          && "TODO: Handle case where DeclContext stack is empty (throw?).");

      assert(Distance <= Last && "TODO: Throw a banana or sthg?");

      size_t Index = DCStack_[ Last - Distance ];
      Artifact& A = Artifacts_.GetVector()[ Index ].second ;
      return A ;
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  bool
    Repository::isCurrentDeclContext(const DeclContext* DC) const
    {
      return CurrentDeclContext().GetAsDeclContext() == DC ;
    }

} // plugin ns.
} // clong ns.
