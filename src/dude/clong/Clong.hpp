#ifndef _PIMPL_PLUGIN_Clong_HPP
#define _PIMPL_PLUGIN_Clong_HPP

#include <set>
#include <clang/AST/ASTConsumer.h>
#include <clang/Frontend/CompilerInstance.h>

#include "Repository.hpp"
#include "lmdb.hpp"
#include "PQXXHelper.hpp"
#include "PPCallbacksTracker.hpp"

// Forward decls
namespace clang {
  class DeclContext;
  class Decl;
  class   TranslationUnitDecl;
  class NamedDecl;
  class   NamespaceDecl;
  class   TypeDecl;
  class     TypedefNameDecl;
  struct PrintingPolicy;
}

namespace clong {
namespace plugin {

  using namespace clang;

  // FIXME: Clang 5.0.1 -> 6.x : strange llvm::Type name conflict.
  //using Type = clang::Type;

  /**
   * The Clong ASTConsumer, this is where things happen.
   *
   * See `HandleTranslationUnit()` for the entry point.
   *
   * Implementation is scatterred among the various `clong-xxx.cpp` files.
   *
   * "Clong" is the sound it makes when it crashes Clang.
   */
  class Clong : public ASTConsumer {
  private:
    CompilerInstance &Instance;

    // FIXME (?): This is set by HandleTranslationUnit().
    const ASTContext *Context_ = nullptr;

    /// Our PostgreSQL database connectivity helper.
    /// TODO: Someday this might become a generic DB wrapper so that we may
    /// TODO: substitute backends, like e.g. SQLite to begin with.
    PQXXHelper PQXX_ ;

    /// The artifacts repository.
    Repository Repo_ ;

    /// FIXME: get rid of this.
    std::set<std::string> ParsedTemplates;

    /// For `PPCallbacksTracker`.
    std::vector<CallbackCall> CallbackCalls;
    llvm::SmallSet<std::string, 4> Ignore;

    // TODO: LMDB impl. still actual ?
    StringRef LMDBDatabasePathName = "./database.lmdb";
    ::lmdb::env LMDB_;

  public:
    /// Ctor
    /// The preprocessor "observer" `PPCallbacksTracker` is instanciated and
    // handed over to Clang.
    Clong(CompilerInstance &Instance,
                          std::set<std::string> ParsedTemplates,
                          const char *PQXXOptionsString);

    /// Dtor
    ~Clong() override;

    /**
     * Entry point: We're run somehow when the/a trans. unit is available.
     */
    void HandleTranslationUnit(ASTContext& context) override;

    /// Helper for getting the printing policy.
    /// (we'll probably need to instantiate a custom one since TypePrinter
    ///  outputs booleans as `_Bool` instead of `bool`.
    const PrintingPolicy& getPrintingPolicy() const;

    // Helper for getting a `StringRef` view into the given `SourceRange`.
    StringRef getSourceCode(SourceRange SR = SourceRange()) const;

    /// Helper that basically invokes `D->getSourceRane()`.
    StringRef getSourceCode(const Decl *D) const ;

    /// Helper that comes up with a name for a given `Decl *D`. Either it is-a
    /// `NamedDecl` from which we can read the `IdentifierInfo`, or it is one of
    /// those other non-named decl.
    StringRef getNameForDecl(const Decl *D) const ;

    /// Ref. to the artifacts repository.
    Repository& getRepository() { return Repo_ ; }

    /// Get the PostgreSQL DB helper.
    PQXXHelper& PQXX() { return PQXX_ ; }

  private:
    /**
     * <http://clang.llvm.org/doxygen/classclang_1_1ASTContext.html#a151571ff7d2efdb6b9b6026773916056>
     */
    void InvestigateASTContextTypes( ASTContext& context );

    /// Borrowed from Clang's tools-extra pp-trace.
    int OutputPreprocessorTrace( llvm::raw_ostream &OS );

    /// Helper for getting the FileEntry from whence this declaration is.
    const FileEntry* getFileEntry(const Decl& D) const;

    /// Helper that checks if the given declaration is from the "main file".
    bool isInMainFile(const Decl& D) const;

  private:
    /// "entry point" of the recursive descent over the AST of a translation
    /// unit.  Note that Decl/s are skipped until we reach one that belong to
    /// the "main file".
    bool TraverseTranslationUnitDecl(const TranslationUnitDecl *TU);

    /// Dispatches processing to specialized `TraverseXxxxDecl()` given the
    /// concrete type of `Decl *D`.
    bool TraverseDecl(const Decl *D);

    /// Helper for `TraverseDecl()` default switch-case: Handle Decl/s for
    /// which we haven't provided a traversal implementation yet.
    bool TraverseUnknownDecl(const Decl *D);

    /// Default DeclContext traversal method.
    bool TraverseDeclContext(const DeclContext *DC);

    /// Helper that invokes `TraverseDeclContext()`
    /// iff `Decl *D` is-a DeclContext.
    bool TraverseDeclContextIfAny(const Decl* D);

    bool TraverseNamespaceDecl(const NamespaceDecl *D);
    bool TraverseTypedefNameDecl(const TypedefNameDecl *D);

  private:
    bool InitLMDB();

    /**
     * Drops the schema first, creates tables `decl` & `decl_kind`
     * and invokes `DBPopulateDeclKinds()`
     */
    void InitPostgresDatabase();

    /**
     * Populate the `decl_kind` database with the constants from enumerations :
     * - IDs in the range [0 ... 99] are reserved.
     * - `Decl::Kind`        : +100-based.
     * - `Type::TypeClass`   : +200-based.
     * - `BuiltinType::Type` : +300-based.
     *
     * WARNING: These have been hard-coded on 2018-01-13.
     * - See file `resources/data-fixtures.sql`,
     * - which was generated from the output of `bin/list-all-clang-nodes.cpp`.
     * - ^ note that the authoritative data is _now in this_ method.
     *
     * Also a "Nil/null/void Decl Context" record is added to the `decl` table.
     *
     * And the BuiltinType/s are added _with their same ID from `decl_kind.id`
     * (and for obscur reasons, to the author himself: these were wired as
     * belonging to the Nil-decl-context).
     *
     * Do note that the starting sequence for the `decl.id` column was set
     * to 1000 when the table was created. See `InitPostgresDatabase()`.
     */
    void DBPopulateDeclKinds();

  };

} // plugin ns
} // clong ns


#endif // _PIMPL_PLUGIN_Clong_HPP
