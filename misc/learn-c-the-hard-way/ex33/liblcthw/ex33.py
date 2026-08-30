import math

import matplotlib.pyplot as plt

bench_result = """
compare_bubble 50000 17.038408
compare_merge 50000 0.049954
compare_merge_inplace 50000 0.008849
bench_merge_inplace 5000 0.001633
bench_merge_inplace 10000 0.002727
bench_merge_inplace 20000 0.006431
bench_merge_inplace 40000 0.008399
bench_merge_inplace 80000 0.038174
bench_merge_inplace 160000 0.066150
bench_merge_inplace 320000 0.136932
bench_merge_inplace 640000 0.290223
bench_merge_inplace 1280000 0.717930
bench_merge_inplace 2560000 1.559077
bench_merge_inplace 5120000 3.286789
bench_merge_inplace 10240000 7.125488
bench_merge_inplace 20480000 15.943984
bench_merge 5000 0.003412
bench_merge 10000 0.018972
bench_merge 20000 0.033385
bench_merge 40000 0.082210
bench_merge 80000 0.124701
bench_merge 160000 0.198572
bench_merge 320000 0.431403
bench_merge 640000 1.002157
bench_merge 1280000 2.243391
bench_merge 2560000 4.864938
bench_merge 5120000 10.726529
bench_merge 10240000 23.556838
bench_bubble 5000 0.075627
bench_bubble 10000 0.343810
bench_bubble 15000 0.836740
bench_bubble 20000 1.556608
bench_bubble 25000 2.557616
bench_bubble 30000 3.726110
bench_bubble 35000 5.273947
bench_bubble 40000 7.400632
bench_bubble 45000 9.423844
bench_bubble 50000 13.135987
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


fit_and_plot("bench_merge_inplace", "n log2(n)", lambda n: n * math.log2(n), "s-")
fit_and_plot("bench_merge", "n log2(n)", lambda n: n * math.log2(n), "^-")
fit_and_plot("bench_bubble", "n^2", lambda n: n * n, ".-")

plt.xlabel("scale")
plt.ylabel("time(s)")
plt.legend()
plt.savefig("time_result.png")
plt.show()
