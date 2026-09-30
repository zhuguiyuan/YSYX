#!/usr/bin/env python3
"""Resize a JPG to 640x480 (aspect-preserving scale + zero padding)
and write a $readmemh-compatible hex file, one RGB888 word per line."""

import argparse
from pathlib import Path

from PIL import Image, ImageOps

# W, H = 64, 128
# STRIDE = 64
W, H = 640, 480
STRIDE = 1 << 10


def letterbox(path: Path, w: int = W, h: int = H) -> Image.Image:
    img = Image.open(path).convert("RGBA")
    bg = Image.new("RGBA", img.size, "white")  # 白底，吞掉 PNG 透明区
    pad = ImageOps.pad(Image.alpha_composite(bg, img), (w, h), color="white")
    return pad.convert("RGB")


if __name__ == "__main__":
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("src", type=Path, nargs="?", default=Path("input.jpg"))
    ap.add_argument("dst", type=Path, nargs="?", default=Path("image.hex"))
    args = ap.parse_args()

    data = letterbox(args.src).tobytes()  # "rrggbb" repeated
    blank = "000000"
    # address = h*STRIDE + v  ==>  outer loop over h, inner loop over v
    lines = [
        data[(v * W + h) * 3 : (v * W + h) * 3 + 3].hex() if h < W else blank
        for v in range(H)
        for h in range(STRIDE)
    ]
    args.dst.write_text("\n".join(lines) + "\n")
