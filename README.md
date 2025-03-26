
# LISP Inspired Software Processor

**LISP Inspired Software Processor** (LISP) is a minimal yet expressive LISP-style interpreter built from scratch in C — designed for clarity, extensibility, and eventually compilation to a custom architecture.

> **Current Codename:** `Sans Phoque` (v1.0)  
> Because it's small, fast, interactive — and absolutely sans phoque.

---

## 🚀 Features

- 🧠 LISP-inspired syntax with prefix notation
- 🧱 First-class support for both integers and strings
- ➕ Polymorphic `+` operator (addition + string concatenation)
- 📤 `print` for output
- 📥 `input` for interactive programs
- 🧾 `def`, `seq`, and nested expressions
- 🔍 Pretty-printed AST for debugging
- ⚡ Written in clean, portable C

---

## 📝 Example Program

```lisp
(seq
  (print "Hello, Welcome to LISP_LITE")
  (def name (input))
  (print (+ "Hello, " name))
)
```

💬 Output:
```
Hello, Welcome to LISP_LITE
IT'S ALIVE
Hello, IT'S ALIVE
```


---

## 🛠️ Milestones

| Version | Codename       | Highlights                      |
|---------|----------------|---------------------------------|
| 0.9     | Pexecuter      | Full numeric evaluation         |
| 1.0     | Sans Phoque    | Strings, input, runtime types   |
| ...     | (coming soon)  | Lambda, conditionals, compiler  |

---

## 🧩 Roadmap

- [x] Add strings and string ops
- [x] Add user input
- [x] `if`, `=`, and conditionals
- [ ] `lambda`, closures, first-class functions
- [ ] Floating-point number support
- [ ] Bytecode generation for the LKS-8 architecture
- [ ] REPL
- [ ] Standard library

---

## 🧪 Philosophy

LISP isn't just a language — it's a state of mind.

This project started as a debate about programming paradigms and grew into a system that puts power into the simplest form possible: an expression tree, evaluated cleanly.

---

## 🙏 Credits

Made with chaos and clarity by Aiden Lortie  
Inspired by LISP, fueled by spite for excessive syntax.

---