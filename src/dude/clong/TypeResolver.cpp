#include <cassert>
#include <llvm/Support/ErrorHandling.h>
#include <clang/AST/TypeLoc.h>
#include <dude/util/Terminal.hpp>
#include "TypeResolver.hpp"
#include "Clong.hpp"

namespace clong {
namespace plugin {

  // ctor
  TypeResolver::TypeResolver(Clong& C)
    : Clong_( C )
  {
    // TPush _log;
  }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  bool
    TypeResolver::Resolve(const QualType QT)
    {
      const Type *type = QT.getTypePtr();
      assert(type != nullptr
          && "Can't handle situation where QualType has no Type* pointer.");
      return ResolveImpl( type );
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  bool
    TypeResolver::ResolveImpl(const Type *T)
    {
      using TC = Type::TypeClass;
      TC tc = T->getTypeClass();
      switch( tc ) {
        case TC::Builtin: {
          const BuiltinType *B = cast<BuiltinType>( T );
          ((terrs().yellow() << "TypeResolver: Got a builtin: ")
            .white() << B->getName(Clong_.getPrintingPolicy()) << "\n")
            .reset();
          return false;
        }
        //
        case TC::Elaborated: {
          const ElaboratedType *E = cast<ElaboratedType>( T );
          QualType QT = E->getNamedType();
          return ResolveImpl( QT );
        }
        // TagType/s
        case TC::Enum:
        case TC::Record: {
          const TagType *TT = cast<TagType>( T );
          const Decl *D = TT->getDecl();
          return false;
        }
        default:
          (((terrs().red().reverse() << "TypeResolver::resolve():")
            .red() << " No impl. for handling Type/s of class `")
            .white() << T->getTypeClassName() << "`.\n")
            .reset();
          return false;
      }

      llvm_unreachable("This shalt not be (see default of switch-case).");
      return false;
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  bool
    TypeResolver::ResolveImpl(const QualType QT)
    {
      const Type *T = QT.getTypePtr();
      assert(T != nullptr
        && "Really: We can't handle situation where QualType has no Type* pointer.");
      return ResolveImpl( T );
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  Artifact&
    TypeResolver::Resolve(const TypeSourceInfo *TSI)
    {
      const TypeLoc  TL = TSI->getTypeLoc();
      return Resolve(TL);
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  Artifact&
    TypeResolver::Resolve(const TypeLoc TL)
    {
      TPush log("TR:Resolve(QT, TL)");

      if (TL.isNull()) {
        terrs() << tmagenta << "[TL]: Reached nil TypeLoc." << treset << tendl;
        return Clong_.getRepository().GetNilArtifact();
      }

      DBIdentifier_t DBID = 0;
      Artifact *Art = nullptr ;

      using TLC = TypeLoc::TypeLocClass;
      switch( TL.getTypeLocClass() )
      {
      case TLC::Builtin:
        Art = & ResolveBuiltinTypeLoc( TL.castAs< BuiltinTypeLoc >() );
        break;
      default:
        TPush log ("[TR]: Resolve( TypeLoc ) - switch-default.");
        *log << "[TL]: " << twhite << GetTypeLocKindName( TL )
             << " // CODE: " << Clong_.getSourceCode( TL.getSourceRange() )
             << tnormal << tendl;
      }

      // ~~ Next TypeLoc in the chain. ~~

      const TypeLoc NextTL = TL.getNextTypeLoc();

      // FIXME: awkward logic here (temp.).

      DBID = Art != nullptr ? Art->getDatabaseID() : 0;

      // ~~ _unless_ this type is final (may not point to / wrap another type ~~
      if (DBID != 0) {
        if ( !NextTL.isNull() ) {
          throw clong_error( "We ended up resolving this type to a database ID"
                             "and yet this Type/Loc is _not_ final: there's a"
                             "\"next TypeLoc\" (WTF?!)" );
        }

        return *Art ;
      }

      return Resolve(NextTL);
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  Artifact&
    TypeResolver::ResolveBuiltinTypeLoc(const BuiltinTypeLoc TL)
    {
      TPush log ("[TR]: BuiltinTypeLoc");
      *log << "-  code: " << Clong_.getSourceCode( TL.getLocalSourceRange() ) << tendl;
      *log << "- code': " << Clong_.getSourceCode( TL.getSourceRange() ) << tendl;

      // return Clong_.getRepository().GetNilArtifact();

      const BuiltinType *B = TL.getTypePtr();
      assert(B != nullptr && "Huh!");

      BuiltinType::Kind K = B->getKind();

      // TODO: refactor this into the Repository or Artifact ?
      // FIXME: hard-code +300.
      Repository::Key_t KK = static_cast<int>( K ) + 300;

      *log << "- Kind: " << K << tendl;
      *log << "- Key: " << KK << tendl;

      Artifact& BArt = Clong_.getRepository().Get( KK );

      *log << "- ID# " << BArt.getDatabaseID() << tendl;

      return BArt ;
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  const char *
    TypeResolver::GetTypeLocKindName(TypeLoc TL)
    {
      using TLC = TypeLoc::TypeLocClass;
      switch( TL.getTypeLocClass() )
      {
#define ABSTRACT_TYPELOC(CLASS, PARENT)
#define TYPELOC(CLASS, PARENT)     \
        case TLC::CLASS:  return "" #CLASS "";
#include "clang/AST/TypeLocNodes.def"

        default:
          return "<<(!) Unknown TypeLoc::TypeLocClass (!)>>";
      }

      return nullptr;
    }

} // plugin ns.
} // clong ns.
