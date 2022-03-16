#include <dmlc/json.h>
#include <fstream>
#include <tvm/runtime/ndarray.h>

class Emitter {
public:
  Emitter (std::vector<std::function<void()>> &ops) : ops_(ops) {
    std::ifstream fs("../stream_assignment/emit_order.json");

    dmlc::JSONReader reader(&fs);
    std::string emit_order;
    std::vector<size_t> order;

    reader.BeginObject();
    reader.NextObjectItem(&emit_order);
    ICHECK(emit_order == "emit_order");
    reader.Read(&order);
    idx.resize(order.size());
    for (size_t i = 0; i < order.size(); i++) {
      idx[order[i]] = i;
    }
  }

  void ExtractExecutable () {
    size_t cnt = 0;
    for (size_t i = 0; i < ops_.size(); i++) {
      if (ops_[i])
        ops_[cnt++] = ops_[i];
    }
    ops_.resize(cnt);
    ICHECK(cnt == idx.size());
  }

  void EmitAll() {
    for (size_t i : idx) {
        ops_[i]();
    }
  }

private:
  std::vector<size_t> idx;
  std::vector<std::function<void()>> &ops_;
};
