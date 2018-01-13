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

  bool
    TypeResolver::Resolve(const TypeSourceInfo *TSI)
    {
      TPush("TR:Resolve:TSI");
      const TypeLoc  TL = TSI->getTypeLoc();
      const QualType QT = TSI->getType();
      return Resolve(QT, TL);
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  bool
    TypeResolver::Resolve(const QualType QT, const TypeLoc TL)
    {
      if (TL.isNull()) {
        terrs() << tmagenta << "[TL]: Reached nil TypeLoc." << treset << tendl;
        return false;
      }

      assert(TL.getType() == QT);

      using TLC = TypeLoc::TypeLocClass;
      switch( TL.getTypeLocClass() )
      {
      case TLC::Builtin:
        ResolveBuiltinTypeLoc( TL.castAs< BuiltinTypeLoc >() );
        break;
      default:
        TPush log ("[TR]: Resolve( TypeLoc ) - switch-default.");
        *log << "[TL]: " << twhite << GetTypeLocKindName( TL )
             << " // CODE: " << Clong_.getSourceCode( TL.getSourceRange() )
             << tnormal << tendl;
      }

      // ~~ Next TypeLoc in the chain. ~~

      const TypeLoc NextTL = TL.getNextTypeLoc();

      return Resolve(NextTL.getType(), NextTL);
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  bool
    TypeResolver::ResolveBuiltinTypeLoc(const BuiltinTypeLoc TL)
    {
      TPush log ("[TR]: BuiltinTypeLoc");
      *log << "-  code: " << Clong_.getSourceCode( TL.getLocalSourceRange() ) << tendl;
      *log << "- code': " << Clong_.getSourceCode( TL.getSourceRange() ) << tendl;

      return false;
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
