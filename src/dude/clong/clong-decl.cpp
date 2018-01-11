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

      switch( D->getKind() )
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

        // DEFAULT //
        // TODO: if instance of NamedDecl / TypeDecl / etc...
        default: {
          const DeclContext *DC = dyn_cast< DeclContext >( D );

          log.heading("Unknown decl. kind:")
            << tbold << tyellow << ' ' << D->getDeclKindName()
            << tnormal << " ( " << D << " ) "
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

      // TEMP(?): Baked unknown DCs now.
      if (! DCArt.hasDatabaseIdentifier()) {
        std::string Name ( DCArt.GetDecl()->getDeclKindName() );

        const NamedDecl *ND = DCArt.GetDeclAs< NamedDecl >();
        if (ND != nullptr) {
          Name += " -name-> ";
          Name += ND->getNameAsString();
        }

        Artifact& PrevDC = Repo_.PreviousDeclContext();

        *log << "- About to baked DC with temporary name: "
             << twhite << Name << tnormal << tendl ;

        auto ID = PQXX_.Insert( R"(
          INSERT INTO decl (kind, context_id, name, fq_name)
          VALUES ($1, NULLIF($2,0), $3, NULL)
          RETURNING id ;)",
            999,
            PrevDC.getDatabaseID(),
            Name );

        DCArt.setDatabaseID( ID );

        *log << "- Baked DC, now has ID#"
             << tblue << DCArt.getDatabaseID() << tendl;
      }

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
      // FIXME: MAYBE have a DoNotPop argument ?
      Artifact& DCPopped = Repo_.PopDeclContext();

      //assert( &DCPopped == &DCArt );
      // ^ don't compare Artifact/s addresses: these may (and does) change
      //   any time at runtime due to container growth
      //   (hence: memory reallocations).

      if (DCPopped.GetDecl() != DCArt.GetDecl()) {
        *log << tred
          << ( "Dude, for some obscur reason we popped a different DeclContext*"
               " off the DeclContextStack, and this isn't a good thing."  )
          << tendl
          << tnormal << " `-> "
                     << tyellow << DCPopped.GetDecl()->getDeclKindName()
                     << " ( " << DCPopped.GetDecl() << " ) " << tendl
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
