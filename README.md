OpenACC to OpenCL Compiler
==========================

This repository is the main repository for an OpenACC to OpenCL Compiler.

## OpenACC Library [./libopenacc]

An implementation of **OpenACC API**, it implements the standard's API (which we call Public API), but also extend this API to cover the manipulation of parallel regions and kernels (the Private API).
Finaly, it contains two APIs specific to OpenCL: the internal API and the Device API.

## ROSE [./rose]

The compiler itself is distributed as part of ROSE Compiler.
The OpenACC to OpenCL Compiler uses four building-blocks from ROSE Compiler.

This submodule is not public, but access can be granted to collaborators.
Releases of the compiler will refers to the official ROSE Compiler repository.

### Multi-File Builder (MFB) [rose/src/midend/MFB]

This module provides support to build consistent compilation units accross multiple files (AST).
It manipulates the ASTs of each file at the API level. APIs are list of symbols (namespaces, functions, classes, variables, methods).

### Directive-based Language eXtension (DLX) [rose/src/frontend/DLX]

This building-block takes the description of one directives language.
The skeleton of a **directive parser** and a **directive-graph builder** is produced by templates extensions.
The **directive parser** takes the string representing a directive and identifies the **construct**, the set of **clauses**,
and the parameters for each clauses. A limited amount of template specializations have to be provided to parse the parameters.

### Kernel from LoopTree (KLT) [rose/src/backend/KLT]

This building-block produces **computation kernels** from a represention of a **LoopTrees** (ie: sequence of loop nests).

LoopTrees represent loop nest and additionnal information about variables in use and possible Language specific information.
For example, annotations from a language as OpenACC can be directly integrated through a template from DLX.

KLT uses templates to provide a generic way of transforming one LoopTrees into many different Kernels that implement the LoopTree.
It uses templates because this building-block aims to be able to produce Kernels for multiple Target Languages and Runtime (ie: execution model).
Multiple Kernels are produced because of the different degree of freedom accessible for each Runtime. 
It is currently demonstrated for OpenACC (runtime) and OpenCL C (target language).

KLT uses MFB to manage the code generation and access the runtime and target language APIs.

### Model Driven Code Generator (MDCG) [rose/src/midend/MDCG]

This module manipulates ASTs at the Model level. Models are build from APIs, they are organized function of the scope of each symbol.
Models con also contains some extra information processed from the original AST.

### OpenACC to OpenCL Compiler [rose/projects/openacc-to-opencl]

The actual OpenACC to OpenCL Compiler uses DLX, KLT, and MDCG ; it uses MFB as it is used by KLT and MDCG.

DLX's OpenACC specialization parses and processes OpenACC directives.
KLT's OpenACC & OpenCL C specialization produces OpenCL C kernels which use OpenACC's Device API.
MFB and MDCG provide API and Model abstractions to manipulate OpenACC API and OpenCL C builtin API.

## Documentation [./docs]

This project is mainly documented using Doxygen, see the submodule at [./docs/doxygen-html/]
which is accessible for browsing at [http://tristanvdb.github.io/acc2ocl-doxy/].

