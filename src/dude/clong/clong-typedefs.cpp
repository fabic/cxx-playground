#include <stdexcept>
#include <clang/AST/ASTContext.h>

#include "Clong.hpp"
#include "TypeResolver.hpp"
#include "dude/util/Terminal.hpp"

namespace clong {
namespace plugin {

  using namespace clang;

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  bool
    Clong::TraverseTypedefNameDecl(const TypedefNameDecl *D)
    {
      TPush log("Typedef / alias:");

      std::string Name = D->getName().str();
      std::string SourceCode = getSourceCode( D->getSourceRange() ).str();

      *log << "- name: " << tcyan << Name << tnormal << tendl;
      *log << "- code: " << SourceCode << tendl;

      auto ID = PQXX_.Insert( R"(
        INSERT INTO decl (kind, type_id, context_id, name, fq_name, code)
        VALUES ($1, $2, $3, $4, NULL, $5)
        RETURNING id ;)",
          D->getKind()+100,
          0,
          Repo_.CurrentDeclContext().getDatabaseID(),
          Name, SourceCode );

      *log << "- ID: " << ID << tendl;

      Repo_.Add(D, ID);

      const TypeSourceInfo *TSI = D->getTypeSourceInfo();
      assert(TSI != nullptr && "Woups!");

      TypeResolver TR ( *this );
      TR.Resolve( TSI );

      return true;
    }

} // plugin ns.
} // clong ns.
