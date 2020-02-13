#include <c10/util/Exception.h>
#include <torch/csrc/jit/fuser/common/arith.h>
#include <torch/csrc/jit/fuser/common/ir.h>
#include <torch/csrc/jit/fuser/common/tensor.h>
#include <torch/csrc/jit/fuser/common/type.h>

namespace torch{
namespace jit{
namespace fuser{
TORCH_API const Val* new_val(ValType vtype, DataType dtype){
  switch (vtype) {
    case (ValType::Tensor):
      return new Tensor(dtype); // TODO add dtype here.
    case (ValType::Scalar):
      switch (dtype) {
        case (DataType::Float):
          return new Float();
        case (DataType::Int):
          return new Int();
        default:
          break;
      }
    default:
      break;
  }
  throw std::runtime_error("Cannot promote types."); //Todo print val and data types in the error
}

TORCH_API const Val* promote_new(const Val* v1, const Val* v2) {
  TORCH_CHECK(v1->isVal() && v2->isVal());
  TORCH_CHECK(
      v1->getDataType() != DataType::Null &&
      v2->getDataType() != DataType::Null);

  ValType out_vtype =
      promote_type(v1->getValType().value(), v2->getValType().value());
  DataType out_dtype =
      promote_type(v1->getDataType().value(), v2->getDataType().value());

  return new_val(out_vtype, out_dtype);
}

TORCH_API Val* add(Val* v1, Val* v2) {
  Val* out = promote_new(v1, v2);
  Statement* expr = new Add(out, v1, v2);
  return out;
}

TORCH_API Val* cast_op(const DataType dtype, Val* v1){
  if( !is_cast_legal(v1->getDataType().value(), dtype) ) {
	std::stringstream err;
	err << "Illegal Cast of DataTypes From: " << v1->getDataType().value() << " To: " << dtype;
    throw std::runtime_error(err.str());
  }
  Val* out = new_val(v1->getValType().value(), dtype);
  Statement* expr = new UnaryOp(UnaryOpType::Cast, out, v1);
  return out;
}

TORCH_API const Val* unary_op(UnaryOpType type, Val* v1){
  const Val* out = new_val(v1->getValType().value(), v1->getDataType().value());
  Statement* expr = new UnaryOp(type, out, v1);
  return out;
}

TORCH_API const Val* binary_op(BinaryOpType type, const Val* v1, const Val* v2){
  const Val* out = promote_new(v1, v2);
  Statement* expr = new BinaryOp(type, out, v1, v2);
  return out;
}

} // namespace fuser
} // namespace jit
} // namespace torch
