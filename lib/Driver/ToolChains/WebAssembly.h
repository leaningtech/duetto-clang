//===--- WebAssembly.h - WebAssembly ToolChain Implementations --*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_WEBASSEMBLY_H
#define LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_WEBASSEMBLY_H

#include "Gnu.h"
#include "clang/Driver/Tool.h"
#include "clang/Driver/ToolChain.h"

namespace clang {
namespace driver {
namespace tools {
namespace wasm {

class LLVM_LIBRARY_VISIBILITY Linker : public GnuTool {
public:
  explicit Linker(const ToolChain &TC);
  bool isLinkJob() const override;
  bool hasIntegratedCPP() const override;
  void ConstructJob(Compilation &C, const JobAction &JA,
                    const InputInfo &Output, const InputInfoList &Inputs,
                    const llvm::opt::ArgList &TCArgs,
                    const char *LinkingOutput) const override;
};

} // end namespace wasm

/// Cheerp tools: llvm-link, opt and llc
namespace cheerp {
  enum CheerpWasmOpt
  {
    INVALID,
    GROWMEM,
    SHAREDMEM,
    EXPORTEDTABLE,
  };
  std::vector<CheerpWasmOpt> getWasmFeatures(const Driver& D, const llvm::opt::ArgList& Args);

  class LLVM_LIBRARY_VISIBILITY Link : public Tool {
  public:
    Link(const ToolChain &TC) : Tool("cheerp::Link", "linker", TC) {}

    virtual bool hasIntegratedCPP() const { return false; }
    virtual bool isLinkJob() const { return true; }

    virtual void ConstructJob(Compilation &C, const JobAction &JA,
                              const InputInfo &Output,
                              const InputInfoList &Inputs,
                              const llvm::opt::ArgList &TCArgs,
                              const char *LinkingOutput) const;
  };

  class LLVM_LIBRARY_VISIBILITY CheerpOptimizer : public Tool {
  public:
    CheerpOptimizer(const ToolChain &TC) : Tool("cheerp::CheerpOptimizer", "optimizer", TC) {}

    virtual bool hasIntegratedCPP() const { return false; }

    virtual void ConstructJob(Compilation &C, const JobAction &JA,
                              const InputInfo &Output,
                              const InputInfoList &Inputs,
                              const llvm::opt::ArgList &TCArgs,
                              const char *LinkingOutput) const;
  };

  class LLVM_LIBRARY_VISIBILITY CheerpCompiler : public Tool {
  public:
    CheerpCompiler(const ToolChain &TC) : Tool("cheerp::CheerpCompiler", "linker", TC) {}

    virtual bool hasIntegratedCPP() const { return false; }

    virtual void ConstructJob(Compilation &C, const JobAction &JA,
                              const InputInfo &Output,
                              const InputInfoList &Inputs,
                              const llvm::opt::ArgList &TCArgs,
                              const char *LinkingOutput) const;
  };
} // end namespace cheerp
} // end namespace tools

namespace toolchains {

class LLVM_LIBRARY_VISIBILITY WebAssembly final : public ToolChain {
public:
  WebAssembly(const Driver &D, const llvm::Triple &Triple,
              const llvm::opt::ArgList &Args);

private:
  bool IsMathErrnoDefault() const override;
  bool IsObjCNonFragileABIDefault() const override;
  bool UseObjCMixedDispatch() const override;
  bool isPICDefault() const override;
  bool isPIEDefault() const override;
  bool isPICDefaultForced() const override;
  bool IsIntegratedAssemblerDefault() const override;
  bool hasBlocksRuntime() const override;
  bool SupportsObjCGC() const override;
  bool SupportsProfiling() const override;
  bool HasNativeLLVMSupport() const override;
  void addClangTargetOptions(const llvm::opt::ArgList &DriverArgs,
                             llvm::opt::ArgStringList &CC1Args,
                             Action::OffloadKind DeviceOffloadKind) const override;
  RuntimeLibType GetDefaultRuntimeLibType() const override;
  CXXStdlibType GetCXXStdlibType(const llvm::opt::ArgList &Args) const override;
  void AddClangSystemIncludeArgs(
      const llvm::opt::ArgList &DriverArgs,
      llvm::opt::ArgStringList &CC1Args) const override;
  void AddClangCXXStdlibIncludeArgs(
      const llvm::opt::ArgList &DriverArgs,
      llvm::opt::ArgStringList &CC1Args) const override;

  const char *getDefaultLinker() const override {
    return "lld";
  }

  Tool *buildLinker() const override;
};

class LLVM_LIBRARY_VISIBILITY Cheerp : public ToolChain {
private:
  mutable std::unique_ptr<tools::cheerp::CheerpCompiler> CheerpCompiler;
  mutable std::unique_ptr<tools::cheerp::CheerpOptimizer> CheerpOptimizer;
public:
  Cheerp(const Driver &D, const llvm::Triple& Triple,
         const llvm::opt::ArgList &Args);

  bool IsUnwindTablesDefault() const override;
  bool isPICDefault() const override;
  bool isPIEDefault() const override;
  bool isPICDefaultForced() const override;

  void
  AddClangSystemIncludeArgs(const llvm::opt::ArgList &DriverArgs,
                            llvm::opt::ArgStringList &CC1Args) const override;
  void
  AddClangCXXStdlibIncludeArgs(const llvm::opt::ArgList &DriverArgs,
                               llvm::opt::ArgStringList &CC1Args) const override;

  Tool *buildLinker() const override;
  Tool *getTool(Action::ActionClass AC) const override;
};
 
} // end namespace toolchains
} // end namespace driver
} // end namespace clang

#endif // LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_WEBASSEMBLY_H
