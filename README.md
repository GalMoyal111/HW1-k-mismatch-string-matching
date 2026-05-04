# HW1-k-mismatch-string-matching
## Project Overview
This project implements and compares multiple algorithms for **string matching** with up to **k mismatches** in large-scale text (up to 100MB).

The focus is on improving performance over naive search by leveraging advanced techniques based on MCS and optimized configuration-based filtering.

---

## Implemented Algorithms

1. Naive Search 
   A straightforward approach that scans the entire text and compares each position with up to k mismatches.  
   High time complexity due to exhaustive comparison.

2. Basic MCS (Configuration-Based Search)
   Generates binary configurations representing meaningful substrings to reduce the number of full comparisons.

3. Weighted MCS 
   Uses a greedy strategy to select configurations that maximize coverage and minimize the number of checks.

4. Positional Associated MCS
   Extends MCS by incorporating positional information, enabling further optimization and faster matching.

---

## Key Parameters
- Word length: ~19  
- Allowed mismatches: ~40% of word length  
- Text size: 100MB  
- Number of query words: ~40  

---

## Performance Results

| Algorithm                  | Runtime (approx) | Speedup vs Naive |
|---------------------------|------------------|------------------|
| Naive                     | ~261 sec         | 1x               |
| Basic MCS                 | ~0.08 sec        | ~30,000x         |
| Weighted MCS              | ~0.047 sec       | ~50,000x         |
| Positional Associated MCS | ~0.014 sec       | ~53,000x         |

