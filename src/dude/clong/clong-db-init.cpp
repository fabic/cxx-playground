
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
          ( 0+100 , 0  , 'AccessSpecDecl'                         , 'Decl::Kind::AccessSpec')                         ,
          ( 1+100 , 1  , 'BlockDecl'                              , 'Decl::Kind::Block')                              ,
          ( 2+100 , 2  , 'CapturedDecl'                           , 'Decl::Kind::Captured')                           ,
          ( 3+100 , 3  , 'ClassScopeFunctionSpecializationDecl'   , 'Decl::Kind::ClassScopeFunctionSpecialization')   ,
          ( 4+100 , 4  , 'EmptyDecl'                              , 'Decl::Kind::Empty')                              ,
          ( 5+100 , 5  , 'ExportDecl'                             , 'Decl::Kind::Export')                             ,
          ( 6+100 , 6  , 'ExternCContextDecl'                     , 'Decl::Kind::ExternCContext')                     ,
          ( 7+100 , 7  , 'FileScopeAsmDecl'                       , 'Decl::Kind::FileScopeAsm')                       ,
          ( 8+100 , 8  , 'FriendDecl'                             , 'Decl::Kind::Friend')                             ,
          ( 9+100 , 9  , 'FriendTemplateDecl'                     , 'Decl::Kind::FriendTemplate')                     ,
          (10+100 , 10 , 'ImportDecl'                             , 'Decl::Kind::Import')                             ,
          (11+100 , 11 , 'LinkageSpecDecl'                        , 'Decl::Kind::LinkageSpec')                        ,
          (12+100 , 12 , 'LabelDecl'                              , 'Decl::Kind::Label')                              ,
          (13+100 , 13 , 'NamespaceDecl'                          , 'Decl::Kind::Namespace')                          ,
          (14+100 , 14 , 'NamespaceAliasDecl'                     , 'Decl::Kind::NamespaceAlias')                     ,
          (15+100 , 15 , 'ObjCCompatibleAliasDecl'                , 'Decl::Kind::ObjCCompatibleAlias')                ,
          (16+100 , 16 , 'ObjCCategoryDecl'                       , 'Decl::Kind::ObjCCategory')                       ,
          (17+100 , 17 , 'ObjCCategoryImplDecl'                   , 'Decl::Kind::ObjCCategoryImpl')                   ,
          (18+100 , 18 , 'ObjCImplementationDecl'                 , 'Decl::Kind::ObjCImplementation')                 ,
          (19+100 , 19 , 'ObjCInterfaceDecl'                      , 'Decl::Kind::ObjCInterface')                      ,
          (20+100 , 20 , 'ObjCProtocolDecl'                       , 'Decl::Kind::ObjCProtocol')                       ,
          (21+100 , 21 , 'ObjCMethodDecl'                         , 'Decl::Kind::ObjCMethod')                         ,
          (22+100 , 22 , 'ObjCPropertyDecl'                       , 'Decl::Kind::ObjCProperty')                       ,
          (23+100 , 23 , 'BuiltinTemplateDecl'                    , 'Decl::Kind::BuiltinTemplate')                    ,
          (24+100 , 24 , 'ClassTemplateDecl'                      , 'Decl::Kind::ClassTemplate')                      ,
          (25+100 , 25 , 'FunctionTemplateDecl'                   , 'Decl::Kind::FunctionTemplate')                   ,
          (26+100 , 26 , 'TypeAliasTemplateDecl'                  , 'Decl::Kind::TypeAliasTemplate')                  ,
          (27+100 , 27 , 'VarTemplateDecl'                        , 'Decl::Kind::VarTemplate')                        ,
          (28+100 , 28 , 'TemplateTemplateParmDecl'               , 'Decl::Kind::TemplateTemplateParm')               ,
          (29+100 , 29 , 'EnumDecl'                               , 'Decl::Kind::Enum')                               ,
          (30+100 , 30 , 'RecordDecl'                             , 'Decl::Kind::Record')                             ,
          (31+100 , 31 , 'CXXRecordDecl'                          , 'Decl::Kind::CXXRecord')                          ,
          (32+100 , 32 , 'ClassTemplateSpecializationDecl'        , 'Decl::Kind::ClassTemplateSpecialization')        ,
          (33+100 , 33 , 'ClassTemplatePartialSpecializationDecl' , 'Decl::Kind::ClassTemplatePartialSpecialization') ,
          (34+100 , 34 , 'TemplateTypeParmDecl'                   , 'Decl::Kind::TemplateTypeParm')                   ,
          (35+100 , 35 , 'ObjCTypeParamDecl'                      , 'Decl::Kind::ObjCTypeParam')                      ,
          (36+100 , 36 , 'TypeAliasDecl'                          , 'Decl::Kind::TypeAlias')                          ,
          (37+100 , 37 , 'TypedefDecl'                            , 'Decl::Kind::Typedef')                            ,
          (38+100 , 38 , 'UnresolvedUsingTypenameDecl'            , 'Decl::Kind::UnresolvedUsingTypename')            ,
          (39+100 , 39 , 'UsingDecl'                              , 'Decl::Kind::Using')                              ,
          (40+100 , 40 , 'UsingDirectiveDecl'                     , 'Decl::Kind::UsingDirective')                     ,
          (41+100 , 41 , 'UsingPackDecl'                          , 'Decl::Kind::UsingPack')                          ,
          (42+100 , 42 , 'UsingShadowDecl'                        , 'Decl::Kind::UsingShadow')                        ,
          (43+100 , 43 , 'ConstructorUsingShadowDecl'             , 'Decl::Kind::ConstructorUsingShadow')             ,
          (44+100 , 44 , 'BindingDecl'                            , 'Decl::Kind::Binding')                            ,
          (45+100 , 45 , 'FieldDecl'                              , 'Decl::Kind::Field')                              ,
          (46+100 , 46 , 'ObjCAtDefsFieldDecl'                    , 'Decl::Kind::ObjCAtDefsField')                    ,
          (47+100 , 47 , 'ObjCIvarDecl'                           , 'Decl::Kind::ObjCIvar')                           ,
          (48+100 , 48 , 'FunctionDecl'                           , 'Decl::Kind::Function')                           ,
          (49+100 , 49 , 'CXXDeductionGuideDecl'                  , 'Decl::Kind::CXXDeductionGuide')                  ,
          (50+100 , 50 , 'CXXMethodDecl'                          , 'Decl::Kind::CXXMethod')                          ,
          (51+100 , 51 , 'CXXConstructorDecl'                     , 'Decl::Kind::CXXConstructor')                     ,
          (52+100 , 52 , 'CXXConversionDecl'                      , 'Decl::Kind::CXXConversion')                      ,
          (53+100 , 53 , 'CXXDestructorDecl'                      , 'Decl::Kind::CXXDestructor')                      ,
          (54+100 , 54 , 'MSPropertyDecl'                         , 'Decl::Kind::MSProperty')                         ,
          (55+100 , 55 , 'NonTypeTemplateParmDecl'                , 'Decl::Kind::NonTypeTemplateParm')                ,
          (56+100 , 56 , 'VarDecl'                                , 'Decl::Kind::Var')                                ,
          (57+100 , 57 , 'DecompositionDecl'                      , 'Decl::Kind::Decomposition')                      ,
          (58+100 , 58 , 'ImplicitParamDecl'                      , 'Decl::Kind::ImplicitParam')                      ,
          (59+100 , 59 , 'OMPCapturedExprDecl'                    , 'Decl::Kind::OMPCapturedExpr')                    ,
          (60+100 , 60 , 'ParmVarDecl'                            , 'Decl::Kind::ParmVar')                            ,
          (61+100 , 61 , 'VarTemplateSpecializationDecl'          , 'Decl::Kind::VarTemplateSpecialization')          ,
          (62+100 , 62 , 'VarTemplatePartialSpecializationDecl'   , 'Decl::Kind::VarTemplatePartialSpecialization')   ,
          (63+100 , 63 , 'EnumConstantDecl'                       , 'Decl::Kind::EnumConstant')                       ,
          (64+100 , 64 , 'IndirectFieldDecl'                      , 'Decl::Kind::IndirectField')                      ,
          (65+100 , 65 , 'OMPDeclareReductionDecl'                , 'Decl::Kind::OMPDeclareReduction')                ,
          (66+100 , 66 , 'UnresolvedUsingValueDecl'               , 'Decl::Kind::UnresolvedUsingValue')               ,
          (67+100 , 67 , 'OMPThreadPrivateDecl'                   , 'Decl::Kind::OMPThreadPrivate')                   ,
          (68+100 , 68 , 'ObjCPropertyImplDecl'                   , 'Decl::Kind::ObjCPropertyImpl')                   ,
          (69+100 , 69 , 'PragmaCommentDecl'                      , 'Decl::Kind::PragmaComment')                      ,
          (70+100 , 70 , 'PragmaDetectMismatchDecl'               , 'Decl::Kind::PragmaDetectMismatch')               ,
          (71+100 , 71 , 'StaticAssertDecl'                       , 'Decl::Kind::StaticAssert')                       ,
          (72+100 , 72 , 'TranslationUnitDecl'                    , 'Decl::Kind::TranslationUnit') ;
        )");

      PQXX_.TXN().exec(R"(
        INSERT INTO decl_kind (id, type_kind, name, class)
        VALUES
          ( 0+200 , 0  , 'BuiltinType'                         , 'Type::TypeClass::Builtin')                         ,
          ( 1+200 , 1  , 'ComplexType'                         , 'Type::TypeClass::Complex')                         ,
          ( 2+200 , 2  , 'PointerType'                         , 'Type::TypeClass::Pointer')                         ,
          ( 3+200 , 3  , 'BlockPointerType'                    , 'Type::TypeClass::BlockPointer')                    ,
          ( 4+200 , 4  , 'LValueReferenceType'                 , 'Type::TypeClass::LValueReference')                 ,
          ( 5+200 , 5  , 'RValueReferenceType'                 , 'Type::TypeClass::RValueReference')                 ,
          ( 6+200 , 6  , 'MemberPointerType'                   , 'Type::TypeClass::MemberPointer')                   ,
          ( 7+200 , 7  , 'ConstantArrayType'                   , 'Type::TypeClass::ConstantArray')                   ,
          ( 8+200 , 8  , 'IncompleteArrayType'                 , 'Type::TypeClass::IncompleteArray')                 ,
          ( 9+200 , 9  , 'VariableArrayType'                   , 'Type::TypeClass::VariableArray')                   ,
          (10+200 , 10 , 'DependentSizedArrayType'             , 'Type::TypeClass::DependentSizedArray')             ,
          (11+200 , 11 , 'DependentSizedExtVectorType'         , 'Type::TypeClass::DependentSizedExtVector')         ,
          (12+200 , 12 , 'DependentAddressSpaceType'           , 'Type::TypeClass::DependentAddressSpace')           ,
          (13+200 , 13 , 'VectorType'                          , 'Type::TypeClass::Vector')                          ,
          (14+200 , 14 , 'ExtVectorType'                       , 'Type::TypeClass::ExtVector')                       ,
          (15+200 , 15 , 'FunctionProtoType'                   , 'Type::TypeClass::FunctionProto')                   ,
          (16+200 , 16 , 'FunctionNoProtoType'                 , 'Type::TypeClass::FunctionNoProto')                 ,
          (17+200 , 17 , 'UnresolvedUsingType'                 , 'Type::TypeClass::UnresolvedUsing')                 ,
          (18+200 , 18 , 'ParenType'                           , 'Type::TypeClass::Paren')                           ,
          (19+200 , 19 , 'TypedefType'                         , 'Type::TypeClass::Typedef')                         ,
          (20+200 , 20 , 'AdjustedType'                        , 'Type::TypeClass::Adjusted')                        ,
          (21+200 , 21 , 'DecayedType'                         , 'Type::TypeClass::Decayed')                         ,
          (22+200 , 22 , 'TypeOfExprType'                      , 'Type::TypeClass::TypeOfExpr')                      ,
          (23+200 , 23 , 'TypeOfType'                          , 'Type::TypeClass::TypeOf')                          ,
          (24+200 , 24 , 'DecltypeType'                        , 'Type::TypeClass::Decltype')                        ,
          (25+200 , 25 , 'UnaryTransformType'                  , 'Type::TypeClass::UnaryTransform')                  ,
          (26+200 , 26 , 'RecordType'                          , 'Type::TypeClass::Record')                          ,
          (27+200 , 27 , 'EnumType'                            , 'Type::TypeClass::Enum')                            ,
          (28+200 , 28 , 'ElaboratedType'                      , 'Type::TypeClass::Elaborated')                      ,
          (29+200 , 29 , 'AttributedType'                      , 'Type::TypeClass::Attributed')                      ,
          (30+200 , 30 , 'TemplateTypeParmType'                , 'Type::TypeClass::TemplateTypeParm')                ,
          (31+200 , 31 , 'SubstTemplateTypeParmType'           , 'Type::TypeClass::SubstTemplateTypeParm')           ,
          (32+200 , 32 , 'SubstTemplateTypeParmPackType'       , 'Type::TypeClass::SubstTemplateTypeParmPack')       ,
          (33+200 , 33 , 'TemplateSpecializationType'          , 'Type::TypeClass::TemplateSpecialization')          ,
          (34+200 , 34 , 'AutoType'                            , 'Type::TypeClass::Auto')                            ,
          (35+200 , 35 , 'DeducedTemplateSpecializationType'   , 'Type::TypeClass::DeducedTemplateSpecialization')   ,
          (36+200 , 36 , 'InjectedClassNameType'               , 'Type::TypeClass::InjectedClassName')               ,
          (37+200 , 37 , 'DependentNameType'                   , 'Type::TypeClass::DependentName')                   ,
          (38+200 , 38 , 'DependentTemplateSpecializationType' , 'Type::TypeClass::DependentTemplateSpecialization') ,
          (39+200 , 39 , 'PackExpansionType'                   , 'Type::TypeClass::PackExpansion')                   ,
          (40+200 , 40 , 'ObjCTypeParamType'                   , 'Type::TypeClass::ObjCTypeParam')                   ,
          (41+200 , 41 , 'ObjCObjectType'                      , 'Type::TypeClass::ObjCObject')                      ,
          (42+200 , 42 , 'ObjCInterfaceType'                   , 'Type::TypeClass::ObjCInterface')                   ,
          (43+200 , 43 , 'ObjCObjectPointerType'               , 'Type::TypeClass::ObjCObjectPointer')               ,
          (44+200 , 44 , 'PipeType'                            , 'Type::TypeClass::Pipe')                            ,
          (45+200 , 45 , 'AtomicType'                          , 'Type::TypeClass::Atomic') ;
      )");

      PQXX_.TXN().exec(R"(
        INSERT INTO decl_kind (id, builtin_type_kind, name, class)
        VALUES
          ( 0+300 , 0  , 'OCLImage1dRO'               , 'BuiltinType::Kind::OCLImage1dRO')               ,
          ( 1+300 , 1  , 'OCLImage1dArrayRO'          , 'BuiltinType::Kind::OCLImage1dArrayRO')          ,
          ( 2+300 , 2  , 'OCLImage1dBufferRO'         , 'BuiltinType::Kind::OCLImage1dBufferRO')         ,
          ( 3+300 , 3  , 'OCLImage2dRO'               , 'BuiltinType::Kind::OCLImage2dRO')               ,
          ( 4+300 , 4  , 'OCLImage2dArrayRO'          , 'BuiltinType::Kind::OCLImage2dArrayRO')          ,
          ( 5+300 , 5  , 'OCLImage2dDepthRO'          , 'BuiltinType::Kind::OCLImage2dDepthRO')          ,
          ( 6+300 , 6  , 'OCLImage2dArrayDepthRO'     , 'BuiltinType::Kind::OCLImage2dArrayDepthRO')     ,
          ( 7+300 , 7  , 'OCLImage2dMSAARO'           , 'BuiltinType::Kind::OCLImage2dMSAARO')           ,
          ( 8+300 , 8  , 'OCLImage2dArrayMSAARO'      , 'BuiltinType::Kind::OCLImage2dArrayMSAARO')      ,
          ( 9+300 , 9  , 'OCLImage2dMSAADepthRO'      , 'BuiltinType::Kind::OCLImage2dMSAADepthRO')      ,
          (10+300 , 10 , 'OCLImage2dArrayMSAADepthRO' , 'BuiltinType::Kind::OCLImage2dArrayMSAADepthRO') ,
          (11+300 , 11 , 'OCLImage3dRO'               , 'BuiltinType::Kind::OCLImage3dRO')               ,
          (12+300 , 12 , 'OCLImage1dWO'               , 'BuiltinType::Kind::OCLImage1dWO')               ,
          (13+300 , 13 , 'OCLImage1dArrayWO'          , 'BuiltinType::Kind::OCLImage1dArrayWO')          ,
          (14+300 , 14 , 'OCLImage1dBufferWO'         , 'BuiltinType::Kind::OCLImage1dBufferWO')         ,
          (15+300 , 15 , 'OCLImage2dWO'               , 'BuiltinType::Kind::OCLImage2dWO')               ,
          (16+300 , 16 , 'OCLImage2dArrayWO'          , 'BuiltinType::Kind::OCLImage2dArrayWO')          ,
          (17+300 , 17 , 'OCLImage2dDepthWO'          , 'BuiltinType::Kind::OCLImage2dDepthWO')          ,
          (18+300 , 18 , 'OCLImage2dArrayDepthWO'     , 'BuiltinType::Kind::OCLImage2dArrayDepthWO')     ,
          (19+300 , 19 , 'OCLImage2dMSAAWO'           , 'BuiltinType::Kind::OCLImage2dMSAAWO')           ,
          (20+300 , 20 , 'OCLImage2dArrayMSAAWO'      , 'BuiltinType::Kind::OCLImage2dArrayMSAAWO')      ,
          (21+300 , 21 , 'OCLImage2dMSAADepthWO'      , 'BuiltinType::Kind::OCLImage2dMSAADepthWO')      ,
          (22+300 , 22 , 'OCLImage2dArrayMSAADepthWO' , 'BuiltinType::Kind::OCLImage2dArrayMSAADepthWO') ,
          (23+300 , 23 , 'OCLImage3dWO'               , 'BuiltinType::Kind::OCLImage3dWO')               ,
          (24+300 , 24 , 'OCLImage1dRW'               , 'BuiltinType::Kind::OCLImage1dRW')               ,
          (25+300 , 25 , 'OCLImage1dArrayRW'          , 'BuiltinType::Kind::OCLImage1dArrayRW')          ,
          (26+300 , 26 , 'OCLImage1dBufferRW'         , 'BuiltinType::Kind::OCLImage1dBufferRW')         ,
          (27+300 , 27 , 'OCLImage2dRW'               , 'BuiltinType::Kind::OCLImage2dRW')               ,
          (28+300 , 28 , 'OCLImage2dArrayRW'          , 'BuiltinType::Kind::OCLImage2dArrayRW')          ,
          (29+300 , 29 , 'OCLImage2dDepthRW'          , 'BuiltinType::Kind::OCLImage2dDepthRW')          ,
          (30+300 , 30 , 'OCLImage2dArrayDepthRW'     , 'BuiltinType::Kind::OCLImage2dArrayDepthRW')     ,
          (31+300 , 31 , 'OCLImage2dMSAARW'           , 'BuiltinType::Kind::OCLImage2dMSAARW')           ,
          (32+300 , 32 , 'OCLImage2dArrayMSAARW'      , 'BuiltinType::Kind::OCLImage2dArrayMSAARW')      ,
          (33+300 , 33 , 'OCLImage2dMSAADepthRW'      , 'BuiltinType::Kind::OCLImage2dMSAADepthRW')      ,
          (34+300 , 34 , 'OCLImage2dArrayMSAADepthRW' , 'BuiltinType::Kind::OCLImage2dArrayMSAADepthRW') ,
          (35+300 , 35 , 'OCLImage3dRW'               , 'BuiltinType::Kind::OCLImage3dRW')               ,
          (36+300 , 36 , 'void'                       , 'BuiltinType::Kind::Void')                       ,
          (37+300 , 37 , 'bool'                       , 'BuiltinType::Kind::Bool')                       ,
          (38+300 , 38 , 'Char_U'                     , 'BuiltinType::Kind::Char_U')                     ,
          (39+300 , 39 , 'UChar'                      , 'BuiltinType::Kind::UChar')                      ,
          (40+300 , 40 , 'WChar_U'                    , 'BuiltinType::Kind::WChar_U')                    ,
          (41+300 , 41 , 'Char16'                     , 'BuiltinType::Kind::Char16')                     ,
          (42+300 , 42 , 'Char32'                     , 'BuiltinType::Kind::Char32')                     ,
          (43+300 , 43 , 'unsigned short'             , 'BuiltinType::Kind::UShort')                     ,
          (44+300 , 44 , 'unsigned int'               , 'BuiltinType::Kind::UInt')                       ,
          (45+300 , 45 , 'ULong'                      , 'BuiltinType::Kind::ULong')                      ,
          (46+300 , 46 , 'ULongLong'                  , 'BuiltinType::Kind::ULongLong')                  ,
          (47+300 , 47 , 'uint128'                    , 'BuiltinType::Kind::UInt128')                    ,
          (48+300 , 48 , 'Char_S'                     , 'BuiltinType::Kind::Char_S')                     ,
          (49+300 , 49 , 'SChar'                      , 'BuiltinType::Kind::SChar')                      ,
          (50+300 , 50 , 'WChar_S'                    , 'BuiltinType::Kind::WChar_S')                    ,
          (51+300 , 51 , 'short'                      , 'BuiltinType::Kind::Short')                      ,
          (52+300 , 52 , 'int'                        , 'BuiltinType::Kind::Int')                        ,
          (53+300 , 53 , 'long'                       , 'BuiltinType::Kind::Long')                       ,
          (54+300 , 54 , 'long long'                  , 'BuiltinType::Kind::LongLong')                   ,
          (55+300 , 55 , 'int128'                     , 'BuiltinType::Kind::Int128')                     ,
          (56+300 , 56 , 'Half'                       , 'BuiltinType::Kind::Half')                       ,
          (57+300 , 57 , 'float'                      , 'BuiltinType::Kind::Float')                      ,
          (58+300 , 58 , 'double'                     , 'BuiltinType::Kind::Double')                     ,
          (59+300 , 59 , 'long double'                , 'BuiltinType::Kind::LongDouble')                 ,
          (60+300 , 60 , 'Float16'                    , 'BuiltinType::Kind::Float16')                    ,
          (61+300 , 61 , 'Float128'                   , 'BuiltinType::Kind::Float128')                   ,
          (62+300 , 62 , 'nullptr'                    , 'BuiltinType::Kind::NullPtr')                    ,
          (63+300 , 63 , 'ObjCId'                     , 'BuiltinType::Kind::ObjCId')                     ,
          (64+300 , 64 , 'ObjCClass'                  , 'BuiltinType::Kind::ObjCClass')                  ,
          (65+300 , 65 , 'ObjCSel'                    , 'BuiltinType::Kind::ObjCSel')                    ,
          (66+300 , 66 , 'OCLSampler'                 , 'BuiltinType::Kind::OCLSampler')                 ,
          (67+300 , 67 , 'OCLEvent'                   , 'BuiltinType::Kind::OCLEvent')                   ,
          (68+300 , 68 , 'OCLClkEvent'                , 'BuiltinType::Kind::OCLClkEvent')                ,
          (69+300 , 69 , 'OCLQueue'                   , 'BuiltinType::Kind::OCLQueue')                   ,
          (70+300 , 70 , 'OCLReserveID'               , 'BuiltinType::Kind::OCLReserveID')               ,
          (71+300 , 71 , 'Dependent'                  , 'BuiltinType::Kind::Dependent')                  ,
          (72+300 , 72 , 'Overload'                   , 'BuiltinType::Kind::Overload')                   ,
          (73+300 , 73 , 'BoundMember'                , 'BuiltinType::Kind::BoundMember')                ,
          (74+300 , 74 , 'PseudoObject'               , 'BuiltinType::Kind::PseudoObject')               ,
          (75+300 , 75 , 'UnknownAny'                 , 'BuiltinType::Kind::UnknownAny')                 ,
          (76+300 , 76 , 'BuiltinFn'                  , 'BuiltinType::Kind::BuiltinFn')                  ,
          (77+300 , 77 , 'ARCUnbridgedCast'           , 'BuiltinType::Kind::ARCUnbridgedCast')           ,
          (78+300 , 78 , 'OMPArraySection'            , 'BuiltinType::Kind::OMPArraySection') ;
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
                 200  AS BuiltinType_Kind,
                   0  AS Nil_DeclContext_id,
                 name AS BuiltinType_Name
          FROM decl_kind
          WHERE builtin_type_kind IS NOT NULL
          ORDER BY id ASC ;
      )");
    }

} // plugin ns.
} // clong ns.
