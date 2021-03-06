#include <stdexcept>
#include <clang/AST/ASTContext.h>

#include "Clong.hpp"
#include "dude/util/Terminal.hpp"

namespace clong {
namespace plugin {

  using namespace clang;

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  bool
    Clong::TraverseNamespaceDecl(const NamespaceDecl *D)
    {
      TPush log( D->getName() );

      auto ID = PQXX_.Insert( R"(
        INSERT INTO decl (kind, context_id, name, fq_name)
        VALUES ($1, $2, $3, NULL)
        RETURNING id ;)",
          D->getKind()+100,
          Repo_.CurrentDeclContext().getDatabaseID(),
          D->getName().str() );

      *log << "- ID: " << ID << tendl;

      Repo_.PushDeclContext(D, ID);

      if (!TraverseDeclContext( D ))
        return false;

      return true;
    }


} // plugin ns.
} // clong ns.
