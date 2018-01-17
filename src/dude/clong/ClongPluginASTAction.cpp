
#include <clang/Frontend/FrontendPluginRegistry.h>
#include "ClongPluginASTAction.hpp"
#include "Clong.hpp"
#include "dude/util/Terminal.hpp"

namespace clong {
namespace plugin {

  // ctor
  ClongPluginASTAction::ClongPluginASTAction()
  {
  }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  // dtor
  ClongPluginASTAction::~ClongPluginASTAction()
  {
  }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  std::unique_ptr<ASTConsumer>
    ClongPluginASTAction::CreateASTConsumer(CompilerInstance &CI,
                                           llvm::StringRef InFile)
    {
      return llvm::make_unique< Clong >(CI,
                                        ParsedTemplates,
                                        PQXXConnectionOptionsStr);
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  PluginASTAction::ActionType
    ClongPluginASTAction::getActionType() {
      TPush log;
      *log << "ActionType set to: AddBeforeMainAction\n";
      // return AddAfterMainAction;
      return AddBeforeMainAction;
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  bool
    ClongPluginASTAction::ParseArgs(const CompilerInstance &CI,
                                   const std::vector<std::string> &args)
    {
      TPush log;

      if (!args.empty() && args[0] == "help")
        PrintHelp( terrs() );

      for (unsigned i = 0, e = args.size(); i != e; ++i) {
        *log << "- arg = " << args[i] << tendl ;
      }

      return true;
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  void
    ClongPluginASTAction::PrintHelp(llvm::raw_ostream& ros) {
      ros << "Help for TraverseTranslationUnit plugin goes here\n";
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  bool
    ClongPluginASTAction::BeginInvocation(CompilerInstance &CI)
    {
      TPush();
      return true;
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  bool
    ClongPluginASTAction::BeginSourceFileAction(CompilerInstance &CI)
    {
      TPush();
      return true;
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  void
    ClongPluginASTAction::ExecuteAction()
    {
      TPush();
      Base::ExecuteAction();
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  void
    ClongPluginASTAction::EndSourceFileAction()
    {
      TPush();
    }

} // plugin ns.
} // clong ns.


// ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~

/**
 * Clang plugin registration
 *
 * * `clang/Frontend/FrontendPluginRegistry.h`
 */
static
  clang::FrontendPluginRegistry::Add< clong::plugin::ClongPluginASTAction >
    X("clong", "Ich bin Clong!");

// ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~
