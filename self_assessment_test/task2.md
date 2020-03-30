- When would you use a sorted binary tree over a linked list?

When we want to keep sorted data.

- What would you change if the tree had to support not just integers, but any
  type of data? 
  
Instead of key being an integer - it would have been an pointer. Then after extracting the key - we would only need to cast it to the type we used.
  
- Why do we use modules instead of putting everything a single file?

Because:
1. It's easier to debug and localize problems
2. Easier to read and reuse code. Easier to add parts and change some implementations without having to rewrite code wherever it was used
3. In big projects with large build trees it signifficantly boosts the build time because after changing one file we only need to rebuild parts that depended on it.


- Is the development cycle (i.e. changing a source file, recompiling and
  running tests) faster when using modules? Explain your answer.
  
Yes. Partly answered in the previous question. After creating a build graph the build system can fastly identify what work it needs to do (what part of the graph is "dirty"). Same applies to CI/CD systems which run only affected tests. 

- What is a _header guard_ and why is it needed?

It prevents a header file being included more than once preventing multiple definition errors

- Why is `bstree` not _defined_ in the header file? What are the
  implications?
  
We want to keep some fields unvisible to the user (private). For example, size - so that no one could easily change it directly.  
  
- Explain the `const` in the parameter list of `bstree_print`, is it required?

yes, to prevent changing given data. (of course we cannot specify it as a const and program will still compile, but never want to give extra rights for reading and writing for security reasons)

- Explain memory leaks. Why are memory leaks bad? You should know how to use `valgrind` to check your program for memory leaks.

It's when a allocated memory has not been set free. Basically that means that program does not require this memory space, but the computer and Operation System does not know that. Thus, many memory can be leaked and it can lead to significant problems. For example, some vital process won't get all the resources he needs.

- What is the reason behind writing _everything_ in English?

It's an international language and most resources, conferences in Computer Science are written/help in English. Documenting everything in English makes possible to hire developers all over the world.

- Why should you use `static` for _non-exported_ functions?

We want to restrict access to these functions only within the file on a compile-level. This way we can guarantee that this function will never be used elsewhere. Also because of this we can use the same names fo functions in different files.

- Why should we comment our source code? Is it always needed? What should the
  comment state? What is _self-documenting code_?
  
We should comment to out colleagues understand the code as fast as possible. No, only where it actually helps to understand what is happening. For example, comment some magic numbers or a link to the resource where this "hack" was found.  _self-documenting code_ is a code that clearly describes what is does. All the variables and functions have logical names and the code structure also helps navigating through it.  
  
- Why should the module not output debug messages?

We want to separate production from debug stages. Not only for clarifying, but in many cases debug messages have some vital information and we don't want to give it the end-user.

- Why and when should you use `assert`?

We use `assert` to check some parts of our code where wwe depend on an incoming parameter or a system call and etc and where we cannot handle the error. For example, we got NULL as parameter in our function and we can't do anything with it. 

- What are the benefits of using `make` over calling the compiler by hand?

We don't need to keep all of the dependecnies in our hand to compile the code wee need. We simply write it once and then everyone can compile our program by running onlu one command. He does not need to know how our files depend on each other and so on. What is more - Make optimizes the compilation and does only the work that is necessary.

- Imagine it was your job to design the interface for the bstree module
  (writing `bstree.h`). What would you have done differently, and why?
  
In addition to using `bool bstree_contains(const bstree* t, int d);` I would declare `const node* bstree_search(const bstree* t, int d);` so that it we could we could do something with it. It might not be necessary for a BST storing integer values (since `bstree_contains` gives us an answer), but it might be very useful for a BST containing some complex data structures where we would to get a pinter to the desired value. 

I also would add a return value to the insert and remove methods. So that we would get a pointer to the given node.