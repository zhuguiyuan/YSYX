import math

import matplotlib.pyplot as plt

bench_result = """
RUNNING: ./tests/list_algos_time_tests
compare_bubble 50000 13.052195
compare_merge 50000 0.044079
compare_merge_inplace 50000 0.014179
compare_merge_bottom_up 50000 0.011177
bench_merge_bottom_up 5000 0.000447
bench_merge_bottom_up 10000 0.000975
bench_merge_bottom_up 20000 0.002565
bench_merge_bottom_up 40000 0.005134
bench_merge_bottom_up 80000 0.020187
bench_merge_bottom_up 160000 0.050896
bench_merge_bottom_up 320000 0.112951
bench_merge_bottom_up 640000 0.260410
bench_merge_bottom_up 1280000 0.617391
bench_merge_bottom_up 2560000 1.354108
bench_merge_bottom_up 5120000 3.071656
bench_merge_bottom_up 10240000 7.034350
bench_merge_bottom_up 20480000 15.836046
bench_merge_inplace 5000 0.001494
bench_merge_inplace 10000 0.003277
bench_merge_inplace 20000 0.006954
bench_merge_inplace 40000 0.016800
bench_merge_inplace 80000 0.019258
bench_merge_inplace 160000 0.045173
bench_merge_inplace 320000 0.106519
bench_merge_inplace 640000 0.245967
bench_merge_inplace 1280000 0.565000
bench_merge_inplace 2560000 1.291202
bench_merge_inplace 5120000 2.925557
bench_merge_inplace 10240000 6.847243
bench_merge_inplace 20480000 15.352949
bench_merge 5000 0.002135
bench_merge 10000 0.004600
bench_merge 20000 0.009817
bench_merge 40000 0.021365
bench_merge 80000 0.057709
bench_merge 160000 0.129230
bench_merge 320000 0.281514
bench_merge 640000 0.611174
bench_merge 1280000 1.352975
bench_merge 2560000 2.851534
bench_merge 5120000 6.360520
bench_merge 10240000 14.103243
bench_bubble 5000 0.078591
bench_bubble 10000 0.315391
bench_bubble 15000 0.801684
bench_bubble 20000 1.512634
bench_bubble 25000 2.428786
bench_bubble 30000 3.582096
bench_bubble 35000 5.198738
bench_bubble 40000 6.973998
bench_bubble 45000 10.307188
bench_bubble 50000 13.488033
ALL TESTS PASSED
Tests run: 5
tests/list_algos_time_tests PASS
"""


def parse_series(name):
    rows = [
        row
        for row in (line.split() for line in bench_result.splitlines())
        if row and row[0] == name
    ]
    scale = [int(row[1]) for row in rows]
    time = [float(row[2]) for row in rows]
    return scale, time


def linear_fit(xs, ys):
    n = len(xs)
    sum_x = sum(xs)
    sum_y = sum(ys)
    sum_xx = sum(x * x for x in xs)
    sum_xy = sum(x * y for x, y in zip(xs, ys))
    denom = n * sum_xx - sum_x * sum_x
    slope = (n * sum_xy - sum_x * sum_y) / denom
    intercept = (sum_y - slope * sum_x) / n
    return slope, intercept


def r_squared(ys, predicted):
    mean_y = sum(ys) / len(ys)
    ss_res = sum((y - p) ** 2 for y, p in zip(ys, predicted))
    ss_tot = sum((y - mean_y) ** 2 for y in ys)
    return 1.0 - ss_res / ss_tot


def fit_and_plot(name, model_name, feature_fn, marker):
    scale, time = parse_series(name)
    features = [feature_fn(n) for n in scale]
    slope, intercept = linear_fit(features, time)
    fitted = [slope * x + intercept for x in features]
    score = r_squared(time, fitted)

    print(
        f"{name}: time = {slope:.12g} * {model_name} + {intercept:.12g}, "
        f"R^2 = {score:.6f}"
    )

    plt.plot(scale, time, marker, label=f"{name} measured")
    plt.plot(scale, fitted, "--", label=f"{name} fit: {model_name}")


fit_and_plot("bench_merge_bottom_up", "n log2(n)", lambda n: n * math.log2(n), "s-")
fit_and_plot("bench_merge_inplace", "n log2(n)", lambda n: n * math.log2(n), "s-")
fit_and_plot("bench_merge", "n log2(n)", lambda n: n * math.log2(n), "^-")
fit_and_plot("bench_bubble", "n^2", lambda n: n * n, ".-")

plt.xlabel("scale")
plt.ylabel("time(s)")
plt.legend()
plt.savefig("time_result.png")
plt.show()
