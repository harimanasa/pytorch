#pragma once

#include <iostream>

namespace torch {
namespace jit {
namespace fuser {

struct Fusion;

struct Statement;

struct Val;
struct Expr;

struct UnaryOp;
struct BinaryOp;

struct ForLoop;
struct IfThenElse;

struct Tensor;
struct TensorDomain;
struct TensorView;
struct IterDomain;
struct TensorIndex;

struct TensorContiguity;


struct Split;
struct Merge;
struct Reorder;

struct Float;
struct Int;
struct Add;

struct TORCH_API IRPrinter {

  std::ostream& os;
  bool print_inline_ = false;

public:

  IRPrinter(std::ostream& _os):os(_os){}

  virtual void print(Fusion* const);
  //Print calls some non const fusion ops,
  //eventhough fusion should remain unchanged.
  //Need to look into this.
  virtual void print(const Fusion* const f){
    print(const_cast<Fusion*>(f));
  }
  virtual void print(Fusion& f){print(&f);}

  virtual void print(const Statement* const);

  virtual void print(const Val* const);
  virtual void print(const Expr* const);

  virtual void print(const Tensor* const);
  virtual void print(const TensorDomain* const);
  virtual void print(const TensorView* const);
  virtual void print(const IterDomain* const);
  virtual void print(const TensorIndex* const);
  virtual void print(const TensorContiguity* const);

  virtual void print(const Float* const);
  virtual void print(const Int* const);

  virtual void print(const UnaryOp* const);
  virtual void print(const BinaryOp* const);

  virtual void print(const ForLoop* const);
  virtual void print(const IfThenElse* const);

  virtual void print(const Split* const);
  virtual void print(const Merge* const);
  virtual void print(const Reorder* const);

  void print_inline(const Statement* const stmt){
    bool prev = print_inline_;
    print_inline_ = true;
    print(stmt);
    print_inline_ = prev;
  }

  };

  TORCH_API std::ostream& operator<< (std::ostream& os, const Statement* const stmt);
  TORCH_API std::ostream& operator<< (std::ostream& os, Fusion* f);
  TORCH_API std::ostream& operator<< (std::ostream& os, Fusion& f);

} // namespace fuser
} // namespace jit
} // namespace torch
