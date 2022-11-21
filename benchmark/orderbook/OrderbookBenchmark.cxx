#include <benchmark/benchmark.h>

#include <cstdlib>
#include <memory>
#include <random>

#include "../../src/orderbook/Container.hxx"
#include "../../src/orderbook/Order.hxx"
#include "../../src/orderbook/Orderbook.hxx"

template <typename Book>
class OrderbookBenchmarkFixture : public benchmark::Fixture {
public:
  OrderbookBenchmarkFixture()
      : book{}, rd{}, gen{rd()}, boolDistrib(0, 1),
        intDistrib(0, 1'000'000'000), doubleDistrib(0, 1'000'000'000){};

  Book book;
  std::random_device rd;
  std::mt19937 gen;
  std::uniform_int_distribution<> boolDistrib;
  std::uniform_int_distribution<> intDistrib;
  std::uniform_real_distribution<> doubleDistrib;

  void SetUp(const ::benchmark::State &state) {}

  void TearDown(const ::benchmark::State &state) {}
};

#define INSERT_ITEM_INTO_BOOK_CONST_REF                                        \
  (benchmark::State & st) {                                                    \
    for (auto _ : st) {                                                        \
      st.PauseTiming();                                                        \
      auto id = intDistrib(gen);                                               \
      auto exchangeId = intDistrib(gen);                                       \
      auto price = intDistrib(gen);                                            \
      auto quantity = intDistrib(gen);                                         \
      auto isBuy = static_cast<bool>(boolDistrib(gen));                        \
      auto order = hermes::Order(id, price, quantity, isBuy, exchangeId);      \
      st.ResumeTiming();                                                       \
      book.insert(order);                                                      \
    }                                                                          \
  }

#define INSERT_ITEM_INTO_BOOK_RVAL_REF                                         \
  (benchmark::State & st) {                                                    \
    for (auto _ : st) {                                                        \
      st.PauseTiming();                                                        \
      auto id = intDistrib(gen);                                               \
      auto exchangeId = intDistrib(gen);                                       \
      auto price = intDistrib(gen);                                            \
      auto quantity = intDistrib(gen);                                         \
      auto isBuy = static_cast<bool>(boolDistrib(gen));                        \
      auto order = hermes::Order(id, price, quantity, isBuy, exchangeId);      \
      st.ResumeTiming();                                                       \
      book.insert(std::move(order));                                           \
    }                                                                          \
  }

BENCHMARK_TEMPLATE_DEFINE_F(
    OrderbookBenchmarkFixture, OrderbookRBTreeContainerConstRefTest,
    hermes::Orderbook<
        hermes::Order, hermes::RBTreeContainer<hermes::Order>,
        hermes::RBTreeContainer<hermes::Order,
                                std::greater<typename hermes::Order::price_t>>>)
INSERT_ITEM_INTO_BOOK_CONST_REF;

BENCHMARK_REGISTER_F(OrderbookBenchmarkFixture,
                     OrderbookRBTreeContainerConstRefTest);

BENCHMARK_TEMPLATE_DEFINE_F(
    OrderbookBenchmarkFixture, OrderbookRBTreeContainerRvalRefTest,
    hermes::Orderbook<
        hermes::Order, hermes::RBTreeContainer<hermes::Order>,
        hermes::RBTreeContainer<hermes::Order,
                                std::greater<typename hermes::Order::price_t>>>)
INSERT_ITEM_INTO_BOOK_CONST_REF;

BENCHMARK_REGISTER_F(OrderbookBenchmarkFixture,
                     OrderbookRBTreeContainerRvalRefTest);

BENCHMARK_MAIN();

//
// #define MODIFY_ITEM_IN_BOOK \
//   (benchmark::State & st) { \
//     for (auto _ : st) { \
//       st.PauseTiming(); \
//       auto id = uint64Distrib(gen); \
//       auto exchangeId = uint64Distrib(gen); \
//       auto price = uint64Distrib(gen); \
//       auto quantity = uint64Distrib(gen); \
//       auto isBuy = static_cast<bool>(boolDistrib(gen)); \
//       auto order = hermes::LimitOrder(id, price, quantity, isBuy,
//       exchangeId); \
//       st.ResumeTiming(); \
//       book.insert(std::move(order)); \
//     } \
//   }
//
// #define WITH_RANGES Range(8, 8 << 5)
//
// BENCHMARK_TEMPLATE_DEFINE_F(
//     OrderbookBenchmarkFixture,
//     branchOrderbookListContainerConstRefTest,
//     hermes::BranchOrderBook<hermes::ListContainer, hermes::LimitOrder>)
// INSERT_ITEM_INTO_BOOK_RVAL_REF;
//
// BENCHMARK_REGISTER_F(OrderbookBenchmarkFixture,
//                      branchOrderbookListContainerConstRefTest);
//
// BENCHMARK_TEMPLATE_DEFINE_F(
//     OrderbookBenchmarkFixture, branchOrderbookListContainerRvalRefTest,
//     hermes::BranchOrderBook<hermes::ListContainer, hermes::LimitOrder>)
// INSERT_ITEM_INTO_BOOK_RVAL_REF;
//
// BENCHMARK_REGISTER_F(OrderbookBenchmarkFixture,
//                      branchOrderbookListContainerRvalRefTest);
//
// BENCHMARK_TEMPLATE_DEFINE_F(
//     OrderbookBenchmarkFixture, branchOrderbookMapContainerConstRefTest,
//     hermes::BranchOrderBook<hermes::MapListContainer,
//     hermes::LimitOrder>)
// INSERT_ITEM_INTO_BOOK_CONST_REF;
//
// BENCHMARK_REGISTER_F(OrderbookBenchmarkFixture,
//                      branchOrderbookMapContainerConstRefTest);
//
// BENCHMARK_TEMPLATE_DEFINE_F(
//     OrderbookBenchmarkFixture, branchOrderbookMapContainerRvalRefTest,
//     hermes::BranchOrderBook<hermes::MapListContainer,
//     hermes::LimitOrder>)
// INSERT_ITEM_INTO_BOOK_RVAL_REF;
//
// BENCHMARK_REGISTER_F(OrderbookBenchmarkFixture,
//                      branchOrderbookMapContainerRvalRefTest);
//
// BENCHMARK_TEMPLATE_DEFINE_F(
//     OrderbookBenchmarkFixture,
//     branchOrderbookVectorContainerConstRefTest,
//     hermes::BranchOrderBook<hermes::VectorContainer,
//     hermes::LimitOrder>)
// INSERT_ITEM_INTO_BOOK_CONST_REF;
//
// BENCHMARK_REGISTER_F(OrderbookBenchmarkFixture,
//                      branchOrderbookVectorContainerConstRefTest);
//
// BENCHMARK_TEMPLATE_DEFINE_F(
//     OrderbookBenchmarkFixture,
//     branchOrderbookVectorContainerRvalRefTest,
//     hermes::BranchOrderBook<hermes::VectorContainer,
//     hermes::LimitOrder>)
// INSERT_ITEM_INTO_BOOK_RVAL_REF;
//
// BENCHMARK_REGISTER_F(OrderbookBenchmarkFixture,
//                      branchOrderbookVectorContainerRvalRefTest);
//
// BENCHMARK_MAIN();
//