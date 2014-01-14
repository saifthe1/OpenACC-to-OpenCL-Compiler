OpenACC to OpenCL Compiler
==========================

This repository is the main repository for an OpenACC to OpenCL Compiler.

## Documentation [./docs]

This project is mainly documented using Doxygen, see the submodule at [./docs/doxygen-html/]
which is accessible for browsing at [http://tristanvdb.github.io/acc2ocl-doxy/].

## OpenACC Library [./libopenacc]

An implementation of **OpenACC API**, libopenacc implements the standard's API (which we call Public API),
but also extend this API to cover the manipulation of parallel regions and computation kernels (the Private API).
Finally, it contains two APIs specific to OpenCL: the Internal API and the Device API.
The Internal API exposes data-structures and functions used to describe and manipulate OpenCL C kernels.
The Device API (in OpenCL C) is used by the kernels.

## ROSE [./rose]

The compiler itself is distributed as part of ROSE Compiler.
The OpenACC to OpenCL Compiler uses four building-blocks (listed below) from ROSE Compiler.

This submodule is not public, but access can be granted to collaborators. However, the documentation is public.
Releases of the compiler will refer to the official ROSE Compiler repository.

### Multi-File Builder (MFB) [rose/src/midend/MFB]

This module provides support to build consistent compilation units accross multiple files (AST).
It manipulates the ASTs of each file at the API level. APIs are list of symbols (namespaces, functions, classes, variables, methods).

### Directive-based Language eXtension (DLX) [rose/src/frontend/DLX]

This building-block takes the description of one directives language.
The skeleton of a **directive parser** and a **directive-graph builder** is produced by templates extensions.
The **directive parser** takes the string representing a directive and identifies the **construct**, the set of **clauses**,
and the parameters for each clauses. A limited amount of template specializations must be provided to parse the parameters.

### Kernel from LoopTree (KLT) [rose/src/backend/KLT]

This building-block produces **computation kernels** from a represention of **LoopTrees** (ie: sequence of loop nests).

LoopTrees represent loop nest and additional information about variables in use and possibly Language specific information.
For example, annotations from OpenACC can be directly integrated through a template from DLX.

KLT uses templates to provide a generic building-block for transforming **LoopTrees** into many different Kernels.
We abstract the specialization of kernel generation based on the runtime and target language. It is currently demonstrated for OpenACC (runtime) and OpenCL C (target language).

KLT uses MFB to manage the code generation and access the runtime and target language APIs.

### Model Driven Code Generator (MDCG) [rose/src/midend/MDCG]

This module manipulates ASTs at the Model level. Models are build from APIs which are organized based on the scope of each symbol.
Model contains information processed from the AST. It incorporates the inheritance graph of classes.
It links variables and functions to their types.

### OpenACC to OpenCL Compiler [rose/projects/openacc-to-opencl]

The actual OpenACC to OpenCL Compiler uses DLX, KLT, and MDCG ; it uses MFB as a dependence for KLT and MDCG.

DLX's OpenACC specialization parses and processes OpenACC directives.
KLT's OpenACC & OpenCL C specialization produces OpenCL C kernels which use OpenACC's Device API.
MFB and MDCG provide API and Model abstractions to manipulate OpenACC API and OpenCL C builtin API.

