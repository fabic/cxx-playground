#include <cassert>
#include <stdexcept>
//#include <llvm/Support/Casting.h> // for dyn_cast<>
#include <clang/AST/Decl.h>

#include "Repository.hpp"
#include "dude/util/Terminal.hpp"
#include "Clong.hpp"

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

  /// Ctor -- beware here: we're being constructed as part of Clong's ctor.
  Repository::Repository(Clong& CL)
    : Clong_(CL)
  {}

  void Repository::Init()
  {
    // TEMP: Add the NIL artifact.
    // TODO: fetch from database ?
    Key_t K = 0;
    auto pair = Artifacts_.insert( {K, Artifact()} );
    bool ok = pair.second;
    if (!ok)
      throw std::runtime_error("Failed to insert the NIL artifact -_-");
    Artifact& A = pair.first->second ;
    A.SetIndex( 0 );

    //
    // Read the builtin artifacts from the database and add "stub artifacts"
    // for these to the map. FIXME: simple impl. : we assume that the builtin
    // types were added to the `decl` table with `decl.id` == `decl_kind.id`.
    //
    {
      TPush log("Repository : fetching the builtin types artifacts.");
      pqxx::result rset = Clong_.PQXX().Exec(R"(
        SELECT D.id AS builtin_decl_id,
               K.builtin_type_kind, -- FIXME:UNUSED.
               D.name
        FROM decl D
        INNER JOIN decl_kind K ON D.kind = K.id
        WHERE D.kind = $1 AND D.context_id = 0
        ORDER BY D.id ASC ;
        )",
          // hard-coded BuiltinType Type kind.
          200
          );

      for(const pqxx::row& row : rset)
      {
        DBIdentifier_t BID = row[0].as<int>() ;
        Key_t K = BID;
        *log << "- Added ID# " << BID
             << " : " << row[2].as<const char*>()
             << tendl;
        auto pair = Artifacts_.insert( {K, Artifact(BID)} );
        bool ok = pair.second;
        if (!ok)
          throw std::runtime_error("Key is already exists in repository (!)");
        Map_t::iterator elt = pair.first ;
        Artifact& A = elt->second ;
        A.SetIndex( GetIndexOfLastArtifact() );
      }
    }
  } // Repository::Init() //

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  Artifact&
    Repository::Add(const Decl *D, DBIdentifier_t ID)
    {
      assert(D != nullptr);

      Key_t K = KeyOf( D );
      auto pair = Artifacts_.insert( {K, Artifact(D, ID)} );

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
    Repository::Add(const Type *T, DBIdentifier_t ID)
    {
      assert(T != nullptr);

      Key_t K = KeyOf( T );
      auto pair = Artifacts_.insert( {K, Artifact(T, Qualifiers(), ID)} );

      bool ok = pair.second;
      if (!ok)
        throw std::runtime_error("Type is already in repository (!)");

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

      assert((!Has(D) || ID == 0)
          && "FIXME: this func. shall not have this ID arg.");

      Artifact& A = Has(D) ? Get(D) : Add(D, ID);

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

  Artifact&
    Repository::Get(Key_t K)
    {
      Map_t::iterator it = Artifacts_.find( K );
      bool Found = it != Artifacts_.end();
      if (! Found)
        throw clong_error("Repository::Get(Key_t): No such Artifact (!)");
      return it->second ;
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  Artifact&
    Repository::GetNilArtifact()
    {
      Map_t::iterator it = Artifacts_.find( 0 );
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
