#include <clang/AST/Decl.h>
#include "clang/AST/DeclCXX.h"
#include "clang/AST/DeclFriend.h"
#include "clang/AST/DeclObjC.h"
#include "clang/AST/DeclOpenMP.h"
#include <clang/AST/Type.h>

//#include <llvm/Support/raw_ostream.h>
#include "dude/util/Terminal.hpp"

/**
 * MAIN!
 */
int main(int argc, char *argv[])
{
  using namespace clang;
  using namespace clong;

  using llvm::outs;

  {
    TPush Out("List of Decl/s ( clang/AST/DeclNodes.inc )");
#   define ABSTRACT_DECL(DECL)
//#   define DECL_RANGE(BASE, START, END)                                             \
//    *Out << (int) Decl::Kind::first##BASE << " Decl::Kind::first" #BASE "" << tendl \
//         << (int) Decl::Kind::last##BASE  << " Decl::Kind::last" #BASE "" << tendl  ;
#   define DECL(CLASS, BASE)                                                   \
    *Out << (int) Decl::Kind::CLASS << " Decl::Kind::" #CLASS                  \
         << " (" << sizeof(CLASS##Decl) << ")"                                 \
         << " is-a " #BASE                                                     \
         << " (" << sizeof(BASE) << ")"                                        \
         << tendl ;
#   include "clang/AST/DeclNodes.inc"
  }

  {
    TPush Out("List of Type/s ( clang/AST/TypeNodes.def )");
#   define ABSTRACT_TYPE(CLASS, PARENT)
#   define TYPE(CLASS, PARENT)                                                 \
    *Out << (int) Type::TypeClass::CLASS << " Type::TypeClass::" #CLASS        \
         << " (" << sizeof(CLASS##Type) << ")"                                 \
         << " is-a " #PARENT                                                   \
         << " (" << sizeof(PARENT) << ")"                                      \
         << " | " << TypeLoc::TypeLocClass::CLASS                              \
                  << " TypeLoc::TypeLocClass::" #CLASS                         \
         << tendl ;
#include "clang/AST/TypeNodes.def"
  }

  {
    TPush Out("List of Builtin type/s ( clang/AST/BuiltinTypes.def )");
    // OpenCL image types
#   define IMAGE_TYPE(ImgType, Id, SingletonId, Access, Suffix)                \
    *Out << (int) BuiltinType::Kind::Id                                        \
         << " BuiltinType::Kind::" #Id " [OpenCL]"                             \
         << tendl ;
#   include "clang/Basic/OpenCLImageTypes.def"
    // All other builtin types
#   define BUILTIN_TYPE(Id, SingletonId)                                       \
    *Out << (int) BuiltinType::Kind::Id                                        \
         << " BuiltinType::Kind::" #Id                                         \
         << tendl ;
#   include "clang/AST/BuiltinTypes.def"
  }

  return 0;
}
