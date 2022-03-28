#include <dmlc/json.h>
#include <fstream>
#include <tvm/runtime/ndarray.h>

class Emitter {
public:
  Emitter (std::vector<std::function<void()>> &ops) : ops_(ops) {}

  void SetEmitOrder(const std::string &emit_order_json) {
    std::ifstream fs(emit_order_json);

    dmlc::JSONReader reader(&fs);
    std::string emit_order;
    std::vector<int> order;

    reader.BeginObject();
    reader.NextObjectItem(&emit_order);
    ICHECK(emit_order == "emit_order");
    reader.Read(&order);
    size_t emit_cnt = static_cast<size_t>(*std::max_element(order.begin(), order.end())) + 1;
    idx.assign(emit_cnt, -1);
    for (size_t i = 0; i < order.size(); i++) {
      if (order[i] != -1) {
        idx[order[i]] = i;
      }
    }
  }

  void EmitAll() {
    for (size_t i : idx) {
        ICHECK(ops_[i]);
        ops_[i]();
    }
  }

private:
  std::vector<size_t> idx;
  std::vector<std::function<void()>> &ops_;
};
