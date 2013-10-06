OpenACC to OpenCL Compiler
==========================

This repository is the main repository for an OpenACC to OpenCL Compiler. 

## ROSE (./rose)

This compiler is based on ROSE Compiler, ./rose is a submodule pointing to the version of ROSE Compiler used by this project.
To obtain the submodule: 'git submodule init ; git submodule update'.

The actual compiler can be found under ./rose/projects/openacc-to-opencl. However, most of the compiler is implemented through different modules spread inside ROSE.
Just a glance on the modules:
 * ./rose/src/frontend/DLX : Directive-based Language eXtension, used to describe/parse/compile a directive-based language. It is applied to OpenACC.
 * ./rose/src/midend/MFB   : Multi-File Builder, provides an interface to load multiple source files and distribute generated code accordingly.
 * ./rose/src/midend/MDCG  : Model Driven Code Generator, provides access to an object oriented point-of-view. It can: load models from files, generate statements from models, extend model, output model. 
 * ./rose/src/backend/KLT  : Kernel from LoopTree, is a kernel generator. It produces OpenCL but was though to be able to target other languages. It also interfaces with the device side runtime. 

## ./libopenacc

An implementation of OpenACC API, it includes extensions used by the code generator to translate the directives. This implementation of OpenACC is using OpenCL.
The extension of the API is classified in four categories:
 * public   : Standardized, accessible to OpenACC users
 * private  : Non-standarized, used in the transformed code to replace the directive.
 * internal : Implementation dependent, used by public, private and generated host code.
 * device   : Implementation dependent, used by generated device code

## ./examples

It contains examples of OpenACC codes and corresponding outputs of the compiler. It does not contains actual outputs, but either expected outputs or refactored/commented outputs

## ./docs

Documentations for the whole project. Done with Doxygen, it compiles the documentation for libopenacc, examples, DLX, MFB, MDCG, KLT, and the actual OpenACC to OpenCL Compiler in ROSE's projects.

## ./include

It contains some standard headers (OpenCL for now) to enable code generation and some compilation without installing OpenCL .


