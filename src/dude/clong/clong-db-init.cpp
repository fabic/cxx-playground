
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
        INSERT INTO decl_kind (id, decl_kind, name)
        VALUES
          -- Stub record:
          (0, NULL, 'Stub entry, neither Decl, nor Type, not even a BuiltinType.'),
          -- ^ Stub record.
          ( 0+1,  0,  'Decl::Kind::AccessSpec'),
          ( 1+1,  1,  'Decl::Kind::Block'),
          ( 2+1,  2,  'Decl::Kind::Captured'),
          ( 3+1,  3,  'Decl::Kind::ClassScopeFunctionSpecialization'),
          ( 4+1,  4,  'Decl::Kind::Empty'),
          ( 5+1,  5,  'Decl::Kind::Export'),
          ( 6+1,  6,  'Decl::Kind::ExternCContext'),
          ( 7+1,  7,  'Decl::Kind::FileScopeAsm'),
          ( 8+1,  8,  'Decl::Kind::Friend'),
          ( 9+1,  9,  'Decl::Kind::FriendTemplate'),
          (10+1, 10,  'Decl::Kind::Import'),
          (11+1, 11,  'Decl::Kind::LinkageSpec'),
          (12+1, 12,  'Decl::Kind::Label'),
          (13+1, 13,  'Decl::Kind::Namespace'),
          (14+1, 14,  'Decl::Kind::NamespaceAlias'),
          (15+1, 15,  'Decl::Kind::ObjCCompatibleAlias'),
          (16+1, 16,  'Decl::Kind::ObjCCategory'),
          (17+1, 17,  'Decl::Kind::ObjCCategoryImpl'),
          (18+1, 18,  'Decl::Kind::ObjCImplementation'),
          (19+1, 19,  'Decl::Kind::ObjCInterface'),
          (20+1, 20,  'Decl::Kind::ObjCProtocol'),
          (21+1, 21,  'Decl::Kind::ObjCMethod'),
          (22+1, 22,  'Decl::Kind::ObjCProperty'),
          (23+1, 23,  'Decl::Kind::BuiltinTemplate'),
          (24+1, 24,  'Decl::Kind::ClassTemplate'),
          (25+1, 25,  'Decl::Kind::FunctionTemplate'),
          (26+1, 26,  'Decl::Kind::TypeAliasTemplate'),
          (27+1, 27,  'Decl::Kind::VarTemplate'),
          (28+1, 28,  'Decl::Kind::TemplateTemplateParm'),
          (29+1, 29,  'Decl::Kind::Enum'),
          (30+1, 30,  'Decl::Kind::Record'),
          (31+1, 31,  'Decl::Kind::CXXRecord'),
          (32+1, 32,  'Decl::Kind::ClassTemplateSpecialization'),
          (33+1, 33,  'Decl::Kind::ClassTemplatePartialSpecialization'),
          (34+1, 34,  'Decl::Kind::TemplateTypeParm'),
          (35+1, 35,  'Decl::Kind::ObjCTypeParam'),
          (36+1, 36,  'Decl::Kind::TypeAlias'),
          (37+1, 37,  'Decl::Kind::Typedef'),
          (38+1, 38,  'Decl::Kind::UnresolvedUsingTypename'),
          (39+1, 39,  'Decl::Kind::Using'),
          (40+1, 40,  'Decl::Kind::UsingDirective'),
          (41+1, 41,  'Decl::Kind::UsingPack'),
          (42+1, 42,  'Decl::Kind::UsingShadow'),
          (43+1, 43,  'Decl::Kind::ConstructorUsingShadow'),
          (44+1, 44,  'Decl::Kind::Binding'),
          (45+1, 45,  'Decl::Kind::Field'),
          (46+1, 46,  'Decl::Kind::ObjCAtDefsField'),
          (47+1, 47,  'Decl::Kind::ObjCIvar'),
          (48+1, 48,  'Decl::Kind::Function'),
          (49+1, 49,  'Decl::Kind::CXXDeductionGuide'),
          (50+1, 50,  'Decl::Kind::CXXMethod'),
          (51+1, 51,  'Decl::Kind::CXXConstructor'),
          (52+1, 52,  'Decl::Kind::CXXConversion'),
          (53+1, 53,  'Decl::Kind::CXXDestructor'),
          (54+1, 54,  'Decl::Kind::MSProperty'),
          (55+1, 55,  'Decl::Kind::NonTypeTemplateParm'),
          (56+1, 56,  'Decl::Kind::Var'),
          (57+1, 57,  'Decl::Kind::Decomposition'),
          (58+1, 58,  'Decl::Kind::ImplicitParam'),
          (59+1, 59,  'Decl::Kind::OMPCapturedExpr'),
          (60+1, 60,  'Decl::Kind::ParmVar'),
          (61+1, 61,  'Decl::Kind::VarTemplateSpecialization'),
          (62+1, 62,  'Decl::Kind::VarTemplatePartialSpecialization'),
          (63+1, 63,  'Decl::Kind::EnumConstant'),
          (64+1, 64,  'Decl::Kind::IndirectField'),
          (65+1, 65,  'Decl::Kind::OMPDeclareReduction'),
          (66+1, 66,  'Decl::Kind::UnresolvedUsingValue'),
          (67+1, 67,  'Decl::Kind::OMPThreadPrivate'),
          (68+1, 68,  'Decl::Kind::ObjCPropertyImpl'),
          (69+1, 69,  'Decl::Kind::PragmaComment'),
          (70+1, 70,  'Decl::Kind::PragmaDetectMismatch'),
          (71+1, 71,  'Decl::Kind::StaticAssert'),
          (72+1, 72,  'Decl::Kind::TranslationUnit') ;
        )");

      PQXX_.TXN().exec(R"(
        INSERT INTO decl_kind (id, type_kind, name)
        VALUES
          ( 0+100,  0,  'Type::TypeClass::Builtin'),
          ( 1+100,  1,  'Type::TypeClass::Complex'),
          ( 2+100,  2,  'Type::TypeClass::Pointer'),
          ( 3+100,  3,  'Type::TypeClass::BlockPointer'),
          ( 4+100,  4,  'Type::TypeClass::LValueReference'),
          ( 5+100,  5,  'Type::TypeClass::RValueReference'),
          ( 6+100,  6,  'Type::TypeClass::MemberPointer'),
          ( 7+100,  7,  'Type::TypeClass::ConstantArray'),
          ( 8+100,  8,  'Type::TypeClass::IncompleteArray'),
          ( 9+100,  9,  'Type::TypeClass::VariableArray'),
          (10+100, 10,  'Type::TypeClass::DependentSizedArray'),
          (11+100, 11,  'Type::TypeClass::DependentSizedExtVector'),
          (12+100, 12,  'Type::TypeClass::DependentAddressSpace'),
          (13+100, 13,  'Type::TypeClass::Vector'),
          (14+100, 14,  'Type::TypeClass::ExtVector'),
          (15+100, 15,  'Type::TypeClass::FunctionProto'),
          (16+100, 16,  'Type::TypeClass::FunctionNoProto'),
          (17+100, 17,  'Type::TypeClass::UnresolvedUsing'),
          (18+100, 18,  'Type::TypeClass::Paren'),
          (19+100, 19,  'Type::TypeClass::Typedef'),
          (20+100, 20,  'Type::TypeClass::Adjusted'),
          (21+100, 21,  'Type::TypeClass::Decayed'),
          (22+100, 22,  'Type::TypeClass::TypeOfExpr'),
          (23+100, 23,  'Type::TypeClass::TypeOf'),
          (24+100, 24,  'Type::TypeClass::Decltype'),
          (25+100, 25,  'Type::TypeClass::UnaryTransform'),
          (26+100, 26,  'Type::TypeClass::Record'),
          (27+100, 27,  'Type::TypeClass::Enum'),
          (28+100, 28,  'Type::TypeClass::Elaborated'),
          (29+100, 29,  'Type::TypeClass::Attributed'),
          (30+100, 30,  'Type::TypeClass::TemplateTypeParm'),
          (31+100, 31,  'Type::TypeClass::SubstTemplateTypeParm'),
          (32+100, 32,  'Type::TypeClass::SubstTemplateTypeParmPack'),
          (33+100, 33,  'Type::TypeClass::TemplateSpecialization'),
          (34+100, 34,  'Type::TypeClass::Auto'),
          (35+100, 35,  'Type::TypeClass::DeducedTemplateSpecialization'),
          (36+100, 36,  'Type::TypeClass::InjectedClassName'),
          (37+100, 37,  'Type::TypeClass::DependentName'),
          (38+100, 38,  'Type::TypeClass::DependentTemplateSpecialization'),
          (39+100, 39,  'Type::TypeClass::PackExpansion'),
          (40+100, 40,  'Type::TypeClass::ObjCTypeParam'),
          (41+100, 41,  'Type::TypeClass::ObjCObject'),
          (42+100, 42,  'Type::TypeClass::ObjCInterface'),
          (43+100, 43,  'Type::TypeClass::ObjCObjectPointer'),
          (44+100, 44,  'Type::TypeClass::Pipe'),
          (45+100, 45,  'Type::TypeClass::Atomic') ;
      )");

      PQXX_.TXN().exec(R"(
        INSERT INTO decl_kind (id, builtin_type_kind, name)
        VALUES
          ( 0+200,  0,  'BuiltinType::Kind::OCLImage1dRO'),
          ( 1+200,  1,  'BuiltinType::Kind::OCLImage1dArrayRO'),
          ( 2+200,  2,  'BuiltinType::Kind::OCLImage1dBufferRO'),
          ( 3+200,  3,  'BuiltinType::Kind::OCLImage2dRO'),
          ( 4+200,  4,  'BuiltinType::Kind::OCLImage2dArrayRO'),
          ( 5+200,  5,  'BuiltinType::Kind::OCLImage2dDepthRO'),
          ( 6+200,  6,  'BuiltinType::Kind::OCLImage2dArrayDepthRO'),
          ( 7+200,  7,  'BuiltinType::Kind::OCLImage2dMSAARO'),
          ( 8+200,  8,  'BuiltinType::Kind::OCLImage2dArrayMSAARO'),
          ( 9+200,  9,  'BuiltinType::Kind::OCLImage2dMSAADepthRO'),
          (10+200, 10,  'BuiltinType::Kind::OCLImage2dArrayMSAADepthRO'),
          (11+200, 11,  'BuiltinType::Kind::OCLImage3dRO'),
          (12+200, 12,  'BuiltinType::Kind::OCLImage1dWO'),
          (13+200, 13,  'BuiltinType::Kind::OCLImage1dArrayWO'),
          (14+200, 14,  'BuiltinType::Kind::OCLImage1dBufferWO'),
          (15+200, 15,  'BuiltinType::Kind::OCLImage2dWO'),
          (16+200, 16,  'BuiltinType::Kind::OCLImage2dArrayWO'),
          (17+200, 17,  'BuiltinType::Kind::OCLImage2dDepthWO'),
          (18+200, 18,  'BuiltinType::Kind::OCLImage2dArrayDepthWO'),
          (19+200, 19,  'BuiltinType::Kind::OCLImage2dMSAAWO'),
          (20+200, 20,  'BuiltinType::Kind::OCLImage2dArrayMSAAWO'),
          (21+200, 21,  'BuiltinType::Kind::OCLImage2dMSAADepthWO'),
          (22+200, 22,  'BuiltinType::Kind::OCLImage2dArrayMSAADepthWO'),
          (23+200, 23,  'BuiltinType::Kind::OCLImage3dWO'),
          (24+200, 24,  'BuiltinType::Kind::OCLImage1dRW'),
          (25+200, 25,  'BuiltinType::Kind::OCLImage1dArrayRW'),
          (26+200, 26,  'BuiltinType::Kind::OCLImage1dBufferRW'),
          (27+200, 27,  'BuiltinType::Kind::OCLImage2dRW'),
          (28+200, 28,  'BuiltinType::Kind::OCLImage2dArrayRW'),
          (29+200, 29,  'BuiltinType::Kind::OCLImage2dDepthRW'),
          (30+200, 30,  'BuiltinType::Kind::OCLImage2dArrayDepthRW'),
          (31+200, 31,  'BuiltinType::Kind::OCLImage2dMSAARW'),
          (32+200, 32,  'BuiltinType::Kind::OCLImage2dArrayMSAARW'),
          (33+200, 33,  'BuiltinType::Kind::OCLImage2dMSAADepthRW'),
          (34+200, 34,  'BuiltinType::Kind::OCLImage2dArrayMSAADepthRW'),
          (35+200, 35,  'BuiltinType::Kind::OCLImage3dRW'),
          (36+200, 36,  'BuiltinType::Kind::Void'),
          (37+200, 37,  'BuiltinType::Kind::Bool'),
          (38+200, 38,  'BuiltinType::Kind::Char_U'),
          (39+200, 39,  'BuiltinType::Kind::UChar'),
          (40+200, 40,  'BuiltinType::Kind::WChar_U'),
          (41+200, 41,  'BuiltinType::Kind::Char16'),
          (42+200, 42,  'BuiltinType::Kind::Char32'),
          (43+200, 43,  'BuiltinType::Kind::UShort'),
          (44+200, 44,  'BuiltinType::Kind::UInt'),
          (45+200, 45,  'BuiltinType::Kind::ULong'),
          (46+200, 46,  'BuiltinType::Kind::ULongLong'),
          (47+200, 47,  'BuiltinType::Kind::UInt128'),
          (48+200, 48,  'BuiltinType::Kind::Char_S'),
          (49+200, 49,  'BuiltinType::Kind::SChar'),
          (50+200, 50,  'BuiltinType::Kind::WChar_S'),
          (51+200, 51,  'BuiltinType::Kind::Short'),
          (52+200, 52,  'BuiltinType::Kind::Int'),
          (53+200, 53,  'BuiltinType::Kind::Long'),
          (54+200, 54,  'BuiltinType::Kind::LongLong'),
          (55+200, 55,  'BuiltinType::Kind::Int128'),
          (56+200, 56,  'BuiltinType::Kind::Half'),
          (57+200, 57,  'BuiltinType::Kind::Float'),
          (58+200, 58,  'BuiltinType::Kind::Double'),
          (59+200, 59,  'BuiltinType::Kind::LongDouble'),
          (60+200, 60,  'BuiltinType::Kind::Float16'),
          (61+200, 61,  'BuiltinType::Kind::Float128'),
          (62+200, 62,  'BuiltinType::Kind::NullPtr'),
          (63+200, 63,  'BuiltinType::Kind::ObjCId'),
          (64+200, 64,  'BuiltinType::Kind::ObjCClass'),
          (65+200, 65,  'BuiltinType::Kind::ObjCSel'),
          (66+200, 66,  'BuiltinType::Kind::OCLSampler'),
          (67+200, 67,  'BuiltinType::Kind::OCLEvent'),
          (68+200, 68,  'BuiltinType::Kind::OCLClkEvent'),
          (69+200, 69,  'BuiltinType::Kind::OCLQueue'),
          (70+200, 70,  'BuiltinType::Kind::OCLReserveID'),
          (71+200, 71,  'BuiltinType::Kind::Dependent'),
          (72+200, 72,  'BuiltinType::Kind::Overload'),
          (73+200, 73,  'BuiltinType::Kind::BoundMember'),
          (74+200, 74,  'BuiltinType::Kind::PseudoObject'),
          (75+200, 75,  'BuiltinType::Kind::UnknownAny'),
          (76+200, 76,  'BuiltinType::Kind::BuiltinFn'),
          (77+200, 77,  'BuiltinType::Kind::ARCUnbridgedCast'),
          (78+200, 78,  'BuiltinType::Kind::OMPArraySection') ;
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
