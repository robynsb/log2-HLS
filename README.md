# Fixed-point log2 vitis HLS

The algorithm for $\log_2(x)$:
1. Find position of most significant bit which gives $e = \lfloor \log_2(x) \rfloor$.
2. Find $m = \frac{m}{2^e} \in [1,2]$.
3. Calculate $\log_2(m)$ using configurable size LUT and linear interpolation.
4. Obtain $\log_2(x) = e+\log_2(m)$.

