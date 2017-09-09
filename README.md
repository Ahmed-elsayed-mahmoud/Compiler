# Three Phase Compiler

Phase 1: (Lexical Analyzer)
- This phase of the project aims to build automatic lexical analyzer generator tools.
- This tool has two input files, one for lexical rules and the other for user input.
- The output of this tool is a list of tokens which matches the user's input file.

Phase 2: (Syntax Analyzer – Parser -)
- This phase of the project aims to build automatic parser generator tools.
- This tool has two inputs, one is the context free grammar as input file, and the other is the output of phase one -stream of tokens- 
- The output of this tool is the left most derivation for deriving the input tokens.

Phase 3: (Byte Code Generation)
- This phase of the project aims to practice techniques of constructing semantics rules to generate intermediate code.
- This tool has one input, the java code.
- The output of this tool is the java byte code.

*Panic mode recovery was used for error handling in the three phases.
