# Cairo

Cairo is a C++ implementation of trading systems testing framework. It's mostly for educational purposes of learning C++ and algorithmic trading validation. A lot of "back-tested" strategies rely primarily on simple historical data backtesting or some variant of t-tests (or summary statistics such as Sharpe and mean), bootstraping, and OOS/Walk-forward.

I am interested in finding out how many of these strategies can survive this more rigorous statistical testing.

* 🎲 Monte‑Carlo Permutation Testing implementation
  * [x] transaction data shuffling
  * [ ] bar data shuffling

* 🧠 Others
  * [ ] bootstraping
  * [ ] t‑testing

* 📊 Data
  * [ ] toy dataset
  * [ ] strategy

* 🧪 Testing
  * [ ] see how things pan out

## Build
Step 1.
```git clone --recursive```

Step 2.
```
mkdir -p build
cd build
cmake ..
make -j
```

Step 3.
Run an executable you want.

## Algorithms

### t-testing and bootstraping

### Monte-Carlo permutation Testing

### Permutation Testing

## Test

### Dataset & Strategies
