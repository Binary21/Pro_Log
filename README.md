# Pro_Log
Programming Logic Interpreter

Overview
The Programming Logic Interpreter (PLI) is a comprehensive tool designed to analyze,
process, and evaluate programming logic. This interpreter offers a suite of functions ranging
from lexical analysis to advanced problem-solving methods using backwards chaining.
Whether you're trying to dissect a logical expression or query a complex knowledge base, PLI has got you covered.

Major Functions

  1. Lexer
  2. Expression Tree
  3. Parser  
  4. Parser for Queries
  5. Knowledge Base
  6. Parser for Knowledge Bases
  7. Knowledge Base Utilities
  8. Backwards Chaining

1. The Lexer

Definition:
Lexing is the process of taking the raw input source, usually ASCII text, and converting it to a sequence of tokens.

How it Works:
Recognizes combinations of characters that are meaningful (tokens) and separates them from their syntax.
Uses finite-state machine (finite automata) design pattern/algorithm.
Reads each character from the stream and toggles between states depending on the character type.

Tokens Used:
OPEN - Indicates the start of a predicate argument list.
CLOSE - Indicates the end of a predicate argument list.
COMMA - Separator for predicate arguments.
IMP - Denotes implication in a clause.
END - Indicates the end of a clause or query.
STRING - Represents predicates, atoms, and variables.

Features:
Ability to handle CSV files and INI file formats.
Strips comments from the input.
Handles delimited strings.
Produces a sequence of tokens with a type and value.

Error Handling:
Detects malformed implications and returns an informative error message.

2. The Expression Tree

Definition:
Expression Trees are non-linear data structures used to define a hierarchial relationship between data. 
This program contains expressions that are best represented in this fashion, enabling the use of several algorithms.

How it Works:
The Expression Tree uses several different node type to assing children and data. Each node consists of its contents, its children and its type.
All node types are declared as Enums. Each node list of children contains the children nodes that form the rest of the tree. These nodes can be travesed
using the breadth-first teqnique.

Enum Types:
NONE - Indicates that a node is completely empty with no children, a leaf
ATOM - This too is a leaf node but has a character type
VARIABLE - This type of node is similar to the atom but can be used for substitution purposes
COMPOUND - This variable is capable of holding children. 
ROOT - This is the root of the tree, it is a object capable of holding children without having contents

3. The Parser

Definition: 
The Parser serves as an interpreter for the tokenized sequences provided by the Lexer. Its function is to validate these sequences against the predefined grammar rules of the language,
ensuring its structure is correct. Once validated, it converts these sequences into Expression Trees for easier manipulation and understanding. This includes processing and parsing queries, 
as well as the broader knowledge base.

How it Works:
Grammar Processing: The parser employs the BNF (Backus-Naur Form) notation, a representation for defining sequences in the language, to validate the structure.
Descent Algorithm: The Recursive Descent Algorithm is the backbone of its parsing mechanism. It breaks down the input systematically, verifying it against the language grammar at each level.
Knowledge Base Parsing: The parser can interpret the detailed BNF grammar of the vt-prolog language, which describes a knowledge-base as an unordered list of clauses, facts, and rules. 
Using existing parser code, the function parseKnowledgeBase is capable of parsing entire knowledge bases.

Features:
Operator Parsing: Recognizes and correctly interprets up to six different input operators.
Direct String Parsing: Beyond token lists, the parser offers a convenient method to parseknowledge bases directly from strings.
Able to parse 6 different input operators.
Uses BNF Notation.
Parsing is done with a Recursive Descent Algorithm.

Error Handeling:
The parser conatians a ParseError object that when flagged exits the parser and returns two outputs, an error boolean and an error message.
The error boolean indecates that an error has taken place while the error message indicates the type of user error that took place.

4. The Knowledge Base

Definition:
The Knowledge Base is the foundation of a Prolog program. It is a structured repository that stores all the logic in the form of clauses. 
Each clause in the Knowledge Base consists of a head and a body, represented as expression trees. The separation between the head and body is typically denoted by ":-" within the input.

How it Works:
Storage: The Knowledge Base is designed to hold a list of clauses. Each clause encapsulates logic in the form of a head and an optional body, which are represented as Expression Trees.
Adding Information: Through the tell member function, programmers can add clauses to the Knowledge Base. This function takes a clause as its parameter and appends it to the list of clauses in the database.
Querying: While the detailed querying mechanism, ask, will be developed in a subsequent milestone, it's intended to allow programmers to query and retrieve information from the Knowledge Base.
Traversal: The Knowledge Base supports forward iteration, allowing programmers to traverse through all the clauses. The begin and end member functions respectively provide iterators to the first and one-past-the-last clauses.

Features:
Size Assessment: The size member function enables users to determine the number of clauses stored in the database.
Constant Iteration: Provides constant iteration capability to loop through the clauses in arbitrary order without modification.

5. Unfication

Definition:
Unification is a crucial component of First-Order Logic, allowing queries to be parsed against an existing knowledge-base. This process determines a set of substitutions for variables, 
ensuring the equivalence of two expressions. The upcoming three milestones focus on implementing the ask member of the KnowledgeBase class to facilitate these queries.

Unification Explained:
Consider the vt-prolog expressions f(a) and f(X) as expression trees. Through unification,
we ascertain an assignment for variable X that equates both trees. Here, the simple substitution is X/a. 
The notation X/a denotes "X substituted by a". In scenarios involving entire subtrees, 
substitutions can be more complex. For instance, to equate f(g(a),b) with f(X,b), the substitution is X/g(a). 
Furthermore, when no variables are involved, unification can simply be a test for equality, 
as seen in f(a,g(b,c)) with itself. However, unification can fail as well, as shown in f(a,b) and f(X,c).

Substitution Set:
This is a data structure that stores substitutions. This is done by rapid mapping from expression trees to multiple arbitrary trees. Essential functions include:

Inserting a substitution.
Looking up substitutions.
Iterating through the substitution set.

Unification Algorithm:
This algorithm returns a result showcasing unification's success and the associated substitution, this algorithm operates recursively and in parallel using threads. 
Its core function is to assess two expression trees and an initial substitution set to determine if they can be unified. If unification succeeds, it updates the UnificationResult passed to it.





































