#ifndef _PIMPL_PLUGIN_Repository_HPP
#define _PIMPL_PLUGIN_Repository_HPP

#include <functional>
#include <memory>

#include <clang/AST/Type.h> // for Qualifiers.
// #include <llvm/ADT/MapVector.h>
// ^ TEMP. imported :
#include "dude/llvm/MapVector.hpp"
#include "types.hpp"
#include "exceptions.hpp"

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

  // Fwd decl.
  class Repository ;

  // Fwd Decl.
  class Clong ;

  /**
   * A "code artifact".
   *
   * TODO: Copy ctor/assign & Move -abilities (?) maybe prevent copying?
   */
  class Artifact {
    // Repository is friend so that it can access private
    // methods `SetIndex()` and `GetIndex0()`.
    friend class Repository ;

  private:
    /// The Clang `Decl` this artifact is bound to.
    const Decl* Decl_ ;

    const Type*      Type_ ;
    const Qualifiers Quals_ ;

    /// The database identifier, 0 meaning "not defined".
    DBIdentifier_t ID_ ;

    /// Unique 1-based index of this artifact within the Repository::Map_t
    /// vector.
    size_t Index_ ;

  private:
    /// [private] Used by Repository::Add() for setting this artifact's index
    /// which is known once it has been inserted into the Repository artifacts
    /// map.
    Artifact& SetIndex(size_t I) {
      Index_ = I ;
      return *this;
    }

    /// Return the 0-based index of this artifact inside the Repository
    /// MapVector of Decl/s.  This was made private, with the Repository
    /// as a friend class.
    size_t GetIndex0() const { return Index_ ; }

  public:
    /// Ctor.
    explicit Artifact(const Decl* D, DBIdentifier_t ID = 0)
      : Decl_(D)
      , ID_(0)
      , Index_(0)
    {
      assert(Decl_ != nullptr);
    }

    /// Ctor.
    explicit Artifact(const Type* T, Qualifiers Q, DBIdentifier_t ID = 0)
      : Decl_(nullptr)
      , Type_(T), Quals_(Q)
      , ID_(0), Index_(0)
    {
      assert(Type_ != nullptr);
    }

  private:
    /// Ctor : Nil artifact.
    Artifact()
      : Decl_(nullptr), Type_(nullptr), Quals_(), ID_(0), Index_(0)
    { /* noop */ }

  public:

    /// Returns _this_ 1-based artifact number (which is set by the Repository
    /// upon insertion in its map of artifacts).  Note: it happens that this
    /// number is _the index within the artifacts vector container, +1_.
    size_t GetNumber() const {
      assert(Index_ > 0 && "Artifact index wasn't initialized ?");
      return Index_ + 1 ;
    }

    /// Shall not return nullptr.
    const Decl* GetDecl() const {
      return Decl_ ;
    }

    /// Helper for casting the Decl* to one of its subtypes.
    template<typename DeclSubType>
      const DeclSubType *
        GetDeclAs() const {
          return cast< DeclSubType >( Decl_ ) ;
        }

    /// Helper that returns a DeclContext if Decl is one, else throws!
    const DeclContext* GetAsDeclContext() const;

    /// True if `Decl_` is-a `DeclContext`.
    bool isDeclContext() const;

    bool hasDatabaseIdentifier() const { return ID_ > 0 ; }

    /// May return 0, is it ok ?
    DBIdentifier_t getDatabaseID() const { return ID_ ; }

    Artifact& setDatabaseID(DBIdentifier_t ID) {
      assert( ID > 0 ); // we do not allow setting it back to 0.
      ID_ = ID;
      return *this;
    }
  };


  /**
   * Repository that indexes Decl/s and Type/s wrapped as `Artifact`/s
   * into a map.
   *
   *
   * TODO: have a Clong& ref. so that we may query the DB for stuff, specif.
   * for querying Artifacts for builtin types.
   */
  class Repository {
  public:
    /// Key type may be a `const Decl*` or `const Type*` (?)
    using Key_t = size_t;
    using Map_t = MapVector< Key_t, Artifact > ;
    using DeclContextStack_t = llvm::SmallVector< size_t, /* nElt= */ 32 > ;

  public:
    /// Comes up with a key for this `Decl*` for insertion in the artifacts map.
    /// Note: we're using the `Decl*` memory address.
    static Key_t KeyOf(const Decl *D) {
      return reinterpret_cast< size_t >( D );
    }

    /// Likewise for Type/s.
    static Key_t KeyOf(const Type *T) {
      return reinterpret_cast< size_t >( T );
    }

  private:
    /// Reference to the owning Clong.
    Clong& Clong_;

    /// The artifacts map. Note that it is a `_llvm::MapVector<>` which is a
    /// `llvm::DenseMap<>` along with a `std::vector` that preserve information
    /// about the insertion order.
    Map_t  Artifacts_ ;

    /// A stack of integers that are indexes into the `Artifacts_` map, for
    /// DeclContext artifacts.
    DeclContextStack_t  DCStack_ ;

  private:
    /// For the impl. of Add() where we need to `Artifact::SetIndex()`.
    size_t GetIndexOfLastArtifact() const {
      // See ctor: we ensure there's at least one "NIL" artifact.
      // if (Artifacts_.empty())
      //   throw clong_error("GetIndexOfLastArtifact(): we have no artifacts (empty map).");
      return Artifacts_.GetVector().size() - 1 ;
    }

  public:
    /// Ctor
    explicit Repository(Clong& CL);

    /// Return the `MapVector(&)` that stores the collected code artifacts.
    Map_t& getArtifactsMap() { return Artifacts_ ; }

    /// Add a Clang `Decl*` to the artifacts map.
    Artifact& Add(const Decl* D, DBIdentifier_t ID = 0);
    Artifact& Add(const Type* T, DBIdentifier_t ID = 0);

    // TODO: impl.
    void Add(const TypeLoc* T);

    /// True if we have an artifact for this `Decl*`.
    bool Has(const Decl* D) const;

    /// Fetch the artifact that corresponds to this `Decl*`.
    /// Probably O(log2(n)).
    Artifact& Get(const Decl* D);

    /// Return the NIL artifact.
    Artifact& GetNilArtifact();

    /// Convenience helper that invokes `Has()`.
    /// fixme: rename as isKnownDeclContext ?
    /// fixme: ^ we're checking the if it's in the MapVector here,
    /// fixme: ^ not if it is in the DeclStack.
    bool HasDeclContext(const DeclContext* DC);

    /// Convenience helper that invokes `Get()`, so that we do not have to
    /// dyn_cast<>() here'n'there.
    Artifact& GetDeclContext(const DeclContext* DC);

    ///{ Convenience helpers around the DeclContext stack.

    /// Get the current "top-most" DeclContext artifact that is on the stack.
    Artifact& CurrentDeclContext();
    const Artifact& CurrentDeclContext() const;

    Artifact& PreviousDeclContext(unsigned int distance = 1);

    /// Tells whether DC is the current DeclContext at the top of the stack.
    bool isCurrentDeclContext(const DeclContext* DC) const;

    /// Push a DC onto the top of the stack.
    Artifact& PushDeclContext(const DeclContext* DC, DBIdentifier_t ID = 0);

    /// Pop the DC at stack top.
    Artifact& PopDeclContext();

    /// Return the current depth of the DeclContex stack, i.e. the current
    /// nesting level.
    size_t GetDeclContextStackDepth() const { return DCStack_.size(); }

    ///}
  };


} // plugin ns
} // clong ns


#endif // _PIMPL_PLUGIN_Repository_HPP
