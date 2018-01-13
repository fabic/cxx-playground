
#include <clang/AST/Decl.h>

#include "dude/util/Terminal.hpp"
#include "Clong.hpp"

namespace clong {
namespace plugin {

  void
    Clong::DBPopulateDeclKinds()
    {
      TPush("Populating table `decl_kind` with Decl::Kind::*");

      PQXX_.TXN().exec(R"(
        INSERT INTO decl_kind (id, decl_kind, name, class)
        VALUES
          -- Stub record:
          (0, NULL, 'Stub entry, neither Decl, nor Type, not even a BuiltinType.', 'NIL'),
          -- ^ Stub record.
          ( 0+1,  0,  'Decl::Kind::AccessSpec', 'AccessSpecDecl'),
          ( 1+1,  1,  'Decl::Kind::Block', 'BlockDecl'),
          ( 2+1,  2,  'Decl::Kind::Captured', 'CapturedDecl'),
          ( 3+1,  3,  'Decl::Kind::ClassScopeFunctionSpecialization', 'ClassScopeFunctionSpecializationDecl'),
          ( 4+1,  4,  'Decl::Kind::Empty', 'EmptyDecl'),
          ( 5+1,  5,  'Decl::Kind::Export', 'ExportDecl'),
          ( 6+1,  6,  'Decl::Kind::ExternCContext', 'ExternCContextDecl'),
          ( 7+1,  7,  'Decl::Kind::FileScopeAsm', 'FileScopeAsmDecl'),
          ( 8+1,  8,  'Decl::Kind::Friend', 'FriendDecl'),
          ( 9+1,  9,  'Decl::Kind::FriendTemplate', 'FriendTemplateDecl'),
          (10+1, 10,  'Decl::Kind::Import', 'ImportDecl'),
          (11+1, 11,  'Decl::Kind::LinkageSpec', 'LinkageSpecDecl'),
          (12+1, 12,  'Decl::Kind::Label', 'LabelDecl'),
          (13+1, 13,  'Decl::Kind::Namespace', 'NamespaceDecl'),
          (14+1, 14,  'Decl::Kind::NamespaceAlias', 'NamespaceAliasDecl'),
          (15+1, 15,  'Decl::Kind::ObjCCompatibleAlias', 'ObjCCompatibleAliasDecl'),
          (16+1, 16,  'Decl::Kind::ObjCCategory', 'ObjCCategoryDecl'),
          (17+1, 17,  'Decl::Kind::ObjCCategoryImpl', 'ObjCCategoryImplDecl'),
          (18+1, 18,  'Decl::Kind::ObjCImplementation', 'ObjCImplementationDecl'),
          (19+1, 19,  'Decl::Kind::ObjCInterface', 'ObjCInterfaceDecl'),
          (20+1, 20,  'Decl::Kind::ObjCProtocol', 'ObjCProtocolDecl'),
          (21+1, 21,  'Decl::Kind::ObjCMethod', 'ObjCMethodDecl'),
          (22+1, 22,  'Decl::Kind::ObjCProperty', 'ObjCPropertyDecl'),
          (23+1, 23,  'Decl::Kind::BuiltinTemplate', 'BuiltinTemplateDecl'),
          (24+1, 24,  'Decl::Kind::ClassTemplate', 'ClassTemplateDecl'),
          (25+1, 25,  'Decl::Kind::FunctionTemplate', 'FunctionTemplateDecl'),
          (26+1, 26,  'Decl::Kind::TypeAliasTemplate', 'TypeAliasTemplateDecl'),
          (27+1, 27,  'Decl::Kind::VarTemplate', 'VarTemplateDecl'),
          (28+1, 28,  'Decl::Kind::TemplateTemplateParm', 'TemplateTemplateParmDecl'),
          (29+1, 29,  'Decl::Kind::Enum', 'EnumDecl'),
          (30+1, 30,  'Decl::Kind::Record', 'RecordDecl'),
          (31+1, 31,  'Decl::Kind::CXXRecord', 'CXXRecordDecl'),
          (32+1, 32,  'Decl::Kind::ClassTemplateSpecialization', 'ClassTemplateSpecializationDecl'),
          (33+1, 33,  'Decl::Kind::ClassTemplatePartialSpecialization', 'ClassTemplatePartialSpecializationDecl'),
          (34+1, 34,  'Decl::Kind::TemplateTypeParm', 'TemplateTypeParmDecl'),
          (35+1, 35,  'Decl::Kind::ObjCTypeParam', 'ObjCTypeParamDecl'),
          (36+1, 36,  'Decl::Kind::TypeAlias', 'TypeAliasDecl'),
          (37+1, 37,  'Decl::Kind::Typedef', 'TypedefDecl'),
          (38+1, 38,  'Decl::Kind::UnresolvedUsingTypename', 'UnresolvedUsingTypenameDecl'),
          (39+1, 39,  'Decl::Kind::Using', 'UsingDecl'),
          (40+1, 40,  'Decl::Kind::UsingDirective', 'UsingDirectiveDecl'),
          (41+1, 41,  'Decl::Kind::UsingPack', 'UsingPackDecl'),
          (42+1, 42,  'Decl::Kind::UsingShadow', 'UsingShadowDecl'),
          (43+1, 43,  'Decl::Kind::ConstructorUsingShadow', 'ConstructorUsingShadowDecl'),
          (44+1, 44,  'Decl::Kind::Binding', 'BindingDecl'),
          (45+1, 45,  'Decl::Kind::Field', 'FieldDecl'),
          (46+1, 46,  'Decl::Kind::ObjCAtDefsField', 'ObjCAtDefsFieldDecl'),
          (47+1, 47,  'Decl::Kind::ObjCIvar', 'ObjCIvarDecl'),
          (48+1, 48,  'Decl::Kind::Function', 'FunctionDecl'),
          (49+1, 49,  'Decl::Kind::CXXDeductionGuide', 'CXXDeductionGuideDecl'),
          (50+1, 50,  'Decl::Kind::CXXMethod', 'CXXMethodDecl'),
          (51+1, 51,  'Decl::Kind::CXXConstructor', 'CXXConstructorDecl'),
          (52+1, 52,  'Decl::Kind::CXXConversion', 'CXXConversionDecl'),
          (53+1, 53,  'Decl::Kind::CXXDestructor', 'CXXDestructorDecl'),
          (54+1, 54,  'Decl::Kind::MSProperty', 'MSPropertyDecl'),
          (55+1, 55,  'Decl::Kind::NonTypeTemplateParm', 'NonTypeTemplateParmDecl'),
          (56+1, 56,  'Decl::Kind::Var', 'VarDecl'),
          (57+1, 57,  'Decl::Kind::Decomposition', 'DecompositionDecl'),
          (58+1, 58,  'Decl::Kind::ImplicitParam', 'ImplicitParamDecl'),
          (59+1, 59,  'Decl::Kind::OMPCapturedExpr', 'OMPCapturedExprDecl'),
          (60+1, 60,  'Decl::Kind::ParmVar', 'ParmVarDecl'),
          (61+1, 61,  'Decl::Kind::VarTemplateSpecialization', 'VarTemplateSpecializationDecl'),
          (62+1, 62,  'Decl::Kind::VarTemplatePartialSpecialization', 'VarTemplatePartialSpecializationDecl'),
          (63+1, 63,  'Decl::Kind::EnumConstant', 'EnumConstantDecl'),
          (64+1, 64,  'Decl::Kind::IndirectField', 'IndirectFieldDecl'),
          (65+1, 65,  'Decl::Kind::OMPDeclareReduction', 'OMPDeclareReductionDecl'),
          (66+1, 66,  'Decl::Kind::UnresolvedUsingValue', 'UnresolvedUsingValueDecl'),
          (67+1, 67,  'Decl::Kind::OMPThreadPrivate', 'OMPThreadPrivateDecl'),
          (68+1, 68,  'Decl::Kind::ObjCPropertyImpl', 'ObjCPropertyImplDecl'),
          (69+1, 69,  'Decl::Kind::PragmaComment', 'PragmaCommentDecl'),
          (70+1, 70,  'Decl::Kind::PragmaDetectMismatch', 'PragmaDetectMismatchDecl'),
          (71+1, 71,  'Decl::Kind::StaticAssert', 'StaticAssertDecl'),
          (72+1, 72,  'Decl::Kind::TranslationUnit', 'TranslationUnitDecl') ;
        )");

      PQXX_.TXN().exec(R"(
        INSERT INTO decl_kind (id, type_kind, name, class)
        VALUES
          ( 0+100,  0,  'Type::TypeClass::Builtin', 'BuiltinType'),
          ( 1+100,  1,  'Type::TypeClass::Complex', 'ComplexType'),
          ( 2+100,  2,  'Type::TypeClass::Pointer', 'PointerType'),
          ( 3+100,  3,  'Type::TypeClass::BlockPointer', 'BlockPointerType'),
          ( 4+100,  4,  'Type::TypeClass::LValueReference', 'LValueReferenceType'),
          ( 5+100,  5,  'Type::TypeClass::RValueReference', 'RValueReferenceType'),
          ( 6+100,  6,  'Type::TypeClass::MemberPointer', 'MemberPointerType'),
          ( 7+100,  7,  'Type::TypeClass::ConstantArray', 'ConstantArrayType'),
          ( 8+100,  8,  'Type::TypeClass::IncompleteArray', 'IncompleteArrayType'),
          ( 9+100,  9,  'Type::TypeClass::VariableArray', 'VariableArrayType'),
          (10+100, 10,  'Type::TypeClass::DependentSizedArray', 'DependentSizedArrayType'),
          (11+100, 11,  'Type::TypeClass::DependentSizedExtVector', 'DependentSizedExtVectorType'),
          (12+100, 12,  'Type::TypeClass::DependentAddressSpace', 'DependentAddressSpaceType'),
          (13+100, 13,  'Type::TypeClass::Vector', 'VectorType'),
          (14+100, 14,  'Type::TypeClass::ExtVector', 'ExtVectorType'),
          (15+100, 15,  'Type::TypeClass::FunctionProto', 'FunctionProtoType'),
          (16+100, 16,  'Type::TypeClass::FunctionNoProto', 'FunctionNoProtoType'),
          (17+100, 17,  'Type::TypeClass::UnresolvedUsing', 'UnresolvedUsingType'),
          (18+100, 18,  'Type::TypeClass::Paren', 'ParenType'),
          (19+100, 19,  'Type::TypeClass::Typedef', 'TypedefType'),
          (20+100, 20,  'Type::TypeClass::Adjusted', 'AdjustedType'),
          (21+100, 21,  'Type::TypeClass::Decayed', 'DecayedType'),
          (22+100, 22,  'Type::TypeClass::TypeOfExpr', 'TypeOfExprType'),
          (23+100, 23,  'Type::TypeClass::TypeOf', 'TypeOfType'),
          (24+100, 24,  'Type::TypeClass::Decltype', 'DecltypeType'),
          (25+100, 25,  'Type::TypeClass::UnaryTransform', 'UnaryTransformType'),
          (26+100, 26,  'Type::TypeClass::Record', 'RecordType'),
          (27+100, 27,  'Type::TypeClass::Enum', 'EnumType'),
          (28+100, 28,  'Type::TypeClass::Elaborated', 'ElaboratedType'),
          (29+100, 29,  'Type::TypeClass::Attributed', 'AttributedType'),
          (30+100, 30,  'Type::TypeClass::TemplateTypeParm', 'TemplateTypeParmType'),
          (31+100, 31,  'Type::TypeClass::SubstTemplateTypeParm', 'SubstTemplateTypeParmType'),
          (32+100, 32,  'Type::TypeClass::SubstTemplateTypeParmPack', 'SubstTemplateTypeParmPackType'),
          (33+100, 33,  'Type::TypeClass::TemplateSpecialization', 'TemplateSpecializationType'),
          (34+100, 34,  'Type::TypeClass::Auto', 'AutoType'),
          (35+100, 35,  'Type::TypeClass::DeducedTemplateSpecialization', 'DeducedTemplateSpecializationType'),
          (36+100, 36,  'Type::TypeClass::InjectedClassName', 'InjectedClassNameType'),
          (37+100, 37,  'Type::TypeClass::DependentName', 'DependentNameType'),
          (38+100, 38,  'Type::TypeClass::DependentTemplateSpecialization', 'DependentTemplateSpecializationType'),
          (39+100, 39,  'Type::TypeClass::PackExpansion', 'PackExpansionType'),
          (40+100, 40,  'Type::TypeClass::ObjCTypeParam', 'ObjCTypeParamType'),
          (41+100, 41,  'Type::TypeClass::ObjCObject', 'ObjCObjectType'),
          (42+100, 42,  'Type::TypeClass::ObjCInterface', 'ObjCInterfaceType'),
          (43+100, 43,  'Type::TypeClass::ObjCObjectPointer', 'ObjCObjectPointerType'),
          (44+100, 44,  'Type::TypeClass::Pipe', 'PipeType'),
          (45+100, 45,  'Type::TypeClass::Atomic', 'AtomicType') ;
      )");

      PQXX_.TXN().exec(R"(
        INSERT INTO decl_kind (id, builtin_type_kind, name, class)
        VALUES
          ( 0+200,  0,  'BuiltinType::Kind::OCLImage1dRO', 'OCLImage1dRO'),
          ( 1+200,  1,  'BuiltinType::Kind::OCLImage1dArrayRO', 'OCLImage1dArrayRO'),
          ( 2+200,  2,  'BuiltinType::Kind::OCLImage1dBufferRO', 'OCLImage1dBufferRO'),
          ( 3+200,  3,  'BuiltinType::Kind::OCLImage2dRO', 'OCLImage2dRO'),
          ( 4+200,  4,  'BuiltinType::Kind::OCLImage2dArrayRO', 'OCLImage2dArrayRO'),
          ( 5+200,  5,  'BuiltinType::Kind::OCLImage2dDepthRO', 'OCLImage2dDepthRO'),
          ( 6+200,  6,  'BuiltinType::Kind::OCLImage2dArrayDepthRO', 'OCLImage2dArrayDepthRO'),
          ( 7+200,  7,  'BuiltinType::Kind::OCLImage2dMSAARO', 'OCLImage2dMSAARO'),
          ( 8+200,  8,  'BuiltinType::Kind::OCLImage2dArrayMSAARO', 'OCLImage2dArrayMSAARO'),
          ( 9+200,  9,  'BuiltinType::Kind::OCLImage2dMSAADepthRO', 'OCLImage2dMSAADepthRO'),
          (10+200, 10,  'BuiltinType::Kind::OCLImage2dArrayMSAADepthRO', 'OCLImage2dArrayMSAADepthRO'),
          (11+200, 11,  'BuiltinType::Kind::OCLImage3dRO', 'OCLImage3dRO'),
          (12+200, 12,  'BuiltinType::Kind::OCLImage1dWO', 'OCLImage1dWO'),
          (13+200, 13,  'BuiltinType::Kind::OCLImage1dArrayWO', 'OCLImage1dArrayWO'),
          (14+200, 14,  'BuiltinType::Kind::OCLImage1dBufferWO', 'OCLImage1dBufferWO'),
          (15+200, 15,  'BuiltinType::Kind::OCLImage2dWO', 'OCLImage2dWO'),
          (16+200, 16,  'BuiltinType::Kind::OCLImage2dArrayWO', 'OCLImage2dArrayWO'),
          (17+200, 17,  'BuiltinType::Kind::OCLImage2dDepthWO', 'OCLImage2dDepthWO'),
          (18+200, 18,  'BuiltinType::Kind::OCLImage2dArrayDepthWO', 'OCLImage2dArrayDepthWO'),
          (19+200, 19,  'BuiltinType::Kind::OCLImage2dMSAAWO', 'OCLImage2dMSAAWO'),
          (20+200, 20,  'BuiltinType::Kind::OCLImage2dArrayMSAAWO', 'OCLImage2dArrayMSAAWO'),
          (21+200, 21,  'BuiltinType::Kind::OCLImage2dMSAADepthWO', 'OCLImage2dMSAADepthWO'),
          (22+200, 22,  'BuiltinType::Kind::OCLImage2dArrayMSAADepthWO', 'OCLImage2dArrayMSAADepthWO'),
          (23+200, 23,  'BuiltinType::Kind::OCLImage3dWO', 'OCLImage3dWO'),
          (24+200, 24,  'BuiltinType::Kind::OCLImage1dRW', 'OCLImage1dRW'),
          (25+200, 25,  'BuiltinType::Kind::OCLImage1dArrayRW', 'OCLImage1dArrayRW'),
          (26+200, 26,  'BuiltinType::Kind::OCLImage1dBufferRW', 'OCLImage1dBufferRW'),
          (27+200, 27,  'BuiltinType::Kind::OCLImage2dRW', 'OCLImage2dRW'),
          (28+200, 28,  'BuiltinType::Kind::OCLImage2dArrayRW', 'OCLImage2dArrayRW'),
          (29+200, 29,  'BuiltinType::Kind::OCLImage2dDepthRW', 'OCLImage2dDepthRW'),
          (30+200, 30,  'BuiltinType::Kind::OCLImage2dArrayDepthRW', 'OCLImage2dArrayDepthRW'),
          (31+200, 31,  'BuiltinType::Kind::OCLImage2dMSAARW', 'OCLImage2dMSAARW'),
          (32+200, 32,  'BuiltinType::Kind::OCLImage2dArrayMSAARW', 'OCLImage2dArrayMSAARW'),
          (33+200, 33,  'BuiltinType::Kind::OCLImage2dMSAADepthRW', 'OCLImage2dMSAADepthRW'),
          (34+200, 34,  'BuiltinType::Kind::OCLImage2dArrayMSAADepthRW', 'OCLImage2dArrayMSAADepthRW'),
          (35+200, 35,  'BuiltinType::Kind::OCLImage3dRW', 'OCLImage3dRW'),
          (36+200, 36,  'BuiltinType::Kind::Void', 'Void'),
          (37+200, 37,  'BuiltinType::Kind::Bool', 'Bool'),
          (38+200, 38,  'BuiltinType::Kind::Char_U', 'Char_U'),
          (39+200, 39,  'BuiltinType::Kind::UChar', 'UChar'),
          (40+200, 40,  'BuiltinType::Kind::WChar_U', 'WChar_U'),
          (41+200, 41,  'BuiltinType::Kind::Char16', 'Char16'),
          (42+200, 42,  'BuiltinType::Kind::Char32', 'Char32'),
          (43+200, 43,  'BuiltinType::Kind::UShort', 'UShort'),
          (44+200, 44,  'BuiltinType::Kind::UInt', 'UInt'),
          (45+200, 45,  'BuiltinType::Kind::ULong', 'ULong'),
          (46+200, 46,  'BuiltinType::Kind::ULongLong', 'ULongLong'),
          (47+200, 47,  'BuiltinType::Kind::UInt128', 'UInt128'),
          (48+200, 48,  'BuiltinType::Kind::Char_S', 'Char_S'),
          (49+200, 49,  'BuiltinType::Kind::SChar', 'SChar'),
          (50+200, 50,  'BuiltinType::Kind::WChar_S', 'WChar_S'),
          (51+200, 51,  'BuiltinType::Kind::Short', 'Short'),
          (52+200, 52,  'BuiltinType::Kind::Int', 'Int'),
          (53+200, 53,  'BuiltinType::Kind::Long', 'Long'),
          (54+200, 54,  'BuiltinType::Kind::LongLong', 'LongLong'),
          (55+200, 55,  'BuiltinType::Kind::Int128', 'Int128'),
          (56+200, 56,  'BuiltinType::Kind::Half', 'Half'),
          (57+200, 57,  'BuiltinType::Kind::Float', 'Float'),
          (58+200, 58,  'BuiltinType::Kind::Double', 'Double'),
          (59+200, 59,  'BuiltinType::Kind::LongDouble', 'LongDouble'),
          (60+200, 60,  'BuiltinType::Kind::Float16', 'Float16'),
          (61+200, 61,  'BuiltinType::Kind::Float128', 'Float128'),
          (62+200, 62,  'BuiltinType::Kind::NullPtr', 'NullPtr'),
          (63+200, 63,  'BuiltinType::Kind::ObjCId', 'ObjCId'),
          (64+200, 64,  'BuiltinType::Kind::ObjCClass', 'ObjCClass'),
          (65+200, 65,  'BuiltinType::Kind::ObjCSel', 'ObjCSel'),
          (66+200, 66,  'BuiltinType::Kind::OCLSampler', 'OCLSampler'),
          (67+200, 67,  'BuiltinType::Kind::OCLEvent', 'OCLEvent'),
          (68+200, 68,  'BuiltinType::Kind::OCLClkEvent', 'OCLClkEvent'),
          (69+200, 69,  'BuiltinType::Kind::OCLQueue', 'OCLQueue'),
          (70+200, 70,  'BuiltinType::Kind::OCLReserveID', 'OCLReserveID'),
          (71+200, 71,  'BuiltinType::Kind::Dependent', 'Dependent'),
          (72+200, 72,  'BuiltinType::Kind::Overload', 'Overload'),
          (73+200, 73,  'BuiltinType::Kind::BoundMember', 'BoundMember'),
          (74+200, 74,  'BuiltinType::Kind::PseudoObject', 'PseudoObject'),
          (75+200, 75,  'BuiltinType::Kind::UnknownAny', 'UnknownAny'),
          (76+200, 76,  'BuiltinType::Kind::BuiltinFn', 'BuiltinFn'),
          (77+200, 77,  'BuiltinType::Kind::ARCUnbridgedCast', 'ARCUnbridgedCast'),
          (78+200, 78,  'BuiltinType::Kind::OMPArraySection', 'OMPArraySection') ;
      )");

      TPush("Adding the \"Nil Decl Context\" to table `decl` ");

      PQXX_.TXN().exec(R"(
        INSERT INTO decl (id, kind, name)
        VALUES (0, 0, E'Nil-Null-Void Decl Context entry in table `decl`.') ;
      )");

      TPush("Populating table `decl` with BuiltinType/s");

      PQXX_.TXN().exec(R"(
        INSERT INTO decl (id, kind, context_id, name)
          SELECT id,
                 100  AS BuiltinType_Kind,
                   0  AS Nil_DeclContext_id,
                 name AS BuiltinType_Name
          FROM decl_kind
          WHERE builtin_type_kind IS NOT NULL
          ORDER BY id ASC ;
      )");
    }

} // plugin ns.
} // clong ns.
