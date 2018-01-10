#include <stdexcept>
#include <clang/AST/ASTContext.h>

#include "Clong.hpp"
#include "dude/util/Terminal.hpp"

namespace clong {
namespace plugin {

  using namespace clang;


  // [overriden, entry point]
  void
    Clong::HandleTranslationUnit(ASTContext& context)
    {
      TPush log ("HEY!");

      // FIXME: Can we (an instance of this Clong) be invoked several times for
      // FIXME: processing different TUs ?
      assert(Context_ == nullptr);

      Context_ = &context;

      *log << "`-> getASTAllocatedMemory() -> "
           << context.getASTAllocatedMemory() << tendl;
      *log << "`-> getSideTableAllocatedMemory() -> "
           << context.getSideTableAllocatedMemory() << tendl;

      try {
        if (true)
          InvestigateASTContextTypes(context);

        if (false)
          InitLMDB();

        if (true)
          InitPostgresDatabase();

        const TranslationUnitDecl *TU = context.getTranslationUnitDecl();

        if (!TraverseTranslationUnitDecl( TU ))
          throw std::runtime_error("TraverseTranslationUnitDecl( TU ) "
                                   "returned false ?!");

        if(false)
          OutputPreprocessorTrace( *log );
      }
      catch(const std::exception &ex) {
        *log << ex;
      }
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  bool
    Clong::TraverseTranslationUnitDecl(const TranslationUnitDecl *TU)
    {
      TPush log;

      Repo_.Add( TU );
      DCStack_.Push( TU );

      pqxx::transaction<> TXN( PQXX_ );

      pqxx::row row = TXN.exec_params1(R"(
        INSERT INTO decl (kind, context_id, name, fq_name)
        VALUES ($1, NULL, $2, NULL)
        RETURNING id ;)",
          2, "HEY!"
          );
      auto id = row[0].as<unsigned int>();

      *log << "- TU id: " << id << tendl;

      for (auto *Child : TU->decls())
      {
        // Skip Decl/s that aren't coming from the "main file".
        if (!isInMainFile( *Child )) {
          // Note that we do want to process those few decls that have no file
          // (typically some compiler builtin types that get injected early on
          // into translation units).
          const FileEntry* FE = getFileEntry( *Child );
          if (FE != nullptr) {
            log.heading(" TU ") << " Skipping decl. : "
                                << Child->getDeclKindName()
                                << '(' << Child << ')'
                                << " [ " << FE->getName() << " ] "
                                << tendl;
            continue;
          }
        }

        // See TraverseDeclContext() about this.
        if (isa<BlockDecl>(Child) || isa<CapturedDecl>(Child))
          throw std::runtime_error("Hummm... that was unexpected -_-");

        TraverseDecl( Child );
      }

      return true;
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

} // plugin ns.
} // clong ns.
