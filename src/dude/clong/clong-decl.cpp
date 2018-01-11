#include <stdexcept>
#include <clang/AST/ASTContext.h>

#include "Clong.hpp"
#include "dude/util/Terminal.hpp"
#include "exceptions.hpp"

namespace clong {
namespace plugin {

  using namespace clang;

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  bool
    Clong::TraverseDecl(const Decl *D)
    {
      TPush log("TraverseDecl");

      *log << "- kind: " << D->getDeclKindName() << tendl
           << "- at: " << D << tendl;

      // Repo_.Add( D );

      switch(D->getKind())
      {
        case Decl::Namespace:
          if (!TraverseNamespaceDecl(cast<NamespaceDecl>(D)))
            return false;
          break;

        // Type < Tag
        case Decl::Enum:
        case Decl::Record:
          case Decl::CXXRecord:
            case Decl::ClassTemplateSpecialization:
              case Decl::ClassTemplatePartialSpecialization:
          return TraverseDeclContextIfAny( D );

        case Decl::TemplateTypeParm:
        case Decl::UnresolvedUsingTypename:
          throw not_implemented_error();

        case Decl::AccessSpec:
          *log << tmagenta << "Got one of those AccessSpec thing, skipping..."
               << tnormal << tendl;
          break;

        // Type < TypedefName
        case Decl::TypeAlias:
        case Decl::Typedef:
          if (!TraverseTypedefNameDecl(cast<TypedefNameDecl>(D)))
            return false;
          break;
          // return TraverseDeclContextIfAny( D );

        default: {
          const DeclContext *DC = dyn_cast< DeclContext >( D );
          log.heading("Unknown decl. kind:")
            << tbold << twhite << ' ' << D->getDeclKindName()
            << tcyan << (DC != nullptr ? " is-a DeclContext" : "")
            << tnormal << tendl;
          if (DC != nullptr) {
            if (!TraverseDeclContext( DC ))
              return false;
          } // if is DC.
        } // default.
      } //switch.

      return true;
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  bool
    Clong::TraverseDeclContextIfAny(const Decl* D)
    {
      const DeclContext *DC = dyn_cast< DeclContext >( D );
      if (DC == nullptr)
        return false;
      return TraverseDeclContext( DC );
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  bool
    Clong::TraverseDeclContext(const DeclContext *DC)
    {
      TPush log("TraverseDeclContext");

      if (DC == nullptr)
        throw clong_error("This ain't no DeclContext yo!");

      *log << "- kind: " << DC->getDeclKindName()
           << " ( " << DC << " ) "
           << tendl;

      // FIXME: TEMP. (?)
      // FIXME: perf.: at least 2 lookups happen here O(log2(N)).
      Artifact& DCArt = Repo_.isCurrentDeclContext(DC) ?
          Repo_.CurrentDeclContext()
        : Repo_.PushDeclContext( DC );

      // See `RecursiveASTVisitor<>::TraverseDeclContextHelper()` :
      //
      //  "BlockDecls and CapturedDecls are traversed through
      //   BlockExprs and CapturedStmts respectively."
      //
      // TODO: ^ Find out wtf that is about?

      for (auto *Child : DC->decls())
      {
        if ( isa<BlockDecl>(Child) ) {
          *log << tred << "[i] Skipping BlockDecl.\n";
          continue;
        }
        else if ( isa<CapturedDecl>(Child) ) {
          *log << tred << "[i] Skipping CapturedDecl.\n";
          continue;
        }

        bool stop = TraverseDecl( Child ) == false;
        // TODO
        // if (stop)
        //   return false;
      }

      // FIXME: yes/no? We're popping off a DC that was possibly pushed
      // FIXME: not by us.
      Artifact& DCPoped = Repo_.PopDeclContext();

      //assert( &DCPoped == &DCArt &&
      if (DCPoped.GetDecl() != DCArt.GetDecl()) {
        *log << tred
          << ( "Dude, for some obscur reason we popped a different DeclContext*"
               " off the DeclContextStack, and this isn't a good thing."  )
          << tendl
          << tnormal << " `-> "
                     << tyellow << DCPoped.GetDecl()->getDeclKindName()
                     << " ( " << DCPoped.GetDecl() << " ) " << tendl
          << twhite  << " `--expected--> "
                     << tyellow << DCArt.GetDecl()->getDeclKindName()
                     << " ( " << DCArt.GetDecl() << " ) " << tendl
          << tnormal << " ` DeclContext stack depth: "
                     << tcyan << Repo_.GetDeclContextStackDepth()
          << tnormal << tendl;
      }

      return true;
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

} // plugin ns.
} // clong ns.
